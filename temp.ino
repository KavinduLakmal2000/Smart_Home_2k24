void readTemp() {
  if (millis() - lastDHTReadTime >= dhtInterval) {
    lastDHTReadTime = millis();  // Update the last read time

    float temp1 = dht2.readTemperature();
    delay(10);
    float temp2 = dht3.readTemperature();
    delay(10);
    float temp3 = dht.readTemperature();
    envH = dht.readHumidity();
    delay(10);

    if (dhtchk > 10) {
      Blynk.virtualWrite(V2, "System temp sensor failed!");
    }

    if (dhtchk1 > 10) {
      Blynk.virtualWrite(V2, "Battery temp sensor failed!");
    }

    if (dhtchk2 > 10) {
      Blynk.virtualWrite(V2, "Environment temp sensor failed!");
    }

    // Apply filters
    if (!isnan(temp1) && temp1 >= 10 && temp1 <= 70) {
      systemTemp = temp1;
      dhtchk = 0;
    } else {
      dhtchk++;
    }

    if (!isnan(temp2) && temp2 >= 10 && temp2 <= 70) {
      batteryTemp = temp2;
      dhtchk1 = 0;
    } else {
      dhtchk1++;
    }

    if (!isnan(temp3) && temp3 >= 10 && temp3 <= 70) {
      envT = temp3;
      dhtchk2 = 0;
    } else {
      dhtchk2++;
    }
  }
}