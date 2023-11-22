#ifndef MENU_HELPER_H
#define MENU_HELPER_H

enum MenuLayer {
  LAYER_MAIN_MENU = 0,
  LAYER_LEDS_MENU,
  LAYER_CONFERENCE_MENU,
  LAYER_CONFERENCE_PAIRING_BANNER,
  LAYER_CONFERENCE_PAIRING_SUCCESS_BANNER,
  LAYER_CONFERENCE_LIST,
  LAYER_CONFERENCE_HELP_BANNER,
  LAYER_TERMINAL,
  LAYER_INFO_MENU,
};

#if defined(RP2040)
enum MainMenuOptions {
  MAIN_MENU_DINO = 0,
  MAIN_MENU_LEDS,
  MAIN_MENU_CONFERENCE,
  MAIN_MENU_INFO,
  MAIN_MENU_TERMINAL,
};
#else
enum MainMenuOptions {
  MAIN_MENU_DINO = 0,
  MAIN_MENU_LEDS,
  MAIN_MENU_CONFERENCE,
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

#if defined(RP2040)
enum PairMenuOptions {
  PAIR_MENU_START = 0,
  PAIR_MENU_CONFERENCES,
  PAIR_MENU_CONFERENCE_COUNTER,
  PAIR_MENU_HELP,
};
#else
enum PairMenuOptions {
  PAIR_MENU_START = 0,
  PAIR_MENU_HELP,
};
#define PAIR_MENU_CONFERENCES -1
#endif

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
    "3. Conferencias",
    "4. Informacion",
    "5. Terminal"};
#else
char *mainOptions[] = {
    "1. Juego",
    "2. LEDs",
    "3. Conferencias",
    "4. Informacion",
    "5. AirTag"};
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

/********************* Conference menu ************************/

#if defined(RP2040)
char *conferenceOptions[] = {
    "1. Vincular",
    "2. Ver conferencias",
    "3. Conferencias:   ",
    "4. Ayuda"};
#else
char *conferenceOptions[] = {
    "1. Vincular",
    "2. Ayuda"};
#endif

char *pairingBanner[] = {
    "",
    "   Vinculando..."};

char *pairingSuccessBanner[] = {
    "  Nueva conferencia  ",
    "      agregada       "};

#if defined(RP2040)
char *conferenceHelpBanner[] = {
    "   Debes conectarte  ",
    "     a un speaker",
    "   mediante serial"};
#else
char *conferenceHelpBanner[] = {
    "   Debes conectarte  ",
    "      a un VIP",
    "   mediante serial"};
#endif

char *conferenceList[700] = {
    "  Las conferencias   ",
    "   apareceran aqui   "};

/********************* Terminal menu ************************/

char *terminalBanner[] = {
    "",
    "   Terminal serial   "
    "       activa        "};

/*********************** Info menu **************************/

char *infoMenu[] = {
    "Desarrollado por:    ",
    "Electronic Cats      ",
    "Agradecemos a: BugCON",
    "y las grandes        ",
    "personas que hacen   ",
    "posible el evento    ",
    "Firmware:            ",
    "Francisco-@DeimosHall",
    "Hardware:            ",
    "Gustavo y Capuchino  ",
    "                     ",
    "#Unetealaresistencia ",
    "Power Mexicano       "};

#endif