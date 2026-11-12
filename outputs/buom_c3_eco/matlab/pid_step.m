function [u,S]=pid_step(ref,meas,S,P,dt)
% Parallel PID, D on measured rate, first-order derivative filter.
e=ref-meas;
S.d=S.d+(dt/(P.dTau+dt))*((meas-S.prev)/dt-S.d);
S.prev=meas;
raw=P.rateKp.*e+S.i-P.rateKd.*S.d;
u=max(-P.uMax,min(P.uMax,raw));
% Conditional integration: integrate only when not pushing into saturation.
di=P.rateKi.*e*dt; ok=(abs(raw)<P.uMax)|(sign(e)~=sign(raw));
S.i=max(-P.iMax,min(P.iMax,S.i+di.*ok));
end
