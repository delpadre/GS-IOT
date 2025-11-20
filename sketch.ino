#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

// -------------------------
#define LED_PIN 4
#define BUZZER_PIN 32
#define DHT_PIN 15
#define TRIG_PIN 5
#define ECHO_PIN 18
// -------------------------

DHTesp dht;
WiFiClient espClient;
PubSubClient client(espClient);

// -------------------------
// WiFi + MQTT
// -------------------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.hivemq.com";

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("GS2025_ESP32")) {
      Serial.println(" OK!");
      client.subscribe("gs2025/comando");
    } else {
      Serial.print(" Falhou. rc=");
      Serial.println(client.state());
      delay(1000);
    }
  }
}

void mqttCallback(char* topic, byte* message, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)message[i];

  if (msg == "ligar_led") digitalWrite(LED_PIN, HIGH);
  if (msg == "desligar_led") digitalWrite(LED_PIN, LOW);
  if (msg == "beep") tone(BUZZER_PIN, 1000, 200);
}

// -------------------------
// HC-SR04 com timeout
// -------------------------
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Timeout de 30ms → evita travar caso o sensor não esteja presente
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return 999;  // sem sensor → devolve distância "muito longe"
  }

  float distance = duration * 0.0343 / 2;
  return distance;
}
// -------------------------

void setup() {
  Serial.begin(115200);
  delay(1500);
  Serial.println("Serial iniciado!");

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  dht.setup(DHT_PIN, DHTesp::DHT22);

  // WIFI
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println(" conectado!");

  // MQTT
  client.setServer(mqttServer, 1883);
  client.setCallback(mqttCallback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  TempAndHumidity data = dht.getTempAndHumidity();
  float distance = getDistance();

  // Debug
  Serial.print("Temp: ");
  Serial.print(data.temperature);
  Serial.print(" °C | Umidade: ");
  Serial.print(data.humidity);
  Serial.print("% | Distância: ");
  Serial.println(distance);

  // Envio dos dados
  String payload = String("{\"temp\":") + data.temperature +
                   ",\"umidade\":" + data.humidity +
                   ",\"distancia\":" + distance + "}";

  client.publish("gs2025/dados", payload.c_str());

  // ALERTA DE APROXIMAÇÃO (distância)
  if (distance < 20 && distance > 0) {
    tone(BUZZER_PIN, 900, 200);
    client.publish("gs2025/alerta", "Objeto muito próximo!");
  }

  // === CONTROLE DE TEMPERATURA PARA HOME OFFICE ===
  if (data.temperature < 20) {
    digitalWrite(LED_PIN, HIGH);  
    client.publish("gs2025/alerta", "Temperatura Baixa para Home Office!");
  }
  else if (data.temperature > 26) {
    digitalWrite(LED_PIN, HIGH);
    client.publish("gs2025/alerta", "Temperatura Alta para Home Office!");
  }
  else {
    digitalWrite(LED_PIN, LOW);   // Confortável
  }

  delay(2000);
}

