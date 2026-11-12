#pragma once
// Fill MACs after flashing once and reading Serial Monitor at 115200.
constexpr uint8_t TX_MAC[6]={0,0,0,0,0,0};
constexpr bool RELEASE_ACTUATORS=false; // true after endpoint/power bench checks.
constexpr int LEFT_PIN=3,RIGHT_PIN=5,VBAT_PIN=4,POWER_PIN=10;
constexpr float VBAT_CAL=1.0f;
constexpr int SERVO_HZ=50; // Change only if specified for the actual servo.
constexpr float CENTER_L=1500,CENTER_R=1500,US_PER_DEG=1000.0f/90.0f;
constexpr float MIN_US=1100,MAX_US=1900,AMPLITUDE=25;
constexpr int SIGN_L=1,SIGN_R=-1;
constexpr int ROLL_SIGN=1,PITCH_SIGN=1;
constexpr bool USE_POWER_SWITCH=true; // Carrier: true. Direct perfboard rail: false.
