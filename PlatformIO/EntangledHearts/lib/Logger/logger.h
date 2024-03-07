#pragma once

#include <Arduino.h>
#include <PubSubClient.h>

class Logger {
public:
    enum class Level {
        STARTUP,
        ERROR,
        WARNING,
        INFO,
        DEBUG,
    };

    Logger(const char *hostname);
    ~Logger() = default;

    void log(Level level, const char *msg);

    void setSerial(HardwareSerial *serial);
    void setMqtt(PubSubClient *mqtt);
private:
    const char *hostname;

    HardwareSerial *serial;
    PubSubClient *mqtt;

    Level level;

    inline const char *levelToString(Level level);
};
