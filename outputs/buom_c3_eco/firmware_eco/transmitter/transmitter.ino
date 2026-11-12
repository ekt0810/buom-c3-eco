#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "config.h"
#include "protocol.h"
uint32_t sessionId,seq=0,lastSend=0;volatile uint32_t lastStatus=0;
volatile uint16_t battery=0;volatile bool aircraftArmed=false,low=false;
void onReceive(const esp_now_recv_info_t* info,const uint8_t* data,int len){
 if(memcmp(info->src_addr,AIR_MAC,6)||len!=sizeof(StatusPacket))return;
 StatusPacket p;memcpy(&p,data,len);if(!check(p))return;
 battery=p.millivolts;aircraftArmed=p.armed;low=p.warning;lastStatus=millis();
}
int16_t axis(int pin){
 int a=analogRead(pin),e=a-AXIS_CENTER;float n=e>=0?float(e)/(AXIS_MAX-AXIS_CENTER):float(e)/(AXIS_CENTER-AXIS_MIN);
 n=bound(n,-1,1);if(fabs(n)<.06f)n=0;else n=(n>0?1:-1)*(fabs(n)-.06f)/.94f;
 return int16_t(1000*n);
}
void setup(){
 Serial.begin(115200);pinMode(ARM_PIN,INPUT_PULLUP);pinMode(LED_PIN,OUTPUT);ledcAttach(BUZZER_PIN,2200,8);
 analogReadResolution(12);analogSetPinAttenuation(ROLL_ADC,ADC_11db);analogSetPinAttenuation(PITCH_ADC,ADC_11db);analogSetPinAttenuation(THROTTLE_ADC,ADC_11db);
 WiFi.mode(WIFI_STA);WiFi.setSleep(false);delay(500);Serial.print("TRANSMITTER MAC: ");Serial.println(WiFi.macAddress());
 if(!configured(AIR_MAC)){while(true){Serial.print("TRANSMITTER MAC: ");Serial.println(WiFi.macAddress());Serial.println("Set AIR_MAC in config.h, then rebuild.");delay(2000);}}
 esp_wifi_set_channel(CHANNEL,WIFI_SECOND_CHAN_NONE);
 if(esp_now_init()!=ESP_OK)while(true)delay(1000);
 esp_now_set_pmk((const uint8_t*)PMK);esp_now_peer_info_t peer={};memcpy(peer.peer_addr,AIR_MAC,6);memcpy(peer.lmk,LMK,16);peer.channel=CHANNEL;peer.encrypt=true;
 if(esp_now_add_peer(&peer)!=ESP_OK)while(true)delay(1000);
 esp_now_register_recv_cb(onReceive);sessionId=esp_random();
}
void loop(){
 uint32_t now=millis();if(now-lastSend<20)return;lastSend=now;
 float th=bound(float(analogRead(THROTTLE_ADC)-THR_MIN)/(THR_MAX-THR_MIN),0,1);
 if(THROTTLE_SIGN<0)th=1-th;
 ControlPacket p={MAGIC,sessionId,++seq,uint16_t(th*1000),int16_t(ROLL_SIGN*axis(ROLL_ADC)),int16_t(PITCH_SIGN*axis(PITCH_ADC)),uint8_t(digitalRead(ARM_PIN)==LOW),0};
 seal(p);esp_now_send(AIR_MAC,(uint8_t*)&p,sizeof(p));
 bool alive=lastStatus&&now-lastStatus<500;digitalWrite(LED_PIN,alive?(aircraftArmed?HIGH:(now/500)%2):LOW);
 ledcWrite(BUZZER_PIN,((low&&alive)||(!alive&&p.arm))&&((now/150)%2)?128:0);
 if(seq%25==0)Serial.printf("roll=%d pitch=%d throttle=%u arm=%u link=%d batt=%.2fV raw=%d,%d,%d\n",p.roll,p.pitch,p.throttle,p.arm,alive,battery*.001,analogRead(ROLL_ADC),analogRead(PITCH_ADC),analogRead(THROTTLE_ADC));
}
