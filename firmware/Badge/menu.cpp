#include "Menu.h"

#include "menu_helper.h"

#ifdef RP2040
ezButton buttonLeft(BUTTON_LEFT);
ezButton buttonRight(BUTTON_RIGHT);
#else
ezTouch buttonLeft(TOUCH_LEFT);
ezTouch buttonRight(TOUCH_RIGHT);
#endif

Menu::Menu() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
               pixels(NUMPIXELS, NEOPIXELS_PIN, NEO_GRB + NEO_KHZ800) {}

void Menu::begin() {
  pixels.begin();
  ledsOff();
  debug.begin(9600);
  debug.waitForSerialConnection();
  debug.println("Board name: " + String(BOARD_NAME));

#if defined(ESP32_DEVKIT) || defined(ESP32_S3)
  debug.println("Touch left pin: " + String(TOUCH_LEFT));
  debug.println("Touch right pin: " + String(TOUCH_RIGHT));
  debug.println("SDA pin: " + String(SDA));
  debug.println("SCL pin: " + String(SCL));
  debug.println("TX: " + String(TX));
  debug.println("RX: " + String(RX));
#else
  debug.println("Button left pin: " + String(BUTTON_LEFT));
  debug.println("Button right pin: " + String(BUTTON_RIGHT));
  debug.println("SDA pin: " + String(I2C_SDA));
  debug.println("SCL pin: " + String(I2C_SCL));
  debug.println("TX: " + String(SERIAL1_TX));
  debug.println("RX: " + String(SERIAL1_RX));
#endif

  buttonLeft.setDebounceTime(DEBOUNCE_TIME_MS);
  buttonRight.setDebounceTime(DEBOUNCE_TIME_MS);

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

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000);  // Pause for 2 seconds
  showVMenu();
}

void Menu::loop() {
  buttonLeft.loop();
  buttonRight.loop();
  static unsigned long buttonLeftPressedTime = 0;
  static unsigned long buttonRightPressedTime = 0;
  static bool buttonLeftPressed = false;
  static bool buttonRightPressed = false;
  static bool leftLongClickDetected = false;
  static bool rightLongClickDetected = false;

  // Print debug info every 1 second
  static unsigned long lastDebugPrint = 0;
  if (millis() - lastDebugPrint > 1000) {
    lastDebugPrint = millis();
    // debug.println("Left button state: " + String(buttonLeft.getStateRaw()));
    // debug.println("Right button state: " + String(buttonRight.getStateRaw()));
  }

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

    showVMenu();
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

    showVMenu();
  }
}

void Menu::showVMenu() {
  char **options = updateVMenuOptions();
  display.clearDisplay();
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  uint8_t startIdx = (selectedOption >= 4) ? selectedOption - 3 : 0;

  for (uint8_t i = startIdx; i < optionsSize; i++) {
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

void Menu::showMenu() {
  showVMenu();
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
    default:
      options = mainOptions;
      optionsSize = sizeof(mainOptions) / sizeof(mainOptions[0]);
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
    default:
      break;
  }

  showMenu();
}

void Menu::updatePreviousLayer() {
  switch (currentLayer) {
    case LAYER_MAIN_MENU:
    case LAYER_LEDS_MENU:
      previousLayer = LAYER_MAIN_MENU;
      break;
    default:
      debug.println("Unknown layer: " + String(currentLayer));
      previousLayer = LAYER_MAIN_MENU;
      break;
  }
}

void Menu::handleBackButton() {
  currentLayer = previousLayer;
  selectedOption = 0;
  // TODO: update orientation
  showMenu();
}

void Menu::mainMenu() {
  switch (selectedOption) {
    case MAIN_MENU_DINO:
      gameSetup();
      break;
    case MAIN_MENU_LEDS:
      currentLayer = LAYER_LEDS_MENU;
      break;
    case MAIN_MENU_TEST1:
      break;
    case MAIN_MENU_TEST2:
      break;
    case MAIN_MENU_TEST3:
      break;
    case MAIN_MENU_TEST4:
      break;
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
