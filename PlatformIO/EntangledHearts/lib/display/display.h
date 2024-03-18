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

    static const int TEXT_HEIGHT;
    static const char *const DEFAULT_TIME;

    int current_hour, current_minute;
    int display_width, display_height;
    int text_width, text_height;

    char text[8];

    U8G2_SSD1306_128X64_NONAME_1_HW_I2C display;
};

#endif