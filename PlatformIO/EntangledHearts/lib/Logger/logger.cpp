// Structure of logging message: [DATE HOST LEVEL] msg

/*
// Allocates storage
char *hello_world = (char*)malloc(13 * sizeof(char));
// Prints "Hello world!" on hello_world
sprintf(hello_world, "%s %s!", "Hello", "world");
*/

#include "logger.h"

Logger::Logger(const char *hostname) :
    hostname(hostname), serial(nullptr), mqtt(nullptr) {
}

void Logger::log(Logger::Level level, const char *msg) {
    //TODO: add level and create function for formatting
    if (serial) {
        serial->printf("%s: %s", hostname, msg);
    }
    if (mqtt) {
        mqtt->publish("/test", msg);
    }
}


void Logger::setSerial(HardwareSerial *serial) {
    this->serial = serial;
}

void Logger::setMqtt(PubSubClient *mqtt) {
    this->mqtt = mqtt;
}

inline const char *Logger::levelToString(Logger::Level level) {
    switch (level) {
    case Level::DEBUG:
        return "";
    case Level::ERROR:
        return "";
    case Level::INFO:
        return "";
    case Level::STARTUP:
        return "";
    case Level::WARNING:
        return "";
    }
}