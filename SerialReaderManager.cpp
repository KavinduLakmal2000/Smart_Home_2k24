#include "SerialReaderManager.h"

#include "BlynkConfig.h"
#define NO_GLOBAL_BLYNK
#include <BlynkSimpleEsp32.h>

namespace SerialReaderManager {

void run(SerialReaderContext& context) {
  if (!Serial.available()) {
    return;
  }

  String data = Serial.readStringUntil('\n');
  data.trim();

  int commaIndex1 = data.indexOf(',');
  int commaIndex2 = data.indexOf(',', commaIndex1 + 1);
  int commaIndex3 = data.indexOf(',', commaIndex2 + 1);
  int commaIndex4 = data.indexOf(',', commaIndex3 + 1);
  int commaIndex5 = data.indexOf(',', commaIndex4 + 1);
  int commaIndex6 = data.indexOf(',', commaIndex5 + 1);

  if (commaIndex1 != -1 && commaIndex2 != -1 && commaIndex3 != -1 && commaIndex4 != -1 && commaIndex5 != -1 && commaIndex6 != -1) {
    String value1 = data.substring(0, commaIndex1);
    String value2 = data.substring(commaIndex1 + 1, commaIndex2);
    String value3 = data.substring(commaIndex2 + 1, commaIndex3);
    String value4 = data.substring(commaIndex3 + 1, commaIndex4);
    String value5 = data.substring(commaIndex4 + 1, commaIndex5);
    String value6 = data.substring(commaIndex5 + 1, commaIndex6);
    String value7 = data.substring(commaIndex6 + 1);

    context.voltSensor = value1.toFloat();
    context.ampSensor = value2.toFloat();
    context.microControllerVoltAnalog = value3.toInt();
    context.batteryVoltageMain = value4.toFloat();
    context.powerW = value5.toFloat();
    context.irSensor = value7;

    if (context.thunder) {
      context.rainSensor = value6.toInt();
    } else {
      context.rainSensor = 1024;
      context.rainCounter = 0;
      Blynk.virtualWrite(V2, "lightning protection off!");
      Blynk.virtualWrite(V2, value6.toInt());
    }
  }

  context.batteryVoltageSys = context.microControllerVoltAnalog * (context.maxSystemBatteryVoltage / context.maxADCValue);
}

}  // namespace SerialReaderManager
