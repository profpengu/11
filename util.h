#pragma once
#include <raylib.h>
#include <math.h>

#define new_ptr(ptr) malloc(sizeof(*ptr))
#define arr_length(a) sizeof(a) / sizeof(a[0])

double get_angle_between_points(Vector2 a, Vector2 b);
Vector2 get_vector_towards_position(Vector2 from, Vector2 to);
double rad2degs(double r);
float get_distance(float x1, float y1, float x2, float y2);

