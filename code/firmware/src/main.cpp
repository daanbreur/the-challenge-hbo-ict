#include "Arduino.h"
#include "Pins.h"
#include "FastLED.h"
#include "Bounce2.h"

#define NUM_LEDS 17

CRGB leds_buffer[NUM_LEDS];

//START VAR DECLARATIONS FADE EFFECT
CRGB leds_start[NUM_LEDS];
CRGB leds_target[NUM_LEDS];
int leds_fadeTime[NUM_LEDS];
int leds_fadeStep[NUM_LEDS];
int leds_fadeDelay[NUM_LEDS];
//END VAR DECLARATIONS FADE EFFECT

Bounce2::Button button1 = Bounce2::Button();
Bounce2::Button button2 = Bounce2::Button();
Bounce2::Button button3 = Bounce2::Button();
Bounce2::Button button4 = Bounce2::Button();

long lastMicros = 0;
bool setRandomColor = true;

/**
 * Sets the color of an LED at the specified index.
 * 
 * @param ledIndex The index of the LED.
 * @param color The color to set the LED to.
 * @param fadeTime The duration of the fade animation (optional, default is 0).
 * @param startDelay The delay before starting the fade animation (optional, default is 0).
 * @return True if the LED color was set immediately, false if a fade animation was started.
 */
bool setLedColor(int ledIndex, CRGB color, int fadeTime = 0, int startDelay = 0){
  if(fadeTime == 0) {
    leds_fadeStep[ledIndex] = 0;
    leds_fadeTime[ledIndex] = 0;
    leds_fadeDelay[ledIndex] = 0;

    leds_buffer[ledIndex] = color;
    return true;
  }

  leds_start[ledIndex] = leds_buffer[ledIndex];
  leds_target[ledIndex] = color;
  leds_fadeTime[ledIndex] = fadeTime;
  leds_fadeStep[ledIndex] = 0;
  leds_fadeDelay[ledIndex] = startDelay;

  return false;
}

/**
 * Sets the color of a block of LEDs.
 *
 * @param blockIndex The index of the block to set the color for.
 * @param color The color to set the LEDs to.
 * @param fadeTime The duration of the fade effect (optional, default is 0).
 * @param startDelay The delay before starting the fade effect (optional, default is 0).
 * @param transferDelay The delay between each LED in the block (optional, default is 0).
 * @return Returns false.
 */
bool setBlockColor(int blockIndex, CRGB color, int fadeTime = 0, int startDelay = 0, int transferDelay = 0) {
  if ((transferDelay || startDelay) && !fadeTime) fadeTime = 1;

  auto setBlockLeds = [&](int startLed, int numLeds) {
    for (int i = 0; i < numLeds; ++i) {
      setLedColor(startLed + i, color, fadeTime, startDelay + transferDelay * i);
    }
  };

  switch (blockIndex) {
    case 0: setLedColor(0, color, fadeTime, startDelay); break;
    case 1: setBlockLeds(1, 4); break;
    case 2: setBlockLeds(5, 4); break;
    case 3: setBlockLeds(9, 4); break;
    case 4: setBlockLeds(13, 4); break;
  }

  return false;
}

/**
 * Generates a random color using the CRGB library.
 * 
 * @return A random CRGB color.
 */
CRGB randomColor(){
  randomSeed(ESP.getCycleCount());
  return CRGB(random(0, 255), random(0, 255), random(0, 255));
}

void handleLedFadeStep() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (leds_fadeTime[i] > 0 && leds_fadeStep[i] < (leds_fadeTime[i] + 1)) {
      if (leds_fadeDelay[i] > 0) {
        leds_fadeDelay[i]--;
      } else {
        leds_buffer[i] = leds_start[i].lerp8(leds_target[i], leds_fadeStep[i] * 255 / leds_fadeTime[i]);
        if (++leds_fadeStep[i] > leds_fadeTime[i]) {
          leds_fadeTime[i] = 0;
          leds_fadeStep[i] = 0;
          leds_fadeDelay[i] = 0;
        }
      }
    }
  }
}

void updateButtons() {
  button1.update();
  button2.update();
  button3.update();
  button4.update();
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

  setLedColor(0, CRGB::OrangeRed);
  setLedColor(0, CRGB::Black, 100, 250);

}

void loop(){
  updateButtons();

  bool button1Pressed = button1.pressed();
  bool button2Pressed = button2.pressed();
  bool button3Pressed = button3.pressed();
  bool button4Pressed = button4.pressed();

  if (button1Pressed) {
    Serial.println("Button1: got pressed");
    
    if(!setRandomColor)setBlockColor(1, CRGB::White);
    if(setRandomColor) setBlockColor(1, randomColor());
  }

  if (button2Pressed) {
    Serial.println("Button2: got pressed");

    if(!setRandomColor)setBlockColor(2, CRGB::White);
    if(setRandomColor) setBlockColor(2, randomColor());
  }

  if (button3Pressed) {
    Serial.println("Button3: got pressed");

    if(!setRandomColor)setBlockColor(3, CRGB::White);
    if(setRandomColor) setBlockColor(3, randomColor());
  }


  if (button4Pressed) {
    Serial.println("Button4: got pressed");

    if(!setRandomColor)setBlockColor(4, CRGB::White);
    if(setRandomColor) setBlockColor(4, randomColor());
  }


  if(button1.released()) {
    setBlockColor(1, CRGB::Black, 100, 0, 0);
  }

  if(button2.released()) {
    setBlockColor(2, CRGB::Black, 100, 0, 0);
  }

  if(button3.released()) {
    setBlockColor(3, CRGB::Black, 100, 0, 0);
  }

  if(button4.released()) {
    setBlockColor(4, CRGB::Black, 100, 0, 0);
  }


  handleLedFadeStep();
  FastLED.show();
  
  if(microsSinceLastRun() < 10000) delayMicroseconds(10000 - microsSinceLastRun());
}