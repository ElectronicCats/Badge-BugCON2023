#ifndef MENU_HELPER_H
#define MENU_HELPER_H

enum MenuLayer {
  LAYER_MAIN_MENU = 0,
  LAYER_LEDS_MENU,
  LAYER_PAIR_MENU,
  LAYER_PAIRING_BANNER,
};

#if defined(RP2040)
enum MainMenuOptions {
  MAIN_MENU_DINO = 0,
  MAIN_MENU_LEDS,
  MAIN_MENU_PAIR,
  MAIN_MENU_INFO,
  MAIN_MENU_TERMINAL,
};
#else
enum MainMenuOptions {
  MAIN_MENU_DINO = 0,
  MAIN_MENU_LEDS,
  MAIN_MENU_PAIR,
  MAIN_MENU_INFO,
  MAIN_MENU_AIRTAG,
  MAIN_MENU_WEB_SERVER,
};
#endif

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

enum PairMenuOptions {
  PAIR_MENU_START = 0,
  PAIR_MENU_CANCEL,
};

/************************* Banners ****************************/

char *errorBanner[] = {
    "",
    "    Error"};

// Option to horizontal menu
char *oneOption[] = {
    "Aceptar"};

// Options to horizontal menu
char *twoOptions[] = {
    "Aceptar",
    "Cancelar"};

/************************ Main menu ***************************/

#if defined(RP2040)
char *mainOptions[] = {
    "1. Juego",
    "2. LEDs",
    "3. Vincular",
    "4. Informacion",
    "5. Terminal"};
#else
char *mainOptions[] = {
    "1. Juego",
    "2. LEDs",
    "3. Vincular",
    "4. Informacion",
    "5. AirTag",
    "6. Servidor web"};
#endif

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

/************************ Pair menu ***************************/

char *pairOptions[] = {
    "1. Iniciar",
    "2. Cancelar"};

char *pairingBanner[] = {
    "",
    "   Vinculando..."};

#endif