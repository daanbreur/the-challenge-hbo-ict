#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

struct
{
  int deviceId = 1;
} settings;

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
void printMAC(const uint8_t *mac_addr)
{
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}

bool addPeer(uint8_t *peer_addr)
{ // add pairing
  // check if the peer exists
  if (esp_now_is_peer_exist(peer_addr))
  {
    // Slave already paired.
    Serial.println("Already Paired");
    return true;
  }
  else
  {
    if (esp_now_add_peer(peer_addr, ESP_NOW_ROLE_COMBO, chan, NULL, 0))
    {
      // Pair success
      Serial.println("Pair success");
      return true;
    }
    else
    {
      Serial.println("Pair failed");
      return false;
    }
  }
}

// callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t status)
{
  Serial.print("Last Packet Send Status: ");
  Serial.print(status == 1 ? "Delivery Success to " : "Delivery Fail to ");
  printMAC(mac_addr);
  Serial.println();
}

// void OnDataRecv(uint8_t *mac_addr, const uint8_t *incomingData, uint8_t len)
void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len)
{
  Serial.print(len);
  Serial.print(" bytes of data received from : ");
  printMAC(mac_addr);
  Serial.println();
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
    Serial.println();
    break;

  case PAIRING: // the message is a pairing request
    memcpy(&pairingData, incomingData, sizeof(pairingData));
    Serial.println(pairingData.msgType);
    Serial.println(pairingData.id);
    Serial.print("Pairing request from: ");
    printMAC(mac_addr);
    Serial.println();
    Serial.println(pairingData.channel);
    if (pairingData.id > 0)
    { // do not replay to server itself
      if (pairingData.msgType == PAIRING)
      {
        pairingData.id = 0;
        pairingData.channel = chan;
        Serial.println("send response");
        esp_now_send(mac_addr, (uint8_t *)&pairingData, sizeof(pairingData));
        addPeer(mac_addr);
      }
    }
    break;
  }
}

void initESP_NOW()
{
  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
}

void setup()
{
  // Initialize Serial Monitor
  Serial.begin(115200);

  EEPROM.begin(sizeof(settings));
  EEPROM.get(0, settings);

  Serial.println();
  WiFi.mode(WIFI_STA);
  chan = 10; // TODO: Bedenk ff iets logisch voor channel scanning.

  Serial.print("MAC Address:  ");
  Serial.println(WiFi.macAddress());

  initESP_NOW();
}

void loop()
{
  static unsigned long lastEventTime = millis();
  static const unsigned long EVENT_INTERVAL_MS = 5000;
  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS)
  {
    lastEventTime = millis();
    readDataToSend();
    esp_now_send(NULL, (uint8_t *)&outgoingSetpoints, sizeof(outgoingSetpoints));
  }
}