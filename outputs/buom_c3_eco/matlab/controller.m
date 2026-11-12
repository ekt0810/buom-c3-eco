function [U,S,C]=controller(t,X,S,P)
% X=[x y z V psi phi theta p q vz ur_act up_act f_act]'.
R=reference(t,P); e=R.pos-X(1:3);
vg=R.vel(1:2)+0.45*e(1:2)-P.wind;
heading=atan2(vg(2),vg(1)); eh=atan2(sin(heading-X(5)),cos(heading-X(5)));
% World heading positive CCW; body positive roll is right bank.
phi=max(-P.phiMax,min(P.phiMax,-atan(P.V0^2/(P.g*P.radius))-0.8*eh));
theta=max(-P.thetaMax,min(P.thetaMax,0.22*(norm(vg)-X(4))));
rateRef=max(-P.rateMax,min(P.rateMax,P.angleKp.*([phi;theta]-X(6:7))));
meas=X(8:9)+P.noise*randn(2,1);
if P.closedLoop
  [u,S.pid]=pid_step(rateRef,meas,S.pid,P,P.dt);
else
  u=[0;0];
end
% Altitude PI + vertical-rate damping, ideal altitude sensor; NOT onboard.
az=2.0*e(3)+1.8*(R.vel(3)-X(10))+S.iz;
S.iz=max(-1.0,min(1.0,S.iz+0.25*e(3)*P.dt));
battery=max(0.8,(8.2-0.015*t)/7.4);
% Controller assumes nominal mass; payload scenario is unknown to controller.
liftRatio=max(0.3,1+az/P.g);
f=P.f0*sqrt(liftRatio/(max(0.5,cos(X(6))*cos(X(7)))*battery*(0.7+0.3*(X(4)/P.V0)^2)));
U=[u;max(P.fMin,min(P.fMax,f))];
C=[phi;theta;rateRef;R.pos];
end
