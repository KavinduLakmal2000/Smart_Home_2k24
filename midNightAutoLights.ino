void midNightAutoLights() {  // automatic turn on lights when house main power is off at 10PM to 5AM (only when inside pir detects someone)

  bool pirState = digitalRead(insidePir);

  if (Hours >= 22 || Hours <= 4) {
    flag6 = true;

    if (pirState && !pirPreviouslyDetected) {
      if (!toggleState) {
        pirCounter = 2000;
      } else {
        pirCounter = 0;
      }
      toggleState = !toggleState;
      pirPreviouslyDetected = true;
    }

    else if (!pirState) {
      pirPreviouslyDetected = false;
    }

    if (pirCounter > 1) {
      rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));  //green off
      rgbLed.show();
      ledcWrite(kitchenCh, 200);
      Blynk.virtualWrite(V7, 250);
      Blynk.virtualWrite(V2, pirCounter);
    } else {

      if (xX) {
        rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 10));  // blue on
      } else {
        rgbLed.setPixelColor(0, rgbLed.Color(0, 10, 0));  // green on
      }
      rgbLed.show();
      ledcWrite(kitchenCh, 0);
      Blynk.virtualWrite(V7, 0);
      pirCounter = 0;
    }

    if (VoltSensor < 210) {

      if (pirCounter > 0) {
        if (!flag3) {
          ledcWrite(stairsCh, 50);
          ledcWrite(livingCh, 0);
          ledcWrite(diningCh, 50);
          ledcWrite(kitchenCh, 200);

          Blynk.virtualWrite(V4, 50);
          Blynk.virtualWrite(V6, 50);
          Blynk.virtualWrite(V7, 200);

          flag3 = true;
        }
      }

      else {

        if (flag3) {
          ledcWrite(stairsCh, 0);
          ledcWrite(diningCh, 0);
          ledcWrite(kitchenCh, 0);
          Blynk.virtualWrite(V4, 0);
          Blynk.virtualWrite(V6, 0);
          Blynk.virtualWrite(V7, 0);

          flag3 = false;
        }
      }
      flag7 = true;
    }

    else {
      if (flag7) {
        ledcWrite(stairsCh, 0);
        ledcWrite(diningCh, 0);
        ledcWrite(kitchenCh, 0);
        Blynk.virtualWrite(V4, 0);
        Blynk.virtualWrite(V6, 0);
        Blynk.virtualWrite(V7, 0);
        flag7 = false;
      }
    }
  }  //---------------------------------------------

  else {
    if (flag6) {
      LedAllOff();
      Blynk.virtualWrite(V1, 0);
      Blynk.virtualWrite(V6, 0);
      Blynk.virtualWrite(V7, 0);
      flag6 = false;
    }
  }


  if (pirCounter > 0) {
    pirCounter--;
  }
}