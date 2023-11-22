#ifndef MENU_H
#define MENU_H

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <ezButton.h>
#include <cmath>

#include "debug.h"
#include "ezTouch.h"
#include "hardware.h"
#include "AirTag.h"
#include "UartCommunication.h"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C ,for 128x32

// #define WHITE 1
// #define BLACK 0
#define VERTICAL_MENU 0
#define HORIZONTAL_MENU 1
#define CHARS_PER_LINE 21
#define OK 0
#define CANCEL 1

/* Dino Game */
#define DINO_WIDTH 25
#define DINO_HEIGHT 26
#define DINO_INIT_X 10  // Dino initial spawn location
// #define DINO_INIT_Y 29 // Dino initial spawn location
#define DINO_INIT_Y 5  // Nueva ubicación inicial del dinosaurio

// #define BASE_LINE_X 0
// #define BASE_LINE_Y 54
// #define BASE_LINE_X1 127
// #define BASE_LINE_Y1 54
#define BASE_LINE_X 0
#define BASE_LINE_Y 21
#define BASE_LINE_X1 127
#define BASE_LINE_Y1 21

#define TREE1_WIDTH 11
#define TREE1_HEIGHT 23

#define TREE2_WIDTH 22
#define TREE2_HEIGHT 23

// #define TREE_Y 35
#define TREE_Y 9

#define JUMP_PIXEL 22  // Number of pixel dino will jump
// #define JUMP_PIXEL 11

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM logo_bmp[] =
    {0b00000000, 0b11000000,
     0b00000001, 0b11000000,
     0b00000001, 0b11000000,
     0b00000011, 0b11100000,
     0b11110011, 0b11100000,
     0b11111110, 0b11111000,
     0b01111110, 0b11111111,
     0b00110011, 0b10011111,
     0b00011111, 0b11111100,
     0b00001101, 0b01110000,
     0b00011011, 0b10100000,
     0b00111111, 0b11100000,
     0b00111111, 0b11110000,
     0b01111100, 0b11110000,
     0b01110000, 0b01110000,
     0b00000000, 0b00110000};

class Menu {
 private:
#ifdef MININO
  Adafruit_SH1106G display;
#else
  Adafruit_SSD1306 display;
#endif
#if defined(ESP32_DEVKIT) || defined(ESP32_S3)
  Airtag airTag;
#endif
  bool terminalEnabled;
  bool terminalEnabledFlag;
  unsigned long startTerminalTime;
  Debug& debug = Debug::getInstance();
  UartCommunication speaker;
  UartCommunication vip;
  uint8_t selectedOption;
  uint8_t previousLayer;
  uint8_t currentLayer;
  uint8_t optionsSize;
  uint8_t bannerSize;
  uint8_t menuOrientation;
  uint16_t talkLineIndex;
  void ledsAnimation();
  void scanKeys();
  void showVMenu();
  void showHMenu();
  void showMenu();
  char **updateVMenuOptions();
  char **updateHMenuBanner();
  char **updateHMenuOptions();
  void animateLeftLongPress(bool longClick);
  void animateRightLongPress(bool longClick);
  void handleSelection();
  void updatePreviousLayer();
  void handleBackButton();
  void updateOrientation();
  void mainMenu();
  void ledsMenu();
  void ledsOff();
  void airTagsMenu();
  void talksMenu();
  void conferenceHelp();
  void conferenceSuccess();
  void updateConferenceCounter();
  void fillTalksList();
  void enableTerminal();
  // Dino Game
  void gameSetup();
  void introMessage();
  void moveDino(int16_t *y, int type = 0);
  void moveTree(int16_t *x, int type = 0);
  void gameOver(int score = 0);
  void displayScore(int score);
  bool play();
  void renderScene(int16_t i = 0);

 public:
  Menu();
  Adafruit_NeoPixel pixels;
  void begin();
  void loop();
  bool isTerminalEnabled();
  void disableTerminal();
};

#endif