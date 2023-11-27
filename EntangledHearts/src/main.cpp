#include <Arduino.h>

#include "credentials.h"

#include <WiFi.h>
#include <PubSubClient.h>

#define GPIO12 12

int prev_state = LOW;
int current_state = LOW;

const char mqtt_id[] = "Entangled Heart";
const char mqtt_topic[] = "/test_topic";

WiFiClient wiFiClient{};
PubSubClient mqttClient{wiFiClient};

void callback(char* topic, byte* payload, unsigned int length); 

void setup()
{
  Serial.begin(9600);

  pinMode(GPIO12, INPUT);

  Serial.println("Starting WiFi-Connection...");

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Next attempt to connect WiFi...");
  }

  Serial.println("WiFi connected!");

  Serial.println("Starting MQTT-Connection...");

  mqttClient.setServer(MQTT_BROKER,  MQTT_PORT);
  mqttClient.setCallback(callback);

  while (!mqttClient.connected()) {
    if (mqttClient.connect(mqtt_id, MQTT_USER, MQTT_PASS)) {
      mqttClient.subscribe(mqtt_topic);
    } else {
      Serial.println("Failed to connect to MQTT-Broker!");
      Serial.println(mqttClient.state());
      delay(500);
      Serial.println("Next attempt to connect to MQTT-Broker...");
    }
  }

  Serial.println("MQTT connected!"); 

  Serial.println("Initialization done!");
}

void loop()
{
  //MQTT
  mqttClient.loop();


  //Button
  current_state = digitalRead(GPIO12);

  if (current_state != prev_state)
  {
    if (current_state == HIGH)
    {
      Serial.println("Button pressed!");
      mqttClient.publish(mqtt_topic, "Hello from Entagled Heart!");
    }

    if (current_state == LOW)
    {
      Serial.println("Button released!");
    }
    
    delay(50);
  }

  prev_state = current_state;
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Nachricht empfangen auf Thema: ");
  Serial.println(topic);
  if (strcmp(topic, mqtt_topic) == 0) {
    String message = "";
    for (int i = 0; i < length; i++) {
      message += (char)payload[i];
    }
    Serial.print("Empfangene Nachricht: ");
    Serial.println(message);
  }
}
