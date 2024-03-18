#include "display.h"

const int Display::TEXT_HEIGHT = 42;
const char *const Display::DEFAULT_TIME = "00:00";


Display::Display() :
    display(U8G2_R0, U8X8_PIN_NONE), text_height(TEXT_HEIGHT), current_contrast(0), wanted_contrast(0) {
}

void Display::setup() {
    display.begin();
    display.setFont(u8g2_font_logisoso42_tf);
    display.setDrawColor(1);
    display.setContrast(current_contrast);

    display_width = display.getWidth();
    display_height = display.getHeight();

    sprintf(text, DEFAULT_TIME);
}

void Display::update() {
    text_width = display.getUTF8Width(text);

    if (current_hour != hour() || current_minute != minute()) {
        int y_offset = text_height + (display_height-text_height)/2;
        int x_offset = (display_width-text_width)/2;

        current_hour = hour();
        current_minute = minute();

        sprintf(text, "%02d:%02d", current_hour, current_minute); 

        display.firstPage();
        do {
            display.drawStr(x_offset, y_offset, text);
        } 
        while (display.nextPage());
    }

    if (current_contrast != wanted_contrast) {
        update_contrast();
    }
    
}

void Display::set_contrast(uint8_t contrast, unsigned long fade) {    
    wanted_contrast = contrast;
}

void Display::update_contrast() {
    display.setContrast(wanted_contrast);
    current_contrast = wanted_contrast;
}
