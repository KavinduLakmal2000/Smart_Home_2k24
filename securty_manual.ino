void Securty_mode() {  // activate with button, all pir's working at same time
  if (SecuMode) {

    if (digitalRead(insidePir)) {
      pirCount1 = 50;
      Blynk.logEvent("motion_detected", "Motion detected inside of the house!");
    }

    if (digitalRead(outsidePir_top)) {
      Blynk.logEvent("motion_detected", "Motion detected top left outside!");
    }



    if (pirCount1 > 0) {

      if (!alarmCutOff) {
        pcf1.digitalWrite(alarm, HIGH);
      }

      Blynk.virtualWrite(V2, pirCount1);

      ledcWrite(KLroomCh, 220);
      ledcWrite(stairsCh, 220);
      ledcWrite(livingCh, 220);
      ledcWrite(diningCh, 220);
      ledcWrite(kitchenCh, 220);

      pcf1.digitalWrite(rf1, LOW);
      pcf1.digitalWrite(rf2, LOW);
      pcf1.digitalWrite(rf3, LOW);
      pcf1.digitalWrite(rf4, LOW);
      pcf1.digitalWrite(RF_CH, HIGH);

      delay(random(50, 150));

      LedAllOff();
      pcf1.digitalWrite(RF_CH, LOW);
      Blynk.virtualWrite(V2, "All RF lights ON");

      delay(random(50, 150));

      pirCount1--;

    }

    else {
      LedAllOff();
      pcf1.digitalWrite(alarm, LOW);
      pcf1.digitalWrite(rf1, HIGH);
      pcf1.digitalWrite(rf2, HIGH);
      pcf1.digitalWrite(rf3, HIGH);
      pcf1.digitalWrite(rf4, HIGH);
      delay(500);
      pcf1.digitalWrite(RF_CH, HIGH);
      delay(500);
      pcf1.digitalWrite(RF_CH, LOW);
      Blynk.virtualWrite(V2, "All RF lights OFF");
    }
  }
}