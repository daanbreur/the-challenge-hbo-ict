#include "Arduino.h"
#include "Pins.h"
#include "Bounce2.h"
#include "ledFunctions.h"

#include <ESP8266WiFi.h>
#define WIFI_MODE_STA WIFI_STA 
#include <QuickEspNow.h>

enum EspNowMessageType {
  STATUS_UPDATE, 
  BUTTON_STATE,
  TIME_SYNC
};

struct EspNowMessage{
  EspNowMessageType messageType;
  int deviceId;
  long timestamp = 0;
  float batteryVolt = -1;
  bool isCharging = false;
  bool usbPowerConnected = false;
  bool button1Pressed = false;
  bool button2Pressed = false;
  bool button3Pressed = false;
  bool button4Pressed = false;
};


Bounce2::Button buttons[4] = {Bounce2::Button(), Bounce2::Button(), Bounce2::Button(), Bounce2::Button()};

long lastMicros = 0;
long lastStatusUpdate = 0;
bool setRandomColor = true;
bool cancer = true;


long timeSync = 0;
long timeSyncLocal = 0; 
int timeSyncDeviation = 0;

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

void dataSent (uint8_t* address, uint8_t status) {
    Serial.printf ("Message sent to " MACSTR ", status: %d\n", MAC2STR (address), status);
    delay(1);
}

void dataReceived (uint8_t* address, uint8_t* data, uint8_t len, signed int rssi, bool broadcast) {
    Serial.print ("Received: ");
    Serial.printf ("%.*s\n", len, data);
    Serial.printf ("RSSI: %d dBm\n", rssi);
    Serial.printf ("From: " MACSTR "\n", MAC2STR (address));
    Serial.printf ("%s\n", broadcast ? "Broadcast" : "Unicast");

    String message = String((char*)data);
    long timestamp = message.toInt();

    if(timestamp > 0) {
      long millisPassed = millis() - timeSyncLocal;
      long expectedMillisPassed = timeSync + millisPassed;
      long actualMillis = timestamp;

      Serial.println("Millis Passed: " + String(millisPassed));
      Serial.println("Expected Millis Passed: " + String(expectedMillisPassed));
      Serial.println("Actual Millis: " + String(actualMillis));



      timeSyncLocal = millis();
      timeSync = timestamp;
    }
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

  WiFi.mode (WIFI_MODE_STA);
  WiFi.disconnect (false);
  quickEspNow.onDataSent(dataSent);
  quickEspNow.onDataRcvd(dataReceived);
  quickEspNow.begin (1);

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


    // EspNowMessage statusMessage = EspNowMessage();
    // statusMessage.messageType = EspNowMessageType::STATUS_UPDATE;
    // statusMessage.deviceId = 1;
    // statusMessage.batteryVolt = batteryVoltage();
    // statusMessage.isCharging = isCharging();
    // statusMessage.usbPowerConnected = usbPowerConnected();
    // String message = String(statusMessage.messageType) + "," + String(statusMessage.deviceId) + "," + String(statusMessage.batteryVolt) + "," + String(statusMessage.isCharging) + "," + String(statusMessage.usbPowerConnected);
    // quickEspNow.send (ESPNOW_BROADCAST_ADDRESS , (uint8_t*)message.c_str (), message.length ());

    lastStatusUpdate = millis();
  }

  handleTransitionStack();
  handleLedFadeStep();
  FastLED.show();

  int microsSinceLast = microsSinceLastRun();
  if(microsSinceLast < 1000) delayMicroseconds(1000 - microsSinceLast);
}