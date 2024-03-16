#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <LiquidCrystal_I2C.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include <U8g2lib.h>

#include <WiFi.h>

#include "credentials.h"
#include "gpio_mapping.h"

#include "mqtt.h"
#include "logger.h"

#define BAUD_RATE 9600

#define PIXELS 6

uint8_t previous_state = HIGH;
uint8_t current_state = HIGH;

Adafruit_NeoPixel pixels{PIXELS, GPIO_NEOPIXEL, NEO_GRB + NEO_KHZ800};

Adafruit_MPU6050 mpu;

U8G2_SSD1306_128X64_NONAME_1_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);

int oled_width, oled_height;
int text_width, text_height;

int hour = 0;
int minute = 0;
int seconds = 0;

char* text = (char*)malloc(8);

WiFiClient wiFiClient{};
Mqtt mqttClient{wiFiClient, HOSTNAME, SUB_TOPIC};

//Logger logger{"Foo"};

void callback(char* topic, byte* payload, unsigned int length); 

void setup() {
  // Setup PinModes
  pinMode(GPIO_BUTTON, INPUT_PULLUP);
  pinMode(GPIO_PHOTO, ANALOG);
  pinMode(GPIO_NEOPIXEL, OUTPUT);

  // Setup of Seriell-Connection
  Serial.begin(BAUD_RATE);
  while (!Serial);

  // Setup WiFi
  WiFi.setHostname(HOSTNAME.c_str());
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Next attempt to connect WiFi...");
  }

  // Setup MQTT
  mqttClient.setup(MQTT_BROKER,  MQTT_PORT, callback);
  mqttClient.reconnect();

  // NeoPixel
  pixels.begin();

  /*
  // I2C

  // OLED
  Wire.begin(GPIO_SDA, GPIO_SCL);

  oled.begin();
  oled.setFont(u8g2_font_logisoso42_tf);
  oled.setDrawColor(1);

  oled_width = oled.getWidth();
  oled_height = oled.getHeight();

  text_height = 42;

  sprintf(text, "00:00");

  // MPU
    if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  */
}

void loop() {
  if (!mqttClient.client.connected()) {
    mqttClient.reconnect();
  }

  mqttClient.client.loop();  
  
  current_state = digitalRead(GPIO_BUTTON);

  if (current_state != previous_state)
  {
    if (current_state == HIGH)
    {
      Serial.println("Butten released!");
    }
    if (current_state == LOW)
    {
      Serial.println("Butten pressed!");
      mqttClient.client.publish(PUB_TOPIC.c_str(), "Butten pressed!");
    }
  }

  previous_state = current_state;

  /*
  logger.log(Logger::Level::INFO, "Hello!");
  //mqttClient.publish("/test", "Hello from Entagled Heart!");
  delay(1000);

  // Photo + Button
  int value = analogRead(GPIO_PHOTO);
  Serial.printf("Light: %d\n", value);

  delay(500);  
  */
 
  /*
  current_state = digitalRead(GPIO_BUTTON);

  if (current_state != previous_state)
  {
    if (current_state == HIGH)
    {
      Serial.println("Butten released!");
    }
    if (current_state == LOW)
    {
      Serial.println("Butten pressed!");
      mqttClient.publish("/test", "Butten pressed!");
    }
  }

  previous_state = current_state; 
  */

  


  /*
  // NeoPixel
  for (size_t i = 0; i < PIXELS; ++i)
  {
    pixels.setPixelColor(i, pixels.Color(255,0,0));
    pixels.show();
    delay(250);
  }

  for (size_t i = 0; i < PIXELS; ++i)
  {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
    delay(250);
  }
  */

  /*
  // I2C
  text_width = oled.getUTF8Width(text);
  static int y_offset = text_height + (oled_height-text_height)/2;
  static int x_offset = (oled_width-text_width)/2;

  sprintf(text, "%02d:%02d", hour, minute); 
  
  oled.firstPage();
  do 
  {
    oled.drawStr(x_offset, y_offset, text);
  } 
  while (oled.nextPage());

  ++minute;
  if (minute >= 60) {
    ++hour;
    minute = 0;
  }

    if (hour >= 24) {
    hour = 0;
  }

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.println("");

  delay(1000);
  */   
}

void search_i2c()
{
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
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  static bool mode = true;


  if(strcmp(topic, SUB_TOPIC.c_str()) == 0) {
    Serial.println("Received message on valid topic");
    Serial.println(topic);

    for (size_t i = 0; i < PIXELS; ++i) {
      pixels.setPixelColor(i, pixels.Color(255,0,0));
      pixels.show();
    }

    if (mode) {
      for (size_t i = 0; i < PIXELS; ++i) {
        pixels.setPixelColor(i, pixels.Color(255,0,0));
        pixels.show();

        mode = false;
      }
    } else {
      for (size_t i = 0; i < PIXELS; ++i) {
        pixels.setPixelColor(i, pixels.Color(0,0,0));
        pixels.show();
      }

      mode = true;
    }
  }
}
