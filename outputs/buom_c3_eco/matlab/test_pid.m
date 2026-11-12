function test_pid()
P=parameters(); S=struct('d',[0;0],'prev',[0;0],'i',[0;0]);
for k=1:2500,[u,S]=pid_step([100;100],[0;0],S,P,P.dt);end
assert(all(abs(u)<=P.uMax)); assert(all(abs(S.i)<1e-9));
for k=1:500,[u,S]=pid_step([0;0],[0;0],S,P,P.dt);end
assert(all(abs(u)<1e-9));
end
