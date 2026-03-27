#include "AutoLightManager.h"

#include "BlynkConfig.h"
#define NO_GLOBAL_BLYNK
#include <BlynkSimpleEsp32.h>

namespace AutoLightManager {

void runMidnightAutoLights(AutoLightContext& context) {
  bool pirState = digitalRead(context.insidePirPin);

  if (context.hours >= 22 || context.hours <= 4) {
    context.flag6 = true;

    if (pirState && !context.pirPreviouslyDetected) {
      if (!context.toggleState) {
        context.pirCounter = 2000;
      } else {
        context.pirCounter = 0;
      }
      context.toggleState = !context.toggleState;
      context.pirPreviouslyDetected = true;
    } else if (!pirState) {
      context.pirPreviouslyDetected = false;
    }

    if (context.pirCounter > 1) {
      context.rgbLed.setPixelColor(0, context.rgbLed.Color(0, 0, 0));
      context.rgbLed.show();
      ledcWrite(context.kitchenChannel, 200);
      Blynk.virtualWrite(V7, 250);
      Blynk.virtualWrite(V2, context.pirCounter);
    } else {
      if (context.xModeEnabled) {
        context.rgbLed.setPixelColor(0, context.rgbLed.Color(0, 0, 10));
      } else {
        context.rgbLed.setPixelColor(0, context.rgbLed.Color(0, 10, 0));
      }
      context.rgbLed.show();
      ledcWrite(context.kitchenChannel, 0);
      Blynk.virtualWrite(V7, 0);
      context.pirCounter = 0;
    }

    if (context.voltSensor < 210) {
      if (context.pirCounter > 0) {
        if (!context.flag3) {
          ledcWrite(context.stairsChannel, 50);
          ledcWrite(context.livingChannel, 0);
          ledcWrite(context.diningChannel, 50);
          ledcWrite(context.kitchenChannel, 200);

          Blynk.virtualWrite(V4, 50);
          Blynk.virtualWrite(V6, 50);
          Blynk.virtualWrite(V7, 200);

          context.flag3 = true;
        }
      } else if (context.flag3) {
        ledcWrite(context.stairsChannel, 0);
        ledcWrite(context.diningChannel, 0);
        ledcWrite(context.kitchenChannel, 0);
        Blynk.virtualWrite(V4, 0);
        Blynk.virtualWrite(V6, 0);
        Blynk.virtualWrite(V7, 0);

        context.flag3 = false;
      }
      context.flag7 = true;
    } else if (context.flag7) {
      ledcWrite(context.stairsChannel, 0);
      ledcWrite(context.diningChannel, 0);
      ledcWrite(context.kitchenChannel, 0);
      Blynk.virtualWrite(V4, 0);
      Blynk.virtualWrite(V6, 0);
      Blynk.virtualWrite(V7, 0);
      context.flag7 = false;
    }
  } else if (context.flag6) {
    context.ledAllOff();
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V6, 0);
    Blynk.virtualWrite(V7, 0);
    context.flag6 = false;
  }

  if (context.pirCounter > 0) {
    context.pirCounter--;
  }
}

void runAutoLight(AutoLightContext& context) {
  if (context.hours >= 18 && context.hours <= 2 && context.voltSensor < 210) {
    if (!context.autoLightTriggered) {
      context.autoLightEnable = true;
      context.autoLightTriggered = true;
      Blynk.virtualWrite(V2, "Auto Lights ON");

      ledcWrite(context.klroomChannel, 0);
      ledcWrite(context.stairsChannel, 50);
      ledcWrite(context.livingChannel, 0);
      ledcWrite(context.diningChannel, 100);
      ledcWrite(context.kitchenChannel, 200);

      Blynk.virtualWrite(V4, 50);
      Blynk.virtualWrite(V6, 100);
      Blynk.virtualWrite(V7, 200);
    }
  } else if (context.autoLightEnable) {
    context.autoLightEnable = false;
    context.autoLightTriggered = false;
    Blynk.virtualWrite(V2, "Auto Lights OFF");
    context.ledAllOff();
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V6, 0);
    Blynk.virtualWrite(V7, 0);
  }
}

}  // namespace AutoLightManager
