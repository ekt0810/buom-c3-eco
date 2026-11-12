#pragma once
constexpr uint8_t AIR_MAC[6]={0,0,0,0,0,0};
constexpr int ROLL_ADC=32,PITCH_ADC=33,THROTTLE_ADC=34,ARM_PIN=25,LED_PIN=26,BUZZER_PIN=27;
// Replace these endpoints with Serial Monitor measurements for your pots.
constexpr int AXIS_MIN=100,AXIS_CENTER=2048,AXIS_MAX=3995,THR_MIN=100,THR_MAX=3995;
constexpr int ROLL_SIGN=1,PITCH_SIGN=1,THROTTLE_SIGN=1;
