#include <Arduino.h>
#include <string.h>
#include "vectorballs.h"
#include "image.h"

Vectorballs::Vectorballs() {}
    
// Initialize the vector balls system
void Vectorballs::init(int16_t width, int16_t height) {
    // Initialize member variables
    screenWidth = width;
    screenHeight = height;

    // Initialize the 3D vectors for different shapes
    initCube();
    initSphere();
    initPlane();
    
    // Create darkened ball variants
    create_ball_variants();
}

// Create 7 darkened versions of the ball sprite (skip the original)
void Vectorballs::create_ball_variants() {
    // Create variants with different darkness levels (skip the original)
    for (int variant = 0; variant < 7; variant++) {
        float factor = 0.875 - (variant * 0.125); // 0.875, 0.75, 0.625, 0.5, 0.375, 0.25, 0.125

        for (int i = 0; i < ball_size * ball_size; i++) {
            uint16_t original_color = img_ball[i];
            if (original_color != 0) { // Skip transparent pixels
                ball_variants[variant][i] = darkenColor(original_color, factor);
            } else {
                ball_variants[variant][i] = 0; // Keep transparent
            }
        }
    }
}

// Initialize cube vertices
void Vectorballs::initCube() {
	int i, j, k, count = 0;

	int rows = 5;
	int cols = 5;
	int slice = 4;
	
	for (i = 0; i < rows; i++) {
		//linearly interpolate between -1 and 1
		float v = ((float) i / (rows - 1) - 0.5) * 2;

		for (j = 0; j < cols; j++) {
			//linearly interpolate between -1 and 1
			float u = ((float) j / (cols - 1) - 0.5) * 2;

			for (k = 0; k < slice; k++) {
				//linearly interpolate between -1 and 1
				float w = ((float) k / (slice - 1) - 0.5) * 2;
				
				cube[count].x = u;
				cube[count].y = v;
				cube[count].z = w;
				count++;
			}
		}
	}
}

// Initialize sphere vertices
void Vectorballs::initSphere() {
	int i, j, count = 0;
	int total_lon = 10;
	int total_lat = 10;
	float r = 1;
	
	for (i = 0; i < total_lat; i++) {
		float u = (float) i / (total_lat - 1);
		float lat = u * M_PI;
		
		for (j = 0; j < total_lon; j++) {
			float v = (float) j / (total_lon - 1);
			float lon = v * TWO_PI;
			
			sphere[count].x = r * sin(lat) * cos(lon);
			sphere[count].y = r * sin(lat) * sin(lon);
			sphere[count].z = r * cos(lat);
			count++;
		}
	}
}

// Initialize plane vertices
void Vectorballs::initPlane() {
	int i, j, count = 0;
	int rows = 10;
	int cols = 10;
	
	for (i = 0; i < rows; i++) {
		//linearly interpolate between -1 and 1
		float v = ((float) i / (rows - 1) - 0.5) * 2;

		for (j = 0; j < cols; j++) {
			//linearly interpolate between -1 and 1
			float u = ((float) j / (cols - 1) - 0.5) * 2;
			
			plane[count].x = u;
			plane[count].y = v;
			plane[count].z = 0;
			count++;
		}
	}
}

void Vectorballs::selection_sort() {
	int i, j;
	int lowest = 0;	

	for(i = 0; i < ball_count; i++) {
		lowest = i;

		for(j = i; j < ball_count; j++) {
			if (v_balls[j].z >= v_balls[lowest].z) {
				lowest = j;
			}
		}
		
		struct vector3d temp = v_balls[i];
		v_balls[i] = v_balls[lowest];
		v_balls[lowest] = temp;
	}
}

// Draw the vector balls
void Vectorballs::draw(LGFX_Sprite *sprite) {
    float interp = (float) inc / 100;

    LGFX_Sprite ball_sprite(sprite);

    if (interp <= 1) {
        inc++;
    }	

    //reset interpolation increment
    if (frame % 500 == 0) {
        frame = 0;
        inc = 0;
        flip++;
        flip %= 3;
    }

    //interpolate vectors between shapes
    if (interp < 1) {
        for (int i = 0; i < ball_count; i++) {
            if (flip == 0) {
                v_balls[i].x = (cube[i].x * interp) + (sphere[i].x * (1 - interp));
                v_balls[i].y = (cube[i].y * interp) + (sphere[i].y * (1 - interp));
                v_balls[i].z = (cube[i].z * interp) + (sphere[i].z * (1 - interp));
                scale = (250 * interp) + (500 * (1 - interp));
            }

            if (flip == 1) {
                v_balls[i].x = (plane[i].x * interp) + (cube[i].x * (1 - interp));
                v_balls[i].y = (plane[i].y * interp) + (cube[i].y * (1 - interp));
                v_balls[i].z = (plane[i].z * interp) + (cube[i].z * (1 - interp));
                scale = (500 * interp) + (250 * (1 - interp));
            }

            if (flip == 2) {
                v_balls[i].x = (sphere[i].x * interp) + (plane[i].x * (1 - interp));
                v_balls[i].y = (sphere[i].y * interp) + (plane[i].y * (1 - interp));
                v_balls[i].z = (sphere[i].z * interp) + (plane[i].z * (1 - interp));
                //scale = (1000 * interp) + (500 * (1 - interp));
            }
        }
    }

    //sort vectors based on there z value
    selection_sort();

    for (int i = 0; i < ball_count; i++) {
        struct vector3d z_translate = {0, 0, 5};	
        struct vector3d world = {v_balls[i].x, v_balls[i].y, v_balls[i].z};
        struct vector2d screen = {world.x, world.y}; 
        struct vector2d screen_translate = {screenWidth / 2, screenHeight / 2};	

        // which sprite variant to use (0-7)
        int variant_index = (int)((world.z + 1.6) * 7.0 / 3.2);
        variant_index = constrain(variant_index, 0, 7); // Ensure it's in range

        //move 3d vectors away from the camera
        add_vector3d(&world, &z_translate);

        //calculate perspective for 2d screen
        divide_vector(&screen, world.z);

        //scale vectors for 2d screen
        multiply_vector(&screen, scale);

        //position vectors
        add_vector(&screen, &screen_translate);

        // Calculate ball position and size
        int32_t x_pos = screen.x - ball_size / 2;
        int32_t y_pos = screen.y - ball_size / 2;

        // Set the appropriate ball variant as buffer
        if (variant_index == 0) {
            // Use original image for the brightest variant
            ball_sprite.setBuffer((void*)img_ball, ball_size, ball_size, 16);
        } else {
            // Use darkened variants (index 0-6 in ball_variants array)
            ball_sprite.setBuffer((void*)ball_variants[variant_index - 1], ball_size, ball_size, 16);
        }

        // Draw the ball using the sprite
        ball_sprite.pushSprite(x_pos, y_pos, TFT_BLACK);

        // Rotate vectors
        rotate_vector3d(&v_balls[i], 1, 'x');
        rotate_vector3d(&v_balls[i], 1, 'y');
        rotate_vector3d(&v_balls[i], 1, 'z');
        rotate_vector3d(&plane[i], 1, 'x');
        rotate_vector3d(&plane[i], 1, 'y');
        rotate_vector3d(&plane[i], 1, 'z');
        rotate_vector3d(&cube[i], 1, 'x');
        rotate_vector3d(&cube[i], 1, 'y');
        rotate_vector3d(&cube[i], 1, 'z');
        rotate_vector3d(&sphere[i], 1, 'x');
        rotate_vector3d(&sphere[i], 1, 'y');
        rotate_vector3d(&sphere[i], 1, 'z');
    }

    frame++;
}
