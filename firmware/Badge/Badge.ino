#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#include "buttons.h"
#include "hardware.h"

#define NEOPIXELS_PIN 19
#define NUMPIXELS 4

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C ,for 128x32

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM logo_bmp[] =
    {0b00000000, 0b11000000,
     0b00000001, 0b11000000,
     0b00000001, 0b11000000,
     0b00000011, 0b11100000,
     0b11110011, 0b11100000,
     0b11111110, 0b11111000,
     0b01111110, 0b11111111,
     0b00110011, 0b10011111,
     0b00011111, 0b11111100,
     0b00001101, 0b01110000,
     0b00011011, 0b10100000,
     0b00111111, 0b11100000,
     0b00111111, 0b11110000,
     0b01111100, 0b11110000,
     0b01110000, 0b01110000,
     0b00000000, 0b00110000};

#if defined(ESP32_DEVKIT) || defined(RP2040)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#else  // ESP32_S3
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXELS_PIN, NEO_GRB + NEO_KHZ800);

#ifdef RP2040
UART Serial2(8, 9, NC, NC);  // TX, RX, RTS, CTS
#endif

void setup() {
  Serial.begin(9600);
#ifdef RP2040
  Serial2.begin(9600);
#endif
  buttonLeft.setDebounceTime(DEBOUNCE_TIME_MS);
  buttonRight.setDebounceTime(DEBOUNCE_TIME_MS);

  // Wait for serial monitor to open
  while (!Serial)
    ;

  Serial.println("Board name: " + String(BOARD_NAME));

#if defined(RP2040)
  Serial.println("SERIAL1_TX: " + String(SERIAL1_TX));
  Serial.println("SERIAL1_RX: " + String(SERIAL1_RX));
#endif

#if defined(ESP32_DEVKIT) || defined(RP2040)
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
#endif

#if defined(ESP32_S3)
  if (!display.begin(SCREEN_ADDRESS)) {
    Serial.println(F("SH110X allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
#endif

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000);  // Pause for 2 seconds
  pixels.begin();
}

void loop() {
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

  // serialTest();

  pixels.clear();  // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));

    pixels.show();  // Send the updated pixel colors to the hardware.

    delay(500);  // Pause before next pass through loop
  }
}

void serialTest() {
  serialSend();
  serialRead();
}

void serialSend() {
  static unsigned long lastTime = millis();
  static int counter = 0;

  if (millis() - lastTime > 1000) {
    Serial.println("Hello " + String(counter++));
#ifdef RP2040
    Serial2.println("Hello " + String(counter));
#endif
    lastTime = millis();
  }
}

void serialRead() {
#ifdef RP2040
  if (Serial2.available()) {
    String input = Serial2.readStringUntil('\n');
#else
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
#endif

    if (input.length() == 0) {
      Serial.println("Empty input");
      return;
    }

    Serial.println("Received: " + input);
    delay(1000);
  }
}
