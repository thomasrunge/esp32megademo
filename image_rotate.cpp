#include <Arduino.h>
#include <math.h>
#include "image_rotate.h"
#include "image.h"

ImageRotate::ImageRotate(const uint16_t *image, uint16_t imageWidth, uint16_t imageHeight, float max_angle, float min_zoom) : _image(image), _imageWidth(imageWidth), _imageHeight(imageHeight), max_angle(max_angle), min_zoom(min_zoom) {}

// Initialize the image rotate system
void ImageRotate::init(int16_t width, int16_t height) {
    screenWidth = width;
    screenHeight = height;
    x_pos = screenWidth / 2 - _imageWidth / 2;
    y_pos = screenHeight / 2 - _imageHeight / 2;
    dx = 1;
    dy = 10;
    angle = 0;
    zoom_x = 1;
    zoom_y = 1;
    angle_delta = 2;
    zoom_x_delta = .02;
    zoom_y_delta = .04;
}

void ImageRotate::draw(LGFX_Sprite* sprite) {
    LGFX_Sprite img_sprite(sprite);
    img_sprite.setBuffer((void*)_image, _imageWidth, _imageHeight, 16);
    img_sprite.pushRotateZoom(x_pos, y_pos, angle, zoom_x, zoom_y, TFT_BLACK);

    if (max_angle != 0) {
        angle += angle_delta;
        if (max_angle < 0) {
            if (angle >= 360) {
                angle -= 360;
            }
            if (angle <= 0) {
                angle += 360;
            }
        } else {
            if (angle > max_angle || angle < -max_angle) {
                angle_delta = -angle_delta;
            }
        }
    }

    if (min_zoom > 0) {
        zoom_x += zoom_x_delta;
        if (zoom_x > 2.) {
            zoom_x_delta = -zoom_x_delta;
        }
        if (zoom_x < min_zoom) {
            zoom_x_delta = -zoom_x_delta;
        }
        zoom_y += zoom_y_delta;
        if (zoom_y > 2.) {
            zoom_y_delta = -zoom_y_delta;
        }
        if (zoom_y < min_zoom) {
            zoom_y_delta = -zoom_y_delta;
        }
    }

    x_pos += dx;
    if (x_pos >= screenWidth) {
        dx = -random(1, 10);
    }
    if (x_pos <= 0) {
        dx = random(1, 10);
    }

    y_pos += dy;
    if (y_pos >= screenHeight) {
        dy = -random(1, 10);
    }
    if (y_pos <= 0) {
        dy = random(1, 10);
    }
    //Serial.printf("angle: %f, zoom_x: %f, zoom_y: %f\n", angle, zoom_x, zoom_y);
}
