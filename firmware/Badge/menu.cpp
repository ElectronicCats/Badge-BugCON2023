#include "Menu.h"

#ifdef RP2040
ezButton buttonLeft(BUTTON_LEFT);
ezButton buttonRight(BUTTON_RIGHT);
#else
ezTouch buttonLeft(TOUCH_LEFT);
ezTouch buttonRight(TOUCH_RIGHT);
#endif

Menu::Menu() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
}

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
}

void Menu::loop() {
  buttonLeft.loop();
  buttonRight.loop();

  if (buttonLeft.isPressed()) {
    debug.println("Left button pressed");
  }

  if (buttonRight.isPressed()) {
    debug.println("Right button pressed");
  }

  if (buttonLeft.isReleased()) {
    debug.println("Left button released");
  }

  if (buttonRight.isReleased()) {
    debug.println("Right button released");
  }
}
