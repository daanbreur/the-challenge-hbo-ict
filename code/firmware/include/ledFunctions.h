#include "Arduino.h"
#include "Pins.h"
#include "FastLED.h"

void setupLeds();
bool setLedColor(int ledIndex, CRGB color, int fadeTime = 0, int startDelay = 0);
bool setBlockColor(int blockIndex, CRGB color, int fadeTime = 0, int startDelay = 0, int transferDelay = 0);
CRGB randomColor();
void handleLedFadeStep();
void handleTransitionStack();

bool addBlockTransitionToStack(int blockIndex, CRGB color, int fadeTime = 0, int startDelay = 0, int transferDelay = 0);
bool addLedTransitionToStack(int ledIndex, CRGB color, int fadeTime = 0, int startDelay = 0);
int getTransitionStackLength();