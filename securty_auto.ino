void autoSecurty() {               // pir sensors at out side automatic work and notifications
  if (Hours >= 18 || Hours < 6) {  // 6PM to 6AM
    if (digitalRead(outsidePir_top)) {
      Blynk.logEvent("motion_detected", "Motion detected top left outside!");
    }
  }
}
