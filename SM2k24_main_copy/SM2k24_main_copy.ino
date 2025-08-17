//////////////////////////////////////////

// i2c Address :
// INA226 sensor - 0x44
// PCF8575 - 0x23
// OLED Display - ox3c

///////////////////////////////////////
const char* SoftVer = "Software 2.8.1";
////////////////////TEST MODE///////////////////
bool testMode = false;
////////////////////////////////////////////////


#define BLYNK_TEMPLATE_ID "TMPL6JKpqPS9E"
#define BLYNK_TEMPLATE_NAME "ESP32 Smart Home"
#define BLYNK_AUTH_TOKEN "Hl5yD-nmduiUih1XUFzHqgPOmrr1nZRT"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include "Arduino.h"
#include "PCF8575.h"
#include "esp_system.h"
#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTTYPE DHT11
#define dht_dpin 42  // D4

#define DHTTYPE2 DHT11  // DHT 11
#define dht_dpin2 14    // D4

#define DHTTYPE3 DHT11
#define dht_dpin3 41


DHT dht(dht_dpin, DHTTYPE);
DHT dht2(dht_dpin2, DHTTYPE2);
DHT dht3(dht_dpin3, DHTTYPE3);

PCF8575 pcf1(0x23);  // 0x21

#define LED_PIN 48
#define NUM_PIXELS 1
Adafruit_NeoPixel rgbLed(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SLT_Fiber_Optic";
char pass[] = "Life1Mal7i";

const long utcOffsetInSeconds = 19800;
char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

int Hours = 0;
int MiN = 0;
int sec = 0;

int i = 0;

#define KLroomPin 2
#define stairsPin 3
#define livingPin 6
#define diningPin 4
#define kitchenPin 10

#define KLroomCh 0
#define stairsCh 1
#define livingCh 2
#define diningCh 3
#define kitchenCh 4

// Common PWM settings
const int freq = 5000;     // 5 kHz
const int resolution = 8;  // 8-bit resolution (0-255)

String cmd = "";
bool xX = false;
unsigned long millisCount = 0;
bool ledState = false;
bool SecuMode = false;
int pirCount1 = 0;
int pirCounter = 0;
int alarmCutOff = true;
bool cmd_systemVolt = false;
bool cmd_sysTemp = false;
bool cmd_batTemp = false;
bool cmd_systemAmp = false;
bool cmd_rainSensor = false;
bool cmd_rstCount = false;

unsigned long resetCounter = 0;

bool flag1 = false;
bool flag3 = false;
bool toggleState = false;
bool pirPreviouslyDetected = false;

unsigned long previousMillis = 0;
const long display_interval = 5000;  // 5 seconds
int currentDisplay = 0;              // Track which display is active

unsigned long lastDHTReadTime = 0;       // Store last read time
const unsigned long dhtInterval = 2000;  // 2-second interval

float VoltSensor = 0;
float AmpSensor = 0;
float powerW = 0;
float microControllVolt_analog = 0;
float mainBatteryVolt_analog = 0;
bool audio = false;
bool autolightEnable = false;
bool autolightTriggered = false;
bool main230Out = false;
bool irEn = false;
bool rainProtect = false;

const float maxSystemBatteryVoltage = 6.0;
const int maxADCValue = 1023;
float batteryVoltage_sys = 0;
float batteryVoltage_main = 0;
int rainSensor = 1023;
int rainDetectCount = 0;

int systemTempCount = 0;
int batteryTempCount = 0;
int envTCount = 0;
int voltHighCount = 0;
int voltLowCount = 0;
int batteryMainHighCount = 0;
int batteryMainLowCount = 0;
int batterySysHighCount = 0;
int batterySysLowCount = 0;
int fireDetectionCount = 0;
int rainCounter = 0;

float systemTemp = 0;
float batteryTemp = 0;
float envH = 0;
float envT = 0;
long dhtchk = 0;
long dhtchk1 = 0;
long dhtchk2 = 0;

bool cmd_testPir1 = false;
bool cmd_testPir2 = false;

#define insidePir 13       // esp pin
#define outsidePir_top 12  // esp pin
#define outsidePir_bot 11  //esp pin
#define alarm 19           // esp pin
#define fan 3              // pcf pin
#define lightninProtect 6  //pcf
#define pwrCut 7           // pcf
#define espEnable 5        // esp
#define ir_pin 4           // pcf

#define rf1 8
#define rf2 9
#define rf3 10
#define rf4 11

#define RF_CH 12

bool testV2cmd = false;

long espLoopCount = 0;

void LedAllOff() {
  ledcWrite(KLroomCh, 0);
  ledcWrite(stairsCh, 0);
  ledcWrite(livingCh, 0);
  ledcWrite(diningCh, 0);
  ledcWrite(kitchenCh, 0);
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);

  unsigned long wifiStart = millis();
  bool wifiConnected = false;

  bool displayInitialized = display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  if (!displayInitialized) {
    Serial.println(F("⚠️ SSD1306 allocation failed. Continuing without display..."));
  }

  rgbLed.begin();
  rgbLed.setPixelColor(0, rgbLed.Color(255, 0, 0));  // Red ON
  rgbLed.show();

  // --- OLED WiFi Connecting Animation ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(22, 0);
  display.print(F("- Smart House -"));
  display.setCursor(45, 9);
  display.print(F("- 2k24 -"));
  display.drawLine(0, SCREEN_HEIGHT / 3.6, SCREEN_WIDTH, SCREEN_HEIGHT / 3.6, SSD1306_WHITE);
  display.setCursor(20, 25);
  display.print(F("Connecting WiFi"));
  display.display();


  int dotX = 115;
  while (millis() - wifiStart < 10000) {
    if (WiFi.status() == WL_CONNECTED) {
      wifiConnected = true;
      break;
    }

    if (displayInitialized) {
      display.fillRect(dotX, 25, 2, 2, SSD1306_WHITE);  // animation dots
      display.display();
      dotX += 2;
      if (dotX > 125) dotX = 115;
    }

    delay(250);
  }

  // --- WiFi Connection Result ---
  if (wifiConnected) {
    Serial.println("✅ WiFi Connected!");
    if (displayInitialized) {
      display.setCursor(25, 35);
      display.print(F("WiFi Connected!"));
      display.display();
    }
    Blynk.config(auth);
    Blynk.connect();
  } else {
    Serial.println("❌ WiFi Failed. Retrying in loop.");
    if (displayInitialized) {
      display.setCursor(10, 35);
      display.print(F("Failed. Retrying..."));
      display.display();
    }
    WiFi.begin(ssid, pass);  // retry automatically
    Blynk.config(auth);      // non-blocking
  }

  // --- Continue other hardware init ---
  pcf1.begin();
  timeClient.begin();
  Wire.begin();
  dht.begin();
  delay(100);
  dht2.begin();
  delay(100);
  dht3.begin();
  Blynk.run();

  rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 200));  // Blue ON
  rgbLed.show();

  Blynk.virtualWrite(V2, "System Starting...");

  if (!testMode) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(22, 0);
    display.print(F("- Smart House -"));
    display.setCursor(45, 9);
    display.print(F("- 2k24 -"));
    display.drawLine(0, SCREEN_HEIGHT / 3.6, SCREEN_WIDTH, SCREEN_HEIGHT / 3.6, SSD1306_WHITE);
    display.setCursor(20, 20);
    display.println("System Loading...");

    for (int i = 0; i < 122; i++) {
      display.setCursor(i, 40);
      display.println("I");
      display.display();
      delay(5);
    }

    display.display();
  }

  // --- PinMode Setup ---
  ledcSetup(KLroomCh, freq, resolution);
  ledcAttachPin(KLroomPin, KLroomCh);

  ledcSetup(stairsCh, freq, resolution);
  ledcAttachPin(stairsPin, stairsCh);

  ledcSetup(livingCh, freq, resolution);
  ledcAttachPin(livingPin, livingCh);

  ledcSetup(diningCh, freq, resolution);
  ledcAttachPin(diningPin, diningCh);

  ledcSetup(kitchenCh, freq, resolution);
  ledcAttachPin(kitchenPin, kitchenCh);


  pinMode(alarm, OUTPUT);
  pinMode(insidePir, INPUT);
  pinMode(outsidePir_top, INPUT);
  pinMode(espEnable, INPUT);

  pcf1.pinMode(rf1, OUTPUT);
  pcf1.pinMode(rf2, OUTPUT);
  pcf1.pinMode(rf3, OUTPUT);
  pcf1.pinMode(rf4, OUTPUT);
  pcf1.pinMode(RF_CH, OUTPUT);
  pcf1.pinMode(3, OUTPUT);
  pcf1.pinMode(lightninProtect, OUTPUT);
  pcf1.pinMode(pwrCut, OUTPUT);
  pcf1.pinMode(fan, OUTPUT);
  pcf1.pinMode(ir_pin, INPUT);

  pcf1.digitalWrite(lightninProtect, HIGH);
  pcf1.digitalWrite(pwrCut, HIGH);
  pcf1.digitalWrite(fan, LOW);
  digitalWrite(alarm, LOW);

  LedAllOff();

  pcf1.digitalWrite(rf1, HIGH);
  pcf1.digitalWrite(rf2, HIGH);
  pcf1.digitalWrite(rf3, HIGH);
  pcf1.digitalWrite(rf4, HIGH);
  delay(300);
  pcf1.digitalWrite(RF_CH, HIGH);
  delay(300);
  pcf1.digitalWrite(RF_CH, LOW);

  // --- Final display screen ---
  if (!testMode) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(22, 0);
    display.print(F("- Smart House -"));
    display.setCursor(45, 9);
    display.print(F("- 2k24 -"));
    display.drawLine(0, SCREEN_HEIGHT / 3.6, SCREEN_WIDTH, SCREEN_HEIGHT / 3.6, SSD1306_WHITE);
    display.setCursor(50, 25);
    display.print(F("Welcome!"));
    display.setCursor(27, 35);
    display.print(SoftVer);

    display.display();
    delay(5000);
    rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));
    rgbLed.show();
    display.clearDisplay();
    display.display();
  }

  delay(100);
}


void loop() {
  espLoopCount++;
  Blynk.run();
  timeClient.update();
  Hours = timeClient.getHours();
  MiN = timeClient.getMinutes();
  sec = timeClient.getSeconds();
  resetCounter++;

  Blynk.virtualWrite(V8, batteryVoltage_main);  // gauge

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


  if (digitalRead(espEnable)) {
    x_Mode();        // the room x mode
    Securty_mode();  // activate with button, all pir's working at same time, alarm lock off
    saftySys();      // checking all the temp sensors and voltage sensors if any problems send notification and cmd lines
    autoLight();     // automatic turn on lights when house main power off at 6PM to 10PM
    readTemp();      // read all the temp sensors and filter output values
    //autoSecurty(); // pir sensors at out side automatic work and notifications
    midNightAutoLights();  // automatic turn on lights when house main power is off at 10PM to 5AM (only when inside pir detects someone)
    Serial_Read();         // read promini serial data
    irSwitch();

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    if (VoltSensor < 50) {
      //Blynk.logEvent("volts_amps_sensors", "Main 230v Power Supply is off"); //------------ enable this after all done
      main230Out = true;

    }

    else {
      if (main230Out) {
        pcf1.digitalWrite(rf1, HIGH);
        pcf1.digitalWrite(rf2, HIGH);
        pcf1.digitalWrite(rf3, HIGH);
        pcf1.digitalWrite(rf4, HIGH);
        delay(300);
        pcf1.digitalWrite(RF_CH, HIGH);
        delay(300);
        pcf1.digitalWrite(RF_CH, LOW);
        main230Out = false;
      }
    }

  }

  else {
    Blynk.virtualWrite(V2, "Connect ESP32 to main board");
    rgbLed.setPixelColor(0, rgbLed.Color(255, 0, 0));  // Red ON
    rgbLed.show();
    delay(200);
    rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));  // Red OFF
    rgbLed.show();
    delay(2000);
  }


  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= display_interval) {
    previousMillis = currentMillis;
    currentDisplay = (currentDisplay + 1) % 2;  // show display count max is 4
  }

  if (testV2cmd) {  // for testing values cmd printing command for active "test"
    //Blynk.virtualWrite(V2, "rain count:");
    //Blynk.virtualWrite(V2, rainCounter);
    Blynk.virtualWrite(V2, "test");
  }


  if (cmd_rstCount) {
    Blynk.virtualWrite(V2, resetCounter);
  }

  if (cmd_rainSensor) {
    Blynk.virtualWrite(V2, rainSensor);
  }

  if (cmd_systemAmp) {
    Blynk.virtualWrite(V2, AmpSensor);
  }

  if (cmd_systemVolt) {
    Blynk.virtualWrite(V2, batteryVoltage_sys);
  }

  if (cmd_sysTemp) {
    Blynk.virtualWrite(V2, systemTemp);
  }

  if (cmd_batTemp) {
    Blynk.virtualWrite(V2, batteryTemp);
  }

  if (cmd_testPir1) {
    Blynk.virtualWrite(V2, digitalRead(insidePir));
  }

  if (cmd_testPir2) {
    Blynk.virtualWrite(V2, digitalRead(outsidePir_top));
  }


  if (alarmCutOff) {
    digitalWrite(alarm, LOW);
  }

  Blynk.virtualWrite(V10, envT);  // gauge

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("⚠️ WiFi Disconnected. Retrying...");
    WiFi.begin(ssid, pass);  // Retry

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(22, 0);
    display.print(F("- Smart House -"));
    display.setCursor(45, 9);
    display.print(F("- 2k24 -"));
    display.drawLine(0, SCREEN_HEIGHT / 3.6, SCREEN_WIDTH, SCREEN_HEIGHT / 3.6, SSD1306_WHITE);
    display.setCursor(15, 25);
    display.print(F("WiFi Disconnected"));
    display.setCursor(40, 35);
    display.print(F("Retrying..."));
    display.display();

    rgbLed.setPixelColor(0, rgbLed.Color(255, 0, 0));  // Blue ON
    rgbLed.show();
    delay(100);
    rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));  // Blue ON
    rgbLed.show();
    delay(1000);
  }

  else {
    if (VoltSensor < 200) {
      display3();
      display.display();
    }


    else {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(20, 0);
      display.print(F("- Smart House -"));
      display.setCursor(0, 0);
      display.print(F("K"));
      display.setCursor(43, 9);
      display.printf("%02d:%02d:%02d", Hours, MiN, sec);
      display.setCursor(0, 9);
      display.print(F("L"));
      display.setCursor(122, 0);
      display.print(F("2"));
      display.setCursor(122, 9);
      display.print(F("4"));
      display.drawLine(0, SCREEN_HEIGHT / 3.6, SCREEN_WIDTH, SCREEN_HEIGHT / 3.6, SSD1306_WHITE);

      switch (currentDisplay) {
        case 0:
          display1();
          break;
        case 1:
          display2();
          break;
        case 2:
          display3();
          break;
        case 3:
          display4();
          break;
      }

      display.display();
    }
  }

  delay(300);
}  // ================================================================================================= loop end ==================================================================

void irSwitch() {
  // String irCode = "";
  // static int irCount = 0;  // Retains value across calls

  // if (IrReceiver.decode()) {
  //   irCode = String(IrReceiver.decodedIRData.decodedRawData, HEX);
  //   irCode.toLowerCase();
  //   //Blynk.virtualWrite(V2, irCode);
  //   irCount = 2;
  //   IrReceiver.resume();
  // }

  // if (irCount > 0) {
  //   irEn = true;
  //   pcf1.digitalWrite(RF_CH, HIGH);
  //   pcf1.digitalWrite(rf3, LOW);
  //   delay(1000);
  //   pcf1.digitalWrite(RF_CH, LOW);
  // }

  // else {
  //   if (irEn) {
  //   pcf1.digitalWrite(RF_CH, HIGH);
  //   pcf1.digitalWrite(rf3, HIGH);
  //   delay(1000);
  //   pcf1.digitalWrite(RF_CH, LOW);
  //     irEn = false;
  //   }
  // }

  // // Countdown
  // if (irCount > 0) irCount--;
}


void Serial_Read() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    data.trim();

    int commaIndex1 = data.indexOf(',');
    int commaIndex2 = data.indexOf(',', commaIndex1 + 1);
    int commaIndex3 = data.indexOf(',', commaIndex2 + 1);
    int commaIndex4 = data.indexOf(',', commaIndex3 + 1);
    int commaIndex5 = data.indexOf(',', commaIndex4 + 1);

    if (commaIndex1 != -1 && commaIndex2 != -1 && commaIndex3 != -1 && commaIndex4 != -1 && commaIndex5 != -1) {
      String value1 = data.substring(0, commaIndex1);
      String value2 = data.substring(commaIndex1 + 1, commaIndex2);
      String value3 = data.substring(commaIndex2 + 1, commaIndex3);
      String value4 = data.substring(commaIndex3 + 1, commaIndex4);
      String value5 = data.substring(commaIndex4 + 1, commaIndex5);
      String value6 = data.substring(commaIndex5 + 1);

      VoltSensor = value1.toFloat();
      AmpSensor = value2.toFloat();
      microControllVolt_analog = value3.toInt();
      batteryVoltage_main = value4.toFloat();
      powerW = value5.toFloat();
      rainSensor = value6.toInt();
    }

    batteryVoltage_sys = microControllVolt_analog * (maxSystemBatteryVoltage / maxADCValue);
    Blynk.virtualWrite(V9, VoltSensor);  // gauge
  }
}

void readTemp() {
  if (millis() - lastDHTReadTime >= dhtInterval) {
    lastDHTReadTime = millis();  // Update the last read time

    float temp1 = dht2.readTemperature();
    delay(10);
    float temp2 = 30.1;  //dht3.readTemperature();
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

void midNightAutoLights() {  // automatic turn on lights when house main power is off at 10PM to 5AM (only when inside pir detects someone)

  bool pirState = digitalRead(insidePir);

  if (Hours >= 22 || Hours <= 5) {

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
    } else {
      rgbLed.setPixelColor(0, rgbLed.Color(0, 10, 0));  // green on
      rgbLed.show();
    }

    if (VoltSensor < 210) {

      if (pirCounter > 0) {
        ledcWrite(KLroomCh, 100);
        ledcWrite(stairsCh, 0);
        ledcWrite(livingCh, 0);
        ledcWrite(diningCh, 50);
        ledcWrite(kitchenCh, 100);
        /////////////////////////
        Blynk.virtualWrite(V1, 100);
        Blynk.virtualWrite(V6, 50);
        Blynk.virtualWrite(V7, 100);
        flag3 = true;
        Blynk.virtualWrite(V2, pirCounter);
      }

      else {
        if (flag3) {
          LedAllOff();
          Blynk.virtualWrite(V1, 0);
          Blynk.virtualWrite(V6, 0);
          Blynk.virtualWrite(V7, 0);
          flag3 = false;
        }
      }
    }
  }

  else {
    if (flag3) {
      LedAllOff();
      Blynk.virtualWrite(V1, 0);
      Blynk.virtualWrite(V6, 0);
      Blynk.virtualWrite(V7, 0);
      flag3 = false;
    }
  }


  if (pirCounter > 0) {
    pirCounter--;
  }
}



void autoLight() {

  if (Hours >= 18 && Hours <= 22 && VoltSensor < 210) {
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
void saftySys() {  // checking all the temp sensors and voltage sensors if any problems send notification and cmd lines
  float avaTemp = envT + 2;

  if (systemTemp > avaTemp) {
    pcf1.digitalWrite(fan, HIGH);
  } else {
    pcf1.digitalWrite(fan, LOW);
  }

  if (VoltSensor > 250) {
    voltHighCount++;
    if (voltHighCount >= 4) {
      pcf1.digitalWrite(pwrCut, LOW);
      delay(1000);
      pcf1.digitalWrite(pwrCut, HIGH);
      Blynk.logEvent("volts_amps_sensors", "Main 230V is too high!");
      Blynk.virtualWrite(V2, "Main 230V is too high! Count: " + String(voltHighCount));
      voltHighCount = 0;
    }
  } else {
    voltHighCount = 0;
  }

  if (VoltSensor > 90 && VoltSensor < 220) {
    voltLowCount++;
    if (voltLowCount >= 5) {
      pcf1.digitalWrite(6, LOW);
      Blynk.logEvent("volts_amps_sensors", "Main 230V is too low!");
      Blynk.virtualWrite(V2, "Main 230V is too Low! Count: " + String(voltLowCount));
      voltLowCount = 0;
    } else {
      pcf1.digitalWrite(6, HIGH);
    }
  } else {
    voltLowCount = 0;
  }


  if (batteryVoltage_main > 17) {
    batteryMainHighCount++;
    if (batteryMainHighCount >= 20) {
      pcf1.digitalWrite(lightninProtect, LOW);
      Blynk.logEvent("volts_amps_sensors", "Main battery is overcharging!");
      LedAllOff();
      Blynk.virtualWrite(V2, "Main battery is overcharging! Count: " + String(batteryMainHighCount));
      batteryMainHighCount = 0;
    } else {
      pcf1.digitalWrite(lightninProtect, HIGH);
    }
  } else {
    batteryMainHighCount = 0;
  }

  if (batteryVoltage_main < 12) {
    batteryMainLowCount++;
    if (batteryMainLowCount >= 5) {
      Blynk.logEvent("volts_amps_sensors", "Main battery voltage is too low!");
      LedAllOff();
      Blynk.virtualWrite(V2, "Main battery voltage is too low! Count: " + String(batteryMainLowCount));
      batteryMainLowCount = 0;
    }
  } else {
    batteryMainLowCount = 0;
  }

  if (batteryVoltage_sys > 6) {
    batterySysHighCount++;
    if (batterySysHighCount >= 5) {
      pcf1.digitalWrite(lightninProtect, LOW);
      Blynk.logEvent("volts_amps_sensors", "System voltage is too high!");
      LedAllOff();
      Blynk.virtualWrite(V2, "System voltage is too high! Count: " + String(batterySysHighCount));
      batterySysHighCount = 0;
    } else {
      pcf1.digitalWrite(lightninProtect, HIGH);
    }

  } else {
    batterySysHighCount = 0;
  }


  if (batteryVoltage_sys < 3.2) {
    batterySysLowCount++;
    if (batterySysLowCount >= 5) {
      Blynk.logEvent("volts_amps_sensors", "System voltage is too low!");
      Blynk.virtualWrite(V2, "System voltage is too low! Count: " + String(batterySysLowCount));
      batterySysLowCount = 0;
    }
  } else {
    batterySysLowCount = 0;
  }


  if (systemTemp > 39) {
    systemTempCount++;
    if (systemTempCount >= 5) {
      Blynk.logEvent("temp", "System Temp is too high!");
      Blynk.virtualWrite(V2, "System Temp is too high! Count: " + String(systemTempCount));
      LedAllOff();
      systemTempCount = 0;
    }
  } else {
    systemTempCount = 0;
  }

  if (batteryTemp > 36) {
    batteryTempCount++;
    if (batteryTempCount >= 5) {
      Blynk.logEvent("temp", "Battery Temp is too high!");
      Blynk.virtualWrite(V2, "Battery Temp is too high! Count: " + String(batteryTempCount));
      LedAllOff();
      batteryTempCount = 0;
    }
  } else {
    batteryTempCount = 0;
  }

  if (envT > 37) {
    envTCount++;
    if (envTCount >= 10) {
      Blynk.logEvent("temp", "Power Supply Temp is too high!");
      Blynk.virtualWrite(V2, "Power Supply Temp is too high! Count: " + String(envTCount));
      LedAllOff();
      envTCount = 0;
    }
  } else {
    envTCount = 0;
  }
  if (systemTemp > 40 || batteryTemp > 45 || envT > 40) {
    fireDetectionCount++;
    if (fireDetectionCount >= 20) {
      Blynk.logEvent("temp", "🔥 Fire Detected!!!");
      Blynk.virtualWrite(V2, "🔥 Fire Detected!!! Count: " + String(fireDetectionCount));
      delay(1000);
      pcf1.digitalWrite(pwrCut, LOW);
      delay(1000);
      pcf1.digitalWrite(pwrCut, HIGH);
      LedAllOff();
      fireDetectionCount = 0;  // Reset counter after triggering
    }
  } else {
    fireDetectionCount = 0;  // Reset if conditions return to normal
  }

  avaTemp = 0;

  //lightnin Protect
  if (rainSensor < 1) {  // 1020
    rainDetectCount++;
    if (rainDetectCount > 10) {
      rainCounter = 300;
      Blynk.logEvent("env", "Rain Detected!!");
    }
  }

  else {
    rainDetectCount = 0;  // Reset if no rain detected
  }

  if (rainCounter > 0) {
    pcf1.digitalWrite(lightninProtect, LOW);
    Blynk.virtualWrite(V2, "Lightning protect on");
    rainProtect = true;
    rainCounter--;
  }

  else {
    if (rainProtect) {
      pcf1.digitalWrite(lightninProtect, HIGH);
      Blynk.virtualWrite(V2, "Lightning protect off");
      rainProtect = false;
    }
  }
}

void autoSecurty() {               // pir sensors at out side automatic work and notifications
  if (Hours >= 18 || Hours < 6) {  // 6PM to 6AM
    if (digitalRead(outsidePir_top)) {
      Blynk.logEvent("motion_detected", "Motion detected top left outside!");
    }
  }
}

void Securty_mode() {  // activate with button, all pir's working at same time
  if (SecuMode) {

    if (digitalRead(insidePir)) {
      pirCount1 = 50;
      Blynk.logEvent("motion_detected", "Motion detected inside of the house!");
    }

    if (digitalRead(outsidePir_top)) {
      Blynk.logEvent("motion_detected", "Motion detected top left outside!");
    }

    //Blynk.virtualWrite(V2, pirCount1);

    if (pirCount1 > 0) {

      if (!alarmCutOff) {
        digitalWrite(alarm, HIGH);
      }


      ledcWrite(KLroomCh, 255);
      ledcWrite(stairsCh, 255);
      ledcWrite(livingCh, 255);
      ledcWrite(diningCh, 255);
      ledcWrite(kitchenCh, 255);

      delay(random(50, 150));

      LedAllOff();

      delay(random(50, 150));

      pirCount1--;

    }

    else {
      LedAllOff();
      digitalWrite(alarm, LOW);
    }
  }
}

void x_Mode() {
  if (xX) {

    if (flag1) {
      Blynk.virtualWrite(V2, "XXX mode is on");
      flag1 = false;
    }

    if (digitalRead(insidePir)) {
      if (millis() - millisCount >= 50) {
        millisCount = millis();

        ledState = !ledState;

        if (ledState) {
          rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 255));  // Red ON
          rgbLed.show();
          delay(500);
        } else {
          rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));  // OFF
          rgbLed.show();
        }
      }

    } else {
      rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));  // Red
      rgbLed.show();
    }

  }

  else {

    if (flag1) {
      Blynk.virtualWrite(V2, "XXX mode is off");
      flag1 = false;
    }

    rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0));  // Red
    rgbLed.show();
  }
}


BLYNK_WRITE(V3) {
  if (param.asInt()) {
    SecuMode = true;
    Blynk.virtualWrite(V2, "Securty Mode is on");
    if (alarmCutOff) {
      Blynk.virtualWrite(V2, "Alarm is Locked! (Alarm Off)");
    }
  } else {
    SecuMode = false;
    Blynk.virtualWrite(V2, "Securty Mode is off");
    alarmCutOff = true;
    digitalWrite(alarm, LOW);

    pirCount1 = 0;

    digitalWrite(KLroomPin, LOW);
    digitalWrite(stairsPin, LOW);
    digitalWrite(livingPin, LOW);
    digitalWrite(diningPin, LOW);
    digitalWrite(kitchenPin, LOW);
  }
}

BLYNK_WRITE(V4) {  // stairs
  int val = param.asInt();
  ledcWrite(stairsCh, val);
}

BLYNK_WRITE(V1) {
  int val = param.asInt();  // KLroom
  ledcWrite(KLroomCh, val);
}

BLYNK_WRITE(V5) {
  int val = param.asInt();
  ledcWrite(livingCh, val);  // Living
}

BLYNK_WRITE(V6) {
  int val = param.asInt();
  ledcWrite(diningCh, val);  // Dining
}

BLYNK_WRITE(V7) {
  int val = param.asInt();
  ledcWrite(kitchenCh, val);  // Kitchen
}

void display4() {
  display.setCursor(0, 20);
  display.print(F("Battery Volt: "));
  display.print(batteryVoltage_main);
}

void display3() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 0);
  display.print(F("- Smart House -"));
  display.setCursor(0, 0);
  display.print(F("K"));
  display.setCursor(43, 9);
  display.printf("Power Out");
  display.setCursor(0, 9);
  display.print(F("L"));
  display.setCursor(122, 0);
  display.print(F("2"));
  display.setCursor(122, 9);
  display.print(F("4"));
  display.drawLine(0, SCREEN_HEIGHT / 3.6, SCREEN_WIDTH, SCREEN_HEIGHT / 3.6, SSD1306_WHITE);

  display.setCursor(0, 20);
  display.print(F("Main Volt:"));
  display.setCursor(80, 20);
  display.print(VoltSensor);
  display.setCursor(112, 20);
  display.print(F(" v"));

  display.setCursor(0, 30);
  display.print(F("System Volt:"));
  display.setCursor(80, 30);
  display.print(batteryVoltage_sys);
  display.setCursor(112, 30);
  display.print(F(" v"));

  display.setCursor(0, 40);
  display.print(F("Battery Volt:"));
  display.setCursor(80, 40);
  display.print(batteryVoltage_main);
  display.setCursor(112, 40);
  display.print(F(" v"));

  display.setCursor(0, 50);
  display.print(F("Amps:"));
  display.setCursor(80, 50);
  display.print(AmpSensor);
  display.setCursor(112, 50);
  display.print(F(" A"));
}

void display2() {
  display.setCursor(0, 20);
  display.print(F("Main Volt:"));
  display.setCursor(80, 20);
  display.print(VoltSensor);
  display.setCursor(112, 20);
  display.print(F(" v"));

  display.setCursor(0, 30);
  display.print(F("System Volt:"));
  display.setCursor(80, 30);
  display.print(batteryVoltage_sys);
  display.setCursor(112, 30);
  display.print(F(" v"));

  display.setCursor(0, 40);
  display.print(F("Battery Volt:"));
  display.setCursor(80, 40);
  display.print(batteryVoltage_main);
  display.setCursor(112, 40);
  display.print(F(" v"));

  display.setCursor(0, 50);
  display.print(F("Amps:"));
  display.setCursor(80, 50);
  display.print(AmpSensor);
  display.setCursor(112, 50);
  display.print(F(" A"));
}

void display1() {
  display.setCursor(0, 20);
  display.print(F("System Temp: "));
  display.setCursor(80, 20);
  display.print(systemTemp);
  display.print(F(" C"));
  display.setCursor(0, 30);
  display.print(F("Battery Temp: "));
  display.setCursor(80, 30);
  display.print(batteryTemp);
  display.print(F(" C"));
  display.setCursor(0, 40);
  display.print(F("Env Temp: "));
  display.setCursor(80, 40);
  display.print(envT);
  display.print(F(" C"));
  display.setCursor(0, 50);
  display.print(F("Env Humidity: "));
  display.setCursor(80, 50);
  display.print(envH);
  display.print(F(" C"));
}

BLYNK_WRITE(V2) {  // ----------------------------------------------------------------------------------------- cmd command reading -----------------------
  cmd = param.asStr();

  if (cmd.startsWith("#")) {
    Serial.print(cmd);
  }

  if (cmd == "pCut") {
    pcf1.digitalWrite(pwrCut, LOW);
    delay(1000);
    pcf1.digitalWrite(pwrCut, HIGH);
    Serial_Read();
    delay(1000);
    if (VoltSensor < 30) {
      Blynk.virtualWrite(V2, "Main Power is off!");
    } else {
      Blynk.virtualWrite(V2, "Power Cut-off is faild!");
    }
  }

  if (cmd == "test") {
    testV2cmd = true;
  } else {
    testV2cmd = false;
  }

  if (cmd == "getAmps") {
    cmd_systemAmp = true;
  } else {
    cmd_systemAmp = false;
  }

  if (cmd == "getRain") {
    cmd_rainSensor = true;
  } else {
    cmd_rainSensor = false;
  }

  if (cmd == "getSystemVolt") {
    cmd_systemVolt = true;
  } else {
    cmd_systemVolt = false;
  }

  if (cmd == "getSystemTemp") {
    cmd_sysTemp = true;
  } else {
    cmd_sysTemp = false;
  }

  if (cmd == "getBatteryTemp") {
    cmd_batTemp = true;
  } else {
    cmd_batTemp = false;
  }

  if (cmd == "testInPir") {
    cmd_testPir1 = true;
  } else {
    cmd_testPir1 = false;
  }

  if (cmd == "rstCount") {
    cmd_rstCount = true;
  } else {
    cmd_rstCount = false;
  }

  if (cmd == "testOutTopPir") {
    cmd_testPir2 = true;
  } else {
    cmd_testPir2 = false;
  }

  if (cmd == "ELightOff") {
    pirCounter = 0;
    LedAllOff();
  }

  if (cmd == "autoLightOff") {
    if (pirCounter > 0) {
      Blynk.virtualWrite(V2, pirCounter);
      pirCounter = 0;
      Blynk.virtualWrite(V2, "Auto emergency light off!");
    } else {
      Blynk.virtualWrite(V2, "No movement detect!");
    }
  }

  if (cmd == "about") {
    Blynk.virtualWrite(V2, SoftVer);
    delay(100);
    Blynk.virtualWrite(V2, "Created BY KLTECHNOLOGY");
    delay(200);
    Blynk.virtualWrite(V2, "© 2024 Kavindu Lakmal. All rights reserved");
  }

  if (cmd == "almLocOn") {
    alarmCutOff = true;
    digitalWrite(alarm, LOW);
    Blynk.virtualWrite(V2, "Alarm Locked!");
  }

  if (cmd == "almLocOff") {
    alarmCutOff = false;
    Blynk.virtualWrite(V2, "Alarm Unlocked!");
  }

  if (cmd == "almOn") {
    if (alarmCutOff) {
      Blynk.virtualWrite(V2, "Alarm is Locked");
    } else {
      digitalWrite(alarm, HIGH);
      Blynk.virtualWrite(V2, "Alarm is on");
    }
  }

  if (cmd == "almOff") {
    digitalWrite(alarm, LOW);
    Blynk.virtualWrite(V2, "Alarm is off");
  }

  if (cmd == "pwrGon") {
    pcf1.digitalWrite(6, LOW);
    delay(100);
    if (pcf1.digitalRead(6) == LOW) {
      Blynk.virtualWrite(V2, "Lightning protection on");
    } else {
      Blynk.virtualWrite(V2, "Lightning protection failed!");
    }
  }

  if (cmd == "pwrGoff") {
    pcf1.digitalWrite(6, HIGH);
    delay(100);
    if (pcf1.digitalRead(6) == HIGH) {
      Blynk.virtualWrite(V2, "Lightning protection off");
    } else {
      Blynk.virtualWrite(V2, "Lightning protection failed!");
    }
  }

  if (cmd == "fan") {
    pcf1.digitalWrite(fan, HIGH);
    delay(5000);
    pcf1.digitalWrite(fan, LOW);
    Blynk.virtualWrite(V2, "Fan Checked!");
  }

  if (cmd == "reset") {
    Blynk.virtualWrite(V2, "Restarting ESP32...");
    delay(1000);
    esp_restart();
  }

  if (cmd == "rf1On") {
    pcf1.digitalWrite(rf1, LOW);
    Blynk.virtualWrite(V2, "RF SW1 ON");
  }
  if (cmd == "rf1Off") {
    pcf1.digitalWrite(rf1, HIGH);
    Blynk.virtualWrite(V2, "RF SW1 OFF");
  }

  if (cmd == "rf2On") {
    pcf1.digitalWrite(rf2, LOW);
    Blynk.virtualWrite(V2, "RF SW2 ON");
  }
  if (cmd == "rf2Off") {
    pcf1.digitalWrite(rf2, HIGH);
    Blynk.virtualWrite(V2, "RF SW2 OFF");
  }

  if (cmd == "rf3On") {
    pcf1.digitalWrite(rf3, LOW);
    Blynk.virtualWrite(V2, "RF SW3 ON");
  }
  if (cmd == "rf3Off") {
    pcf1.digitalWrite(rf3, HIGH);
    Blynk.virtualWrite(V2, "RF SW3 OFF");
  }

  if (cmd == "rf4On") {
    pcf1.digitalWrite(rf4, LOW);
    Blynk.virtualWrite(V2, "RF SW4 ON");
  }
  if (cmd == "rf4Off") {
    pcf1.digitalWrite(rf4, HIGH);
    Blynk.virtualWrite(V2, "RF SW4 OFF");
  }

  if (cmd == "rfCH1") {
    pcf1.digitalWrite(RF_CH, HIGH);
    Blynk.virtualWrite(V2, "RF Channel 1");
  }

  if (cmd == "rfCH2") {
    pcf1.digitalWrite(RF_CH, LOW);
    Blynk.virtualWrite(V2, "RF Channel 2");
  }

  if (cmd == "rfAllOff") {
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

  if (cmd == "rfAllOn") {
    pcf1.digitalWrite(rf1, LOW);
    pcf1.digitalWrite(rf2, LOW);
    pcf1.digitalWrite(rf3, LOW);
    pcf1.digitalWrite(rf4, LOW);
    delay(500);
    pcf1.digitalWrite(RF_CH, HIGH);
    delay(500);
    pcf1.digitalWrite(RF_CH, LOW);
    Blynk.virtualWrite(V2, "All RF lights ON");
  }

  if (cmd == "rfStatus") {

    Blynk.virtualWrite(V2, "RF Status Report");
    delay(100);

    if (pcf1.digitalRead(RF_CH)) {  //////////////////////////// rf ch 1
      Blynk.virtualWrite(V2, "RF Channel 1 (Ground Floor)");
      delay(100);

      if (pcf1.digitalRead(rf1)) {
        Blynk.virtualWrite(V2, "RF SW1 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW1 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf2)) {
        Blynk.virtualWrite(V2, "RF SW2 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW2 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf3)) {
        Blynk.virtualWrite(V2, "RF SW3 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW3 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf4)) {
        Blynk.virtualWrite(V2, "RF SW4 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW4 ON");
      }
    }

    else {  //////////////////////////////////////////////// rf ch 2
      Blynk.virtualWrite(V2, "RF Channel 2 (Mid Floor)");
      delay(100);

      if (pcf1.digitalRead(rf1)) {
        Blynk.virtualWrite(V2, "RF SW1 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW1 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf2)) {
        Blynk.virtualWrite(V2, "RF SW2 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW2 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf3)) {
        Blynk.virtualWrite(V2, "RF SW3 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW3 ON");
      }

      delay(100);

      if (pcf1.digitalRead(rf4)) {
        Blynk.virtualWrite(V2, "RF SW4 OFF");
      } else {
        Blynk.virtualWrite(V2, "RF SW4 ON");
      }
    }
  }


  if (cmd == "Xon") {
    xX = true;
    flag1 = true;
  }
  if (cmd == "Xoff") {
    xX = false;
    flag1 = true;
  }
}
