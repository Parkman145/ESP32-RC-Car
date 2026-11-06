#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>


int servoPin = 13;
int ESCPin = 12;

struct JoystickState {
  int x;
  int y;
} joystick_state;

Servo ESC;
Servo servo;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&joystick_state, incomingData, sizeof(joystick_state));

}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  ESP32PWM::allocateTimer(0);
  
  servo.setPeriodHertz(50);
  servo.attach(servoPin, 500, 2400);
  // analogSetPinAttenuation(joystickXPin, ADC_ATTENDB_MAX);

  ESC.setPeriodHertz(50);
  ESC.attach(ESCPin, 1000, 2000);
  ESC.writeMicroseconds(1500);
}

void loop() {
int PWM_val = map(joystick_state.x, 0, 4096, 1000, 2000);
  ESC.writeMicroseconds(PWM_val);
  

  int servo_val = map(joystick_state.y, 0, 4096, 0, 180);
  servo.write(servo_val);
  Serial.print("Joystick X: ");
  Serial.println(joystick_state.x);
  Serial.print("PWM_val: ");
  Serial.println(PWM_val);
  Serial.println("Joystick Y: ");
  Serial.println(joystick_state.y);

  

}
