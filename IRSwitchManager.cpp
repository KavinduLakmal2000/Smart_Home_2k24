#include "IRSwitchManager.h"

namespace IRSwitchManager {

void run(IRSwitchContext& context) {
  if (context.irSensor == "i") {
    if (context.flag5) {
      context.pcf.digitalWrite(context.rfChannelPin, HIGH);
      context.pcf.digitalWrite(context.rf3Pin, LOW);
      delay(1000);
      context.pcf.digitalWrite(context.rfChannelPin, LOW);
      context.flag5 = false;
    }
  }

  if (context.irSensor == "o") {
    if (!context.flag5) {
      context.pcf.digitalWrite(context.rfChannelPin, HIGH);
      context.pcf.digitalWrite(context.rf3Pin, HIGH);
      delay(1000);
      context.pcf.digitalWrite(context.rfChannelPin, LOW);
      context.flag5 = true;
    }
  }
}

}  // namespace IRSwitchManager
