#include <stdlib.h>
#include <stdbool.h>
#include "monkey.h"
#include "util.h"

void spawn_new_monkey(struct Monkey* monkeys[], Vector2 position)
{
  bool found_spot_for_monkey = false;
  for (int i = 0; i < MAX_MONKEYS && !found_spot_for_monkey; i++)
  {
    if (monkeys[i] == NULL)
    {
      struct Monkey *nm = new_ptr(nm);
      nm->range = 200.f;
      nm->rect = (Rectangle){position.x, position.y, 10.f, 10.f};
      nm->reload_time = 1.f;
      nm->rotation = 0.f;
      monkeys[i] = nm;
      found_spot_for_monkey = true;
    }
  }
}
