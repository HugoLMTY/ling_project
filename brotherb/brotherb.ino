#include <WiFi.h>
#include <esp_camera.h>

// Remplacez ces informations par les détails de votre réseau Wi-Fi
const char* ssid = "Votre_SSID";
const char* password = "Votre_Mot_De_Passe";

// Paramètres de la caméra
#define CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    4
#define SIOD_GPIO_NUM    18
#define SIOC_GPIO_NUM    23
#define Y9_GPIO_NUM      36
#define Y8_GPIO_NUM      37
#define Y7_GPIO_NUM      38
#define Y6_GPIO_NUM      39
#define Y5_GPIO_NUM      35
#define Y4_GPIO_NUM      14
#define Y3_GPIO_NUM      13
#define Y2_GPIO_NUM      34
#define VSYNC_GPIO_NUM   5
#define HREF_GPIO_NUM    27
#define PCLK_GPIO_NUM    25

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Initialise la caméra
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Erreur lors de l'initialisation de la caméra : 0x%x", err);
    return;
  }

  // Connexion au réseau Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au réseau Wi-Fi en cours...");
  }

  // Affiche l'adresse IP sur le moniteur série
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());

  // Démarrage du serveur
  server.begin();
}

void loop() {
  // Attente de la connexion d'un client
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Lecture des données de la caméra
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Erreur de capture d'image");
    return;
  }

  // Envoi des en-têtes HTTP
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: image/jpeg");
  client.println("Connection: close");
  client.println();

  // Envoi des données de l'image
  client.write(fb->buf, fb->len);

  // Libération de la mémoire utilisée par l'image
  esp_camera_fb_return(fb);

  // Attente de la fin de la réponse HTTP
  delay(10);
  client.stop();
}
