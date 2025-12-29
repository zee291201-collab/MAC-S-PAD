#include <Keyboard.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define LED_COUNT 2

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


const int keyS = 8;
const int keyA = 9;
const int keyW = 10;
const int keyD = 11;


uint8_t hue = 0;
uint8_t speed = 1;      
unsigned long lastInputTime = 0;

void setup() {
  pinMode(keyS, INPUT_PULLUP);
  pinMode(keyA, INPUT_PULLUP);
  pinMode(keyW, INPUT_PULLUP);
  pinMode(keyD, INPUT_PULLUP);

  Keyboard.begin();

  strip.begin();
  strip.setBrightness(80);
  strip.show();
}

void loop() {
  bool activity = false;

  activity |= handleKey(keyS, 's');
  activity |= handleKey(keyA, 'a');
  activity |= handleKey(keyW, 'w');
  activity |= handleKey(keyD, 'd');

  if (activity) {
    speed = min(speed + 1, 12);  
    lastInputTime = millis();
  }

  if (millis() - lastInputTime > 200) {
    if (speed > 1) speed--;
  }

  animateRGB();
  delay(10);
}

bool handleKey(int pin, char key) {
  static bool lastState[12];
  bool current = digitalRead(pin);

  if (lastState[pin] == HIGH && current == LOW) {
    Keyboard.press(key);
    lastState[pin] = LOW;
    return true;
  }

  if (lastState[pin] == LOW && current == HIGH) {
    Keyboard.release(key);
    lastState[pin] = HIGH;
  }

  return false;
}

void animateRGB() {
  hue += speed;

  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(hue * 256)));
  }

  strip.show();
}
