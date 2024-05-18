#include <WiFi.h>
#include <PubSubClient.h>

// Remplacez par vos identifiants WiFi
const char* ssid = "ODC_L1_2.4";
const char* password = "12345678";

// Remplacez par l'adresse IP de votre broker MQTT
const char* mqtt_server = "192.168.250.211";

WiFiClient espClient;
PubSubClient client(espClient);

const int socketPin = 2; // Remplacez par la broche où la prise connectée est contrôlée

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; // Assurez-vous que le message est une chaîne terminée par null
  String message = String((char*)payload);

  if (String(topic) == "esp/socket") {
    if(message == "ON") {
      digitalWrite(socketPin, HIGH); // Allumer la prise connectée
    } else if(message == "OFF") {
      digitalWrite(socketPin, LOW); // Éteindre la prise connectée
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      client.subscribe("esp/socket");
    }
    delay(5000);
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(socketPin, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
