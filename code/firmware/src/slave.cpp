#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <Debug.h>

#include <Bounce2.h>
#include <ledFunctions.h>
#include <Pins.h>
#include <clickerUtils.h>

#define MAX_CHANNEL 13

uint8_t serverAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

Bounce2::Button buttons[] = {Bounce2::Button(), Bounce2::Button(), Bounce2::Button(), Bounce2::Button()};

struct
{
  uint8_t deviceId = 1;
} settings;

typedef struct struct_message
{
  uint8_t msgType;
  uint8_t id;
  float temp;
  float hum;
  unsigned int readingId;
} struct_message;

typedef struct struct_pairing
{
  uint8_t msgType;
  uint8_t id;
  uint8_t macAddr[6];
  uint8_t channel;
} struct_pairing;

struct_message myData;
struct_message inData;
struct_pairing pairingData;

enum PairingStatus
{
  NOT_PAIRED,
  PAIR_REQUEST,
  PAIR_REQUESTED,
  PAIR_PAIRED,
};
PairingStatus pairingStatus = NOT_PAIRED;

enum MessageType
{
  PAIRING,
  DATA,
};
MessageType messageType;

int channel = 1;

// simulate temperature and humidity data
float t = 0;
float h = 0;

unsigned long currentMillis = millis();
unsigned long previousMillis = 0; // Stores last time temperature was published
const long interval = 10000;      // Interval at which to publish sensor readings
unsigned long start;              // used to measure Pairing time
unsigned int readingId = 0;

// simulate temperature reading
float readDHTTemperature()
{
  t = random(0, 40);
  return t;
}

// simulate humidity reading
float readDHTHumidity()
{
  h = random(0, 100);
  return h;
}

void addPeer(uint8_t *mac_addr, uint8_t chan)
{
  esp_now_del_peer(mac_addr);
  if (esp_now_add_peer(mac_addr, ESP_NOW_ROLE_COMBO, chan, NULL, 0) != 0)
  {
    D_println("Failed to add peer");
    return;
  }
  memcpy(serverAddress, mac_addr, sizeof(uint8_t[6]));
}

void printMAC(const uint8_t *mac_addr)
{
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  D_print(macStr);
}

void OnDataSent(uint8_t *mac_addr, uint8_t status)
{
  D_print("\r\nLast Packet Send Status: ");
  D_print(status == 0 ? "Delivery Success to " : "Delivery Fail to ");
  printMAC(mac_addr);
  D_println();
}

void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len)
{
  D_print("Packet received from: ");
  printMAC(mac_addr);
  D_println();
  D_print("data size = ");
  D_println(sizeof(incomingData));
  uint8_t type = incomingData[0];
  switch (type)
  {
  case DATA: // we received data from server
    memcpy(&inData, incomingData, sizeof(inData));
    D_print("ID  = ");
    D_println(inData.id);
    D_print("Setpoint temp = ");
    D_println(inData.temp);
    D_print("SetPoint humidity = ");
    D_println(inData.hum);
    D_print("reading Id  = ");
    D_println(inData.readingId);

    break;

  case PAIRING: // we received pairing data from server
    memcpy(&pairingData, incomingData, sizeof(pairingData));
    if (pairingData.id == 0)
    { // the message comes from server
      printMAC(mac_addr);
      D_print("Pairing done for ");
      printMAC(pairingData.macAddr);
      D_print(" on channel ");
      D_print(pairingData.channel); // channel used by the server
      D_print(" in ");
      D_print(millis() - start);
      D_println("ms");
      addPeer(pairingData.macAddr, pairingData.channel); // add the server  to the peer list
      pairingStatus = PAIR_PAIRED;                       // set the pairing status
    }
    break;
  }
}

PairingStatus autoPairing()
{
  switch (pairingStatus)
  {
  case PAIR_REQUEST:
    D_print("Pairing request on channel ");
    D_println(channel);

    pairingData.msgType = PAIRING;
    pairingData.id = settings.deviceId;
    pairingData.channel = channel;

    addPeer(serverAddress, channel);
    esp_now_send(serverAddress, (uint8_t *)&pairingData, sizeof(pairingData));
    previousMillis = millis();
    pairingStatus = PAIR_REQUESTED;
    break;

  case PAIR_REQUESTED:
    // time out to allow receiving response from server
    currentMillis = millis();
    if (currentMillis - previousMillis > 1000)
    {
      previousMillis = currentMillis;
      // time out expired,  try next channel
      channel++;
      if (channel > MAX_CHANNEL)
      {
        channel = 1;
      }
      pairingStatus = PAIR_REQUEST;
    }
    break;

  case PAIR_PAIRED:
    break;
  }
  return pairingStatus;
}

void initESP_NOW()
{
  if (esp_now_init() != 0)
  {
    D_println("Error initializing ESP-NOW");
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
}

void setup()
{
  Serial.begin(115200);
  D_println();

  // Load settings from EEPROM

  EEPROM.begin(sizeof(settings));
  EEPROM.get(0, settings);

  // Setup pinmodes and buttons

  pinMode(CHARGE, INPUT);
  pinMode(STANDBY, INPUT);
  pinMode(BATTERY, INPUT);

  buttons[0].attach(BUTTON_1, INPUT);
  buttons[1].attach(BUTTON_2, INPUT);
  buttons[2].attach(BUTTON_3, INPUT);
  buttons[3].attach(BUTTON_4, INPUT);

  for (int i = 0; i < 4; i++)
  {
    buttons[i].interval(25);
    buttons[i].setPressedState(HIGH);
  }

  // Setup leds and play startup animation

  setupLeds();
  for (int i = 0; i < 5; i++)
  {
    addBlockTransitionToStack(i, CRGB::Red, 25, i * 5, 5);
    addBlockTransitionToStack(i, CRGB::Orange, 25, 0, 5);
    addBlockTransitionToStack(i, CRGB::Yellow, 25, 0, 5);
    addBlockTransitionToStack(i, CRGB::Green, 25, 0, 5);
    addBlockTransitionToStack(i, CRGB::Blue, 25, 0, 5);
    addBlockTransitionToStack(i, CRGB::Indigo, 25, 0, 5);
    addBlockTransitionToStack(i, CRGB::Black, 25, 0, 5);
  }

  // Startup networking and begin pairing process

  D_print("MAC Address:  ");
  D_println(WiFi.macAddress());
  WiFi.mode(WIFI_STA);
  start = millis();

  initESP_NOW();

  pairingStatus = PAIR_REQUEST;
}

void loop()
{
  for (int i = 0; i < 4; i++)
  {
    buttons[i].update();
  }

  for (int i = 0; i < 4; i++)
  {
    if (buttons[i].pressed())
    {
      setBlockColor(i + 1, randomColor());
    }
    else if (buttons[i].released())
    {
      setBlockColor(i + 1, CRGB::Black, 100, 0, 0);
    }
  }

  if (autoPairing() == PAIR_PAIRED)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
      // Save the last time a new reading was published
      previousMillis = currentMillis;
      // Set values to send
      myData.msgType = DATA;
      myData.id = settings.deviceId;
      myData.temp = readDHTTemperature();
      myData.hum = readDHTHumidity();
      myData.readingId = readingId++;
      esp_now_send(serverAddress, (uint8_t *)&myData, sizeof(myData));
    }
  }

  if (Serial.available())
  {
    StaticJsonDocument<128> doc;
    DeserializationError err = deserializeJson(doc, Serial);

    if (err == DeserializationError::Ok)
    {
      if (doc["type"] == "set_id")
      {
        uint8_t id = doc["data"]["id"];
        settings.deviceId = id;

        EEPROM.put(0, settings);
        EEPROM.commit();
      }
    }
    else
    {
      D_print("deserializeJson() returned ");
      D_println(err.c_str());
      while (Serial.available() > 0)
        Serial.read();
    }
  }

  handleTransitionStack();
  handleLedFadeStep();
  FastLED.show();

  // TODO: implement RTOS on this fucker
}