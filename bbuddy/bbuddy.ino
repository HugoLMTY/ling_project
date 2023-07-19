#include <V:\Arduino Stuff\bbuddy\WiFiCredentials.h>
#include <V:\Arduino Stuff\bbuddy\oled.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <Adafruit_SSD1306.h>
#define width 128 //* OLED display width,  in pixels
#define height 64 //* OLED display height, in pixels
Adafruit_SSD1306 oled(width, height, &Wire, -1);

const int messageTimeout = 3000;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const String retrieveKillUrl = "https://hook.eu1.make.com/pl2lr3hqf5brnqt5l0cscqwdejb949ie";
const String listPlayersUrl = "";

const String PlayerId = "420";
int killCount = 0;

const int btnKillPin = 14;
const int btnAltPin = 12;

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(btnKillPin, INPUT);

  oled_setup();

  initConnection();
}

void loop() {
  int btnKillState = digitalRead(btnKillPin);

  if (WiFi.status() != WL_CONNECTED) {
    resetConnection();
  }

  if (btnKillState == HIGH) {
    sendRequest(retrieveKillUrl);
    delay(200);
  }
}

void initConnection() {
  oline("Hola!", true);
  delay(1420);

  oline("NW: " + String(ssid), true);
  setNavMessage(String(ssid), "network");
  delay(1100);

  oline("Connexion..", true);

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

    if (false && dotsCpt == maxDots) {
      oline("Connexion au WiFi..", true, true);
    }
    setNavMessage(dots, "status");
    delay(messageTimeout / 4);
  }

  // oline("Connexion réussie :)", true, true);
  setNavMessage("", "status");
  
  IPAddress ip = WiFi.localIP();
  String stringIP = ip.toString();

  oline("WiFi: ", false, true);
  oline(String(ssid), true);
  delay(420);

  oline("Joueur", false);
  oline(PlayerId);

  setNavMessage("", "kill");

  delay(messageTimeout);
}

void resetConnection() {
    oline("WiFi KO :( sadge", true, true);
    setNavMessage(":(", "status");
    delay(messageTimeout);
    initConnection();
}

void sendRequest(String url) {
  Serial.print("Sending query as player "); Serial.print(PlayerId); Serial.println(", to: "); 
  Serial.println(url);
  
  oline("Clik bam pow", false);

  HTTPClient http;
  WiFiClientSecure wifi;
  wifi.setInsecure(); 

  http.begin(wifi, url);
  http.addHeader("Content-Type", "application/json"); 
  String json = "{\"Player\":\"" + PlayerId + "\"}";
  
  int responseType = http.POST(json); 
  String response = http.getString();


  if (responseType > 0) {
    oline("bim");
    killCount++;
    setNavMessage("", "kill");
  } else {
    oline(http.errorToString(responseType).c_str());
    resetConnection();
  }

  http.end();
}
