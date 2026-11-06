#include <algorithm>
#include <ESP32Servo.h>

int offset = 0;
int joystickPin = 15;
int servoPin = 18;

Servo servo;

void setup() {
  pinMode(joystickPin, INPUT);
  pinMode(0, INPUT);
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
  servo.setPeriodHertz(50);
  servo.attach(servoPin, 500, 2400);
  // analogSetPinAttenuation(joystickPin, ADC_ATTENDB_MAX);
}


void loop() {
  int raw_val = analogRead(joystickPin);
  if (digitalRead(0) == LOW){
    offset = -raw_val+2048;
  }
  
  int val = std::clamp(raw_val+offset, 0, 4096);

  int servo_val = map(val, 0, 4096, 0, 180);

  Serial.println(val);
  Serial.println(servo_val);
  servo.write(servo_val);
  
}
