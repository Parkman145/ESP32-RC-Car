#include <algorithm>
#include <ESP32Servo.h>

int joystickXPin = 15;
int joystickYPin = 4;
int servoPin = 18;
int ESCPin = 5;

int x_offset = 0;
int y_offset = 0;

Servo ESC;
Servo servo;

void setup() {
  Serial.begin(115200);
  pinMode(joystickXPin, INPUT);
  pinMode(joystickYPin, INPUT);
  pinMode(0, INPUT);
  
  ESP32PWM::allocateTimer(0);
  
  servo.setPeriodHertz(50);
  servo.attach(servoPin, 500, 2400);
  // analogSetPinAttenuation(joystickXPin, ADC_ATTENDB_MAX);

  ESC.setPeriodHertz(50);
  ESC.attach(ESCPin, 1000, 2000);
  ESC.writeMicroseconds(1500);
}


void loop() {
  int raw_x_val = analogRead(joystickXPin);
  int raw_y_val = analogRead(joystickYPin);
  if (digitalRead(0) == LOW){
    x_offset = -raw_x_val+2048;
    y_offset = -raw_y_val+2048;
  }

  int x_val = std::clamp(raw_x_val+x_offset, 0, 4096);
  int PWM_val = map(x_val, 0, 4096, 1000, 2000);
  ESC.writeMicroseconds(PWM_val);
  

  int y_val = std::clamp(raw_y_val+y_offset, 0, 4096);
  int servo_val = map(y_val, 0, 4096, 0, 180);
  servo.write(servo_val);

  Serial.println(PWM_val);
  Serial.println(servo_val);
  
}
