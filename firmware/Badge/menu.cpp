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
