#include <Arduino.h>
#include <Pins.h>

bool usbPowerConnected()
{
  return digitalRead(CHARGE) == LOW || digitalRead(STANDBY) == LOW;
}

bool isCharging()
{
  return digitalRead(CHARGE) == LOW;
}

float batteryVoltage()
{
  float voltage = analogRead(BATTERY);
  return (voltage * 0.00416918) + 0.0;
}

bool powerSwitchOn()
{
  return batteryVoltage() < 1;
}
