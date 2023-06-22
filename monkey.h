#pragma once
#include <raylib.h>
#define MAX_MONKEYS 15

struct Monkey
{
  Rectangle rect;
  float range;
  float reload_time;
  double rotation;
};

void spawn_new_monkey(struct Monkey* monkeys[], Vector2 position);
