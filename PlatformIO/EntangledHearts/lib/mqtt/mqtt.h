#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

struct Mqtt
{
    Mqtt(WiFiClient &client, const String &id, const String &sub_topic);

    void setup(const String &broker, int port, std::function<void(char *, uint8_t *, unsigned int)> callback) const;
    void reconnect() const;

    mutable PubSubClient client;
    const String ID;
    String SUB_TOPIC;
};

#endif