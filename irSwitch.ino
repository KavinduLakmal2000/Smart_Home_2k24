void irSwitch() {

  if (irSensor == "i") {
    if (flag5) {
      pcf1.digitalWrite(RF_CH, HIGH);
      pcf1.digitalWrite(rf3, LOW);
      delay(1000);
      pcf1.digitalWrite(RF_CH, LOW);
      flag5 = false;
    }
  }

  if (irSensor == "o") {
    if (!flag5) {
      pcf1.digitalWrite(RF_CH, HIGH);
      pcf1.digitalWrite(rf3, HIGH);
      delay(1000);
      pcf1.digitalWrite(RF_CH, LOW);
      flag5 = true;
    }
  }
}

