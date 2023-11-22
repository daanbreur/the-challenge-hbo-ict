#include "Arduino.h"
#include "Pins.h"
#include "FastLED.h"
#include "Bounce2.h"

#ifndef NUM_LEDS
#define NUM_LEDS 17
#endif

CRGB leds_buffer[NUM_LEDS];
Bounce2::Button button1 = Bounce2::Button();
Bounce2::Button button2 = Bounce2::Button();
Bounce2::Button button3 = Bounce2::Button();
Bounce2::Button button4 = Bounce2::Button();

void setup()
{
  FastLED.addLeds<WS2812B, LEDS_DATA, RGB>(leds_buffer, NUM_LEDS);
  button1.attach(BUTTON_1, INPUT);
  button2.attach(BUTTON_2, INPUT);
  button3.attach(BUTTON_3, INPUT);
  button4.attach(BUTTON_4, INPUT);

  button1.interval(25);
  button2.interval(25);
  button3.interval(25);
  button4.interval(25);

  button1.setPressedState(HIGH);
  button2.setPressedState(HIGH);
  button3.setPressedState(HIGH);
  button4.setPressedState(HIGH);

  ledStartup();
}

void loop()
{
  updateButtons();

  if (button1.pressed()) {
    Serial.println("Button1: got pressed");
    setLed(1, 255, 255, 255);
  }

  if (button2.pressed()) {
    Serial.println("Button2: got pressed");
    setLed(2, 255, 255, 255);
  }

  if (button3.pressed()) {
    Serial.println("Button3: got pressed");
    setLed(3, 255, 255, 255);
  }


  if (button4.pressed()) {
    Serial.println("Button4: got pressed");
    setLed(4, 255, 255, 255);
  }



  if(button1.released()) {
    setLed(1, 0, 0, 0);
  }

  if(button2.released()) {
    setLed(2, 0, 0, 0);
  }

  if(button3.released()) {
    setLed(3, 0, 0, 0);
  }

  if(button4.released()) {
    setLed(4, 0, 0, 0);
  }


}

void checkButtons() {

}


void updateButtons() {
  button1.update();
  button2.update();
  button3.update();
  button4.update();
}

enum LED_ID
{
  STATUS = 0,
  A = 1,
  B = 2,
  C = 3,
  D = 4
};

void ledStartup() {
  setAllButton(0, 0, 0);
  delay(10);
  setAllButton(255, 255, 255);
  delay(250);
  setAllButton(0, 0, 0);
}

void setLed(int _ledId, int R, int G, int B)
{
  LED_ID ledId = static_cast<LED_ID>(_ledId);
  setLed(ledId, R, G, B);
}

void setAllButton(int R, int G, int B) {
    setLed(1, R, G, B);
    setLed(2, R, G, B);
    setLed(3, R, G, B);
    setLed(4, R, G, B);
}

void setLed(LED_ID ledId, int R, int G, int B)
{
  switch (ledId)
  {
  case LED_ID::STATUS:
    leds_buffer[0] = CRGB(R, G, B);
    break;

  case LED_ID::A:
    leds_buffer[1] = CRGB(R, G, B);
    leds_buffer[2] = CRGB(R, G, B);
    leds_buffer[3] = CRGB(R, G, B);
    leds_buffer[4] = CRGB(R, G, B);
    break;

  case LED_ID::B:
    leds_buffer[5] = CRGB(R, G, B);
    leds_buffer[6] = CRGB(R, G, B);
    leds_buffer[7] = CRGB(R, G, B);
    leds_buffer[8] = CRGB(R, G, B);
    break;

  case LED_ID::C:
    leds_buffer[9] = CRGB(R, G, B);
    leds_buffer[10] = CRGB(R, G, B);
    leds_buffer[11] = CRGB(R, G, B);
    leds_buffer[12] = CRGB(R, G, B);
    break;

  case LED_ID::D:
    leds_buffer[13] = CRGB(R, G, B);
    leds_buffer[14] = CRGB(R, G, B);
    leds_buffer[15] = CRGB(R, G, B);
    leds_buffer[16] = CRGB(R, G, B);
    break;
  }
  FastLED.show();
}