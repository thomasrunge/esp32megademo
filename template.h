#ifndef TEMPLATE_H
#define TEMPLATE_H

#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>
#include "utils.h"


// Starfield class declaration
class Template {
private:
    int16_t screenWidth;
    int16_t screenHeight;

public:
    Template();
    
    // Initialize the template system
    void init(int16_t width, int16_t height);

    // Draw the image rotate
    void draw(LGFX_Sprite* sprite);
};

#endif // TEMPLATE_H
