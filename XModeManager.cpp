#include "XModeManager.h"

#include "BlynkConfig.h"
#define NO_GLOBAL_BLYNK
#include <BlynkSimpleEsp32.h>

namespace XModeManager {

void run(XModeContext& context) {
  if (context.enabled) {
    if (context.announceFlag) {
      Blynk.virtualWrite(V2, "XXX mode is on");
      context.announceFlag = false;
    }

    if (digitalRead(context.insidePirPin)) {
      if (millis() - context.millisCount >= 50) {
        context.millisCount = millis();
        context.ledState = !context.ledState;

        if (context.ledState) {
          context.rgbLed.setPixelColor(0, context.rgbLed.Color(0, 0, 255));
          context.rgbLed.show();
          delay(500);
        } else {
          context.rgbLed.setPixelColor(0, context.rgbLed.Color(0, 0, 0));
          context.rgbLed.show();
        }
      }
    } else {
      context.rgbLed.setPixelColor(0, context.rgbLed.Color(0, 0, 0));
      context.rgbLed.show();
    }

    return;
  }

  if (context.announceFlag) {
    Blynk.virtualWrite(V2, "XXX mode is off");
    context.announceFlag = false;
  }

  context.rgbLed.setPixelColor(0, context.rgbLed.Color(0, 0, 0));
  context.rgbLed.show();
}

}  // namespace XModeManager
