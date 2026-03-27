void timeDateUpdate() {
  timeClient.update();
  Hours = timeClient.getHours();
  MiN = timeClient.getMinutes();
  sec = timeClient.getSeconds();

  unsigned long epochTime = timeClient.getEpochTime();
  int currentYear = year(epochTime);
  int currentMonth = month(epochTime);
  int currentDay = day(epochTime);

  sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d",
          currentYear, currentMonth, currentDay,
          Hours, MiN, sec);

  // ---------- Daily SD space check ----------
  if (Hours == 0 && MiN == 0) {  // At 00:00 midnight
    if (currentDay != lastCheckedDay) {
      checkCardSpace();
      lastCheckedDay = currentDay;
    }
  }
}

