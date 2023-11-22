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
String defaultSSID;
String defaultPassword;
int status;
URL webRequest = INDEX_HTML;
Debug &debug = Debug::getInstance();
void setupWiFi();
void loadPageContent(WiFiClient client);
void showPageContent(WiFiClient client, const char *pageContent);
String decodeURL(char *url);
void updateWebRequest(String url);
void handleRequests();
void handleURLParameters(String url);
void runServer();
String ipToString(IPAddress ip);
#endif

void setup() {
  menu.begin();
#ifdef ESP32
  setupWiFi();
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
  runServer();
#endif
}

#ifdef ESP32
void setupWiFi() {
#ifdef ESP32
  debug.println("");
  debug.println("Configuring access point...");
  defaultSSID += menu.speaker.getSSID();
  defaultPassword = menu.speaker.getPassword();

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(defaultSSID.c_str(), defaultPassword.c_str())) {
    log_e("Soft AP creation failed.");
    while (1)
      ;
  }
  IPAddress myIP = WiFi.softAPIP();
  debug.print("AP IP address: ");
  debug.println(ipToString(myIP));
  menu.speaker.setIP(ipToString(WiFi.softAPIP()));
  server.begin();

  debug.println("Server started");
#endif
}

void runServer() {
  WiFiClient client = server.available();  // listen for incoming clients

  if (client) {  // if you get a client,
    unsigned long speedTestTime = millis();
    String currentLine = "";  // make a String to hold incoming data from the client

    while (client.connected()) {  // loop while the client's connected
      if (client.available()) {   // if there's bytes to read from the client,
        char c = client.read();   // read a byte, then
        // debug.write(c);  // print it out the debug monitor

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
          debug.println("\nRequest: " + currentLine);
          debug.println("URL: " + url);

          updateWebRequest(url);
          handleURLParameters(url);
        }
      }
    }
    client.stop();
    // debug.println("client disconnected");
    // debug.println("Time to run server: " + String(millis() - speedTestTime) + " ms");
  }

  handleRequests();
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
}

void handleURLParameters(String url) {
  // ? is the start of request parameters
  // URL: /index.html?red=255&green=0&blue=255
  if (url.startsWith("/index.html?")) {
    String redValue = "";
    String greenValue = "";
    String blueValue = "";

    int index = url.indexOf("red=");
    if (index != -1) {
      redValue = url.substring(index + 4, url.indexOf("&green="));
    }

    index = url.indexOf("green=");
    if (index != -1) {
      greenValue = url.substring(index + 6, url.indexOf("&blue="));
    }

    index = url.indexOf("blue=");
    if (index != -1) {
      blueValue = url.substring(index + 5, url.length());
    }

    debug.println("redValue: " + redValue);
    debug.println("greenValue: " + greenValue);
    debug.println("blueValue: " + blueValue);

    int red = redValue.toInt();
    int green = greenValue.toInt();
    int blue = blueValue.toInt();

    for (int i = 0; i < NUMPIXELS; i++) {
      menu.pixels.setPixelColor(i, menu.pixels.Color(red, green, blue));
    }
    menu.pixels.show();
  }
}

String ipToString(IPAddress ip) {
  String result = "";
  for (int i = 0; i < 4; i++) {
    result += String(ip[i]);
    if (i < 3) {
      result += ".";
    }
  }
  return result;
}
#endif