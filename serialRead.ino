void Serial_Read() {
  if (Serial.available()) {
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

      VoltSensor = value1.toFloat();
      AmpSensor = value2.toFloat();
      microControllVolt_analog = value3.toInt();
      batteryVoltage_main = value4.toFloat();
      powerW = value5.toFloat();
      irSensor = value7;

      if (thunder) {
        rainSensor = value6.toInt();
      } else {
        rainSensor = 1024;
        rainCounter = 0;
        Blynk.virtualWrite(V2, "lightning protection off!");
        Blynk.virtualWrite(V2, value6.toInt());
      }
    }

    batteryVoltage_sys = microControllVolt_analog * (maxSystemBatteryVoltage / maxADCValue);
  }
}
