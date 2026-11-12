function D=simulate(P)
rng(P.seed); t=(0:P.dt:P.T)'; n=numel(t);
x=[P.radius;0;1.5;P.V0;pi/2;-atan(P.V0^2/(P.g*P.radius));0;0;0;0;0;0;P.f0];
S.pid=struct('d',[0;0],'prev',[0;0],'i',[0;0]); S.iz=0;
X=zeros(n,13); U=zeros(n,3); C=zeros(n,7);
delayN=round(P.delay/P.dt); hist=repmat([0;0;P.f0],1,delayN+1);
for k=1:n
  [u,S,c]=controller(t(k),x,S,P); X(k,:)=x'; U(k,:)=u'; C(k,:)=c';
  hist=[hist(:,2:end),u]; ua=hist(:,1);
  % ZOH controller + RK4 plant integration.
  h=P.dt; a=dynamics(t(k),x,ua,P); b=dynamics(t(k)+h/2,x+h*a/2,ua,P);
  cc=dynamics(t(k)+h/2,x+h*b/2,ua,P); d=dynamics(t(k)+h,x+h*cc,ua,P);
  x=x+h*(a+2*b+2*cc+d)/6;
end
D=struct('t',t,'X',X,'U',U,'C',C,'P',P);
D.rmsPosition=sqrt(mean(sum((X(:,1:3)-C(:,5:7)).^2,2)));
D.maxAngle=max(max(abs(X(:,6:7))))*180/pi;
D.minHeight=min(X(:,3)); D.saturation=mean(any(abs(U(:,1:2))>=P.uMax-1e-6,2));
end
