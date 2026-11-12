#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <esp_task_wdt.h>
#include "config.h"
#include "protocol.h"
portMUX_TYPE mutex=portMUX_INITIALIZER_UNLOCKED;
ControlPacket latest={}; uint32_t receivedAt=0,acceptedSession=0,acceptedSeq=0;
bool haveSession=false,armed=false,seenLow=false,prevArm=false;
uint32_t tick=0,armedAt=0,lastTelemetry=0;float vbat=0,phase=0,throttle=0,ur=0,up=0;
void onReceive(const esp_now_recv_info_t* info,const uint8_t* data,int len){
 if(memcmp(info->src_addr,TX_MAC,6)||len!=sizeof(ControlPacket))return;
 ControlPacket p;memcpy(&p,data,len);
 if(!check(p)||p.throttle>1000||abs(p.roll)>1000||abs(p.pitch)>1000||p.arm>1)return;
 portENTER_CRITICAL(&mutex);
 if(!haveSession||p.session!=acceptedSession){
   // Transmitter reset cannot arm: new session must first send arm-low/throttle-low.
   if(p.arm||p.throttle>40){portEXIT_CRITICAL(&mutex);return;}
   acceptedSession=p.session;acceptedSeq=p.seq-1;haveSession=true;
 }
 if(int32_t(p.seq-acceptedSeq)>0){acceptedSeq=p.seq;latest=p;receivedAt=millis();}
 portEXIT_CRITICAL(&mutex);
}
void setPulse(int pin,float deg,int sign,float center){
 float us=bound(center+sign*deg*US_PER_DEG,MIN_US,MAX_US);
 ledcWrite(pin,uint32_t(us*SERVO_HZ*65535.0f/1e6f));
}
void disarm(){
 ledcWrite(LEFT_PIN,0);ledcWrite(RIGHT_PIN,0);
 if(USE_POWER_SWITCH)digitalWrite(POWER_PIN,LOW);
 armed=false;throttle=ur=up=0;
}
void setup(){
 pinMode(POWER_PIN,OUTPUT);digitalWrite(POWER_PIN,LOW);
 ledcAttach(LEFT_PIN,SERVO_HZ,16);ledcAttach(RIGHT_PIN,SERVO_HZ,16);disarm();
 Serial.begin(115200);delay(700);WiFi.mode(WIFI_STA);WiFi.setSleep(false);
 Serial.print("AIRCRAFT MAC: ");Serial.println(WiFi.macAddress());
 if(!configured(TX_MAC)){while(true){Serial.print("AIRCRAFT MAC: ");Serial.println(WiFi.macAddress());Serial.println("Set TX_MAC in config.h, then rebuild.");delay(2000);}}
 esp_wifi_set_channel(CHANNEL,WIFI_SECOND_CHAN_NONE);
 if(esp_now_init()!=ESP_OK){Serial.println("ESP-NOW init failed");while(true)delay(1000);}
 esp_now_set_pmk((const uint8_t*)PMK);
 esp_now_peer_info_t peer={};memcpy(peer.peer_addr,TX_MAC,6);memcpy(peer.lmk,LMK,16);peer.channel=CHANNEL;peer.encrypt=true;
 if(esp_now_add_peer(&peer)!=ESP_OK){Serial.println("Peer failed");while(true)delay(1000);}
 esp_now_register_recv_cb(onReceive);analogSetPinAttenuation(VBAT_PIN,ADC_11db);
 vbat=analogReadMilliVolts(VBAT_PIN)*.001f*133/33*VBAT_CAL;
 esp_task_wdt_config_t wdt={.timeout_ms=1000,.idle_core_mask=0,.trigger_panic=true};
 if(esp_task_wdt_init(&wdt)==ESP_ERR_INVALID_STATE)esp_task_wdt_reconfigure(&wdt);
 esp_task_wdt_add(NULL);tick=micros();
 Serial.println("t_ms,V,armed,throttle,wingL,wingR,fHz");
}
void loop(){
 esp_task_wdt_reset();
 uint32_t elapsed=micros()-tick;if(elapsed<5000)return;tick=micros();float dt=bound(elapsed*1e-6f,.001f,.02f);
 ControlPacket p;uint32_t rt;portENTER_CRITICAL(&mutex);p=latest;rt=receivedAt;portEXIT_CRITICAL(&mutex);
 uint32_t now=millis();bool fresh=rt&&now-rt<150;bool healthy=fresh&&elapsed<=20000;
 vbat+=.015f*(analogReadMilliVolts(VBAT_PIN)*.001f*133/33*VBAT_CAL-vbat);
 if(healthy&&!p.arm)seenLow=true;
 if(!healthy||!p.arm){if(armed)seenLow=false;disarm();}
 if(RELEASE_ACTUATORS&&healthy&&seenLow&&p.arm&&!prevArm&&p.throttle<40&&vbat>7.2f){
   armed=true;seenLow=false;armedAt=now;phase=0;setPulse(LEFT_PIN,0,SIGN_L,CENTER_L);setPulse(RIGHT_PIN,0,SIGN_R,CENTER_R);
   if(USE_POWER_SWITCH)digitalWrite(POWER_PIN,HIGH);
 }
 prevArm=healthy&&p.arm;
 float l=0,r=0,f=0;
 if(armed){
  float target=now-armedAt<500?0:p.throttle/1000.0f;
  throttle+=bound(target-throttle,-dt,dt);
  ur+=bound(ROLL_SIGN*p.roll/1000.0f-ur,-3*dt,3*dt);
  up+=bound(PITCH_SIGN*p.pitch/1000.0f-up,-3*dt,3*dt);
  f=1.5f+2.3f*throttle;phase=fmodf(phase+2*PI*f*dt,2*PI);
  float ramp=bound(throttle/.15f,0,1);
  l=((AMPLITUDE+6*ur)*sinf(phase)+5*up)*ramp;
  r=((AMPLITUDE-6*ur)*sinf(phase)+5*up)*ramp;
  setPulse(LEFT_PIN,l,SIGN_L,CENTER_L);setPulse(RIGHT_PIN,r,SIGN_R,CENTER_R);
 }
 if(now-lastTelemetry>=100){
  lastTelemetry=now;StatusPacket s={MAGIC,acceptedSeq,uint16_t(bound(vbat*1000,0,16000)),uint8_t(armed),uint8_t(vbat<7.0f),0};seal(s);esp_now_send(TX_MAC,(uint8_t*)&s,sizeof(s));
  if(Serial.availableForWrite()>100)Serial.printf("%lu,%.3f,%d,%.3f,%.2f,%.2f,%.2f\n",now,vbat,armed,throttle,l,r,f);
 }
}
