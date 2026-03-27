#include "SecurityModeManager.h"

#include "BlynkConfig.h"
#define NO_GLOBAL_BLYNK
#include <BlynkSimpleEsp32.h>

namespace SecurityModeManager {

void run(SecurityModeContext& context) {
  if (!context.securityModeEnabled) {
    return;
  }

  if (digitalRead(context.insidePirPin)) {
    context.pirCount = 50;
    Blynk.logEvent("motion_detected", "Motion detected inside of the house!");
  }

  if (digitalRead(context.outsidePirTopPin)) {
    Blynk.logEvent("motion_detected", "Motion detected top left outside!");
  }

  if (context.pirCount > 0) {
    if (!context.alarmCutOff) {
      context.pcf.digitalWrite(context.alarmPin, HIGH);
    }

    Blynk.virtualWrite(V2, context.pirCount);

    ledcWrite(context.klroomChannel, 220);
    ledcWrite(context.stairsChannel, 220);
    ledcWrite(context.livingChannel, 220);
    ledcWrite(context.diningChannel, 220);
    ledcWrite(context.kitchenChannel, 220);

    context.pcf.digitalWrite(context.rf1Pin, LOW);
    context.pcf.digitalWrite(context.rf2Pin, LOW);
    context.pcf.digitalWrite(context.rf3Pin, LOW);
    context.pcf.digitalWrite(context.rf4Pin, LOW);
    context.pcf.digitalWrite(context.rfChannelPin, HIGH);

    delay(random(50, 150));

    context.ledAllOff();
    context.pcf.digitalWrite(context.rfChannelPin, LOW);
    Blynk.virtualWrite(V2, "All RF lights ON");

    delay(random(50, 150));

    context.pirCount--;
    return;
  }

  context.ledAllOff();
  context.pcf.digitalWrite(context.alarmPin, LOW);
  context.pcf.digitalWrite(context.rf1Pin, HIGH);
  context.pcf.digitalWrite(context.rf2Pin, HIGH);
  context.pcf.digitalWrite(context.rf3Pin, HIGH);
  context.pcf.digitalWrite(context.rf4Pin, HIGH);
  delay(500);
  context.pcf.digitalWrite(context.rfChannelPin, HIGH);
  delay(500);
  context.pcf.digitalWrite(context.rfChannelPin, LOW);
  Blynk.virtualWrite(V2, "All RF lights OFF");
}

}  // namespace SecurityModeManager
