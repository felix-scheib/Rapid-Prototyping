#include <Arduino.h>

#include "credentials.h"

#include <WiFi.h>
#include <PubSubClient.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define GPIO12 12
#define GPIO32 32

#define PIXELS 5

int prev_state = LOW;
int current_state = LOW;

const char mqtt_id[] = "Entangled Heart";
const char mqtt_topic[] = "/test_topic";
const char mqtt_color[] = "/color";

WiFiClient wiFiClient{};
PubSubClient mqttClient{wiFiClient};

Adafruit_NeoPixel pixels{PIXELS, GPIO32, NEO_GRB + NEO_KHZ800};

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
      mqttClient.subscribe(mqtt_color);
    } else {
      Serial.println("Failed to connect to MQTT-Broker!");
      Serial.println(mqttClient.state());
      delay(500);
      Serial.println("Next attempt to connect to MQTT-Broker...");
    }
  }

  Serial.println("MQTT connected!");

  pinMode(GPIO32, OUTPUT);
  pixels.begin();

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

  if (strcmp(topic, mqtt_color) == 0) {
    String message = "";
    for (int i = 0; i < length; i++) {
      message += (char)payload[i];
    }

    if (message == "red")
    {
      for (size_t i = 0; i < PIXELS; ++i)
      {
        pixels.setPixelColor(i, pixels.Color(255,0,0));
      }
      pixels.show();
    }

    if (message == "green")
    {
      for (size_t i = 0; i < PIXELS; ++i)
      {
        pixels.setPixelColor(i, pixels.Color(0,255,0));
      }
      pixels.show();
    }

    if (message == "blue")
    {
      for (size_t i = 0; i < PIXELS; ++i)
      {
        pixels.setPixelColor(i, pixels.Color(0,0,255));
      }
      pixels.show();
    }
  }
}
