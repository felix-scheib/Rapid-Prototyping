#include "neopixel.h"

const unsigned long Neopixel::PERIOD_BLUE = 2 * 1000;
const float Neopixel::PHASE_BLUE = PI / 7;
const uint8_t Neopixel::MIN_BLUE = 0;
const uint8_t Neopixel::MAX_BLUE = 63;
const float Neopixel::RANGE_BLUE = (Neopixel::MAX_BLUE - Neopixel::MIN_BLUE) / 2.0f;
const float Neopixel::OFFSET_BLUE = Neopixel::MIN_BLUE + Neopixel::RANGE_BLUE;

const unsigned long Neopixel::PERIOD_RED = 1 * 1000;
const float Neopixel::PHASE_RED = 0;
const uint8_t Neopixel::MIN_RED = 0;
const uint8_t Neopixel::MAX_RED = 127;
const float Neopixel::RANGE_RED = (Neopixel::MAX_RED - Neopixel::MIN_RED) / 2.0f;
const float Neopixel::OFFSET_RED = Neopixel::MIN_RED + Neopixel::RANGE_RED;

Neopixel::Neopixel(uint8_t gpio, uint16_t count) : 
    pixel{count, gpio, NEO_GRB + NEO_KHZ800}, mode(Mode::NONE), count(count) {
}

void Neopixel::setup() {
    pixel.begin();
}

void Neopixel::update() {
    if (mode != Mode::NONE) {
        float output;

        if (mode == Mode::BLUE) {
            period = PERIOD_BLUE;
            phase = PHASE_BLUE;
            range = RANGE_BLUE;
            offset = OFFSET_BLUE;
        }

        if (mode == Mode::RED) {
            period = PERIOD_RED;
            phase = PHASE_RED;
            range = RANGE_RED;
            offset = OFFSET_RED;
        }

        current_time = millis() % period;
        factor = (float)current_time/(float)period;

        for(int i=0; i<count; ++i) {
            output = sin(PI * 2 * factor + (phase * i)) * range + offset;

            if (mode == Mode::BLUE) {
                pixel.setPixelColor(i, Adafruit_NeoPixel::Color(0, 0, (uint8_t)output));
            }

            if (mode == Mode::RED) {
                pixel.setPixelColor(i, Adafruit_NeoPixel::Color((uint8_t)output, 0, 0));
            }
        }

        pixel.show();
    }
}

void Neopixel::set_mode(Mode new_mode) {
    if (new_mode != mode) {
        mode = new_mode;

        if (mode == Mode::NONE) {
            pixel.clear();
            pixel.show();
        }
    }
}