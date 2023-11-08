#include "menu.h"

#define NEOPIXELS_PIN 19
#define NUMPIXELS 4

Menu menu;

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXELS_PIN, NEO_GRB + NEO_KHZ800);

#ifdef RP2040
UART Serial2(8, 9, NC, NC);  // TX, RX, RTS, CTS
#endif

void setup() {
  Serial.begin(9600);
#ifdef RP2040
  Serial2.begin(9600);
#endif

  // Wait for serial monitor to open
  while (!Serial)
    ;

  Serial.println("Board name: " + String(BOARD_NAME));

#if defined(RP2040)
  Serial.println("SERIAL1_TX: " + String(SERIAL1_TX));
  Serial.println("SERIAL1_RX: " + String(SERIAL1_RX));
#endif

  menu.begin();
  pixels.begin();
}

void loop() {
  menu.loop();
  // serialTest();

  pixels.clear();  // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  // for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...

  //   // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
  //   // Here we're using a moderately bright green color:
  //   pixels.setPixelColor(i, pixels.Color(0, 150, 0));

  //   pixels.show();  // Send the updated pixel colors to the hardware.

  //   delay(500);  // Pause before next pass through loop
  // }
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
