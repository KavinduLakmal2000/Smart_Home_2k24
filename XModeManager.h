#ifndef X_MODE_MANAGER_H
#define X_MODE_MANAGER_H

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

namespace XModeManager {

struct XModeContext {
  bool& enabled;
  bool& announceFlag;
  unsigned long& millisCount;
  bool& ledState;
  int insidePirPin;
  Adafruit_NeoPixel& rgbLed;
};

void run(XModeContext& context);

}  // namespace XModeManager

#endif
