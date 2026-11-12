#pragma once
#include <stdint.h>
#include <string.h>
constexpr uint32_t MAGIC=0xB3C32026;
constexpr uint8_t CHANNEL=6;
// Change these two 16-byte keys identically on BOTH units before use.
constexpr char PMK[]="BuomC3PairKey001";
constexpr char LMK[]="BuomC3LinkKey001";
#pragma pack(push,1)
struct ControlPacket {uint32_t magic,session,seq; uint16_t throttle; int16_t roll,pitch; uint8_t arm; uint16_t crc;};
struct StatusPacket {uint32_t magic,seq; uint16_t millivolts;uint8_t armed,warning;uint16_t crc;};
#pragma pack(pop)
inline uint16_t crc16(const void *data,int len){auto p=(const uint8_t*)data;uint16_t c=0xFFFF;while(len--){c^=uint16_t(*p++)<<8;for(int i=0;i<8;i++)c=(c&0x8000)?(c<<1)^0x1021:c<<1;}return c;}
template<typename T> void seal(T &p){p.crc=crc16(&p,sizeof(p)-2);}
template<typename T> bool check(const T &p){return p.magic==MAGIC&&p.crc==crc16(&p,sizeof(p)-2);}
inline bool configured(const uint8_t* m){uint8_t b=0;for(int i=0;i<6;i++)b|=m[i];return b!=0;}
inline float bound(float x,float lo,float hi){return x<lo?lo:(x>hi?hi:x);}
