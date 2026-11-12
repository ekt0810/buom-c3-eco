#include <cassert>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "../firmware_eco/aircraft/protocol.h"
#include "../pid_reference/control.h"
int main(){
 assert(strlen(PMK)==16&&strlen(LMK)==16);
 assert(crc16("123456789",9)==0x29B1);
 ControlPacket p={MAGIC,12,1,0,0,0,0,0};seal(p);assert(check(p));
 p.throttle=1;assert(!check(p));
 RatePID pid(.32,.12,.018);
 for(int i=0;i<5000;i++)assert(pid.step(100,0,.004)<=.35f);
 assert(fabs(pid.i)<1e-6);
 for(int i=0;i<100;i++)pid.step(0,0,.004);
 assert(fabs(pid.step(0,0,.004))<1e-6);
 Mixer m;float l,r,f;
 for(int i=0;i<10000;i++){m.update(1,.35,.35,.004,l,r,f);assert(fabs(l)<=36.001&&fabs(r)<=36.001);}
 m.update(0,.35,.35,.004,l,r,f);assert(l==0&&r==0);
 puts("PASS: CRC known vector, packet corruption, 16-byte keys, PID saturation/recovery, mixer envelope.");
}
