#include "Menu.h"

#define NEOPIXELS_PIN 19
#define NUMPIXELS 4

Menu menu;

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXELS_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  menu.begin();
  pixels.begin();
}

void loop() {
  menu.loop();
}

// void serialTest() {
//   serialSend();
//   serialRead();
// }

// void serialSend() {
//   static unsigned long lastTime = millis();
//   static int counter = 0;

//   if (millis() - lastTime > 1000) {
//     Serial.println("Hello " + String(counter++));
// #ifdef RP2040
//     Serial2.println("Hello " + String(counter));
// #endif
//     lastTime = millis();
//   }
// }

// void serialRead() {
// #ifdef RP2040
//   if (Serial2.available()) {
//     String input = Serial2.readStringUntil('\n');
// #else
//   if (Serial.available()) {
//     String input = Serial.readStringUntil('\n');
// #endif

//     if (input.length() == 0) {
//       Serial.println("Empty input");
//       return;
//     }

//     Serial.println("Received: " + input);
//     delay(1000);
//   }
// }
