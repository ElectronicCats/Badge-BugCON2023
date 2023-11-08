#include "menu.h"

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
  buttonLeft.setDebounceTime(DEBOUNCE_TIME_MS);
  buttonRight.setDebounceTime(DEBOUNCE_TIME_MS);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
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
    Serial.println("Left button pressed");
  }

  if (buttonRight.isPressed()) {
    Serial.println("Right button pressed");
  }

  if (buttonLeft.isReleased()) {
    Serial.println("Left button released");
  }

  if (buttonRight.isReleased()) {
    Serial.println("Right button released");
  }
}
