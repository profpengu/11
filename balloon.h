#pragma once
#include <raylib.h>
#define MAX_BALLOONS 10

struct Balloon
{
  Vector2 position;
  int health;
  float pathing_radius;
  int next_pathing_point;
};

void spawn_new_balloon(struct Balloon* balloons[], Vector2 path[]);
