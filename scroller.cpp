#include <Arduino.h>
#include <string.h>
#include <math.h>
#include "scroller.h"
#include "utils.h"


// Define static const member
const char Scroller::char_map[] = "abcdefghijklmnopqrstuvwxyz0123456789?!().,";

Scroller::Scroller(char* msg) {
    message = msg;
    message_length = strlen(msg);
}

// Initialize the scroller system
void Scroller::init(int16_t width, int16_t height) {
    screenWidth = width;
    screenHeight = height;

    // Initialize scrolltext variables
    c_offset = screenWidth;
    d = 0;

    // Initialize sine table for wave effect
    for (uint16_t i = 0; i < 360; i++) {
        sin_t[i] = sin(i * M_PI / 180) * 25;
    }
}

// Get character offset in font image
uint32_t Scroller::get_char_offset(char c) {
    uint32_t offset = 0;

    for(uint16_t i = 0; i < 7; i++) {
        offset = i*6*CHAR_WIDTH*CHAR_HEIGHT;
        for(uint16_t j = 0; j < 6; j++) {
            if (c == char_map[i*6+j]) {
                return offset;
            }
            offset += CHAR_WIDTH;
        }
    }

    return 41; // '.'
}

// Draw a character slice
void Scroller::blit_char(LGFX_Sprite* sprite, uint32_t offset, uint16_t char_offset, uint16_t char_width, int32_t x_dest, int32_t y_dest) {
    for (uint16_t y = 0; y < CHAR_HEIGHT; y++) {
        for (uint16_t x = 0; x < char_width; x++) {
            // Calculate source coordinates in font image
            uint32_t src = offset + x + char_offset + y*6*CHAR_WIDTH;

            uint16_t color = img_260[src];
            // Draw pixel
            if (color != 0) {
                sprite->drawPixel(x_dest+x, y_dest+y, color);
            }
        }
    }
}

// Draw the scrolling text
void Scroller::draw(LGFX_Sprite* sprite) {
    LGFX_Sprite font_sprite(sprite);
    font_sprite.setBuffer((void*)img_260, 288, 350, 16);

    for (uint16_t i = 0; i < message_length; i++) {
        if (message[i] == ' ') {
            c_offset += CHAR_WIDTH;
            continue;
        }

        uint32_t offset = get_char_offset(message[i]);

        for (uint8_t j = 0; j < CHAR_WIDTH; j+=SLICE_SIZE) {
            int32_t x_pos = c_offset;
            if (x_pos > screenWidth) break;
            if (x_pos <= -SLICE_SIZE) {
                c_offset += SLICE_SIZE;
                continue;
            };
            int32_t y_pos = screenHeight / 2 + sin_t[(int) (1.5*fabs(c_offset)) % 360];
            // Draw slice of character
            blit_char(sprite, offset, j, SLICE_SIZE, x_pos, y_pos);
            c_offset += SLICE_SIZE;
        }
    }

    if (d > message_length * CHAR_WIDTH + screenWidth) {
        d = 0;
    }

    c_offset = screenWidth - d;
    d += 5;
}
