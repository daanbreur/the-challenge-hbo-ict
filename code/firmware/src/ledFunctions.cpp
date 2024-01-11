#include "Arduino.h"
#include "Pins.h"
#include "Globals.h"
#include "FastLED.h"

CRGB leds_buffer[NUM_LEDS];
CRGB leds_start[NUM_LEDS];
CRGB leds_target[NUM_LEDS];
int leds_fadeTime[NUM_LEDS];
int leds_fadeStep[NUM_LEDS];
int leds_fadeDelay[NUM_LEDS];

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
    #ifdef CCLICKER
    case 0: setLedColor(0, color, fadeTime, startDelay); break;
    case 1: setBlockLeds(1, 4); break;
    case 2: setBlockLeds(5, 4); break;
    case 3: setBlockLeds(9, 4); break;
    case 4: setBlockLeds(13, 4); break;
    #endif
    #ifdef CCLICKERV2
    case 0: setLedColor(2, color, fadeTime, startDelay); break;
    case 1: setBlockLeds(0, 2); break;
    case 2: setBlockLeds(3, 2); break;
    case 3: setBlockLeds(5, 2); break;
    case 4: setBlockLeds(7, 2); break;
    #endif
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

/**
 * Checks if an LED has an ongoing transition.
 *
 * @param ledIndex The index of the LED to check.
 * @return True if the LED has a transition in progress, false otherwise.
 */
bool ledHasTransition(int ledIndex) {
  return leds_fadeTime[ledIndex] > 0;
}

/**
 * @brief Initializes the LEDs by adding the LED strip and setting the brightness.
 * 
 * This function adds the LED strip using the FastLED library and sets the brightness to the maximum value (255).
 * 
 * @param None
 * @return None
 */
void setupLeds(){
  FastLED.addLeds<WS2812B, LEDS_DATA, RGB>(leds_buffer, NUM_LEDS);
  FastLED.setBrightness(128);
}

class ledTransition { 
  public:             
    int ledIndex;        
    CRGB color;  
    int fadeTime;
    int startDelay;
    bool isRunning = false;
    bool completed = false;

    ledTransition(int ledIndex = -1, CRGB color = CRGB::Black, int fadeTime = 0, int startDelay = 0, bool completed = false, bool isRunning = false) {
      this->ledIndex = ledIndex;
      this->color = color;
      this->fadeTime = fadeTime;
      this->startDelay = startDelay;
      this->completed = false;
      this->isRunning = false;
    }

    void execute() {
      setLedColor(ledIndex, color, fadeTime, startDelay);
      isRunning = true;
    }

    void complete() {
      completed = true;
    }
};

ledTransition ledTransitions[1024];

/**
 * @brief Adds a new LED transition to the stack.
 * 
 * This function adds a new LED transition to the stack. The stack is a queue of LED transitions that are executed in order.
 * 
 * @param ledIndex The index of the LED to transition.
 * @param color The color to transition to.
 * @param fadeTime The duration of the fade animation (optional, default is 0).
 * @param startDelay The delay before starting the fade animation (optional, default is 0).
 * @return True if the LED transition was added to the stack, false if the stack is full.
 */
bool addLedTransitionToStack(int ledIndex, CRGB color, int fadeTime = 0, int startDelay = 0){
  for(int i = 0; i < 1024; i++) {
    if(ledTransitions[i].ledIndex == -1) {
      ledTransitions[i] = ledTransition(ledIndex, color, fadeTime, startDelay);
      return true;
    }
  }

  return false;
}

/**
 * Adds a block transition to the LED stack.
 *
 * @param blockIndex The index of the block to add the transition to.
 * @param color The color of the transition.
 * @param fadeTime The duration of the fade for each LED in the block (default: 0).
 * @param startDelay The delay before starting the transition (default: 0).
 * @param transferDelay The delay between each LED in the block (default: 0).
 * @return Always returns false.
 */
bool addBlockTransitionToStack(int blockIndex, CRGB color, int fadeTime = 0, int startDelay = 0, int transferDelay = 0) {
  if ((transferDelay || startDelay) && !fadeTime) fadeTime = 1;

  auto addBlockLeds = [&](int startLed, int numLeds) {
    for (int i = 0; i < numLeds; ++i) {
      addLedTransitionToStack(startLed + i, color, fadeTime, startDelay + transferDelay * i);
    }
  };

  switch (blockIndex) {
    #ifdef CCLICKER
    case 0: addLedTransitionToStack(0, color, fadeTime, startDelay); break;
    case 1: addBlockLeds(1, 4); break;
    case 2: addBlockLeds(5, 4); break;
    case 3: addBlockLeds(9, 4); break;
    case 4: addBlockLeds(13, 4); break;
    #endif
    #ifdef CCLICKERV2
    case 0: addLedTransitionToStack(2, color, fadeTime, startDelay); break;
    case 1: addBlockLeds(0, 2); break;
    case 2: addBlockLeds(3, 2); break;
    case 3: addBlockLeds(5, 2); break;
    case 4: addBlockLeds(7, 2); break;
    #endif
  }

  return false;
}

void handleTransitionStack() {
  for(int i = 0; i < 1024; i++) {
    if(ledTransitions[i].ledIndex != -1 && !ledTransitions[i].completed && !ledTransitions[i].isRunning && !ledHasTransition(ledTransitions[i].ledIndex)) {
      ledTransitions[i].execute();
    }

    if(ledTransitions[i].ledIndex != -1 && !ledTransitions[i].completed && ledTransitions[i].isRunning && !ledHasTransition(ledTransitions[i].ledIndex)) {
      ledTransitions[i].complete();
      ledTransitions[i] = ledTransition();
    }
  }
}

int getTransitionStackLength() {
  int length = 0;
  for(int i = 0; i < 1024; i++) {
    if(ledTransitions[i].ledIndex != -1) length++;
  }

  return length;
}