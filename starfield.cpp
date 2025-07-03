#include <Arduino.h>
#include <math.h>
#include "starfield.h"
#include "image.h"
#include "utils.h"

Starfield::Starfield() {}

// Initialize the starfield system
void Starfield::init(int16_t width, int16_t height) {
    screenWidth = width;
    screenHeight = height;
    
    for (uint16_t i = 0; i < NUM_STARS; i++) {
		stars[i].location.x = (float) (random(-100, 101)) / 100;
		stars[i].location.y = (float) (random(-100, 101)) / 100;
		stars[i].location.z = (float) (random(50, 151)) / 100;
		stars[i].location.x *= screenWidth / 2;
		stars[i].location.y *= screenHeight / 2;
		stars[i].location.z *= 2;
	}
}

void Starfield::draw_stars(LGFX_Sprite* sprite) {
	for (uint16_t i = 0; i < NUM_STARS; i++) {
		//translate to screen space;
		struct vector3d translate = {screenWidth / 2, screenHeight / 2, 0};
		struct vector3d screen = stars[i].location;

		divide_vector3d(&screen, screen.z);		//calculate prespective projection
		add_vector3d(&screen, &translate); 		//convert world space to screen space

		if (stars[i].location.z > 1.5) {
			stars[i].colour = TFT_DARKGREY;
		} else if (stars[i].location.z > 1 && stars[i].location.z < 1.5) {
			stars[i].colour = TFT_LIGHTGREY;
		} else {
			stars[i].colour = TFT_WHITE;
		}

		sprite->drawPixel(screen.x, screen.y, stars[i].colour);
	}
}

void Starfield::update_stars() {
	for (uint16_t i = 0; i < NUM_STARS; i++) {
        stars[i].location.z -= .03;

		//star is behind camera, reset star
		if (stars[i].location.z < 0 ) {
			stars[i].location.x = (float) (random(-100, 101)) / 100;
			stars[i].location.y = (float) (random(-100, 101)) / 100;
			stars[i].location.z = (float) (random(1, 101)) / 100;
			stars[i].location.x *= screenWidth / 2;
			stars[i].location.y *= screenHeight / 2;
			stars[i].location.z *= 3;
		}
	}
}

void Starfield::draw(LGFX_Sprite* sprite) {
    draw_stars(sprite);
    update_stars();
}

