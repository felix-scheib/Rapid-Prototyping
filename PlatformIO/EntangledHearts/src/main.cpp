#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <Wire.h>

#include <WiFi.h>

#include "credentials.h"
#include "gpio_mapping.h"

#include "display.h"
#include "mpu.h"
#include "mqtt.h"
#include "ntp.h"

#define BAUD_RATE 9600

#define PIXELS 6

static const uint16_t INTENSITY_THRESHOLD = 100;
static const uint16_t BRIGHTNESS_MAX = 4095;

uint8_t previous_state = HIGH;
uint8_t current_state = HIGH;

Adafruit_NeoPixel pixels{PIXELS, GPIO_NEOPIXEL, NEO_GRB + NEO_KHZ800};

WiFiClient wiFiClient{};
Mqtt mqttClient{wiFiClient, HOSTNAME, SUB_TOPIC};

Display display{};

Mpu mpu{};

void callback(char* topic, byte* payload, unsigned int length);

uint16_t get_intensity();
void set_neo_pixel(uint8_t r = 0, uint8_t b = 0, uint8_t g = 0);

static bool pair_signal = false;
static bool button_pressed = false;

static bool attraction_mode = false;

static unsigned long start_time = 0;
static unsigned long ellapsed_time = 0;

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

    // Setup mpu
    mpu.setup();

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

    if (!attraction_mode) {
        uint32_t intensity = get_intensity();
        bool is_bright = (intensity > INTENSITY_THRESHOLD);

        bool is_movement = mpu.update();

        bool button_pressed = (digitalRead(GPIO_BUTTON) == LOW);

        if ((is_bright && is_movement) || button_pressed) {
            Serial.print("Enable Display by: ");

            if ((is_bright && is_movement)) {
                Serial.println("movement");
            }

            if (button_pressed) {
                Serial.println("button");
            }

            mqttClient.client.publish(PUB_TOPIC.c_str(), "y");

            display.display.setContrast(255);
            attraction_mode = true;

            start_time = millis();
        }
    } else {
        if ((millis() - start_time) > 5000) {
            Serial.println("Disable Display...");

            mqttClient.client.publish(PUB_TOPIC.c_str(), "n");

            display.display.setContrast(0);
            attraction_mode = false;
        }
    }

    if (attraction_mode && pair_signal) {
        Serial.println("Set NeoPixel red...");
        set_neo_pixel(255, 0, 0);
    } else if (pair_signal) {
        Serial.println("Set NeoPixel blue...");
        set_neo_pixel(0, 0, 255);
    } else {
        Serial.println("Disable NeoPixel...");
        set_neo_pixel(0, 0, 0);
    }
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
        }

        if (length > 0 && payload[0] == 'n') {
            pair_signal = false;
        }
    }
}

uint16_t get_intensity() {
    uint32_t measurements = 0;

    for (size_t i = 0; i < 32; ++i) {
        measurements += analogRead(GPIO_PHOTO);
    }

    measurements /= 32;

    return BRIGHTNESS_MAX - measurements;
}

void set_neo_pixel(uint8_t r, uint8_t g, uint8_t b) {
        for (size_t i = 0; i < PIXELS; ++i) {
        pixels.setPixelColor(i, r, g, b);
        pixels.show();
    }
}
