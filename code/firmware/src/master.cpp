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
  DATA,
};
MessageType messageType;

int counter = 0;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message
{
  uint8_t msgType;
  uint8_t id;
  float temp;
  float hum;
  unsigned int readingId;
} struct_message;

typedef struct struct_pairing
{ // new structure for pairing
  uint8_t msgType;
  uint8_t id;
  uint8_t macAddr[6];
  uint8_t channel;
} struct_pairing;

struct_message incomingReadings;
struct_message outgoingSetpoints;
struct_pairing pairingData;

void readDataToSend()
{
  outgoingSetpoints.msgType = DATA;
  outgoingSetpoints.id = 0;
  outgoingSetpoints.temp = random(0, 40);
  outgoingSetpoints.hum = random(0, 100);
  outgoingSetpoints.readingId = counter++;
}

// ---------------------------- esp_ now -------------------------
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

void OnDataSent(uint8_t *mac_addr, uint8_t status)
{
  D_print("\r\nLast Packet Send Status: ");
  D_print(status == 0 ? "Delivery Success to " : "Delivery Fail to ");
  printMAC(mac_addr);
  D_println();
}

// void OnDataRecv(uint8_t *mac_addr, const uint8_t *incomingData, uint8_t len)
void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len)
{
  D_printf("\r\n --- \r\n%d bytes of data received from: ", len);
  printMAC(mac_addr);
  D_println();
  StaticJsonDocument<1000> root;
  String payload;
  uint8_t type = incomingData[0]; // first message byte is the type of message
  switch (type)
  {
  case DATA: // the message is data type
    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
    // create a JSON document with received data and send it by event to the web page
    root["id"] = incomingReadings.id;
    root["temperature"] = incomingReadings.temp;
    root["humidity"] = incomingReadings.hum;
    root["readingId"] = String(incomingReadings.readingId);
    // serializeJson(root, payload);
    Serial.print("event send :");
    serializeJson(root, Serial);
    // events.send(payload.c_str(), "new_readings", millis());
    D_println();
    break;

  case PAIRING: // the message is a pairing request
    memcpy(&pairingData, incomingData, sizeof(pairingData));
    D_printf("msgtype: %d | id: %d | channel: %d\r\n", pairingData.msgType, pairingData.id, pairingData.channel);
    D_print("Pairing request from: ");
    printMAC(mac_addr);
    D_println();
    if (pairingData.id > 0)
    { // do not replay to server itself
      if (pairingData.msgType == PAIRING)
      {
        char macStr[18];
        getMAC(macStr, mac_addr);

        StaticJsonDocument<128> doc;
        doc["type"] = "pairing";
        doc["data"]["id"] = pairingData.id;
        doc["data"]["macaddr"] = macStr;
        serializeJson(doc, Serial);
        D_println();
      }
    }
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

  D_println();
  WiFi.mode(WIFI_STA);
  chan = 10; // TODO: Bedenk ff iets logisch voor channel scanning.

  D_print("MAC Address:  ");
  D_println(WiFi.macAddress());

  initESP_NOW();
}

void loop()
{
  // static unsigned long lastEventTime = millis();
  // static const unsigned long EVENT_INTERVAL_MS = 5000;
  // if ((millis() - lastEventTime) > EVENT_INTERVAL_MS)
  // {
  //   lastEventTime = millis();
  //   readDataToSend();
  //   esp_now_send(NULL, (uint8_t *)&outgoingSetpoints, sizeof(outgoingSetpoints));
  // }

  if (Serial.available())
  {
    StaticJsonDocument<128> doc;
    DeserializationError err = deserializeJson(doc, Serial);

    if (err == DeserializationError::Ok)
    {
      if (doc["type"] == "accept_pairing")
      {
        const char *macStr = doc["data"]["macaddr"];
        uint8_t mac_addr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        readMAC(macStr, mac_addr);

        struct_pairing pairData;
        pairData.msgType = PAIRING;
        pairData.id = 0;
        pairData.channel = chan;
        WiFi.softAPmacAddress(pairData.macAddr);

        esp_now_send(mac_addr, (uint8_t *)&pairData, sizeof(pairData));
        addPeer(mac_addr);
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