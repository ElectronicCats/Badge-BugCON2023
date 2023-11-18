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

#include <cww_MorseTx.h>

#include "Adafruit_TinyUSB.h"
#include "ascii_art.h"
#include "enums.h"
#include "hardware/flash.h"

// #include <PDM.h>
#include <JorgeBigbang200-project-1_inferencing.h>

// GPIO21 Back LED
//   1    2   4   8
bool p3error = 1;
int led[4] = {10, 25, 26, 8};
int i;
long temps;  // Variable para guardar valor del cronómetro
static inference_t inference;
static signed short sampleBuffer[2048];
static bool debug_nn = false;  // Set this to true to see e.g. features generated from the raw signal
char caracteres[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'ñ', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', '?', '"', '!'};
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
    {"luz ", "", "", ""},                    // FS[2] = /BUGCON        "luz ", "escucha ", "cantar", ""
    {" ", "", "", ""}                        // FS[3] = /curiosidades
};

bool cnf, reto_1, reto_2, reto_3, doNotOpen = 0;
int candado_reto = 0;

// Function prototypes
void terminalSetup();
void terminalLoop();
void reset(void);
void leds();
void prueba_2(void);
void prueba_3(void);
void juego_1(void);
void juego_2(void);
void juego_3(void);
void traducirPalabra();
void emitirMorse();
void printFS(int index);
int16_t processSerialNumber(void);
int8_t processSerialCommand(void);
void printIndexFS(int index);
bool sendHIDInfo();

// HID report descriptor using TinyUSB's template
// uint8_t const desc_hid_report[] =
//     {
//         TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD)),
//         TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(RID_MOUSE)),
//         TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(RID_CONSUMER_CONTROL))};

// Adafruit_USBD_HID usb_hid;

// cww_MorseTx morse(8, 10, 18, 1000);

void terminalSetup() {
  //   for (i = 0; i < 4; i++) pinMode(led[i], OUTPUT);
  //   pinMode(amp_en, OUTPUT);
  //   pinMode(A2, OUTPUT);  // buzzer
  //   pinMode(btn, INPUT_PULLUP);

  //   usb_hid.setPollInterval(2);
  //   usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  //   usb_hid.setStringDescriptor("BugCON badge!");
  //   digitalWrite(amp_en, HIGH);

  // usb_hid.begin();
  // while (!Serial) {
  //   int rndnum = 0;
  //   if (!digitalRead(btn)) {
  //     for (int k = 0; k < 8; k++) {
  //       for (int j = 0; j < 5; j++) digitalWrite(led[j], LOW);
  //       rndnum = random(1, 5);
  //       switch (rndnum) {
  //         case 1:
  //           digitalWrite(10, HIGH);
  //           break;
  //         case 2:
  //           digitalWrite(25, HIGH);
  //           break;
  //         case 3:
  //           digitalWrite(26, HIGH);
  //           break;
  //         case 4:
  //           digitalWrite(8, HIGH);
  //           break;
  //       }
  //       delay(500);
  //     }
  //     analogWrite(A2, 200);
  //     delay(1000);
  //     for (int j = 0; j < 5; j++) digitalWrite(led[j], LOW);
  //     analogWrite(A2, 0);
  //   }
  // }
  // digitalWrite(amp_en, LOW);
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
        case CM_EXEB:  // Reto 1 (Codigo de luces)
          if (FS_index != FS_BUGCON) {
            Serial.println("Command not found");
            break;  // check we are in the folder
          }
          if (candado_reto <= 0) reto_1 = 0;
          Serial.println("La luz de tu brujula será la guia:");
          do {
            int16_t number = -2;
            while (number == -2 && candado_reto == 0) {
              number = processSerialNumber();
            }
            if (number == 1337 || candado_reto > 0) {
              Serial.println("Seguiste correctamente la luz. Ahora te escuchamos...");
              Serial.println("...");
              Serial.println("...");
              Serial.println("Recuerda, la palabra que buscas reconoce a aquellas personas que se unen a venerar una verdad");
              FS[2][2] = "escucha";
              reto_1 = 1;
              if (candado_reto < 1) candado_reto = 1;
            } else {
              int n;
              n++;
              if (n == 3) {
                Serial.println("Equivocaciones excedidas.Saliendo...");
                reto_1 = 1;
                n = 0;
              } else {
                Serial.print(3 - n);
                Serial.println(" intentos restantes");
              }
            }
          } while (!reto_1);
          delay(10);
          break;
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
            prueba_2();
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
          while (candado_reto != 3) prueba_3();
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
          juego_2();
          conf = 0;
          break;

        case CM_EXEF:  // execute file A = Config file                              juego_3
          // flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
          if (FS_index != FS_HERRAMIENTAS) {
            Serial.println("Command not found");
            break;  // check we are in the folder
          }
          juego_3();
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
        if (serialCache == "cd curiosidades") return CM_CDC;
        if (serialCache == "cd herramientas/") return CM_CDA;
        if (serialCache == "cd bugcon/") return CM_CDB;
        if (serialCache == "cd curiosidades/") return CM_CDC;
        if (serialCache == "cd /herramientas/") return CM_CDA;
        if (serialCache == "cd /bugcon/") return CM_CDB;
        if (serialCache == "cd /curiosidades/") return CM_CDC;
        if (serialCache == "cd /herramientas") return CM_CDA;
        if (serialCache == "cd /bugcon") return CM_CDB;
        if (serialCache == "cd /curiosidades") return CM_CDC;
        if (serialCache == "./luz") return CM_EXEB;
        if (serialCache == "./escucha") return CM_EXEC;
        if (serialCache == "./cantar") return CM_EXED;
        if (serialCache == "./leds") return CM_EXEA;
        // if (serialCache == "./decibelimetro") return CM_EXEE;
        if (serialCache == "./wav_file") return CM_EXEF;
        if (serialCache == "./text.tx") return CM_EXEG;
        if (serialCache == "./cheems") return CM_EXEH;
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

bool sendHIDInfo() {
  //   // Remote wakeup
  //   if (TinyUSBDevice.suspended()) {
  //     // Wake up host if we are in suspend mode
  //     // and REMOTE_WAKEUP feature is enabled by host
  //     TinyUSBDevice.remoteWakeup();
  //   }
  //   while (1) {
  //     /*------------- Mouse -------------*/
  //     if (usb_hid.ready()) {
  //       int8_t const delta = 20;
  //       usb_hid.mouseMove(RID_MOUSE, delta, delta);  // right + down
  //       // delay a bit before attempt to send keyboard report
  //       delay(10);
  //     }

  //     /*------------- Keyboard -------------*/
  //     if (usb_hid.ready()) {
  //       // use to send key release report
  //       static bool has_key = false;

  //       if (1) {
  //         uint8_t keycode[6] = {0};
  //         keycode[0] = HID_KEY_A;
  //         usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
  //         has_key = true;
  //       } else {
  //         // send empty key report if previously has key pressed
  //         if (has_key) usb_hid.keyboardRelease(RID_KEYBOARD);
  //         has_key = false;
  //       }

  //       // delay a bit before attempt to send consumer report
  //       delay(10);
  //     }

  //     /*------------- Consumer Control -------------*/
  //     if (usb_hid.ready()) {
  //       // Consumer Control is used to control Media playback, Volume, Brightness etc ...
  //       // Consumer report is 2-byte containing the control code of the key
  //       // For list of control check out https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h#L544

  //       // use to send consumer release report
  //       static bool has_consumer_key = false;

  //       // send volume down (0x00EA)
  //       usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_DECREMENT);
  //       has_consumer_key = true;
  //       delay(10);
  //     }
  //   }
  //   return 1;
}

void prueba_2(void) {
  //   delay(500);
  //   bool m = microphone_inference_record();
  //   if (!m) {
  //     Serial.println("ERR: Failed to record audio...\n");
  //     return;
  //   }
  //   signal_t signal;
  //   signal.total_length = EI_CLASSIFIER_RAW_SAMPLE_COUNT;
  //   signal.get_data = &microphone_audio_signal_get_data;
  //   ei_impulse_result_t result = {0};

  //   EI_IMPULSE_ERROR r = run_classifier(&signal, &result, debug_nn);
  //   if (r != EI_IMPULSE_OK) {
  //     Serial.println("Error corriendo el clasificador");
  //     return;
  //   }
  //   if (result.classification[0].value > .9) {
  //     Serial.println("Bienvenido al culto!");
  //     Serial.println("Ahora sigue nuestras ordenes");
  //     FS[2][3] = " cantar";
  //     printFS(2);
  //     reto_2 = 0;
  //     candado_reto = 2;
  //   }
  // #if EI_CLASSIFIER_HAS_ANOMALY == 1
  //   Serial.println("    anomaly score: %.3f", result.anomaly);
  // #endif
}

void prueba_3(void) {
  //   digitalWrite(amp_en, HIGH);
  //   if (!digitalRead(btn)) {
  //     traducirPalabra();
  //     emitirMorse();
  //     while (digitalRead(btn) && candado_reto != 3) {
  //       String readString;
  //       while (Serial.available()) {
  //         delay(3);
  //         char contrasena = Serial.read();
  //         readString += contrasena;
  //       }
  //       readString.trim();
  //       if (readString.length() > 0) {
  //         if (readString == "iluminar") {
  //           Serial.println("Bienvenido al Nuevo Orden Mundial, tu silla te espera");
  //           Serial.println("Manda un correo a core@bugcon.org con una captura de la terminal completa con este mensaje");
  //           digitalWrite(amp_en, LOW);
  //           p3error = 0;
  //           candado_reto = 3;
  //         } else {
  //           p++;
  //           if (p == 3) {
  //             Serial.println("Equivocaciones excedidas.Saliendo...");
  //             candado_reto = 3;
  //             p3error = 1;
  //             p = 0;
  //             digitalWrite(amp_en, LOW);
  //           } else {
  //             Serial.print(3 - p);
  //             Serial.println(" intentos restantes");
  //           }
  //         }
  //         readString = "";
  //       }
  //     }
  //   }
}

void juego_1(void) {
  //   int rndnum = 0;
  //   Serial.println("Quieres jugar a botella?");
  //   Serial.println("Oprime PRESS para girar!");
  //   while (digitalRead(btn))
  //     ;
  //   Serial.println("Comienza!");
  //   for (int k = 0; k < 12; k++) {
  //     for (int j = 0; j < 5; j++) digitalWrite(led[j], LOW);
  //     rndnum = random(1, 5);
  //     switch (rndnum) {
  //       case 1:
  //         digitalWrite(10, HIGH);
  //         break;
  //       case 2:
  //         digitalWrite(8, HIGH);
  //         break;
  //       case 3:
  //         digitalWrite(25, HIGH);
  //         break;
  //       case 4:
  //         digitalWrite(26, HIGH);
  //         break;
  //     }
  //     delay(1000);
  //   }
  //   digitalWrite(amp_en, HIGH);
  //   analogWrite(A2, 200);
  //   Serial.println("Gracias por jugar!");
  //   delay(5000);
  //   analogWrite(A2, 0);
  //   digitalWrite(amp_en, LOW);
  //   for (int j = 0; j < 5; j++) digitalWrite(led[j], LOW);
}

void juego_2(void) {
  //   //   static const char channels = 1;
  //   //   static const int frequency = 16000;
  //   //   // Wait for samples to be read
  //   //    PDM.onReceive(onPDMdata);
  //   //    if (!PDM.begin(channels, frequency)) {
  //   //            Serial.println("Failed to start PDM!");
  //   //            while (1);
  //   //     }
  //   //     Serial.println("Decibelimetro iniciado. Presiona Press para terminar");
  //   //     while(digitalRead(btn)){
  //   //       if (samplesRead) {
  //   //        // Print samples to the serial monitor or plotter
  //   //          for (int i = 0; i < samplesRead; i++) {
  //   //           if (sampleBuffer[i] >=7000)digitalWrite(26, HIGH);
  //   //           if (sampleBuffer[i] >=5200)digitalWrite(25, HIGH);
  //   //           if (sampleBuffer[i] >=3800)digitalWrite(10, HIGH);
  //   //           if (sampleBuffer[i] >=2200)digitalWrite(8, HIGH);
  //   //        for (int j = 0; j < 5; j++)digitalWrite(led[j],LOW);
  //   //        }
  //   //     // Clear the read count
  //   //     samplesRead = 0;
  //   //   }
  //   //  }
  //   //  Serial.println("Decibelimetro detenido");
}

void juego_3(void) {
  // }
  // static void pdm_data_ready_inference_callback(void) {
  //   // int bytesAvailable = PDM.available();

  //   // // read into the sample buffer
  //   // int bytesRead = PDM.read((char *)&sampleBuffer[0], bytesAvailable);

  //   // if (inference.buf_ready == 0) {
  //   //     for(int i = 0; i < bytesRead>>1; i++) {
  //   //         inference.buffer[inference.buf_count++] = sampleBuffer[i];

  //   //         if(inference.buf_count >= inference.n_samples) {
  //   //             inference.buf_count = 0;
  //   //             inference.buf_ready = 1;
  //   //             break;
  //   //         }
  //   //     }
  //   // }
}

#endif  // RP2040