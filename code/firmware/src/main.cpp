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

void checkButtons() {
  return;
}

void updateButtons() {
  button1.update();
  button2.update();
  button3.update();
  button4.update();
}

void setBlock(int ledId, int R, int G, int B)
{
  switch (ledId)
  {
  case 0:
    leds_buffer[0] = CRGB(R, G, B);
    break;

  case 1:
    leds_buffer[1] = CRGB(R, G, B);
    leds_buffer[2] = CRGB(R, G, B);
    leds_buffer[3] = CRGB(R, G, B);
    leds_buffer[4] = CRGB(R, G, B);
    break;

  case 2:
    leds_buffer[5] = CRGB(R, G, B);
    leds_buffer[6] = CRGB(R, G, B);
    leds_buffer[7] = CRGB(R, G, B);
    leds_buffer[8] = CRGB(R, G, B);
    break;

  case 3:
    leds_buffer[9] = CRGB(R, G, B);
    leds_buffer[10] = CRGB(R, G, B);
    leds_buffer[11] = CRGB(R, G, B);
    leds_buffer[12] = CRGB(R, G, B);
    break;

  case 4:
    leds_buffer[13] = CRGB(R, G, B);
    leds_buffer[14] = CRGB(R, G, B);
    leds_buffer[15] = CRGB(R, G, B);
    leds_buffer[16] = CRGB(R, G, B);
    break;
  }
  FastLED.show();
}

void setLed(int ledId, int R, int G, int B)
{
  leds_buffer[ledId] = CRGB(R, G, B);
  FastLED.show();
}

void setAllButton(int R, int G, int B) {
  setBlock(1, R, G, B);
  setBlock(2, R, G, B);
  setBlock(3, R, G, B);
  setBlock(4, R, G, B);
}

void fadeLed(int ledId, int R, int G, int B, int fadeTime) {
  int leds[] = {0, 13, 14, 15, 16, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  int ledId = leds[ledId];
  CRGB led = leds_buffer[ledId];
  int fadeSteps = fadeTime / 10;
  int fadeR = (R - led.r) / fadeSteps;
  int fadeG = (G - led.g) / fadeSteps;
  int fadeB = (B - led.b) / fadeSteps;

  for(int i = 0; i < fadeSteps; i++) {
    led.r += fadeR;
    led.g += fadeG;
    led.b += fadeB;
    setLed(ledId, led.r, led.g, led.b);
    delay(10);
  }
}

void fadeAllLed(int R, int G, int B, int fadeTime) {
  for(int i = 0; i < NUM_LEDS; i++) {
    fadeLed(i, R, G, B, fadeTime);
  }
}

void ledStartup() {
  setAllButton(0, 0, 0);
  delay(10);
  //fade all leds in out
  int fadeTime = 100;
  fadeAllLed(255, 255, 255, fadeTime);
  fadeAllLed(255, 0, 0, fadeTime);
  fadeAllLed(0, 255, 0, fadeTime);
  fadeAllLed(0, 0, 255, fadeTime);
  fadeAllLed(255, 255, 255, fadeTime);
  fadeAllLed(0, 0, 0, fadeTime);
}

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
    setBlock(1, 255, 255, 255);
  }

  if (button2.pressed()) {
    Serial.println("Button2: got pressed");
    setBlock(2, 255, 255, 255);
  }

  if (button3.pressed()) {
    Serial.println("Button3: got pressed");
    setBlock(3, 255, 255, 255);
  }


  if (button4.pressed()) {
    Serial.println("Button4: got pressed");
    setBlock(4, 255, 255, 255);
  }



  if(button1.released()) {
    setBlock(1, 0, 0, 0);
  }

  if(button2.released()) {
    setBlock(2, 0, 0, 0);
  }

  if(button3.released()) {
    setBlock(3, 0, 0, 0);
  }

  if(button4.released()) {
    setBlock(4, 0, 0, 0);
  }


}