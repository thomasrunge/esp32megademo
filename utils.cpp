#include <math.h>
#include "utils.h"


uint16_t darkenColor(uint16_t color, float factor) {
    if (factor < 0) factor = 0;
    if (factor > 1) factor = 1;

    // Bytes tauschen (Little Endian zu Big Endian)
    uint16_t swapped = (color >> 8) | (color << 8);

    // Extrahiere RGB
    uint8_t r = (swapped >> 11) & 0x1F;  // 5 Bit
    uint8_t g = (swapped >> 5) & 0x3F;   // 6 Bit
    uint8_t b = swapped & 0x1F;          // 5 Bit

    // Skaliere mit Faktor
    r = (uint8_t)(r * factor);
    g = (uint8_t)(g * factor);
    b = (uint8_t)(b * factor);

    // Setze wieder zusammen
    uint16_t result = (r << 11) | (g << 5) | b;

    // Bytes wieder zurücktauschen
    return (result >> 8) | (result << 8);
}

void add_vector(struct vector2d* a, struct vector2d* b) {
	a->x += b->x;
	a->y += b->y;
}

void add_vector3d(struct vector3d* a, struct vector3d* b) {
	a->x += b->x;
	a->y += b->y;
	a->z += b->z;
}

void divide_vector(struct vector2d* v, float n) {
	v->x /= n;
	v->y /= n;
}

void divide_vector3d(struct vector3d* v, float n) {
	v->x /= n;
	v->y /= n;
	v->z /= n;
}

void multiply_vector(struct vector2d* v, float n) {
	v->x *= n;
	v->y *= n;
}

void rotate_vector3d(struct vector3d* v, float degrees, char axis) {
	//calculate radians
	float angle = degrees * M_PI / 180;
	float sine = sin(angle);
	float cosine = cos(angle);

	float x = v->x;
	float y = v->y;
	float z = v->z;

	if (axis == 'x') {
		//rotation matix around x axis
		float matrix[3][3] = {{1, 0, 0}, {0, cosine, -sine}, {0, sine, cosine}};

		v->x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z;
		v->y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z;
		v->z = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z;
	}

	if (axis == 'y') {
		//rotation matix around y axis
		float matrix[3][3] = {{cosine, 0, sine}, {0, 1, 0}, {-sine, 0, cosine}};
		
		v->x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z;
		v->y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z;
		v->z = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z;
	}

	if (axis == 'z') {
		//rotation matix around z axis
		float matrix[3][3] = {{cosine, -sine, 0}, {sine, cosine, 0}, {0, 0, 1}};

		v->x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z;
		v->y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z;
		v->z = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z;
	}
}
