#ifndef IMAGE_ROTATE_H
#define IMAGE_ROTATE_H

#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>
#include "utils.h"


// ImageRotate class declaration
class ImageRotate {
private:
    int16_t screenWidth, screenHeight;
    int32_t x_pos, y_pos;
    int16_t dx, dy;
    float angle, zoom_x, zoom_y;
    float angle_delta, zoom_x_delta, zoom_y_delta;
    float max_angle, min_zoom;

    // Image properties
    const uint16_t *_image;
    const uint16_t _imageWidth;
    const uint16_t _imageHeight;

public:
    ImageRotate(const uint16_t *image, uint16_t imageWidth, uint16_t imageHeight, float max_angle, float min_zoom);
    
    // Initialize the image rotate system
    void init(int16_t width, int16_t height);

    // Draw the image rotate
    void draw(LGFX_Sprite* sprite);
};

#endif // IMAGE_ROTATE_H 