// /*********************************************************************************
//   Firmware for BugCON Badge 2023
//   by Francisco Torres, Electronic Cats (https://electroniccats.com/)
//   Date: 2023/11/09

//   This program demonstrates how to use several features of the BugCON Badge 2023
//   by Electronic Cats
//   https://github.com/ElectronicCats/Badge-BugCON2023.git

//   Development environment specifics:
//   IDE: Visual Studio Code + Arduino CLI Version 0.34.2
//   Hardware Platform:
//   BugCON Badge, VIP edition
//   - RP2040
//   BugCON Badge, Speaker edition
//   - ESP32-S3

//   Electronic Cats invests time and resources providing this open source code,
//   please support Electronic Cats and open-source hardware by purchasing
//   products from Electronic Cats!

//   This code is beerware; if you see me (or any other Electronic Cats
//   member) at the local, and you've found our code helpful,
//   please buy us a round!
//   Distributed as-is; no warranty is given.
// ***********************************************************************************/
// #include "server.h"
#include "terminal.h"

#ifdef ESP32
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>

int port = 80;
WiFiServer server(port);
String defaultSSDI;
String defaultPassword;
int status;
int webRequest;
Debug &debug = Debug::getInstance();
void loadPageContent(WiFiClient client);
void showPageContent(WiFiClient client, const char *pageContent);
String decodeURL(char *url);
void updateWebRequest(String url);
void handleRequests();
void handleURLParameters(String url);
#endif

const char *ssid = "Badge";
const char *password = "password";

// void setup() {
//   // menu.begin();
//   debug.begin(9600);
//   // defaultSSDI = "Badge";
//   // defaultPassword = "password";
//   // status = WL_IDLE_STATUS;
//   // webRequest = INDEX_HTML;
// #ifdef ESP32
//   debug.println("Creating access point named: " + defaultSSDI);

//   // You can remove the password parameter if you want the AP to be open.
//   // if (!WiFi.softAP(defaultSSDI.c_str(), defaultPassword.c_str())) {
//   //   debug.println("Creating access point failed");
//   //   while (true)
//   //     ;
//   // }

//   if (!WiFi.softAP(ssid, password)) {
//     log_e("Soft AP creation failed.");
//     while(1);
//   }

//   IPAddress myIP = WiFi.softAPIP();
//   debug.print("AP IP address: ");
//   Serial.println(myIP);
//   debug.println("Access point created");
// #endif
// }

void setup() {
  menu.begin();
#ifdef ESP32
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while(1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
#endif
}

void loop() {
  menu.loop();
#ifdef RP2040
  if (menu.isTerminalEnabled()) {
    terminalSetup();
    terminalLoop();
    menu.disableTerminal();
  }
#endif

#ifdef ESP32 
  WiFiClient client = server.available();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        // if (currentLine.endsWith("GET /H")) {
        //   digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
        // }
        // if (currentLine.endsWith("GET /L")) {
        //   digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
        // }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
#endif
}

/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/


// void loop() {
//   menu.loop();
//   WiFiClient client = server.available();   // listen for incoming clients

//   if (client) {                             // if you get a client,
//     Serial.println("New Client.");           // print a message out the serial port
//     String currentLine = "";                // make a String to hold incoming data from the client
//     while (client.connected()) {            // loop while the client's connected
//       if (client.available()) {             // if there's bytes to read from the client,
//         char c = client.read();             // read a byte, then
//         Serial.write(c);                    // print it out the serial monitor
//         if (c == '\n') {                    // if the byte is a newline character

//           // if the current line is blank, you got two newline characters in a row.
//           // that's the end of the client HTTP request, so send a response:
//           if (currentLine.length() == 0) {
//             // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
//             // and a content-type so the client knows what's coming, then a blank line:
//             client.println("HTTP/1.1 200 OK");
//             client.println("Content-type:text/html");
//             client.println();

//             // the content of the HTTP response follows the header:
//             client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
//             client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

//             // The HTTP response ends with another blank line:
//             client.println();
//             // break out of the while loop:
//             break;
//           } else {    // if you got a newline, then clear currentLine:
//             currentLine = "";
//           }
//         } else if (c != '\r') {  // if you got anything else but a carriage return character,
//           currentLine += c;      // add it to the end of the currentLine
//         }

//         // // Check to see if the client request was "GET /H" or "GET /L":
//         // if (currentLine.endsWith("GET /H")) {
//         //   digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
//         // }
//         // if (currentLine.endsWith("GET /L")) {
//         //   digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
//         // }
//       }
//     }
//     // close the connection:
//     client.stop();
//     Serial.println("Client Disconnected.");
//   }
// }
