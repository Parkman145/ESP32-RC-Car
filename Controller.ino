#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0x68, 0x09, 0x47, 0x5f, 0xc2, 0x2c};

int joystickXPin = 32;
int joystickYPin = 33;

int x_offset = 0;
int y_offset = 0;

esp_now_peer_info_t peerInfo;

struct JoystickState {
  int x;
  int y;
} joystick_state;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  pinMode(joystickXPin, INPUT);
  pinMode(joystickYPin, INPUT);
  pinMode(0, INPUT_PULLUP);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));

  //Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  //Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }



}

void loop() {
  int raw_x_val = analogRead(joystickXPin);
  int raw_y_val = analogRead(joystickYPin);
  if (digitalRead(0) == LOW){
    x_offset = -raw_x_val+2048;
    y_offset = -raw_y_val+2048;
  }

  int x_val = std::clamp(raw_x_val+x_offset, 0, 4096);
  int y_val = std::clamp(raw_y_val+y_offset, 0, 4096);

  joystick_state.x = x_val;
  joystick_state.y = y_val;


  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &joystick_state, sizeof(joystick_state));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  Serial.print("Joystick X: ");
  Serial.println(joystick_state.x);
  Serial.println("Joystick Y: ");
  Serial.println(joystick_state.y);
  


}

