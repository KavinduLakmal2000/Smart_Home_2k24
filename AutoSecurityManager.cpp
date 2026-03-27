#include "AutoSecurityManager.h"

#include "BlynkConfig.h"
#define NO_GLOBAL_BLYNK
#include <BlynkSimpleEsp32.h>

namespace AutoSecurityManager {

void run(const AutoSecurityContext& context) {
  if (context.hours >= 18 || context.hours < 6) {
    if (digitalRead(context.outsidePirTopPin)) {
      Blynk.logEvent("motion_detected", "Motion detected top left outside!");
    }
  }
}

}  // namespace AutoSecurityManager
