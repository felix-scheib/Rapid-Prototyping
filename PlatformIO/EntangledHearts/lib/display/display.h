#ifndef DISPLAY_H
#define DISPLAY_H

#include <LiquidCrystal_I2C.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include <U8g2lib.h>
#include <TimeLib.h>

struct Display {
    Display();

    void setup();
    void update();

    void set_contrast(uint8_t contrast, unsigned long fade = 0);
    void update_contrast();

    static const int TEXT_HEIGHT;
    static const char *const DEFAULT_TIME;

    int current_hour, current_minute;
    int display_width, display_height;
    int text_width, text_height;
    uint8_t current_contrast, wanted_contrast;

    char text[8];

    U8G2_SSD1306_128X64_NONAME_1_HW_I2C display;
};

#endif