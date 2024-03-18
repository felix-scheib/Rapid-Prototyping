#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <WiFi.h>

#include "credentials.h"
#include "gpio_mapping.h"

#include "display.h"
#include "mqtt.h"
#include "ntp.h"

#define BAUD_RATE 9600

#define PIXELS 6

uint8_t previous_state = HIGH;
uint8_t current_state = HIGH;

Adafruit_NeoPixel pixels{PIXELS, GPIO_NEOPIXEL, NEO_GRB + NEO_KHZ800};

Adafruit_MPU6050 mpu;

WiFiClient wiFiClient{};
Mqtt mqttClient{wiFiClient, HOSTNAME, SUB_TOPIC};

Display display{};

void callback(char* topic, byte* payload, unsigned int length);

static bool pair_signal = false;
static bool button_pressed = false;

void setup() {
    // Setup PinModes
    pinMode(GPIO_BUTTON, INPUT_PULLUP);
    pinMode(GPIO_PHOTO, ANALOG);
    pinMode(GPIO_NEOPIXEL, OUTPUT);

    Wire.begin(GPIO_SDA, GPIO_SCL);

    // Setup of Seriell-Connection
    Serial.begin(BAUD_RATE);
    while (!Serial);

    // Setup WiFi
    WiFi.setHostname(HOSTNAME.c_str());
    WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Next attempt to connect WiFi...");
    }

    // Setup time
    fetch_time();
    Serial.printf("%02d:%02d:%02d\n", hour(), minute(), second());

    // Setup display
    display.setup();

    // Setup MQTT
    mqttClient.setup(MQTT_BROKER,  MQTT_PORT, callback);
    mqttClient.reconnect();

    // NeoPixel
    pixels.begin();
}

void loop() {
    display.update();

    if (!mqttClient.client.connected()) {
        mqttClient.reconnect();
    }

    mqttClient.client.loop();  

    current_state = digitalRead(GPIO_BUTTON);

    if (current_state != previous_state) {
        if (current_state == HIGH) {
            Serial.println("Butten released!");
            display.display.setContrast(0);

            mqttClient.client.publish(PUB_TOPIC.c_str(), "n");

            button_pressed = false;

            if (pair_signal) {
                for (size_t i = 0; i < PIXELS; ++i) {
                    pixels.setPixelColor(i, pixels.Color(0,0,255));
                    pixels.show();
                }
            }
            

        }
        if (current_state == LOW) {
            Serial.println("Butten pressed!");
            display.display.setContrast(255);

            mqttClient.client.publish(PUB_TOPIC.c_str(), "y");

            if (pair_signal) {
                for (size_t i = 0; i < PIXELS; ++i) {
                    pixels.setPixelColor(i, pixels.Color(255,0,0));
                    pixels.show();
                }
            }

            button_pressed = true;
        }
    }

    previous_state = current_state;
}

void search_i2c() {
    Serial.println("Scanning for I2C Device...");

    byte error, address;
    int nDevices;
    nDevices = 0;

    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address<16) {
                Serial.print("0");
            }
            Serial.println(address,HEX);
            nDevices++;
        } else if (error==4) {
            Serial.print("Unknow error at address 0x");
            if (address<16) {
                Serial.print("0");
            }
            Serial.println(address,HEX);
        }    
    }
    if (nDevices == 0) {
        Serial.println("No I2C devices found\n");
    } else {
        Serial.println("done\n");
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    if(strcmp(topic, SUB_TOPIC.c_str()) == 0) {
        Serial.println("Received message on valid topic");
        Serial.println(topic);

        if (length > 0 && payload[0] == 'y') {
            pair_signal = true;

            if (button_pressed) {
                for (size_t i = 0; i < PIXELS; ++i) {
                    pixels.setPixelColor(i, pixels.Color(255,0,0));
                    pixels.show();
                }
            } else {
                for (size_t i = 0; i < PIXELS; ++i) {
                    pixels.setPixelColor(i, pixels.Color(0,0,255));
                    pixels.show();
                }
            }
        }

        if (length > 0 && payload[0] == 'n') {
            pair_signal = false;

            for (size_t i = 0; i < PIXELS; ++i) {
                pixels.setPixelColor(i, pixels.Color(0,0,0));
                pixels.show();
            }
        }
    }
}
