# 1. Credentials

To enter your credentials create `include/credentials.h` and use the follwoing template. Git will ignore this file so your credentials stay local!

```c
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

const String WIFI_SSID = "Your WiFi-SSID";
const String WIFI_PASS = "Your WiFi-Password";

const String MQTT_BROKER = "Your MQTT-Broker";
const int MQTT_PORT = 1883;
const String MQTT_USER = "Your MQTT-User";
const String MQTT_PASS = "Your MQTT-Password";

const String HOSTNAME = "Your Hostname";
const String PUB_TOPIC = "Your Topic to publish";
const String SUB_TOPIC = "Topic to listen to";

#endif
```

