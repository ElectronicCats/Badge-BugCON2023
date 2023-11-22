#include "Menu.h"

#include "menu_helper.h"

#ifdef RP2040
ezButton buttonLeft(BUTTON_LEFT);
ezButton buttonRight(BUTTON_RIGHT);
#else
ezTouch buttonLeft(TOUCH_LEFT);
ezTouch buttonRight(TOUCH_RIGHT);
#endif

// 'logo', 128x32px
// Created with GIMP and converted with https://javl.github.io/image2cpp/
const unsigned char epd_bitmap_logo[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x1b, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x3c, 0x70, 0x03, 0xf8, 0x18, 0x18, 0xff, 0xc7, 0xfe, 0x3f, 0xf1, 0xfe, 0x00, 0x00,
    0x00, 0x03, 0x73, 0x99, 0x83, 0xf8, 0x18, 0x18, 0xff, 0xc7, 0xfe, 0x3f, 0xf1, 0xfe, 0x00, 0x00,
    0x00, 0x07, 0xe7, 0xcf, 0xc3, 0x18, 0x18, 0x18, 0xc0, 0x06, 0x00, 0x30, 0x31, 0xc1, 0xc0, 0x00,
    0x00, 0x0f, 0xc7, 0xc7, 0xe3, 0x18, 0x18, 0x18, 0xc0, 0x06, 0x00, 0x30, 0x31, 0xc1, 0xc0, 0x00,
    0x00, 0x0e, 0xd7, 0xd6, 0x63, 0x18, 0x18, 0x18, 0xc0, 0x06, 0x00, 0x30, 0x31, 0xc1, 0xc0, 0x00,
    0x00, 0x04, 0xb7, 0xda, 0x03, 0xf8, 0x18, 0x18, 0xc0, 0xc6, 0x00, 0x30, 0x31, 0xc1, 0xc0, 0x00,
    0x00, 0x00, 0x3b, 0xb8, 0x03, 0xf8, 0x18, 0x18, 0xc0, 0xc6, 0x00, 0x30, 0x31, 0xc1, 0xc0, 0x00,
    0x00, 0x00, 0xa4, 0x4a, 0x03, 0x07, 0x18, 0x18, 0xc0, 0xc6, 0x00, 0x30, 0x31, 0xc1, 0xc0, 0x00,
    0x00, 0x00, 0xc7, 0xc3, 0x03, 0x03, 0x18, 0x18, 0xc0, 0xc6, 0x00, 0x30, 0x31, 0xc1, 0xc0, 0x00,
    0x00, 0x01, 0xcf, 0xe7, 0x03, 0x03, 0x18, 0x18, 0xc0, 0xc6, 0x00, 0x30, 0x31, 0xc1, 0xc0, 0x00,
    0x00, 0x03, 0xed, 0x6f, 0x83, 0xff, 0x1f, 0xf8, 0xff, 0xc7, 0xfe, 0x3f, 0xf1, 0xc1, 0xc0, 0x00,
    0x00, 0x07, 0x33, 0x99, 0xc3, 0xff, 0x1f, 0xf8, 0xff, 0xc7, 0xfe, 0x3f, 0xf1, 0xc1, 0xc0, 0x00,
    0x00, 0x06, 0x1c, 0x70, 0xc3, 0xff, 0x1f, 0xf8, 0xff, 0xc7, 0xfe, 0x3f, 0xf1, 0xc1, 0xc0, 0x00,
    0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xbe, 0xf7, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa2, 0x10, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xaa, 0xf1, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x22, 0x80, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xbe, 0xf7, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

Menu::Menu() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
               pixels(NUMPIXELS, NEOPIXELS_PIN, NEO_GRB + NEO_KHZ800) {}

void Menu::begin() {
  this->talkLineIndex = 0;
  this->terminalEnabled = false;
  this->terminalEnabledFlag = false;
  pixels.begin();
  ledsOff();
#if defined(ESP32_S3)
  // Disable old left touch pin
  pinMode(OLD_TOUCH_LEFT, INPUT);
#endif

  debug.begin(9600);
  debug.waitForSerialConnection();  // For testing purposes
  // debug.disable();  // Disable debug for production
  debug.println("Board name: " + String(BOARD_NAME));
  speaker.setTalk(4);  // Value from 0 to 28, check UartCommunication.cpp
  vip.begin();
  // vip.ereaseFlash();  // Uncomment to erease conference list
  debug.println("ID: " + String(speaker.getID()));

#if defined(ESP32_DEVKIT) || defined(ESP32_S3)
  debug.println("Touch left pin: " + String(TOUCH_LEFT));
  debug.println("Touch right pin: " + String(TOUCH_RIGHT));
  debug.println("SDA pin: " + String(SDA));
  debug.println("SCL pin: " + String(SCL));
  debug.println("TX: " + String(TX));
  debug.println("RX: " + String(RX));
#elif defined(ARDUINO_ARCH_MBED_RP2040)
  debug.println("Button left pin: " + String(BUTTON_LEFT));
  debug.println("Button right pin: " + String(BUTTON_RIGHT));
  debug.println("SDA pin: " + String(I2C_SDA));
  debug.println("SCL pin: " + String(I2C_SCL));
  debug.println("TX: " + String(SERIAL1_TX));
  debug.println("RX: " + String(SERIAL1_RX));
#endif

  buttonLeft.setDebounceTime(DEBOUNCE_TIME_MS);
  buttonRight.setDebounceTime(DEBOUNCE_TIME_MS);

#if defined(ARDUINO_ARCH_RP2040)
#ifndef ARDUINO_ARCH_MBED_RP2040
  Wire.setSDA(12);
  Wire.setSCL(13);
#endif
#endif

#if defined(MININO)
  debug.println("MININO");
  if (!display.begin(SCREEN_ADDRESS)) {
    Serial.println(F("SH110X allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
#else
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    debug.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
#endif

  // Display logo
  display.clearDisplay();
  display.drawBitmap(0, 0, epd_bitmap_logo, 128, 64, WHITE);
  display.display();
  ledsAnimation();

  showVMenu();
  fillTalksList();
}

void Menu::ledsAnimation() {
#ifdef RP2040
  // Order 0, 1, 2, 3
  for (uint8_t i = 0; i < 4; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 255));
    pixels.show();
    delay(250);
  }
  delay(250);

  // Order 3, 2, 1, 0
  for (uint8_t i = 3; i > 0; i--) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
    delay(250);
  }
#else
  // Order 2, 1, 0, 3
  pixels.setPixelColor(2, pixels.Color(0, 0, 255));
  pixels.show();
  delay(250);
  pixels.setPixelColor(1, pixels.Color(0, 0, 255));
  pixels.show();
  delay(250);
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show();
  delay(250);
  pixels.setPixelColor(3, pixels.Color(0, 0, 255));
  pixels.show();
  delay(250);

  // Order 3, 0, 1, 2
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
  pixels.show();
  delay(250);
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
  delay(250);
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.show();
  delay(250);
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.show();
  delay(250);
#endif

  ledsOff();
}

void Menu::scanKeys() {
  buttonLeft.loop();
  buttonRight.loop();
  static unsigned long buttonLeftPressedTime = 0;
  static unsigned long buttonRightPressedTime = 0;
  static bool buttonLeftPressed = false;
  static bool buttonRightPressed = false;
  static bool leftLongClickDetected = false;
  static bool rightLongClickDetected = false;

  animateLeftLongPress(leftLongClickDetected);
  animateRightLongPress(rightLongClickDetected);
  updatePreviousLayer();

  if (buttonLeft.isPressed()) {
    debug.println("Left button pressed");
    buttonLeftPressedTime = millis();
    buttonLeftPressed = true;
  }

  if (buttonRight.isPressed()) {
    debug.println("Right button pressed");
    buttonRightPressedTime = millis();
    buttonRightPressed = true;
  }

  // Verify left long click
  if (buttonLeftPressed && !leftLongClickDetected && (millis() - buttonLeftPressedTime > LONG_CLICK_TIME_MS)) {
    debug.println("Left button long pressed");
    leftLongClickDetected = true;
    handleBackButton();
  }

  // Verify right long click
  if (buttonRightPressed && !rightLongClickDetected && (millis() - buttonRightPressedTime > LONG_CLICK_TIME_MS)) {
    debug.println("Right button long pressed");
    rightLongClickDetected = true;
    handleSelection();
  }

  if (buttonLeft.isReleased()) {
    debug.println("Left button released");
    buttonLeftPressed = false;
    if (leftLongClickDetected) {
      leftLongClickDetected = false;
      return;
    }
    selectedOption--;

    if (selectedOption == 255)  // Underflow
      selectedOption = 0;

    showMenu();
  }

  if (buttonRight.isReleased()) {
    debug.println("Right button released");
    buttonRightPressed = false;
    if (rightLongClickDetected) {
      rightLongClickDetected = false;
      return;
    }
    selectedOption++;

    if (selectedOption > optionsSize - 1)
      selectedOption = optionsSize - 1;

    showMenu();
  }
}

void Menu::loop() {
  scanKeys();

  // Print debug info every 1 second
  static unsigned long lastDebugPrint = 0;
  if (millis() - lastDebugPrint > 1000) {
    lastDebugPrint = millis();
    // debug.println("Left button state: " + String(buttonLeft.getStateRaw()));
    // debug.println("Right button state: " + String(buttonRight.getStateRaw()));
    // debug.println("Orientation: " + String(menuOrientation == VERTICAL_MENU ? "Vertical" : "Horizontal"));
    // debug.println("Selected option: " + String(selectedOption));
    // debug.println("Current layer: " + String(currentLayer));
  }

  if (speaker.isCommunicationEnabled()) {
    speaker.sendTalkName();
  }

  if (vip.isCommunicationEnabled()) {
    vip.receiveTalkName();
  }

  if (vip.receivedSuccess) {
    vip.receivedSuccess = false;
    currentLayer = LAYER_CONFERENCE_PAIRING_SUCCESS_BANNER;
    selectedOption = 0;
    vip.disableCommunication();
    speaker.disableCommunication();
    updateOrientation();
    showMenu();
  }

  // If currentLayer is LAYER_TERMINAL, then enable terminal after 1 second
  if (currentLayer == LAYER_TERMINAL && millis() - startTerminalTime > 1000 && terminalEnabledFlag) {
    enableTerminal();
    terminalEnabledFlag = false;
  }
}

void Menu::showVMenu() {
  char **options = updateVMenuOptions();
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  uint8_t startIdx = (selectedOption >= 4) ? selectedOption - 3 : 0;

  for (uint16_t i = startIdx; i < optionsSize; i++) {
    debug.println(options[i]);
    if (i == selectedOption) {
      display.setTextColor(BLACK, WHITE);
      display.println(options[i]);
    } else {
      display.setTextColor(WHITE);
      display.println(options[i]);
    }
  }

  display.display();
}

void Menu::showHMenu() {
  char **banner = updateHMenuBanner();
  char **options = updateHMenuOptions();

  // Structure: 64x32 pixels
  /*
   *  -------------------------
   * |         Banner         |
   * |------------------------|
   * |   Aceptar   Cancelar   |
   * -------------------------
   */

  display.clearDisplay();
  display.setTextSize(1);

  // Show banner
  for (uint8_t i = 0; i < bannerSize; i++) {
    display.setCursor(0, i * 8);
    display.setTextColor(WHITE);
    display.println(banner[i]);
  }

  if (optionsSize == 1) {
    display.setCursor(38, 24);
    display.setTextColor(BLACK, WHITE);
    display.println(options[0]);
  } else if (optionsSize == 2) {
    if (selectedOption == 0) {
      display.setCursor(0, 24);
      display.setTextColor(BLACK, WHITE);
      display.println(options[0]);
      display.setCursor(64, 24);
      display.setTextColor(WHITE);
      display.println(options[1]);
    } else {
      display.setCursor(0, 24);
      display.setTextColor(WHITE);
      display.println(options[0]);
      display.setCursor(64, 24);
      display.setTextColor(BLACK, WHITE);
      display.println(options[1]);
    }
  }
}

void Menu::showMenu() {
  if (menuOrientation == VERTICAL_MENU) {
    showVMenu();
  } else {
    showHMenu();
  }
}

char **Menu::updateVMenuOptions() {
  char **options;

  switch (currentLayer) {
    case LAYER_MAIN_MENU:
      options = mainOptions;
      optionsSize = sizeof(mainOptions) / sizeof(mainOptions[0]);
      break;
    case LAYER_LEDS_MENU:
      options = ledsOptions;
      optionsSize = sizeof(ledsOptions) / sizeof(ledsOptions[0]);
      break;
    case LAYER_CONFERENCE_MENU:
      updateConferenceCounter();
      options = conferenceOptions;
      optionsSize = sizeof(conferenceOptions) / sizeof(conferenceOptions[0]);
      break;
    case LAYER_CONFERENCE_LIST:
      fillTalksList();
      options = conferenceList;
      optionsSize = sizeof(conferenceList) / sizeof(conferenceList[0]);
      break;
    case LAYER_INFO_MENU:
      options = infoMenu;
      optionsSize = sizeof(infoMenu) / sizeof(infoMenu[0]);
      break;
    case LAYER_SERVER_MENU:
      updateWiFiParams();
      options = serverOptions;
      optionsSize = sizeof(serverOptions) / sizeof(serverOptions[0]);
      break;
    default:
      options = errorBanner;
      optionsSize = sizeof(errorBanner) / sizeof(errorBanner[0]);
      debug.println("Unknown vertical layer: " + String(currentLayer));
      break;
  }

  return options;
}

char **Menu::updateHMenuBanner() {
  char **banner;

  switch (currentLayer) {
    case LAYER_CONFERENCE_PAIRING_BANNER:
      banner = pairingBanner;
      bannerSize = sizeof(pairingBanner) / sizeof(pairingBanner[0]);
      break;
    case LAYER_CONFERENCE_HELP_BANNER:
      banner = conferenceHelpBanner;
      bannerSize = sizeof(conferenceHelpBanner) / sizeof(conferenceHelpBanner[0]);
      break;
    case LAYER_CONFERENCE_PAIRING_SUCCESS_BANNER:
      banner = pairingSuccessBanner;
      bannerSize = sizeof(pairingSuccessBanner) / sizeof(pairingSuccessBanner[0]);
      break;
    case LAYER_TERMINAL:
      banner = terminalBanner;
      bannerSize = sizeof(terminalBanner) / sizeof(terminalBanner[0]);
      break;
    default:
      banner = errorBanner;
      bannerSize = sizeof(errorBanner) / sizeof(errorBanner[0]);
      debug.println("Unknown layer: " + String(currentLayer));
      break;
  }

  return banner;
}

char **Menu::updateHMenuOptions() {
  char **options;

  switch (currentLayer) {
    case LAYER_CONFERENCE_PAIRING_BANNER:
    case LAYER_TERMINAL:
      options = oneOption;
      optionsSize = 0;  // Don't show options
      break;
    case LAYER_CONFERENCE_HELP_BANNER:
    case LAYER_CONFERENCE_PAIRING_SUCCESS_BANNER:
      options = oneOption;
      optionsSize = sizeof(oneOption) / sizeof(oneOption[0]);
      break;
    default:
      options = oneOption;
      optionsSize = 0;
      break;
  }

  return options;
}

void Menu::animateLeftLongPress(bool longClick) {
  static bool animation = false;
  static unsigned long lastTime = 0;
  static unsigned long lastClickTime = 0;
  static int16_t i = 0;
  static uint8_t lastButtonState = buttonLeft.getState();

  if (!buttonLeft.getState() && !longClick) {
    animation = true;
    if (lastButtonState != buttonLeft.getState()) {
      lastButtonState = buttonLeft.getState();
      lastClickTime = millis();
    }
  } else if (longClick || buttonLeft.getState()) {
    lastButtonState = buttonLeft.getState();
    animation = false;
    i = 0;
    display.drawLine(0, 31, 127, 31, BLACK);
    display.display();
  }

  if (!animation) return;

  if (millis() - lastClickTime > BACK_ANIMATION_TIME_MS) {
    if (millis() - lastTime > 1) {
      lastTime = millis();
      i -= 10;
      if (i < 0) {
        i = 127;
        animation = false;
      }
      display.drawLine(i, 31, 127, 31, WHITE);
      display.display();
    }
  }
}

void Menu::animateRightLongPress(bool longClick) {
  static bool animation = false;
  static unsigned long lastTime = 0;
  static unsigned long lastClickTime = 0;
  static int16_t i = 0;
  static uint8_t lastButtonState = buttonRight.getState();

  if (!buttonRight.getState() && !longClick) {
    animation = true;
    if (lastButtonState != buttonRight.getState()) {
      lastButtonState = buttonRight.getState();
      lastClickTime = millis();
    }
  } else if (longClick || buttonRight.getState()) {
    lastButtonState = buttonRight.getState();
    animation = false;
    i = 0;
    display.drawLine(0, 31, 127, 31, BLACK);
    display.display();
  }

  if (!animation) return;

  if (millis() - lastClickTime > BACK_ANIMATION_TIME_MS) {
    if (millis() - lastTime > 1) {
      lastTime = millis();
      i += 10;
      if (i > 127) {
        i = 0;
        animation = false;
      }
      display.drawLine(0, 31, i, 31, WHITE);
      display.display();
    }
  }
}

void Menu::handleSelection() {
  switch (currentLayer) {
    case LAYER_MAIN_MENU:
      mainMenu();
      break;
    case LAYER_LEDS_MENU:
      ledsMenu();
      break;
    case LAYER_CONFERENCE_MENU:
      talksMenu();
      break;
    case LAYER_CONFERENCE_HELP_BANNER:
      conferenceHelp();
      break;
    case LAYER_CONFERENCE_PAIRING_SUCCESS_BANNER:
      conferenceSuccess();
      break;
    default:
      break;
  }

  selectedOption = 0;
  updateOrientation();
  showMenu();
}

void Menu::updatePreviousLayer() {
  switch (currentLayer) {
    case LAYER_MAIN_MENU:
    case LAYER_LEDS_MENU:
    case LAYER_CONFERENCE_MENU:
    case LAYER_TERMINAL:
    case LAYER_INFO_MENU:
    case LAYER_SERVER_MENU:
      previousLayer = LAYER_MAIN_MENU;
      break;
    case LAYER_CONFERENCE_PAIRING_BANNER:
    case LAYER_CONFERENCE_LIST:
    case LAYER_CONFERENCE_HELP_BANNER:
      previousLayer = LAYER_CONFERENCE_MENU;
      break;
    case LAYER_CONFERENCE_PAIRING_SUCCESS_BANNER:
      fillTalksList();
      previousLayer = LAYER_CONFERENCE_LIST;
      break;
    default:
      debug.println("Unknown layer: " + String(currentLayer));
      previousLayer = LAYER_MAIN_MENU;
      break;
  }
}

void Menu::handleBackButton() {
  switch (currentLayer) {
    case LAYER_CONFERENCE_PAIRING_BANNER:
      speaker.disableCommunication();
      vip.disableCommunication();
      break;
    case LAYER_TERMINAL:
      disableTerminal();
      break;
    default:
      break;
  }

  currentLayer = previousLayer;
  selectedOption = 0;
  updateOrientation();
  showMenu();
}

void Menu::updateOrientation() {
  debug.println("Updating orientation for layer: " + String(currentLayer));

  switch (currentLayer) {
    case LAYER_CONFERENCE_PAIRING_BANNER:
    case LAYER_CONFERENCE_HELP_BANNER:
    case LAYER_CONFERENCE_PAIRING_SUCCESS_BANNER:
    case LAYER_TERMINAL:
      menuOrientation = HORIZONTAL_MENU;
      break;
    default:  // Most of the menus are vertical
      menuOrientation = VERTICAL_MENU;
      break;
  }
}

void Menu::mainMenu() {
  switch (selectedOption) {
    case MAIN_MENU_DINO:
      gameSetup();
      break;
    case MAIN_MENU_LEDS:
      currentLayer = LAYER_LEDS_MENU;
      break;
    case MAIN_MENU_CONFERENCE:
      currentLayer = LAYER_CONFERENCE_MENU;
      break;
    case MAIN_MENU_INFO:
      currentLayer = LAYER_INFO_MENU;
      break;
#if defined(RP2040)
    case MAIN_MENU_TERMINAL:
      currentLayer = LAYER_TERMINAL;
      startTerminalTime = millis();
      terminalEnabledFlag = true;
      break;
#else
    case MAIN_MENU_AIRTAG:
      airTagsMenu();
      break;
    case MAIN_MENU_WEB_SERVER:
      currentLayer = LAYER_SERVER_MENU;
      break;
#endif
    default:
      break;
  }
}

void Menu::ledsOff() {
  pixels.clear();
  pixels.show();
}

void Menu::ledsMenu() {
  switch (selectedOption) {
    case LEDS_MENU_OFF:
      ledsOff();
      break;
    case LEDS_MENU_RED:
      pixels.clear();
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      }
      pixels.show();
      break;
    case LEDS_MENU_GREEN:
      pixels.clear();
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      }
      pixels.show();
      break;
    case LEDS_MENU_BLUE:
      pixels.clear();
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      }
      pixels.show();
      break;
    case LEDS_MENU_YELLOW:
      pixels.clear();
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 255, 0));
      }
      pixels.show();
      break;
    case LEDS_MENU_PINK:
      pixels.clear();
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(100, 0, 100));
      }
      pixels.show();
      break;
    case LEDS_MENU_CYAN:
      pixels.clear();
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 255, 255));
      }
      pixels.show();
      break;
    case LEDS_MENU_WHITE:
      pixels.clear();
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 255, 255));
      }
      pixels.show();
      break;
    default:
      break;
  }
}

void Menu::airTagsMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println();
  display.setCursor(64 - 3 * 6, 10);
  display.println("Emulando");
  display.setCursor(64 - 3 * 4, 20);
  display.println("AirTag");
  display.display();

#if defined(ESP32_DEVKIT) || defined(ESP32_S3)
  debug.println("Starting BLE advertising");
  airTag.start();
#endif

  for (;;) {
    buttonLeft.loop();

    if (buttonLeft.isPressed()) {
      break;
    }

#if defined(ESP32_DEVKIT) || defined(ESP32_S3)
    airTag.loop();
#endif
  }

#if defined(ESP32_DEVKIT) || defined(ESP32_S3)
  debug.println("Stopping BLE advertising");
  airTag.stop();
#endif
}

void Menu::talksMenu() {
  switch (selectedOption) {
    case PAIR_MENU_START:
      speaker.enableCommunication();
      vip.enableCommunication();
      currentLayer = LAYER_CONFERENCE_PAIRING_BANNER;
      break;
    case PAIR_MENU_CONFERENCES:
      currentLayer = LAYER_CONFERENCE_LIST;
      break;
    case PAIR_MENU_HELP:
      currentLayer = LAYER_CONFERENCE_HELP_BANNER;
      break;
    default:
      break;
  }
}

void Menu::conferenceHelp() {
  switch (selectedOption) {
    case OK:
      currentLayer = LAYER_CONFERENCE_MENU;
      break;
    default:
      break;
  }
}

void Menu::conferenceSuccess() {
  switch (selectedOption) {
    case OK:
      currentLayer = LAYER_CONFERENCE_LIST;
      break;
    default:
      break;
  }
}

void Menu::updateConferenceCounter() {
#ifdef RP2040
  uint8_t counter = vip.talksCounter;
  char *counterStr = (char *)malloc(40);
  sprintf(counterStr, "3. Conferencias: %d", counter);
  conferenceOptions[PAIR_MENU_CONFERENCE_COUNTER] = counterStr;
  debug.println(String(conferenceOptions[PAIR_MENU_CONFERENCE_COUNTER]));
#endif
}

void Menu::fillTalksList() {
  std::vector<String> talksList = vip.getTalkList();
  // debug.println("Talk list size: " + String(talksList.size()));
  static uint8_t previousTalkListSize = 0;

  if (talksList.size() == previousTalkListSize) return;

  previousTalkListSize = talksList.size();

  // Fill conferenceList with talksList
  // size_t i = talksList.size() - 1;  // Start from the last talk
  size_t i = 0;  // Start from the first talk
  // talkLineIndex = 0;
  uint8_t lineIndex = 0;
  for (i = i; i < talksList.size(); i++) {
    uint8_t chunksNumber = ceil(static_cast<float>(talksList[i].length()) / CHARS_PER_LINE);
    // debug.println("Chunks number: " + String(chunksNumber));
    // debug.println("talk size: " + String(talksList[i].length()));

    if (chunksNumber == 1) {
      char *talkLine = (char *)malloc(150);
      sprintf(talkLine, "%d. %s", i + 1, talksList[i].c_str());
      conferenceList[lineIndex] = talkLine;
      debug.println("Talk: " + String(i + 1) + String(conferenceList[lineIndex]));
      talkLineIndex += 2;
      lineIndex += 2;
      continue;
    }

    // Get first line
    char *firstLine = (char *)malloc(150);
    sprintf(firstLine, "%d. %s", i + 1, talksList[i].substring(0, CHARS_PER_LINE - 3).c_str());
    conferenceList[lineIndex] = firstLine;
    debug.println("Talk: " + String(conferenceList[lineIndex]));

    // Get remaining lines
    for (size_t j = 1; j < chunksNumber; j++) {
      talkLineIndex++;
      lineIndex++;
      char *talkLine = (char *)malloc(150);
      uint8_t from = j * CHARS_PER_LINE - 3;
      uint8_t to = j * CHARS_PER_LINE + CHARS_PER_LINE - 3;
      // debug.println("From: " + String(from));
      // debug.println("To: " + String(to));
      to = to > talksList[i].length() ? talksList[i].length() : to;

      sprintf(talkLine, "%s", talksList[i].substring(from, to).c_str());
      conferenceList[lineIndex] = talkLine;
      debug.println("Talk: " + String(conferenceList[lineIndex]));
    }

    talkLineIndex += 2;
    lineIndex += 2;
  }

  // Print conferenceList
  debug.println("Conference list:");
  for (size_t i = 0; i < talkLineIndex; i++) {
    debug.println("Talk: " + String(conferenceList[i]));
  }
}

bool Menu::isTerminalEnabled() {
  return this->terminalEnabled;
}

void Menu::enableTerminal() {
  this->terminalEnabled = true;
}

void Menu::disableTerminal() {
  this->terminalEnabled = false;
}

void Menu::updateWiFiParams() {
  char *ssid = (char *)malloc(40);
  char *password = (char *)malloc(40);
  char *ip = (char *)malloc(40);
  sprintf(ssid, "SSID: %s", speaker.getSSID().c_str());
  sprintf(password, "Password: %s", speaker.getPassword().c_str());
  sprintf(ip, "IP: %s", speaker.getIP().c_str());
  serverOptions[SERVER_MENU_SSID] = ssid;
  serverOptions[SERVER_MENU_PASSWORD] = password;
  serverOptions[SERVER_MENU_IP] = ip;
}

/**
 * Author:    Harsh Mittal
 * Created:   10.07.2021
 * Github: www.github.com/harshmittal2210
 * Email: harshmittal2210@gmail.com
 *
 * (c) Copyright by Harsh Mittal.
 **/

static const unsigned char PROGMEM dino1[] = {
    // 'dino', 25x26px
    0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x06, 0xff, 0x00, 0x00, 0x0e, 0xff, 0x00,
    0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xc0, 0x00,
    0x00, 0x0f, 0xfc, 0x00, 0x40, 0x0f, 0xc0, 0x00, 0x40, 0x1f, 0x80, 0x00, 0x40, 0x7f, 0x80, 0x00,
    0x60, 0xff, 0xe0, 0x00, 0x71, 0xff, 0xa0, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x7f, 0xff, 0x80, 0x00,
    0x7f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00,
    0x03, 0xfc, 0x00, 0x00, 0x01, 0xdc, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00,
    0x01, 0x0c, 0x00, 0x00, 0x01, 0x8e, 0x00, 0x00};

static const unsigned char PROGMEM tree1[] = {
    // 'tree1', 11x23px
    0x1e, 0x00, 0x1f, 0x00, 0x1f, 0x40, 0x1f, 0xe0, 0x1f, 0xe0, 0xdf, 0xe0, 0xff, 0xe0, 0xff, 0xe0,
    0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xff, 0xc0, 0xff, 0x00, 0xff, 0x00, 0x7f, 0x00,
    0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00};

static const unsigned char PROGMEM tree2[] = {
    // 'tree2', 22x23px
    0x1e, 0x01, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x4f, 0xe8, 0x1f, 0xff, 0xfc, 0x1f, 0xff, 0xfc, 0xdf,
    0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff,
    0xfc, 0xff, 0xef, 0xfc, 0xff, 0x83, 0xfc, 0xff, 0x03, 0xfc, 0xff, 0x03, 0xf8, 0x7f, 0x03, 0xe0,
    0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f,
    0x03, 0xe0, 0x1f, 0x03, 0xe0};

void Menu::gameSetup() {
  // Clear the buffer
  display.clearDisplay();

  introMessage();
  // Run game in loop
  while (1) {
    buttonRight.loop();
    buttonLeft.loop();

    if (buttonRight.isPressed()) {
      bool continuePlaying = play();
      if (!continuePlaying) break;
    }

    if (buttonLeft.isPressed()) {
      break;
    }
  }
}

void Menu::introMessage() {
  display.setTextSize(1);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30, 5);
  display.println("Dino Chrome");

  display.setTextSize(1);

  //   display.setCursor(5,45);
  display.println("Presiona seleccionar");
  display.println("     para jugar");

  display.display();
}

// Move dino function
void Menu::moveDino(int16_t *y, int type) {
  display.drawBitmap(DINO_INIT_X, *y, dino1, DINO_WIDTH, DINO_HEIGHT, SSD1306_WHITE);
}

// Move tree funcitons
void Menu::moveTree(int16_t *x, int type) {
  if (type == 0) {
    display.drawBitmap(*x, TREE_Y, tree1, TREE1_WIDTH, TREE1_HEIGHT, SSD1306_WHITE);
  } else if (type == 1) {
    display.drawBitmap(*x, TREE_Y, tree2, TREE2_WIDTH, TREE2_HEIGHT, SSD1306_WHITE);
  }
}

// Game over display with score
void Menu::gameOver(int score) {
  display.clearDisplay();

  display.setTextSize(1);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 5);
  display.println("Juego terminado");

  //   display.setTextSize(1);

  //   display.setCursor(10,30);
  display.println("Puntaje: " + String(score));
  //   display.print(score);

  //   display.setCursor(1,45);
  display.println("Presiona seleccionar");

  display.display();
}

// Display score while running the game
void Menu::displayScore(int score) {
  display.setTextSize(1);
  display.setCursor(60, 10);
  display.print("Puntaje: ");
  display.print(score);
}

// Main play function
bool Menu::play() {
  bool continuePlaying = true;
  int16_t tree_x = 127;
  int16_t tree1_x = 195;
  int tree_type = random(0, 2);
  int tree_type1 = random(0, 2);

  int16_t dino_y = DINO_INIT_Y;
  int input_command;
  int jump = 0;

  int score = 0;

  for (;;) {
    display.clearDisplay();
    buttonRight.loop();
    buttonLeft.loop();

    if (buttonLeft.isPressed()) {
      continuePlaying = false;
    }

    if (buttonRight.isPressed()) {
      Serial.println("Jump");
      if (jump == 0) jump = 1;
    }

    if (jump == 1) {
      dino_y--;
      if (dino_y == (DINO_INIT_Y - JUMP_PIXEL)) {
        jump = 2;
        score++;
      }
    } else if (jump == 2) {
      dino_y++;
      if (dino_y == DINO_INIT_Y) {
        jump = 0;
      }
    }

    if (tree_x <= (DINO_INIT_X + DINO_WIDTH) && tree_x >= (DINO_INIT_X + (DINO_WIDTH / 2))) {
      //      Serial.println("Might be Collision Happend");
      if (dino_y >= (DINO_INIT_Y - 3)) {
        // Collision Happened
        Serial.println("Collision Happend");
        break;
      }
    }

    if (tree1_x <= (DINO_INIT_X + DINO_WIDTH) && tree1_x >= (DINO_INIT_X + (DINO_WIDTH / 2))) {
      //      Serial.println("Might be Collision Happend");
      if (dino_y >= (DINO_INIT_Y - 3)) {
        // Collision Happened
        Serial.println("Collision Happend");
        break;
      }
    }

    displayScore(score);
    moveTree(&tree_x, tree_type);
    moveTree(&tree1_x, tree_type1);
    moveDino(&dino_y);
    // display.drawLine(0, 54, 127, 54, SSD1306_WHITE);
    display.drawLine(0, 31, 127, 31, SSD1306_WHITE);

    tree_x--;
    tree1_x--;
    if (tree_x == 0) {
      tree_x = 127;
      tree_type = random(0, 1);
    }

    if (tree1_x == 0) {
      tree1_x = 195;
      tree_type1 = random(0, 1);
    }
    display.display();
  }

  Serial.println("Game Over");
  gameOver(score);

  return continuePlaying;
}

void Menu::renderScene(int16_t i) {
  display.drawBitmap(10, 29, dino1, 25, 26, SSD1306_WHITE);
  display.drawBitmap(50, TREE_Y, tree1, 11, 23, SSD1306_WHITE);
  display.drawBitmap(100, TREE_Y, tree2, 22, 23, SSD1306_WHITE);
  display.drawLine(0, 54, 127, 54, SSD1306_WHITE);
  display.drawPixel(i, 60, SSD1306_WHITE);
}
