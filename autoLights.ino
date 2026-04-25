void autoLight() {

  if (Hours >= 18 && Hours <= 23 && VoltSensor < 180) { 
    if (!autolightTriggered) {

      autolightEnable = true;
      autolightTriggered = true;
      Blynk.virtualWrite(V2, "Auto Lights ON");

      ledcWrite(KLroomCh, 0);
      ledcWrite(stairsCh, 50);
      ledcWrite(livingCh, 0);
      ledcWrite(diningCh, 100);
      ledcWrite(kitchenCh, 200);
      ////////////////////////
      Blynk.virtualWrite(V4, 50);
      Blynk.virtualWrite(V6, 100);
      Blynk.virtualWrite(V7, 200);
    }
  } else {
    if (autolightEnable) {

      autolightEnable = false;
      autolightTriggered = false;
      Blynk.virtualWrite(V2, "Auto Lights OFF");
      LedAllOff();
      Blynk.virtualWrite(V1, 0);
      Blynk.virtualWrite(V6, 0);
      Blynk.virtualWrite(V7, 0);
    }
  }
}
