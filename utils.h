#ifndef UTILS_H
#define UTILS_H


// Struct definitions
struct vector2d {
	float x, y;
};

struct vector3d {
	float x, y, z;
};

// Constants
#define M_PI 3.14159265358979323846
#define TWO_PI M_PI * 2

// Function declarations
uint16_t darkenColor(uint16_t color, float factor);
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);

// Vector operations
void add_vector(struct vector2d* a, struct vector2d* b);
void add_vector3d(struct vector3d* a, struct vector3d* b);
void divide_vector(struct vector2d* v, float n);
void divide_vector3d(struct vector3d* v, float n);
void multiply_vector(struct vector2d* v, float n);
void rotate_vector3d(struct vector3d* v, float degrees, char axis);

#endif // UTILS_H 
