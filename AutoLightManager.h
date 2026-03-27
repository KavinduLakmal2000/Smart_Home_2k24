#ifndef AUTO_LIGHT_MANAGER_H
#define AUTO_LIGHT_MANAGER_H

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

namespace AutoLightManager {

struct AutoLightContext {
  int hours;
  float voltSensor;
  bool xModeEnabled;
  bool& autoLightEnable;
  bool& autoLightTriggered;
  bool& flag3;
  bool& flag6;
  bool& flag7;
  bool& toggleState;
  bool& pirPreviouslyDetected;
  int& pirCounter;
  int insidePirPin;
  int klroomChannel;
  int stairsChannel;
  int livingChannel;
  int diningChannel;
  int kitchenChannel;
  Adafruit_NeoPixel& rgbLed;
  void (*ledAllOff)();
};

void runMidnightAutoLights(AutoLightContext& context);
void runAutoLight(AutoLightContext& context);

}  // namespace AutoLightManager

#endif
