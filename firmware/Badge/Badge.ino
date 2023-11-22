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
#include "index.html.h"
#include "main.js.h"
#include "styles.css.h"

#ifdef ESP32
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>

enum URL {
  INDEX_HTML = 0,
  MAIN_JS,
  STYLES_CSS,
  NOT_FOUND
};

int port = 80;
WiFiServer server(port);
String defaultSSDI;
String defaultPassword;
int status;
URL webRequest;
Debug &debug = Debug::getInstance();
void setupWiFi();
void loadPageContent(WiFiClient client);
void showPageContent(WiFiClient client, const char *pageContent);
String decodeURL(char *url);
void updateWebRequest(String url);
void handleRequests();
void handleURLParameters(String url);
void runServer();
#endif

const char *ssid = "Badge";
const char *password = "password";

void setup() {
  menu.begin();
  setupWiFi();
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

  runServer();
}

void setupWiFi() {
#ifdef ESP32
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1)
      ;
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
#endif
}

// void runServer() {
// #ifdef ESP32
//   WiFiClient client = server.available();  // listen for incoming clients

//   if (client) {                     // if you get a client,
//     Serial.println("New Client.");  // print a message out the serial port
//     String currentLine = "";        // make a String to hold incoming data from the client
//     while (client.connected()) {    // loop while the client's connected
//       if (client.available()) {     // if there's bytes to read from the client,
//         char c = client.read();     // read a byte, then
//         Serial.write(c);            // print it out the serial monitor
//         if (c == '\n') {            // if the byte is a newline character

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
//           } else {  // if you got a newline, then clear currentLine:
//             currentLine = "";
//           }
//         } else if (c != '\r') {  // if you got anything else but a carriage return character,
//           currentLine += c;      // add it to the end of the currentLine
//         }

//         // Check to see if the client request was "GET /H" or "GET /L":
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
// #endif
// }


void runServer() {
  WiFiClient client = server.available();  // listen for incoming clients

  if (client) {  // if you get a client,
    unsigned long speedTestTime = millis();
    String currentLine = "";  // make a String to hold incoming data from the client

    while (client.connected()) {  // loop while the client's connected
      if (client.available()) {   // if there's bytes to read from the client,
        char c = client.read();   // read a byte, then
        // debug.write(c);  // print it out the serial monitor

        if (c == '\n') {  // if the byte is a newline character
          if (currentLine.length() == 0) {
            loadPageContent(client);
            break;
          } else {
            currentLine = "";  // if you got a newline, then clear currentLine:
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Only check for URL if it's a GET <url options> HTTP
        if (currentLine.startsWith("GET /") && currentLine.endsWith("HTTP/1.1")) {
          String url = currentLine.substring(4, currentLine.indexOf("HTTP/1.1"));
          // debug.println("\nRequest: " + currentLine);
          // debug.println("URL: " + url);

          updateWebRequest(url);
          handleURLParameters(url);
        }
      }
    }
    client.stop();
    // debug.println("client disconnected");
    // debug.println("Time to run server: " + String(millis() - speedTestTime) + " ms");
  }
}

void loadPageContent(WiFiClient client) {
  static URL currentHTML;

  switch (webRequest) {
    case STYLES_CSS:
      showPageContent(client, styles_css);
      webRequest = currentHTML;
      break;
    case MAIN_JS:
      showPageContent(client, main_js);
      webRequest = currentHTML;
      break;
    case INDEX_HTML:
      showPageContent(client, index_html);
      currentHTML = INDEX_HTML;
      break;
    default:
      showPageContent(client, index_html);
      currentHTML = INDEX_HTML;
      break;
  }
}

void showPageContent(WiFiClient client, const char *pageContent) {
  String contentType;
  if (webRequest == INDEX_HTML) {
    contentType = "text/html";
  } else if (webRequest == STYLES_CSS) {
    contentType = "text/css";
  } else if (webRequest == MAIN_JS) {
    contentType = "application/javascript";
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:" + contentType);
  client.println();

  // if (webRequest == JAVASCRIPT_URL) {
  //   client.println("// BomberCat Variables");
  //   client.println("let pollMode = `" + pollMode + "`;");
  //   client.println("let sensRes = `" + sensRes + "`;");
  //   client.println("let selRes = `" + selRes + "`;");
  //   client.println("let nfcID = `" + nfcID + "`;");
  //   client.println("let nfcDiscoverySuccess = " + String(nfcDiscoverySuccess ? "true" : "false") + ";");
  //   client.println("let ssid = `" + preferences.getString("ssid", defaultSSID) + "`;");
  //   client.println("let password = `" + preferences.getString("password", defaultPassword) + "`;");
  // }

  // Create a temporary string to hold the page content
  char tempString[1001];
  tempString[1000] = 0;
  // Flag to indicate if we've reached the end of the page content
  boolean lastString = false;
  // Pointer to determine where we are in the page content
  char *charPtr = (char *)pageContent;

  // Loop to read the page content in chunks of 1000 bytes
  while (1) {
    for (int i = 0; i < 1000; i++) {
      // Check if we've reached the end of the page content
      if ((byte)*charPtr == 0) {
        lastString = true;
        tempString[i] = 0;
      }
      // Copy the page content to the temporary string
      tempString[i] = *charPtr;
      charPtr++;
    }
    // Send the temporary string to the client
    client.print(tempString);
    if (lastString == true)
      break;  // Exit the loop if we've reached the end of the page content
  }

  client.println("");  // Send a blank line to indicate the end of the page content
}

String decodeURL(char *url) {
  // Check if the URL starts with "%B" and return as it is.
  if (strncmp(url, "%B", 2) == 0) {
    String decodedURL = String(url);
    decodedURL.trim();
    decodedURL.replace("+", " ");
    return decodedURL;
  }

  // Create two pointers that point to the start of the data
  char *leader = url;
  char *follower = leader;

  // While we're not at the end of the string (current character not NULL)
  while (*leader) {
    // Check to see if the current character is a %
    if (*leader == '%') {
      // Grab the next two characters and move leader forwards
      leader++;
      char high = *leader;
      leader++;
      char low = *leader;

      // Convert ASCII 0-9A-F to a value 0-15
      if (high > 0x39)
        high -= 7;
      high &= 0x0f;

      // Same again for the low byte:
      if (low > 0x39)
        low -= 7;
      low &= 0x0f;

      // Combine the two into a single byte and store in follower:
      *follower = (high << 4) | low;
    } else {
      // All other characters copy verbatim
      *follower = *leader;
    }

    // Move both pointers to the next character:
    leader++;
    follower++;
  }
  // Terminate the new string with a NULL character to trim it off
  *follower = 0;

  // Return the new string
  String decodedURL = String(url);
  decodedURL.trim();
  decodedURL.replace("+", " ");
  return decodedURL;
}

void updateWebRequest(String url) {
  if (url.startsWith("/styles.css")) {
    webRequest = STYLES_CSS;
  } else if (url.startsWith("/main.js")) {
    webRequest = MAIN_JS;
  } else if (url.startsWith("/index.html?") || url.startsWith("/index.html")) {
    webRequest = INDEX_HTML;
  } else {
    webRequest = NOT_FOUND;
  }
}

void handleRequests() {
  static unsigned long detectTagsTime = millis();

  // if (webRequest == MAGSPOOF_URL) {
  //   magspoof();
  // }

  // // Run the NFC detect tags function every DETECT_TAGS_DELAY_MS milliseconds READ_ATTEMPTS times
  // if (millis() - detectTagsTime > DETECT_TAGS_DELAY_MS && webRequest == NFC_URL && runDetectTags) {
  //   detectTagsTime = millis();
  //   nfcExecutionCounter++;

  //   // Wait one attempt before starting the NFC discovery
  //   if (nfcExecutionCounter > 1) {
  //     detectTags();
  //   }

  //   if (nfcExecutionCounter == READ_ATTEMPTS) {
  //     nfc.StopDiscovery();
  //     nfcExecutionCounter = 0;
  //     runDetectTags = false;
  //   }
  // }

  // // Reset NFC variables when the page loaded is not related with NFC
  // if (webRequest != NFC_URL || clearNFCValuesFlag) {
  //   clearNFCValuesFlag = false;
  //   clearTagValues();
  // }

  // // Run emulateNFCID function after EMULATE_NFCID_DELAY_MS milliseconds
  // if (millis() - emulateNFCIDTimer > EMULATE_NFCID_DELAY_MS && emulateNFCFlag && webRequest == NFC_URL) {
  //   emulateNFCID();
  // }

  // // Reset BomberCat after 1500 milliseconds
  // if (millis() - rebootTimer > 1500 && rebootFlag) {
  //   // debug.println("Rebooting...");
  //   // rebootFlag = false;
  //   // NVIC_SystemReset();  // Reboot the RP2040
  // }
}

void handleURLParameters(String url) {
  // ? is the start of request parameters
  // if (url.startsWith("/config.html?")) {
  //   String btnSaveWiFiConfig = "";
  //   String ssid = "";
  //   String password = "";
  //   String debugStatus = "";
  //   int index = 0;

  //   index = url.indexOf("btnSaveWiFiConfig=");
  //   if (index != -1) {
  //     btnSaveWiFiConfig = url.substring(index + 18, url.indexOf("&ssid="));  // true or false
  //   }

  //   index = url.indexOf("ssid=");
  //   if (index != -1) {
  //     ssid = url.substring(index + 5, url.indexOf("&password="));
  //     ssid = decodeURL((char *)ssid.c_str());
  //   }

  //   index = url.indexOf("password=");
  //   if (index != -1) {
  //     password = url.substring(index + 9, url.length());
  //     password = decodeURL((char *)password.c_str());
  //   }

  //   index = url.indexOf("debug=");
  //   if (index != -1) {
  //     debugStatus = url.substring(index + 6, url.length());
  //   }

  //   debug.println("btnSaveWiFiConfig: ", btnSaveWiFiConfig);
  //   debug.println("ssid: '", ssid, "'");
  //   debug.println("password: '", password, "'");
  //   debug.println("debugStatus: '", debugStatus, "'");

  //   if (btnSaveWiFiConfig.startsWith("true")) {
  //     debug.println("Saving WiFi config...");
  //     preferences.putString("ssid", ssid);
  //     preferences.putString("password", password);
  //     rebootFlag = true;
  //     rebootTimer = millis();
  //   }

  //   if (debugStatus.startsWith("true")) {
  //     Serial.println("Enabling debug...");
  //     preferences.putBool("debug", true);
  //   } else if (debugStatus.startsWith("false")) {
  //     debug.println("Disabling debug...");
  //     preferences.putBool("debug", false);
  //   }
  // } else if (url.startsWith("/nfc.html?")) {
  //   String btnRunDetectTags = url.substring(url.indexOf("runDetectTags=") + 14, url.length());
  //   String btnClear = url.substring(url.indexOf("clear=") + 6, url.length());
  //   String btnEmulateNFC = "";
  //   int index = url.indexOf("emulateState=");
  //   if (index != -1) {
  //     btnEmulateNFC = url.substring(index + 13);
  //   }

  //   if (btnClear.startsWith("true")) {
  //     clearNFCValuesFlag = true;
  //   }

  //   if (btnRunDetectTags.startsWith("true")) {
  //     mode = 1;
  //     resetMode();
  //     runDetectTags = true;
  //     nfcExecutionCounter = 0;
  //     nfcDiscoverySuccess = false;
  //     emulateNFCFlag = false;
  //   }

  //   if (btnEmulateNFC.startsWith("true")) {
  //     mode = 2;
  //     resetMode();
  //     emulateNFCFlag = true;
  //     debug.println("\nWaiting for reader command...");
  //     emulateNFCIDTimer = millis();
  //   } else if (btnEmulateNFC.startsWith("false")) {
  //     emulateNFCFlag = false;
  //     attempts = 0;
  //     nfc.StopDiscovery();
  //   }
  // } else if (url.startsWith("/magspoof.html?")) {
  //   updateTracks(url);

  //   String button = url.substring(url.indexOf("button=") + 7, url.length());

  //   if (button.startsWith("Emulate")) {
  //     runMagspoof = true;
  //   }
  // }
}
