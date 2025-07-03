#include <Arduino.h>
#include <math.h>
#include "template.h"

Template::Template() {
}

// Initialize the demo
void Template::init(int16_t width, int16_t height) {
    screenWidth = width;
    screenHeight = height;
}

void Template::draw(LGFX_Sprite* sprite) {
    // TODO
}
