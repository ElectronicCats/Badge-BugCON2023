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
#include "terminal.h"
// #include "menu.h"

// Menu menu;

void setup() {
  menu.begin();
}

void loop() {
  menu.loop();
#ifdef RP2040
  if (menu.isTerminalEnabled()) {
    terminalSetup();
    terminalLoop();
    menu.disableTerminal();
  }
#endif
}
