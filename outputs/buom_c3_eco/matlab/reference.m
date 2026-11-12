function R=reference(t,P)
% Wide circular flight, not hover. Ideal external navigation in simulation.
w=P.V0/P.radius; a=w*t;
R.pos=[P.radius*cos(a);P.radius*sin(a);1.5+0.25*sin(0.15*t)];
R.vel=[-P.V0*sin(a);P.V0*cos(a);0.0375*cos(0.15*t)];
end
