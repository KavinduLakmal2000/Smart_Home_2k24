#include "TemperatureReaderManager.h"

#include "BlynkConfig.h"
#define NO_GLOBAL_BLYNK
#include <BlynkSimpleEsp32.h>

namespace TemperatureReaderManager {

void run(TemperatureReaderContext& context) {
  if (millis() - context.lastDHTReadTime < context.dhtInterval) {
    return;
  }

  context.lastDHTReadTime = millis();

  float temp1 = context.dht2.readTemperature();
  delay(10);
  float temp2 = context.dht3.readTemperature();
  delay(10);
  float temp3 = context.dht.readTemperature();
  context.envH = context.dht.readHumidity();
  delay(10);

  if (context.dhtchk > 10) {
    Blynk.virtualWrite(V2, "System temp sensor failed!");
  }

  if (context.dhtchk1 > 10) {
    Blynk.virtualWrite(V2, "Battery temp sensor failed!");
  }

  if (context.dhtchk2 > 10) {
    Blynk.virtualWrite(V2, "Environment temp sensor failed!");
  }

  if (!isnan(temp1) && temp1 >= 10 && temp1 <= 70) {
    context.systemTemp = temp1;
    context.dhtchk = 0;
  } else {
    context.dhtchk++;
  }

  if (!isnan(temp2) && temp2 >= 10 && temp2 <= 70) {
    context.batteryTemp = temp2;
    context.dhtchk1 = 0;
  } else {
    context.dhtchk1++;
  }

  if (!isnan(temp3) && temp3 >= 10 && temp3 <= 70) {
    context.envT = temp3;
    context.dhtchk2 = 0;
  } else {
    context.dhtchk2++;
  }
}

}  // namespace TemperatureReaderManager
