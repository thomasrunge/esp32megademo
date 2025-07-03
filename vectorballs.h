#ifndef VECTORBALLS_H
#define VECTORBALLS_H

#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>
#include "utils.h"


// Vectorballs class declaration
class Vectorballs {
private:
    static const uint16_t ball_size = 25;
    static const uint16_t ball_count = 100;

    struct vector3d v_balls[ball_count]; //array of vectors used to display final shape
    struct vector3d plane[ball_count];
    struct vector3d cube[ball_count];
    struct vector3d sphere[ball_count];

    int16_t screenWidth;
    int16_t screenHeight;

    unsigned int inc = 0;	//used for interpolation if vector balls
    unsigned int frame = 1;	//used to count frames of animation
    unsigned int flip = 0;  //used to change vector balls between shapes
    int scale;              //used to change scale of vector ball shapes

    // Ball sprite variants (7 different darkness levels) - static arrays on stack
    uint16_t ball_variants[7][ball_size * ball_size];

    void initCube();
    void initSphere();
    void initPlane();
    void selection_sort();
    void create_ball_variants();
    
public:
    Vectorballs();
    
    // Initialize the vector balls system
    void init(int16_t width, int16_t height);
    
    // Draw the vector balls
    void draw(LGFX_Sprite *sprite);
};

#endif // VECTORBALLS_H
