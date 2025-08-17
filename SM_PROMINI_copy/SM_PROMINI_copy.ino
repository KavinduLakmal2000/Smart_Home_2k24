/////////////////////////////////////////////
// ------------ A0 - Audio in
// ------------ A1 - Rain sensor
// ------------ A2 - ZMPT 230v volt sensor
// ------------ A3 - micro Control Volt
// ------------ A4 - SDA
// ------------ A5 - SCL

#include <Adafruit_NeoPixel.h>
#include <ZMPT101B.h>
#include <INA226.h>

INA226 INA(0x44);

#define LED_PIN 10         // WS2812B Data Pin
#define NUM_LEDS 16        // Total LEDs
#define AUDIO_PIN A0       // Analog input for audio
#define IDLE_THRESHOLD 43  // Adjust this value for switching between effects
#define SENSITIVITY 450.0f

ZMPT101B voltageSensor(A2, 50.0);

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define OFFSET 2.5  // Midpoint voltage (2.5V for 5V supply)

unsigned long lastSensorRead = 0;  // Stores the last time the sensor was read
const int sensorInterval = 1000;   // Read sensor every 100ms
float voltage = 0;

int count = 0;
int peak = 0;        // Peak hold effect
int hueShift = 0;    // Variable for rainbow effect
int tailPos = 0;     // Position of the tail
int tailLength = 5;  // Length of fading tail

boolean effect1 = true;
boolean effect2 = false;
boolean effect3 = false;
boolean effect4 = false;

void setup() {
  strip.begin();
  Serial.begin(9600);
  Wire.begin();
  pinMode(13, OUTPUT);
  voltageSensor.setSensitivity(SENSITIVITY);

  if (!INA.begin()) {
    Serial.println("INA226 not connected!");
    digitalWrite(13, HIGH);
    while (1)
      ;
  }

  INA.setMaxCurrentShunt(6.0, 0.0025);  // 5A max, 0.1 ohm shunt

  //strip.setBrightness(150); // 0 to 255
  strip.show();  // Clear LEDs
  digitalWrite(13, LOW);
  delay(500);
}

void loop() {

  if (millis() - lastSensorRead >= sensorInterval) {
    lastSensorRead = millis();

    voltage = voltageSensor.getRmsVoltage();

    float voltageBAT = INA.getBusVoltage();  // Volts
    float current = INA.getCurrent();        // Amps
    float power   = INA.getPower();        // W 

    Serial.print(voltage);  // AC 230v
    Serial.print(",");
    Serial.print(current);  // BAT A
    Serial.print(",");
    Serial.print(analogRead(A3));  // Micro Control volt
    Serial.print(",");
    Serial.print(voltageBAT);  // main bat
    Serial.print(",");
    Serial.print(power);  // main bat power (W)
    Serial.print(",");
    Serial.println(analogRead(A1));  // Rain sensor
    digitalWrite(13, HIGH);
    delay(5);
  }


  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    data.trim();
    if (data == "#idle") {
      effect1 = true;
      effect2 = false;
      effect3 = false;
    }
    if (data == "#tail") {
      effect1 = false;
      effect3 = false;
      effect2 = true;
    }
    if (data == "#vu") {
      effect1 = false;
      effect2 = false;
      effect3 = true;
    }
    if (data == "#off") {
      effect1 = false;
      effect2 = false;
      effect3 = false;
      strip.clear();
      strip.show();
    }
  }

  if (effect1) {
    idleEffect();
  }

  else if (effect2) {
    tail();
  }

  if (effect3) {
    vumeterCall();
  }


  //dynamicFlowEffect()
  //vumeterCall();
  //idleEffect();
  //tail();
  digitalWrite(13, LOW);
  delay(5);

}  //====================================================================================================================== loop end ==================================================


void vumeterCall() {
  int audioValue = analogRead(AUDIO_PIN);  // Read audio signal
  int level = abs(audioValue - 512);       // Remove DC bias (center 2.5V)

  if (level > IDLE_THRESHOLD) {
    count = min(count + 3, 60);
  } else {
    count = max(count - 1, 0);
  }

  if (count > 10) {
    updateVUMeter(level);  // Pass the current level directly
  } else {
    idleEffect();
  }
}

void updateVUMeter(int level) {
  int ledCount = map(level, 0, 150, 0, NUM_LEDS);
  if (ledCount > peak) peak = ledCount;

  for (int i = 0; i < NUM_LEDS; i++) {
    int hue = (hueShift + (i * 20)) % 255;

    if (i < ledCount) {
      strip.setPixelColor(i, Wheel(hue));  // Active LEDs
    } else if (i == peak) {
      strip.setPixelColor(i, Wheel(hue + 128));  // Peak indicator
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));  // Turn off inactive LEDs
    }
  }

  strip.show();

  if (peak > 0) peak--;  // Reduce peak slowly for a smooth effect

  hueShift = (hueShift + 3) % 255;  // Smooth color transition

  // ⚡ Removed delay(10) to improve response time ⚡
}


// 🌊 Smooth Flowing Gradient Idle Effect
void dynamicFlowEffect() {
  static float offset = 0.0;  // Controls the color shifting movement

  for (int i = 0; i < NUM_LEDS; i++) {
    // Create a smooth wave effect using sine function
    float wave = (sin((i * 0.3) + offset) + 1.0) / 2.0;  // Normalize from -1 to 1 → 0 to 1

    // Generate dynamic colors with smooth transitions
    int red = int(150 + (105 * wave));   // Ranges between 150-255
    int green = int(100 + (80 * wave));  // Softer greens for warmth
    int blue = int(50 + (50 * wave));    // Gentle blues (not overpowering)

    strip.setPixelColor(i, strip.Color(red, green, blue));  // Apply color to LED
  }

  strip.show();

  offset += 0.15;                     // Controls the speed of the wave movement (higher = faster)
  if (offset > PI * 2) offset = 0.0;  // Reset offset to keep wave looping smoothly

  delay(30);  // Adjusts speed (lower = faster, higher = slower)
}



// 🌈 Function for smooth rainbow effect (Idle Mode)
void idleEffect() {
  for (int i = 0; i < NUM_LEDS; i++) {
    int hue = (hueShift + (i * 10)) % 255;  // Different hues for each LED
    strip.setPixelColor(i, Wheel(hue));     // Set color
  }

  strip.show();

  hueShift += 2;  // Slow smooth transition
  if (hueShift >= 255) hueShift = 0;

  delay(50);  // Adjust speed of color change
}


// 🌠 Ultra-Smooth White Tail Effect (Soft Fade In & Out)
void tail() {
  static bool waiting = false;
  static unsigned long lastTailTime = 0;

  if (tailPos >= NUM_LEDS + tailLength) {  // Ensure full fade-out before resetting
    if (!waiting) {
      waiting = true;
      lastTailTime = millis();  // Store the time when the tail finished
    }

    // Wait for 1 seconds before starting a new tail
    if (millis() - lastTailTime < 1000) {
      return;  // Do nothing, just wait
    }

    // Reset tail position after the delay
    tailPos = 0;
    waiting = false;
  }

  strip.clear();  // Clear LEDs before drawing the tail

  for (int i = 0; i < tailLength; i++) {
    int pos = tailPos - i;  // Trail follows behind the head

    if (pos >= 0 && pos < NUM_LEDS) {                        // Only draw within valid range
      float fadeFactor = float(i) / float(tailLength - 1);   // Smooth factor from 0 to 1
      int brightness = int(255 * pow(1 - fadeFactor, 2.5));  // Smoother exponential fade

      strip.setPixelColor(pos, strip.Color(brightness, brightness, brightness));  // White fading tail
    }
  }

  strip.show();

  delay(80);  // Adjust speed of movement

  tailPos++;  // Move the tail forward
}



// 🎨 Function to generate rainbow colors
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
