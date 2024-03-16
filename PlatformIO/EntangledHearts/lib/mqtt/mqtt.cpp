#include "mqtt.h"

Mqtt::Mqtt(WiFiClient &client, const String &id, const String &sub_topic) :
    client(client), ID(id), SUB_TOPIC(sub_topic) {
}

void Mqtt::setup(const String &broker, int port, std::function<void(char *, uint8_t *, unsigned int)> callback) const {
    Serial.println("Setup of MQTT-Connection...");

    client.setServer(broker.c_str(), port);
    client.setCallback(callback);
}

void Mqtt::reconnect() const {
    Serial.println("Connecting to MQTT-Broker...");

    while (!client.connected()) {
        if (client.connect(ID.c_str())) {
            client.subscribe(SUB_TOPIC.c_str());
            
            break;
        } else {
            Serial.println("Failed to connect to MQTT-Broker");
            Serial.println(client.state());

            delay(500);
        }
    }

    Serial.println("Successfully connected to MQTT-Broker");
}
