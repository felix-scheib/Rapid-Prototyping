#ifndef NEOPIXEL_H
#define NEOPIXEL_H

//#include "gpio_mapping.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

struct Neopixel {
    enum class Mode {
        NONE,
        BLUE,
        RED,
    };

    explicit Neopixel(uint8_t gpio, uint16_t count);

    void setup();
    void update();

    void set_mode(Mode new_mode);

    Adafruit_NeoPixel pixel;
    Mode mode;
    uint16_t count;

    unsigned long current_time;
    unsigned long period;

    float factor;
    float phase;
    float range;
    float offset;

    static const unsigned long PERIOD_BLUE;
    static const float PHASE_BLUE;
    static const uint8_t MIN_BLUE;
    static const uint8_t MAX_BLUE;
    static const float RANGE_BLUE;
    static const float OFFSET_BLUE;

    static const unsigned long PERIOD_RED;
    static const float PHASE_RED;
    static const uint8_t MIN_RED;
    static const uint8_t MAX_RED;
    static const float RANGE_RED;
    static const float OFFSET_RED;
};

#endif