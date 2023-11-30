#include "Arduino.h"
#include "Pins.h"
#include "Globals.h"
#include "Bounce2.h"
#include "ledFunctions.h"

#include <espnow.h>
#include <ESP8266WiFi.h>

uint8_t broadcastAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;
struct_message myData;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(sendStatus == 1 ? "Delivery Success" : "Delivery Fail");
}

Bounce2::Button buttons[4] = {Bounce2::Button(), Bounce2::Button(), Bounce2::Button(), Bounce2::Button()};

long lastMicros = 0;
long lastStatusUpdate = 0;
bool setRandomColor = true;

bool usbPowerConnected() {
  return digitalRead(CHARGE) == LOW || digitalRead(STANDBY) == LOW;
}
bool isCharging() {
  return digitalRead(CHARGE) == LOW;
}
float batteryVoltage() {
  float voltage = analogRead(BATTERY);
  return (voltage * 0.00416918) + 0.0;
}
bool powerSwitchOn() {
  return batteryVoltage() < 1;
}


void updateButtons() {
  for(int i = 0; i < 4; i++) {
    buttons[i].update();
  }
}

/**
 * Calculates the time elapsed in microseconds since the last run of the function.
 * 
 * @return The time elapsed in microseconds.
 */
int microsSinceLastRun() {
  long currentNano = micros();
  int diff = (currentNano - lastMicros + 4294967295) % 4294967296;
  lastMicros = currentNano;
  return diff;
}

void setup(){
  Serial.begin(115200);

  setupLeds();

  pinMode(CHARGE, INPUT);
  pinMode(STANDBY, INPUT);
  pinMode(BATTERY, INPUT);

  for(int i = 0; i < 4; i++) {
    if(i == 0) buttons[i].attach(BUTTON_1, INPUT);
    if(i == 1) buttons[i].attach(BUTTON_2, INPUT);
    if(i == 2) buttons[i].attach(BUTTON_3, INPUT);
    if(i == 3) buttons[i].attach(BUTTON_4, INPUT);

    buttons[i].interval(25);
    buttons[i].setPressedState(HIGH);
  }

  for(int i = 0; i < 5; i++) {
    addBlockTransitionToStack(i, CRGB::Red, 25, i * 5, 5);
    addBlockTransitionToStack(i, CRGB::Orange, 25, 0, 5);
    addBlockTransitionToStack(i, CRGB::Yellow, 25, 0, 5);
    addBlockTransitionToStack(i, CRGB::Green, 25, 0, 5);
    addBlockTransitionToStack(i, CRGB::Blue, 25, 0, 5);
    addBlockTransitionToStack(i, CRGB::Indigo, 25, 0, 5);
    addBlockTransitionToStack(i, CRGB::Black, 25, 0, 5);
  }

  // Serial.println(getTransitionStackLength());
  // Serial.println("heyy");
  // Serial.println(getTransitionStackLength());
  Serial.println("Device MAC: " + WiFi.macAddress());

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);

  if (esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0)){ 
    Serial.println("Failed to add peer");
    return;
  } else {
    Serial.println("Added peer?");
  }
}

void loop(){
  updateButtons();

  for(int i = 0; i < 4; i++) {
    if(buttons[i].pressed()) {
      if(!setRandomColor)setBlockColor(i + 1, CRGB::White);
      if(setRandomColor) setBlockColor(i + 1, randomColor());
    } else if (buttons[i].released()) {
      setBlockColor(i + 1, CRGB::Black, 100, 0, 0);
    }
  }

  if(usbPowerConnected() == true && isCharging() == false) {
    setBlockColor(0, CRGB::Green);
  }

  if(millis() - lastStatusUpdate > 1000) {
    Serial.print("Battery: ");
    Serial.print(batteryVoltage());
    Serial.print(" | Charge: ");
    Serial.print(isCharging());
    Serial.print(" | 1: ");
        Serial.print(digitalRead(CHANGE));
    Serial.print(" | 2: ");
        Serial.print(digitalRead(STANDBY));
    Serial.print(" | USB: ");
    Serial.println(usbPowerConnected());

    if(usbPowerConnected() == true && isCharging() == false) {
      Serial.println("USB Power Connected");
      addLedTransitionToStack(0, CRGB(0, 10, 0), 100, 0);
      addLedTransitionToStack(0, CRGB::Black, 100, 0);
    } else if (usbPowerConnected() == true && isCharging() == true) {
      Serial.println("Charging");
      addLedTransitionToStack(0, CRGB(10, 0, 0), 100, 0);
      addLedTransitionToStack(0, CRGB::Black, 100, 0);
    } else if (usbPowerConnected() == false) {
      Serial.println("USB Power Disconnected");
      setLedColor(0, CRGB(0, 10, 0), 100, 0);
    }

    lastStatusUpdate = millis();
  }

  handleTransitionStack();
  handleLedFadeStep();
  FastLED.show();

  int microsSinceLast = microsSinceLastRun();
  if(microsSinceLast < 1000) delayMicroseconds(1000 - microsSinceLast);
}
