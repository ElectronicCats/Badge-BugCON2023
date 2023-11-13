/*********************************************************************************
  Firmware for BugCON Badge 2023
  by Francisco Torres, Electronic Cats (https://electroniccats.com/)
  Date: 2023/11/09

  This program demonstrates how to use several features of the BugCON Badge 2023
  by Electronic Cats
  https://github.com/ElectronicCats/Badge-BugCON2023.git

  Development environment specifics:
  IDE: Visual Studio Code + Arduino CLI Version 0.34.2
  Hardware Platform:
  BugCON Badge, VIP edition
  - RP2040
  BugCON Badge, Speaker edition
  - ESP32-S3

  Electronic Cats invests time and resources providing this open source code,
  please support Electronic Cats and open-source hardware by purchasing
  products from Electronic Cats!

  This code is beerware; if you see me (or any other Electronic Cats
  member) at the local, and you've found our code helpful,
  please buy us a round!
  Distributed as-is; no warranty is given.
***********************************************************************************/
#include "Menu.h"

Menu menu;

void setup() {
  menu.begin();
}

void loop() {
  menu.loop();
  // if (touchRead(11) > 50000) {
  //   Serial.println("Touched!");
  //   Serial.println(touchRead(11) > 50000);
  //   delay(1000);
  // }
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
