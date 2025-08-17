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
//#include <IRremote.h>

#define LED_PIN     48    
#define NUM_PIXELS  1 
Adafruit_NeoPixel rgbLed(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C 


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTTYPE DHT11   
#define dht_dpin 42 // D4

#define DHTTYPE2 DHT11   // DHT 11
#define dht_dpin2 14 // D4

#define DHTTYPE3 DHT11 
#define dht_dpin3 41


DHT dht(dht_dpin, DHTTYPE);
DHT dht2(dht_dpin2, DHTTYPE2);
DHT dht3(dht_dpin3, DHTTYPE3);

const int IR_PIN = 2; // ir sensor 

PCF8575 pcf1(0x23); // 0x21

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SLT_Fiber_Optic";
char pass[] = "Life1Mal7i";

const long utcOffsetInSeconds = 19800;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

int Hours = 0;
int MiN = 0;
int sec = 0;

int i = 0;

#define KLroomPin 35
#define stairsPin 47
#define livingPin 21
#define diningPin 20
#define kitchenPin 19

#define KLroomCh    7
#define stairsCh    2
#define livingCh    6
#define diningCh    5
#define kitchenCh   4

// Common PWM settings
const int freq = 5000;       // 5 kHz
const int resolution = 8;    // 8-bit resolution (0-255)

String cmd= "";
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
const long display_interval = 5000; // 5 seconds
int currentDisplay = 0; // Track which display is active

unsigned long lastDHTReadTime = 0;  // Store last read time
const unsigned long dhtInterval = 2000;  // 2-second interval

float VoltSensor = 0;
float AmpSensor = 0;
float microControllVolt_analog = 0;
float mainBatteryVolt_analog = 0;
bool audio = false;
bool autolightEnable = false;
bool autolightTriggered = false;
bool main230Out = false;
bool irEn = false;
bool rainProtect = false;

const float maxSystemBatteryVoltage = 6.0;
const float maxMainBatteryVoltage = 17.5;
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

bool cmd_testPir1 = false;
bool cmd_testPir2 = false;

#define insidePir 40 // esp pin
#define outsidePir_top 39 // esp pin
#define alarm 4 // esp pin
#define fan 3 // pcf pin
#define lightninProtect 6 //pcf
#define pwrCut 7 // pcf
#define espEnable 5 // esp

#define rf1 8
#define rf2 9
#define rf3 10
#define rf4 11

#define RF_CH 12

bool testV2cmd = false; 

long espLoopCount = 0;

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
  //IrReceiver.begin(IR_PIN);

  rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 200));  // Blue ON
  rgbLed.show();

  Blynk.virtualWrite(V2, "System Starting...");


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


  // --- PinMode Setup ---
  //pinMode(KLroomPin, OUTPUT);
  //pinMode(stairsPin, OUTPUT);
  // pinMode(livingPin, OUTPUT);
  // pinMode(diningPin, OUTPUT);
  // pinMode(kitchenPin, OUTPUT);

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

  pcf1.digitalWrite(lightninProtect, HIGH);
  pcf1.digitalWrite(pwrCut, HIGH);
  pcf1.digitalWrite(fan, LOW);
  digitalWrite(alarm, LOW);

  analogWrite(KLroomPin, 0);
  //analogWrite(stairsPin, 0);
  analogWrite(livingPin, 0);
  analogWrite(diningPin, 0);
  analogWrite(kitchenPin, 0);

  pcf1.digitalWrite(rf1, HIGH);
  pcf1.digitalWrite(rf2, HIGH);
  pcf1.digitalWrite(rf3, HIGH);
  pcf1.digitalWrite(rf4, HIGH);
  delay(300);
  pcf1.digitalWrite(RF_CH, HIGH);
  delay(300);
  pcf1.digitalWrite(RF_CH, LOW);

  // --- Final display screen ---

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
    display.setCursor(20, 35);
    display.print(F("Software: 1.8.0v"));

    display.display();
    delay(5000);
    rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 0)); 
    rgbLed.show();
    display.clearDisplay();
    display.display();
  

  delay(100);
}


void loop() {
espLoopCount++;
Blynk.run();

  if(testV2cmd){
    Blynk.virtualWrite(V2, espLoopCount);
  }

delay(1000);
}


BLYNK_WRITE(V2){ // ----------------------------------------------------------------------------------------- cmd command reading -----------------------
  cmd = param.asStr();

  if(cmd == "test"){
    testV2cmd = true;
  }
    else{
    testV2cmd = false;
  }

}


BLYNK_WRITE(V4) { // stairs
  int val = param.asInt();
  ledcWrite(stairsCh, val);
}

BLYNK_WRITE(V1) {
  int val = param.asInt(); // KLroom
  ledcWrite(KLroomCh, val);
}

BLYNK_WRITE(V5) {
  int val = param.asInt();
  ledcWrite(livingCh, val); // Living
}

BLYNK_WRITE(V6) {
  int val = param.asInt();
  ledcWrite(diningCh, val); // Dining
}

BLYNK_WRITE(V7) {
  int val = param.asInt();
  ledcWrite(kitchenCh, val); // Kitchen
}

