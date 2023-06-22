#include "util.h"

double get_angle_between_points(Vector2 a, Vector2 b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return atan2(dy, dx);
}

Vector2 get_vector_towards_position(Vector2 from, Vector2 to)
{
    double angle_between_points = get_angle_between_points(from, to);
    return (Vector2){-cos(angle_between_points), -sin(angle_between_points)};
}

double rad2degs(double r)
{
    return r * 180 / 3.1416;
}

float get_distance(float x1, float y1, float x2, float y2)
{
    return sqrtf(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}
