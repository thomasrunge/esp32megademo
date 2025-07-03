#ifndef SCROLLER_H
#define SCROLLER_H

#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>
#include "image.h"

// Scroller class declaration
class Scroller {
private:
    int16_t screenWidth;
    int16_t screenHeight;

    // Scrolltext variables
    int c_offset;
    int d;
    float sin_t[360];

    // Character map and dimensions
    static const char char_map[];
    static const uint16_t CHAR_WIDTH = 48;
    static const uint16_t CHAR_HEIGHT = 50;
    static const uint8_t SLICE_SIZE = 6;

    // Message to display
    char* message;
    uint16_t message_length;

    // Helper functions
    uint32_t get_char_offset(char c);
    void blit_char(LGFX_Sprite* sprite, uint32_t offset, uint16_t char_offset, uint16_t char_width, int32_t x_dest, int32_t y_dest);

public:
    Scroller(char* msg);

    // Initialize the scroller system
    void init(int16_t width, int16_t height);

    // Draw the scrolling text
    void draw(LGFX_Sprite* sprite);
};

#endif // SCROLLER_H
