#ifndef MENU_HELPER_H
#define MENU_HELPER_H

enum MenuLayer {
  LAYER_MAIN_MENU = 0,
};

enum MainMenuOptions {
  MAIN_MENU_DINO = 0,
  MAIN_MENU_TEST,
  MAIN_MENU_TEST1,
  MAIN_MENU_TEST2,
  MAIN_MENU_TEST3,
  MAIN_MENU_TEST4,
};

/************************ Main menu ***************************/

char *mainOptions[] = {
    "1. Juego",
    "2. Test",
    "3. Test",
    "4. Test",
    "5. Test",
    "6. Test"};

#endif