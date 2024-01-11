#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <Debug.h>

int chan;

enum MessageType
{
  PAIRING,
  QUESTION,
  END_QUESTION,
  ANSWER,
  POWER_STATUS,
  UNPAIR,
};
MessageType messageType;

int counter = 0;

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

void getMAC(char *buf, const uint8_t *mac_addr)
{
  sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x",
          mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}

void printMAC(const uint8_t *mac_addr)
{
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  D_print(macStr);
}

void readMAC(const char *buf, uint8_t *mac_addr)
{
  sscanf(buf, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
         &mac_addr[0], &mac_addr[1], &mac_addr[2], &mac_addr[3], &mac_addr[4], &mac_addr[5]);
}

bool addPeer(uint8_t *peer_addr)
{
  if (esp_now_is_peer_exist(peer_addr))
  {
    D_println("Already Paired");
    return true;
  }
  else
  {
    if (esp_now_add_peer(peer_addr, ESP_NOW_ROLE_COMBO, chan, NULL, 0) == 0)
    {
      D_println("Pair success");
      return true;
    }
    else
    {
      D_print("Failed to add peer: ");
      printMAC(peer_addr);
      D_println("");
      return false;
    }
  }
}

bool removePeer(uint8_t *peer_addr)
{
  if (esp_now_is_peer_exist(peer_addr))
  {
    if (esp_now_del_peer(peer_addr) == 0)
    {
      D_println("Remove success");
      return true;
    }
    else
    {
      D_print("Failed to remove peer: ");
      printMAC(peer_addr);
      D_println("");
      return false;
    }
  }
  else
  {
    D_println("Peer does not exist");
    return true;
  }
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

  StaticJsonDocument<128> doc;
  uint8_t type = incomingData[0];
  switch (type)
  {
  case POWER_STATUS:
    struct power_status_message powerStatusData;
    memcpy(&powerStatusData, incomingData, sizeof(powerStatusData));
    if (powerStatusData.id == 0)
      return;

    // D_printf("power_status_message | id: %d | charging: %ld | answer: %d", answerData.id, answerData.timeToAnswer, answerData.answer);
    // D_println();

    doc["type"] = "power_status";
    doc["data"]["id"] = powerStatusData.id;
    doc["data"]["charging"] = powerStatusData.isCharging;
    doc["data"]["usbPower"] = powerStatusData.usbPowerConnected;
    doc["data"]["voltage"] = powerStatusData.batteryVoltage;
    serializeJson(doc, Serial);
    Serial.println();

    break;
    
  case ANSWER:
    struct answer_message answerData;
    memcpy(&answerData, incomingData, sizeof(answerData));
    if (answerData.id == 0)
      return;

    D_printf("answer_message | id: %d | timeToAnswer: %ld | answer: %d", answerData.id, answerData.timeToAnswer, answerData.answer);
    D_println();

    doc["type"] = "answer";
    doc["data"]["id"] = answerData.id;
    doc["data"]["timeToAnswer"] = answerData.timeToAnswer;
    doc["data"]["answer"] = answerData.answer;
    serializeJson(doc, Serial);
    Serial.println();

    break;

  case PAIRING:
    struct pairing_message pairingData;
    memcpy(&pairingData, incomingData, sizeof(pairingData));
    if (pairingData.id == 0)
      return;

    D_printf("pairing_message | id: %d | channel: %d\r\n", pairingData.id, pairingData.channel);
    D_print("Pairing request from: ");
    printMAC(mac_addr);
    D_println();

    char macStr[18];
    getMAC(macStr, mac_addr);

    doc["type"] = "pairing";
    doc["data"]["id"] = pairingData.id;
    doc["data"]["macaddr"] = macStr;
    serializeJson(doc, Serial);
    Serial.println();

    break;
  }
}

void initESP_NOW()
{
  if (esp_now_init() != 0)
  {
    D_println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_STA);
  chan = 10; // TODO: Bedenk ff iets logisch voor channel scanning.

  D_print("MAC Address:  ");
  D_println(WiFi.macAddress());

  initESP_NOW();
}

void loop()
{
  if (Serial.available())
  {
    StaticJsonDocument<128> doc;
    DeserializationError err = deserializeJson(doc, Serial);

    if (err == DeserializationError::Ok)
    {
      if (doc["type"] == "remove_pairing")
      {
        const char *macStr = doc["data"]["macaddr"];
        uint8_t mac_addr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        readMAC(macStr, mac_addr);

        struct unpair_message unpairingData;
        unpairingData.msgType = UNPAIR;
        unpairingData.id = 0;

        esp_now_send(mac_addr, (uint8_t *)&unpairingData, sizeof(unpairingData));
        removePeer(mac_addr);
      }
      else if (doc["type"] == "accept_pairing")
      {
        const char *macStr = doc["data"]["macaddr"];
        uint8_t mac_addr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        readMAC(macStr, mac_addr);

        struct pairing_message pairingData;
        pairingData.msgType = PAIRING;
        pairingData.id = 0;
        pairingData.channel = chan;
        WiFi.softAPmacAddress(pairingData.macAddr);

        esp_now_send(mac_addr, (uint8_t *)&pairingData, sizeof(pairingData));
        addPeer(mac_addr);
      }
      else if (doc["type"] == "new_question")
      {
        struct question_message questionData;
        questionData.msgType = QUESTION;
        questionData.id = 0;
        questionData.answerAmount = (uint8_t)doc["data"]["amount_answers"];

        esp_now_send(NULL, (uint8_t *)&questionData, sizeof(questionData));
      }
      else if (doc["type"] == "end_question")
      {
        struct end_question_message endQuestionData;
        endQuestionData.msgType = END_QUESTION;
        endQuestionData.id = 0;
        endQuestionData.answer = (uint8_t)doc["data"]["correct_answer"];

        esp_now_send(NULL, (uint8_t *)&endQuestionData, sizeof(endQuestionData));
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
}