#include "Arduino.h"
#include "Pins.h"
#include "Bounce2.h"
#include "ledFunctions.h"

Bounce2::Button buttons[4] = {Bounce2::Button(), Bounce2::Button(), Bounce2::Button(), Bounce2::Button()};

long lastMicros = 0;
bool setRandomColor = true;

void updateButtons() {
  for(int i = 0; i < 4; i++) {
    buttons[i].update();
  }
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

  setupLeds();

  pinMode(CHARGE, INPUT);
  pinMode(STANDBY, INPUT);
  pinMode(BATTERY, INPUT);

  for(int i = 0; i < 4; i++) {
    if(i == 0) buttons[i].attach(BUTTON_1, INPUT);
    if(i == 1) buttons[i].attach(BUTTON_2, INPUT);
    if(i == 2) buttons[i].attach(BUTTON_3, INPUT);
    if(i == 3) buttons[i].attach(BUTTON_4, INPUT);

    buttons[i].interval(25);
    buttons[i].setPressedState(HIGH);
  }

    for(int i = 0; i < 5; i++) {
      addBlockTransitionToStack(i, CRGB::Red, 25, i * 5, 5);
      addBlockTransitionToStack(i, CRGB::Orange, 25, 0, 5);
      addBlockTransitionToStack(i, CRGB::Yellow, 25, 0, 5);
      addBlockTransitionToStack(i, CRGB::Green, 25, 0, 5);
      addBlockTransitionToStack(i, CRGB::Blue, 25, 0, 5);
      addBlockTransitionToStack(i, CRGB::Indigo, 25, 0, 5);
      addBlockTransitionToStack(i, CRGB::Black, 25, 0, 5);
    }

  Serial.println(getTransitionStackLength());
  Serial.println("heyy");
  Serial.println(getTransitionStackLength());

}

void loop(){
  updateButtons();

  for(int i = 0; i < 4; i++) {
    if(buttons[i].pressed()) {
      if(!setRandomColor)setBlockColor(i + 1, CRGB::White);
      if(setRandomColor) setBlockColor(i + 1, randomColor());
    } else if (buttons[i].released()) {
      setBlockColor(i + 1, CRGB::Black, 100, 0, 0);
    }
  }

  handleTransitionStack();
  handleLedFadeStep();
  FastLED.show();
  if(microsSinceLastRun() < 10000) delayMicroseconds(10000 - microsSinceLastRun());
}