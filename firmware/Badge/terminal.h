// /*********************************************************************************
//   liveHacking Badge
//   Eduardo Wero @ Electronic Cats
//   Andres Sabas @ Electronic Cats
//   Jorge Uri    @ Electronic Cats
//   Original Creation Date: Jun 7, 2022
//   This code is beerware; if you see me (or any other Electronic Cats
//   member) at the local, and you've found our code helpful,
//   please buy us a round!
//   Distributed as-is; no warranty is given.
// **********************************************************************************/
#include "hardware.h"
#include "menu.h"

Menu menu;

#ifndef ESP32

#include "ascii_art.h"
#include "enums.h"
#include "hardware/flash.h"

// GPIO21 Back LED
//   1    2   4   8
bool p3error = 1;
int led[4] = {10, 25, 26, 8};
int i;
long temps;  // Variable para guardar valor del cronómetro
static inference_t inference;
static signed short sampleBuffer[2048];
static bool debug_nn = false;  // Set this to true to see e.g. features generated from the raw signal
#if defined(ARDUINO_ARCH_MBED_RP2040)
char caracteres[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'ñ', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', '?', '"', '!'};
#elif defined(ARDUINO_ARCH_RP2040)
char caracteres[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', '?', '"', '!'};
#endif
String codigo[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "--.--", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..--", "...--", "....-", ".....", "-....", "--...", "---..", "---..", "----.", ".-.-.-", "--..--", "..--..", ".-..-.", "-.-.--"};
String traduccion = "";
char vacio = '/';
boolean enviado = false;
int interval_ms = 1000;
long time_now = 0;
int decode2counter = 0;
int8_t command = 0;
volatile int samplesRead;
int p = 0;

// Index indicates in which part of the filesystem are we
// 0 = /
// 1 = /juegos
// 2 = /BUGCON

int FS_index = 0;

String FS[4][4] = {
    {"bugcon/ ", "herramientas/ ", "", ""},  // FS[0] = Root
    {"leds ", " ", "", ""},                  // FS[1] = /juegos
    {"info ", "", "", ""},                   // FS[2] = /BUGCON        "luz ", "escucha ", "cantar", ""
    {" ", "", "", ""}                        // FS[3] = /curiosidades
};

bool cnf, reto_1, reto_2, reto_3, doNotOpen = 0;
int candado_reto = 0;

// Function prototypes
void terminalSetup();
void terminalLoop();
void reset(void);
void leds();
void info();
void printFS(int index);
int16_t processSerialNumber(void);
int8_t processSerialCommand(void);
void printIndexFS(int index);

void terminalSetup() {
  Serial.println("Bug CON Badge by:");  // Introduccion
  Serial.println("Electronic Cats");

  // ASCII ART print
  for (int x = 0; x < num_linesEC; x++) {
    for (int z = 0; z <= charsPerLineEC; z++) {
      Serial.write(epd_bitmap_EC[x][z]);
    }
    Serial.println();
  }
  Serial.println("#   bash_completion - programmable completion functions for bash Electronic Cats v0.5 Alpha");
  Serial.println("#   BASH_COMPLETION_VERSINFO=(0 01)");
}

void terminalLoop() {
  for (;;) {
    // if BUTTON_LEFT is pressed, exit
    if (!digitalRead(BUTTON_LEFT)) {
      Serial.println("Saliendo...");
      break;
    }

    command = processSerialCommand();  // Evalua que comandos se ejecutan
    if (command > 0) {
      Serial.println("Command: " + String(command));
      bool conf;
      switch (command) {
        case CM_LS:  // LS
          printFS(FS_index);
          break;
        case CM_PWD:  // TODO: Print path where the FS_index points to
          printIndexFS(FS_index);
          break;
        case CM_CD:  // cd /root
          FS_index = FS_ROOT;
          break;
        case CM_CDA:  // cd /juegos
          FS_index = FS_HERRAMIENTAS;
          break;
        case CM_CDB:  // cd /BUGCON
          FS_index = FS_BUGCON;
          break;
        case CM_CDC:  // cd /curiosidades
          FS_index = FS_CURIOSIDADES;
          break;
        case CM_EXEB: {
          if (FS_index != FS_BUGCON) {
            Serial.println("Command not found");
            break;  // check we are in the folder
          }
          Serial.println("Desarrollado por Electronic Cats");
          Serial.println("Agradecemos a: ");
          Serial.println("BugCON y las grandes personas que hacen posible el evento");
          Serial.println("Firmware:");
          Serial.println("Francisco - @DeimosHall");
          Serial.println("Hardware:");
          Serial.println("Gustavo y Capuchino\n");
          Serial.println("#Unetealaresistencia");
          Serial.println("Power Mexicano");
          break;
        }
        // Reto 2
        case CM_EXEC: {
          if (FS_index != FS_BUGCON) {
            Serial.println("Command not found");
            break;  // check we are in the folder
          }
          if (candado_reto == 0) {
            Serial.println("Permiso denegado");
            break;  // check we are in the folder
          }
          if (candado_reto >= 2) {
            Serial.println("Ya te escuchamos lo suficiente");
            break;  // check we are in the folder
          }
          reto_2 = 1;
          // if (microphone_inference_start(EI_CLASSIFIER_RAW_SAMPLE_COUNT) == false) {
          //   Serial.println("ERR: Failed to setup audio sampling\r\n");
          //   return;
          // }
          Serial.println("Manten presionado PRESS para escucharte y no olvides repetirlo hasta que lo creas ");
          while (digitalRead(btn))
            ;
          while (reto_2 == 1 && !digitalRead(btn)) {
            // prueba_2();
          }
          // microphone_inference_end();
          delay(10);
          Serial.println("Dejamos de escucharte");
          break;
        }
        // Reto 3
        case CM_EXED: {
          if (FS_index != FS_BUGCON) {
            Serial.println("Command not found");
            break;  // check we are in the folder
          }
          if (candado_reto <= 1) {
            Serial.println("Permiso denegado");
            break;  // check we are in the folder
          }
          if (candado_reto == 3 && p3error != 1) {
            Serial.println("Bienvenido al Nuevo Orden Mundial, tu silla te espera");
            Serial.println("Manda un correo a core@bugcon.org con una captura de la terminal completa con este mensaje");
            break;
          }
          if (p3error == 1) {
            candado_reto = 2;
          }
          Serial.println("El pajaro minusculo cantara con PRESSion...");
          Serial.println("Dinos lo que escuchas:");
          // while (candado_reto != 3) prueba_3();
          break;
        }
        case CM_EXEA:  // execute file A = Config file                              juego_1
          if (FS_index != FS_HERRAMIENTAS) {
            Serial.println("Command not found");
            break;  // check we are in the folder
          }
          // juego_1();
          leds();
          conf = 0;
          break;
        case CM_EXEE:  // execute file A = Config file                              juego_2
          // flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
          if (FS_index != FS_HERRAMIENTAS) {
            Serial.println("Command not found");
            break;  // check we are in the folder
          }
          // juego_2();
          conf = 0;
          break;

        case CM_EXEF:  // execute file A = Config file                              juego_3
          // flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
          if (FS_index != FS_HERRAMIENTAS) {
            Serial.println("Command not found");
            break;  // check we are in the folder
          }
          // juego_3();
          conf = 0;
          break;
        case CM_EXEG:  // execute file A = Config file                              Rick
          // flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
          if (FS_index != FS_CURIOSIDADES) {
            Serial.println("Command not found");
            break;  // check we are in the folder
          }
          conf = 0;
          break;
        case CM_EXEH:  // execute file A = Config file                              cheems
          // flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
          /*if (FS_index != FS_CURIOSIDADES) {
            Serial.println("Command not found");
            break; //check we are in the folder
          }*/
          for (int x = 0; x < num_linesCheems; x++) {
            for (int z = 0; z <= charsPerLineCheems; z++) {
              Serial.write(epd_bitmap_Cheems[x][z]);
            }
            Serial.println();
          }
          conf = 0;
          break;
        case CM_HLP:
          Serial.println("#   ");
          Serial.println("#   bash_completion - programmable completion functions for bash Electronic Cats v0.6 Alpha");
          Serial.println("#   BASH_COMPLETION_VERSINFO=(0 02)");
          Serial.println("#   Microcontroller Terminal Emulator");
          Serial.println("#   For Bug CON 2023");  // Add another
          Serial.println("#   Use at your own risk");
          Serial.println("#   ");
          Serial.println("Options and commands available (may be more):");
          Serial.println("pwd: This shows the PATH where your are, we dont know if this really works");
          Serial.println("./: Add before a binary to ejecute it meow meow meow");
          Serial.println("ls: List files or folders, meow meow meow");
          Serial.println("cd: Change meow directory meow meow");
          break;
        case 99:
          Serial.println("Command not found");
          break;
      }
    }
    reset();
  }
}

void reset(void) {
  if (command == 0) {  // Ctrl+C Pressed
    if (cnf || reto_1 || reto_2) Serial.println("Reto detenido");
    cnf = 0;
    reto_1 = 0;
    reto_2 = 0;
    doNotOpen = 0;
  }
}

void printFS(int index) {
  for (int c = 0; c < 4; c++) {
    Serial.print(FS[index][c]);
  }
  Serial.println();
}

void printIndexFS(int index) {
  switch (index) {
    case FS_ROOT:
      Serial.println("/");
      break;
    case FS_HERRAMIENTAS:
      Serial.println("/LEDs");
      break;
    case FS_BUGCON:
      Serial.println("/BUGCON");
      break;
    case FS_CURIOSIDADES:
      Serial.println("/curiosidades");
      break;
  }
}

int16_t processSerialNumber(void) {
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      int serialDataInt = Serial.read();
      if (serialDataInt == 0x03)  // CTRL+C Pressed
        return -1;
      if (serialDataInt == 0x7F)  // backspace
        serialDataString.remove(serialDataString.length() - 1, 1);
      if (serialDataInt == 0x0D) {
        Serial.println();
        String serialCache = serialDataString;
        serialDataString = "";
        // Check
        uint16_t numberString = serialCache.toInt();
        return numberString;
        // Convert to number and return.
      }
      if (serialDataInt > 0x1F && serialDataInt < 0x7F)
        serialDataString += (char)serialDataInt;
      Serial.print((char)serialDataInt);
    }
  }
  return -2;
}

int8_t processSerialCommand(void) {  // Redirige al menu secundario segun entrada de serial
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      int serialDataInt = Serial.read();
      if (serialDataInt == 0x03)  // CTRL+C Pressed
        return 0;
      if (serialDataInt == 0x7F)  // backspace
        serialDataString.remove(serialDataString.length() - 1, 1);
      if (serialDataInt == 0x0D) {
        Serial.println();
        String serialCache = serialDataString;
        serialDataString = "";
        // Enter Pressed
        if (serialCache == "ls") return CM_LS;
        if (serialCache == "pwd") return CM_PWD;
        if (serialCache == "cd") return CM_CD;
        if (serialCache == "cd ..") return CM_CD;
        if (serialCache == "cd herramientas") return CM_CDA;
        if (serialCache == "cd bugcon") return CM_CDB;
        if (serialCache == "cd herramientas/") return CM_CDA;
        if (serialCache == "cd bugcon/") return CM_CDB;
        if (serialCache == "cd /herramientas/") return CM_CDA;
        if (serialCache == "cd /bugcon/") return CM_CDB;
        if (serialCache == "cd /herramientas") return CM_CDA;
        if (serialCache == "cd /bugcon") return CM_CDB;
        if (serialCache == "./info") return CM_EXEB;
        if (serialCache == "./leds") return CM_EXEA;
        if (serialCache == "help") return CM_HLP;
        return 99;  // return -2 if its not a command
      }
      if (serialDataInt > 0x1F && serialDataInt < 0x7F)
        serialDataString += (char)serialDataInt;
      Serial.print((char)serialDataInt);
    }
  }
  return -1;
}

void leds() {
  int16_t r = -2, g = -2, b = -2;
  Serial.println("Personaliza tus LEDs");
  Serial.println("Introduce el valor RGB");
  Serial.print("Valor R: ");
  while (r == -2) {
    r = processSerialNumber();
  }
  Serial.println();
  Serial.print("Valor G: ");
  while (g == -2) {
    g = processSerialNumber();
  }
  Serial.println();
  Serial.print("Valor B: ");
  while (b == -2) {
    b = processSerialNumber();
  }
  Serial.println();
  uint8_t red = r;
  uint8_t green = g;
  uint8_t blue = b;
  Serial.println("Color: " + String(red) + ", " + String(green) + ", " + String(blue));
  for (int i = 0; i < 4; i++) {
    menu.pixels.setPixelColor(i, menu.pixels.Color(red, green, blue));
  }
  menu.pixels.show();
}

#endif  // RP2040