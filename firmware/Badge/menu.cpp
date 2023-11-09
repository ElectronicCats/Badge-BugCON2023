
#include "Menu.h"

#include "menu_helper.h"

#ifdef RP2040
ezButton buttonLeft(BUTTON_LEFT);
ezButton buttonRight(BUTTON_RIGHT);
#else
ezTouch buttonLeft(TOUCH_LEFT);
ezTouch buttonRight(TOUCH_RIGHT);
#endif

Menu::Menu() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

void Menu::begin() {
  debug.begin(9600);
  debug.waitForSerialConnection();
  debug.println("Board name: " + String(BOARD_NAME));

  buttonLeft.setDebounceTime(DEBOUNCE_TIME_MS);
  buttonRight.setDebounceTime(DEBOUNCE_TIME_MS);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    debug.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000);  // Pause for 2 seconds
  showVMenu();
  gameSetup();
}

void Menu::loop() {
  buttonLeft.loop();
  buttonRight.loop();

  if (buttonLeft.isPressed()) {
    debug.println("Left button pressed");
    selectedOption--;

    if (selectedOption == 255)  // Underflow
      selectedOption = 0;

    showVMenu();
  }

  if (buttonRight.isPressed()) {
    debug.println("Right button pressed");
    selectedOption++;

    if (selectedOption > optionsSize - 1)
      selectedOption = optionsSize - 1;

    showVMenu();
  }

  if (buttonLeft.isReleased()) {
    debug.println("Left button released");
  }

  if (buttonRight.isReleased()) {
    debug.println("Right button released");
  }
}

void Menu::showVMenu() {
  char **options = updateVMenuOptions();
  display.clearDisplay();
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  uint8_t startIdx = (selectedOption >= 4) ? selectedOption - 3 : 0;
  uint8_t endIdx = (selectedOption >= 4) ? selectedOption + 1 : optionsSize - 1;

  for (uint8_t i = startIdx; i <= endIdx; i++) {
    debug.println(options[i]);
    if (i == selectedOption) {
      // ssd1306_drawstr(0, (i - startIdx) * 8, options[i], BLACK);
      display.setTextColor(BLACK, WHITE);
      display.println(options[i]);
    } else {
      // ssd1306_drawstr(0, (i - startIdx) * 8, options[i], WHITE);
      display.setTextColor(WHITE);
      display.println(options[i]);
    }
  }

  debug.println("");
  display.display();
}

char **Menu::updateVMenuOptions() {
  char **options;

  switch (currentLayer) {
    case LAYER_MAIN:
      // break;
    default:
      options = mainOptions;
      optionsSize = sizeof(mainOptions) / sizeof(mainOptions[0]);
      break;
  }

  return options;
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
  display.setCursor(10, 5);
  display.println("Dino Game");

  display.setTextSize(1);

  //   display.setCursor(5,45);
  display.println("Enter 1 To Play ");

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
  display.setCursor(10, 5);
  display.println("Game Over");

  //   display.setTextSize(1);

  //   display.setCursor(10,30);
  display.println("Score: " + String(score));
  //   display.print(score);

  //   display.setCursor(1,45);
  display.println("Enter 1 To Play Again");

  display.display();
}

// Display score while running the game
void Menu::displayScore(int score) {
  display.setTextSize(1);
  display.setCursor(64, 10);
  display.print("Score: ");
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
