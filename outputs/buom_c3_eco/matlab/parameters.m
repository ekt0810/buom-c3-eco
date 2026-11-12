function P = parameters()
% SI units. These aerodynamic/control coefficients are ASSUMPTIONS to identify.
P.dt=0.004; P.T=60; P.m0=0.0775; P.mass=P.m0; P.g=9.81;
P.V0=2; P.radius=8; P.f0=3; P.A0=25*pi/180;
P.tauAct=0.055; P.slewAct=4; P.rollGain=30; P.pitchGain=24;
P.rateDamp=4; P.vzDamp=1.4; P.wind=[0.20;-0.10];
P.angleKp=[3;3]; P.rateKp=[0.32;0.38];
P.rateKi=[0.12;0.14]; P.rateKd=[0.018;0.020];
P.dTau=0.025; P.uMax=0.35; P.iMax=0.12;
P.rateMax=1.2; P.phiMax=25*pi/180; P.thetaMax=15*pi/180;
P.fMin=1.5; P.fMax=3.8; P.noise=0.008; P.seed=42;
P.closedLoop=true; P.disturbance=true; P.delay=0;
end
