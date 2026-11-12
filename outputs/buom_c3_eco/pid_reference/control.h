#pragma once
#include <math.h>
inline float clip(float a,float lo,float hi){return a<lo?lo:(a>hi?hi:a);}
struct RatePID {
 float kp,ki,kd,i=0,d=0,previous=0; bool initialized=false;
 RatePID(float p,float in,float de):kp(p),ki(in),kd(de){}
 void reset(){i=d=previous=0;initialized=false;}
 float step(float ref,float measured,float dt){
   if(!initialized){previous=measured;initialized=true;}
   d+=dt/(0.025f+dt)*((measured-previous)/dt-d); previous=measured;
   float e=ref-measured,raw=kp*e+i-kd*d;
   if(fabsf(raw)<0.35f || e*raw<0) i=clip(i+ki*e*dt,-0.12f,0.12f);
   return clip(raw,-0.35f,0.35f);
 }
};
struct Mixer {
 float phase=0;
 void update(float throttle,float ur,float up,float dt,float &l,float &r,float &freq){
   freq=1.5f+2.3f*clip(throttle,0,1);phase=fmodf(phase+6.2831853f*freq*dt,6.2831853f);
   float ramp=clip(throttle/0.15f,0,1);
   l=((25+6*ur/0.35f)*sinf(phase)+5*up/0.35f)*ramp;
   r=((25-6*ur/0.35f)*sinf(phase)+5*up/0.35f)*ramp;
 }
};
