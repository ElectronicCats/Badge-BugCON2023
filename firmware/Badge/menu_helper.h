#ifndef MENU_HELPER_H
#define MENU_HELPER_H

enum MenuLayer {
  LAYER_MAIN_MENU = 0,
  LAYER_LEDS_MENU,
};

enum MainMenuOptions {
  MAIN_MENU_DINO = 0,
  MAIN_MENU_LEDS,
  MAIN_MENU_TEST1,
  MAIN_MENU_TEST2,
  MAIN_MENU_TEST3,
  MAIN_MENU_TEST4,
};

enum LedsMenuOptions {
  LEDS_MENU_OFF = 0,
  LEDS_MENU_RED,
  LEDS_MENU_GREEN,
  LEDS_MENU_BLUE,
  LEDS_MENU_YELLOW,
  LEDS_MENU_PINK,
  LEDS_MENU_CYAN,
  LEDS_MENU_WHITE,
};

/************************ Main menu ***************************/

char *mainOptions[] = {
    "1. Juego",
    "2. LEDs",
    "3. Vincular",
    "4. Informacion",
    "5. Test",
    "6. Test"};

/************************ LEDs menu ***************************/

char *ledsOptions[] = {
    "1. Apagar",
    "2. Rojo",
    "3. Verde",
    "4. Azul",
    "5. Amarillo",
    "6. Rosa",
    "7. Cyan",
    "8. Blanco"};

#endif