#include <Arduino.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

#define BOARD_ID 1
#define MAX_CHANNEL 13

uint8_t serverAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

struct
{
  int deviceId = 1;
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

#ifdef SAVE_CHANNEL
int lastChannel;
#endif
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
  // ESP_ERROR_CHECK(esp_wifi_set_channel(chan, WIFI_SECOND_CHAN_NONE)); // TODO: wtf is this
  esp_now_del_peer(mac_addr);
  if (esp_now_add_peer(mac_addr, ESP_NOW_ROLE_COMBO, chan, NULL, 0) != 0)
  {
    Serial.println("Failed to add peer");
    return;
  }
  memcpy(serverAddress, mac_addr, sizeof(uint8_t[6]));
}

void printMAC(const uint8_t *mac_addr)
{
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}

void OnDataSent(uint8_t *mac_addr, uint8_t status)
{
  Serial.print("\r\nLast Packet Send Status:");
  Serial.println(status == 0 ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len)
{
  Serial.print("Packet received from: ");
  printMAC(mac_addr);
  Serial.println();
  Serial.print("data size = ");
  Serial.println(sizeof(incomingData));
  uint8_t type = incomingData[0];
  switch (type)
  {
  case DATA: // we received data from server
    memcpy(&inData, incomingData, sizeof(inData));
    Serial.print("ID  = ");
    Serial.println(inData.id);
    Serial.print("Setpoint temp = ");
    Serial.println(inData.temp);
    Serial.print("SetPoint humidity = ");
    Serial.println(inData.hum);
    Serial.print("reading Id  = ");
    Serial.println(inData.readingId);

    break;

  case PAIRING: // we received pairing data from server
    memcpy(&pairingData, incomingData, sizeof(pairingData));
    if (pairingData.id == 0)
    { // the message comes from server
      printMAC(mac_addr);
      Serial.print("Pairing done for ");
      printMAC(pairingData.macAddr);
      Serial.print(" on channel ");
      Serial.print(pairingData.channel); // channel used by the server
      Serial.print(" in ");
      Serial.print(millis() - start);
      Serial.println("ms");
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
    Serial.print("Pairing request on channel ");
    Serial.println(channel);

    // set pairing data to send to the server
    pairingData.msgType = PAIRING;
    pairingData.id = BOARD_ID;
    pairingData.channel = channel;

    // add peer and send request
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
    // nothing to do here
    break;
  }
  return pairingStatus;
}

void initESP_NOW()
{
  // set WiFi channel
  // ESP_ERROR_CHECK(esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE));
  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
  }

  // set callback routines
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  EEPROM.begin(sizeof(settings));
  EEPROM.get(0, settings);

  Serial.print("MAC Address:  ");
  Serial.println(WiFi.macAddress());
  WiFi.mode(WIFI_STA);
  start = millis();

  initESP_NOW();

  pairingStatus = PAIR_REQUEST;
}

void loop()
{
  if (autoPairing() == PAIR_PAIRED)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
      // Save the last time a new reading was published
      previousMillis = currentMillis;
      // Set values to send
      myData.msgType = DATA;
      myData.id = BOARD_ID;
      myData.temp = readDHTTemperature();
      myData.hum = readDHTHumidity();
      myData.readingId = readingId++;
      esp_now_send(serverAddress, (uint8_t *)&myData, sizeof(myData));
    }
  }
}