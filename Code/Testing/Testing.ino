#include <algorithm>
#include <ESP32Servo.h>

int offset = 0;
int joystickPin = 15;

Servo ESC;
int ESCPin = 5;

void setup() {
  pinMode(joystickPin, INPUT);
  pinMode(0, INPUT);
  ESP32PWM::allocateTimer(0);
  Serial.begin(115200);
  ESC.setPeriodHertz(50);
  ESC.attach(ESCPin, 1000, 2000);
  ESC.writeMicroseconds(1500);
  // analogSetPinAttenuation(joystickPin, ADC_ATTENDB_MAX);
}


void loop() {
  int raw_val = analogRead(joystickPin);
  if (digitalRead(0) == LOW){
    offset = -raw_val+2048;
  }
  
  int val = std::clamp(raw_val+offset, 0, 4096);

  int PWM_val = map(val, 0, 4096, 1000, 2000);
  ESC.writeMicroseconds(PWM_val);
  Serial.println(val);
  Serial.println(PWM_val);
  
}
