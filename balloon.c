#include <stdlib.h>
#include <stdbool.h>
#include "util.h"
#include "balloon.h"

void spawn_new_balloon(struct Balloon* balloons[], Vector2 path[])
{
  bool found_spot_for_balloon = false;
  for (int i = 0; i < MAX_BALLOONS && !found_spot_for_balloon; i++)
  {
    if (balloons[i] == NULL)
    {
      struct Balloon *new_balloon = new_ptr(new_balloon);
      new_balloon->position = path[0];
      new_balloon->health = 10;
      new_balloon->pathing_radius = 10.f;
      new_balloon->next_pathing_point = 1;
      balloons[i] = new_balloon;
      found_spot_for_balloon = true;
    }
  }
}

