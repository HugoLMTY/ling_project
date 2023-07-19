#include <V:\Arduino Stuff\battlemommy\WiFiCredentials.h>
#include <V:\Arduino Stuff\battlemommy\oled.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <Adafruit_SSD1306.h>
#define width 128 //* OLED display width,  in pixels
#define height 64 //* OLED display height, in pixels
Adafruit_SSD1306 oled(width, height, &Wire, -1);

const int messageTimeout = 3000;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

void setup() {
  Serial.begin(115200);
  delay(10);

  oled_setup();

  // initConnection();
  initAccessPoint();
}

void loop() {
  // if (WiFi.status() != WL_CONNECTED) {
  //   resetConnection();
  // }  
}

void initAccessPoint() {
  oline("Access pointing happening at " + String(AP_SSID) + "...? ", false);
  WiFi.mode(WIFI_AP);
  oline(WiFi.softAP(AP_SSID) ? "ඞ" : "no");
}

void initConnection() {
  oline("Hola!", true);
  delay(1420);

  oline("NW: " + String(ssid), true);
  // setNavMessage(String(ssid), "network");
  delay(1100);

  WiFi.begin(ssid, password);
  
  const int minDots = 0;
  const int maxDots = 4;
  int dotsCpt = minDots;

  while (WiFi.status() != WL_CONNECTED) {
    String dots = "";
    for (int i = minDots; i < dotsCpt; i++) {
        dots += ".";
    }

    if (dotsCpt >= maxDots) {
      dotsCpt = minDots;
    } else {
      dotsCpt++;
    }

    oline("Connexion au WiFi" + dots, true, true);
    // setNavMessage(dots, "status");

    delay(messageTimeout / 4);
  }

  // oline("Connexion réussie :)", true, true);
  // setNavMessage("", "status");
  
  IPAddress ip = WiFi.localIP();
  String stringIP = ip.toString();

  oline("WiFi: ", false, true);
  oline(String(ssid), true);
  delay(420);

  // setNavMessage("", "kill");

  delay(messageTimeout);

  initAccessPoint();
}

void resetConnection() {
    oline("WiFi KO :( sadge", true, true);
    // setNavMessage(":(", "status");
    delay(messageTimeout);
    initConnection();
}

