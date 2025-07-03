#ifndef STARFIELD_H
#define STARFIELD_H

#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>
#include "utils.h"


// Starfield class declaration
class Starfield {
private:
    int16_t screenWidth;
    int16_t screenHeight;
    
    // Star properties
    static const uint16_t NUM_STARS = 200;

    struct Star {
        struct vector3d location;
        uint32_t colour;
    };

    Star stars[NUM_STARS];
    void draw_stars(LGFX_Sprite* sprite);
    void update_stars();
    
public:
    Starfield();
    
    // Initialize the starfield system
    void init(int16_t width, int16_t height);

    // Draw the starfield
    void draw(LGFX_Sprite* sprite);
};

#endif // STARFIELD_H 