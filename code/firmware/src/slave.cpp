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

struct answer_message
{
  uint8_t msgType;
  uint8_t id;
  unsigned long timeToAnswer;
  uint8_t answer;
};

struct question_message
{
  uint8_t msgType;
  uint8_t id;
  uint8_t answerAmount;
};

struct end_question_message
{
  uint8_t msgType;
  uint8_t id;
  uint8_t answer;
};

struct pairing_message
{
  uint8_t msgType;
  uint8_t id;
  uint8_t macAddr[6];
  uint8_t channel;
};

struct power_status_message
{
  uint8_t msgType;
  uint8_t id;
  bool isCharging;
  bool usbPowerConnected;
  float batteryVoltage;
};

struct unpair_message
{
  uint8_t msgType;
  uint8_t id;
  uint8_t macAddr[6];
};

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
  QUESTION,
  END_QUESTION,
  ANSWER,
  POWER_STATUS,
  UNPAIR,
};

int channel = 1;

bool canAnswer = true; // TODO: default false
bool activeButtons[4] = {false, false, false, false};

unsigned long lastStatusUpdate = 0;
unsigned long currentMillis = millis();
unsigned long previousMillis = 0;      // Stores last time temperature was published
unsigned long lastPowerStatusUpdate = 0;
unsigned long startQuestionMillis = 0; // Stores start time of question for scoring reasons
const long interval = 10000;           // Interval at which to publish sensor readings
unsigned long start;                   // used to measure Pairing time
uint8_t lastPressedAnswer = 0;             // used to store last pressed answer

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
  D_printf("\r\n --- \r\n%d bytes of data received from: ", len);
  printMAC(mac_addr);
  D_println();

  uint8_t type = incomingData[0];
  switch (type)
  {
  case UNPAIR:
    struct unpair_message unpairData;
    memcpy(&unpairData, incomingData, sizeof(unpairData));
    if (unpairData.id != 0)
      return;

    if (pairingStatus != PAIR_PAIRED)
      return;

    //reset esp device
    ESP.restart();
    
    break;
  case END_QUESTION:
    struct end_question_message endQuestionData;
    memcpy(&endQuestionData, incomingData, sizeof(endQuestionData));
    if (endQuestionData.id != 0)
      return;

    if (pairingStatus != PAIR_PAIRED)
      return;

    D_printf("Device ID: %d | Correct answer: %d", endQuestionData.id, endQuestionData.answer);
    D_println();

    canAnswer = false;
    for (int j = 0; j < 4; j++)
    {
      activeButtons[j] = false;
      if (lastPressedAnswer != endQuestionData.answer) {
        setBlockColor(j + 1, CRGB::Red, 100, 0, 0);
      } else {
        setBlockColor(j + 1, CRGB::Green, 100, 0, 0);
      }
    }

    lastPressedAnswer = 0;    
    break;

  case QUESTION:
    struct question_message questionData;
    memcpy(&questionData, incomingData, sizeof(questionData));
    if (questionData.id != 0)
      return;

    if (pairingStatus != PAIR_PAIRED)
      return;

    D_printf("Device ID: %d | Amount of buttons: %d", questionData.id, questionData.answerAmount);
    D_println();

    canAnswer = false;
    lastPressedAnswer = 0;
    for (int j = 0; j < 4; j++)
    {
      activeButtons[j] = false;
      setBlockColor(j + 1, CRGB::Black, 100, 0, 0);
    }

    if (questionData.answerAmount >= 1)
    {
      canAnswer = true;
      activeButtons[0] = true;
      setBlockColor(1, CRGB::Cyan, 100, 0, 0);
    }
    if (questionData.answerAmount >= 2)
    {
      activeButtons[1] = true;
      setBlockColor(2, CRGB::Magenta, 100, 0, 0);
    }
    if (questionData.answerAmount >= 3)
    {
      activeButtons[2] = true;
      setBlockColor(3, CRGB::Yellow, 100, 0, 0);
    }
    if (questionData.answerAmount >= 4)
    {
      activeButtons[3] = true;
      setBlockColor(4, CRGB(25, 255, 25), 100, 0, 0);
    }

    startQuestionMillis = millis();
    break;

  case PAIRING: // we received pairing data from server
    struct pairing_message pairingData;
    memcpy(&pairingData, incomingData, sizeof(pairingData));
    if (pairingData.id != 0)
      return;

    D_print("Pairing done for ");
    printMAC(mac_addr);
    D_print(" on channel ");
    D_print(pairingData.channel); // channel used by the server
    D_print(" in ");
    D_print(millis() - start);
    D_println("ms");
    esp_now_del_peer(serverAddress);
    addPeer(mac_addr, pairingData.channel); // add the server  to the peer list
    pairingStatus = PAIR_PAIRED;            // set the pairing status
    break;
  }
}

PairingStatus autoPairing()
{
  switch (pairingStatus)
  {
  case NOT_PAIRED:
  case PAIR_REQUEST:
    struct pairing_message pairingData;
    D_print("Pairing request on channel ");
    D_println(channel);

    pairingData.msgType = PAIRING;
    pairingData.id = settings.deviceId;
    pairingData.channel = channel;
    WiFi.softAPmacAddress(pairingData.macAddr);

    addPeer(serverAddress, channel);
    esp_now_send(serverAddress, (uint8_t *)&pairingData, sizeof(pairingData));
    previousMillis = millis();
    pairingStatus = PAIR_REQUESTED;
    break;

  case PAIR_REQUESTED:
    currentMillis = millis();
    if (currentMillis - previousMillis > 1000)
    {
      previousMillis = currentMillis;
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
  Serial.println();

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
  if (millis() - lastStatusUpdate > 1000)
  {
    if (pairingStatus == PAIR_REQUEST || pairingStatus == PAIR_REQUESTED)
    {
      addBlockTransitionToStack(0, CRGB::Blue, 100, 0, 0);
      addBlockTransitionToStack(0, CRGB::Black, 100, 0, 0);
    }

    if (usbPowerConnected() == true && isCharging() == false)
    {
      D_println("powerstatus | USB Power Connected");
      addBlockTransitionToStack(0, CRGB::Green, 100, 0, 0);
      addBlockTransitionToStack(0, CRGB::Black, 100, 0, 0);
    }
    else if (usbPowerConnected() == true && isCharging() == true)
    {
      D_println("powerstatus | Charging");
      addBlockTransitionToStack(0, CRGB::Red, 100, 0, 0);
      addBlockTransitionToStack(0, CRGB::Black, 100, 0, 0);
    }

    if (usbPowerConnected() == false)
    {
      D_println("powerstatus | USB Power Disconnected");
      addBlockTransitionToStack(0, CRGB::Green, 100, 0, 0);
    }
  }

  for (int i = 0; i < 4; i++)
  {
    buttons[i].update();
  }

  if (autoPairing() == PAIR_PAIRED)
  {
    unsigned long currentMillis = millis();

    if (currentMillis - lastPowerStatusUpdate > 10000) {
      struct power_status_message powerStatusData;
      powerStatusData.msgType = POWER_STATUS;
      powerStatusData.id = settings.deviceId;
      powerStatusData.isCharging = isCharging();
      powerStatusData.usbPowerConnected = usbPowerConnected();
      powerStatusData.batteryVoltage = batteryVoltage();

      esp_now_send(NULL, (uint8_t *)&powerStatusData, sizeof(powerStatusData));
      lastPowerStatusUpdate = currentMillis;
    }

    for (int i = 0; i < 4; i++)
    {
      if (buttons[i].pressed() && canAnswer && activeButtons[i])
      {
        canAnswer = false;
        for (int j = 0; j < 4; j++)
        {
          activeButtons[j] = false;
          setBlockColor(j + 1, CRGB::Black, 100, 0, 0);
        }

        unsigned long timeToAnswer = currentMillis - startQuestionMillis;

        struct answer_message answerData;
        answerData.msgType = ANSWER;
        answerData.id = settings.deviceId;
        answerData.timeToAnswer = timeToAnswer;
        answerData.answer = (uint8_t)(i + 1);
        lastPressedAnswer = (uint8_t)(i + 1);

        esp_now_send(NULL, (uint8_t *)&answerData, sizeof(answerData));
      }
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