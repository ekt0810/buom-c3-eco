function run_all(makePlots)
% MATLAB R2018b+ base language; also checked using GNU Octave.
if nargin<1, makePlots=true; end
close all; here=fileparts(mfilename('fullpath')); cd(here);
if ~exist('results','dir'), mkdir('results'); end
P=parameters(); D=simulate(P);
P.closedLoop=false; O=simulate(P);
save('results/nominal.mat','D','O','-v7');
cases={'nominal','mass_plus_20pct','delay_40ms','strong_wind'};
M=zeros(4,4);
for j=1:4
 Q=parameters();
 if j==2,Q.mass=1.2*Q.mass;end
 if j==3,Q.delay=0.040;end
 if j==4,Q.wind=[0.8;-0.4];end
 Z=simulate(Q); M(j,:)=[Z.rmsPosition,Z.maxAngle,Z.minHeight,Z.saturation];
 assert(all(isfinite(Z.X(:)))); assert(Z.minHeight>0.3);
 assert(Z.maxAngle<40); assert(Z.rmsPosition<3);
end
fid=fopen('results/metrics.csv','w'); fprintf(fid,'case,rms_position_m,max_attitude_deg,min_z_m,saturation_fraction\n');
for j=1:4,fprintf(fid,'%s,%.6f,%.6f,%.6f,%.6f\n',cases{j},M(j,:));end; fclose(fid);
% Numeric export shared by the independent WebGL playback.
if makePlots
f=figure('visible','off','position',[50 50 1200 760]);
subplot(2,2,1); plot3(D.X(:,1),D.X(:,2),D.X(:,3),'b',D.C(:,5),D.C(:,6),D.C(:,7),'k--'); grid on; axis equal;
xlabel('x (m)');ylabel('y (m)');zlabel('z (m)');title('Ideal-navigation circular flight');legend('simulated','reference');
subplot(2,2,2); plot(D.t,D.X(:,6:7)*180/pi,D.t,D.C(:,1:2)*180/pi,'--');grid on;ylabel('angle (deg)');xlabel('t (s)');title('Roll / pitch tracking');
subplot(2,2,3); plot(D.t,D.U(:,1:2));grid on;ylabel('normalized command');xlabel('t (s)');title('Rate PID outputs');legend('roll','pitch');
subplot(2,2,4); plot(D.t,D.U(:,3));grid on;ylabel('flap frequency (Hz)');xlabel('t (s)');title('Ideal altitude loop');
print(f,'results/flight_control.png','-dpng','-r140');
f=figure('visible','off','position',[50 50 1000 450]);
plot(D.t,D.X(:,6)*180/pi,O.t,O.X(:,6)*180/pi,'--');grid on;xlim([15 28]);xlabel('t (s)');ylabel('roll (deg)');legend('PID enabled','rate control disabled');title('Same gust, same plant: attitude feedback comparison');
print(f,'results/gust_comparison.png','-dpng','-r140');
end
idx=1:5:length(D.t); phase=cumtrapz(D.t,D.U(:,3))*2*pi;
aL=(25+6*D.U(:,1)/P.uMax).*sin(phase)+5*D.U(:,2)/P.uMax;
aR=(25-6*D.U(:,1)/P.uMax).*sin(phase)+5*D.U(:,2)/P.uMax;
data=[D.t(idx),D.X(idx,1:7),aL(idx),aR(idx),D.U(idx,:)];
fid=fopen('results/trajectory.csv','w');fprintf(fid,'t,x,y,z,V,psi,phi,theta,wingL_deg,wingR_deg,uRoll,uPitch,f_Hz\n');fclose(fid);
dlmwrite('results/trajectory.csv',data,'-append','precision',8);
fid=fopen('results/trajectory.json','w');fprintf(fid,'%s',jsonencode(data));fclose(fid);
test_pid(); fprintf('PASS: 4 plant scenarios, PID saturation/recovery.\n'); disp(M);
end
