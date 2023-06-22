#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "balloon.h"
#include "monkey.h"
#include "util.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define PLAYER_MAX_HEALTH 10.f
// https://www.raylib.com/cheatsheet/cheatsheet.html

int main()
{
  Vector2 tri[] = {(Vector2){0, 400}, (Vector2){220, 400}, (Vector2){220, 200}, (Vector2){500, 200}, (Vector2){500, 450}, (Vector2){850, 450}, (Vector2){850, 330}, (Vector2){1350, 330}};
  struct Monkey *monkeys[MAX_MONKEYS];
  struct Balloon *balloons[MAX_BALLOONS];

  for (int i = 0; i < MAX_BALLOONS; i++)
  {
    balloons[i] = NULL;
  }
  for (int i = 0; i < MAX_MONKEYS; i++)
  {
    monkeys[i] = NULL;
  }
  const int screenWidth = 1280;
  const int screenHeight = 720;
  const int balloon_speed = 400;
  int player_health = 10;
  InitWindow(screenWidth, screenHeight, "tower defense");
  const Texture2D backgroundTexture = LoadTexture("./Background.png");
  const Texture2D bruhTex = LoadTexture("./balloon.png");
  const Texture2D towerTex = LoadTexture("./tower.png");
  // SetTargetFPS(1);
  float time_until_balloon_spawns = 0.1;
  while (!WindowShouldClose() && player_health > 0)
  {
    float dt = MIN(GetFrameTime(), 10.f / 1000.f);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
      spawn_new_monkey(monkeys, GetMousePosition());
    }
    time_until_balloon_spawns -= dt;
    if (time_until_balloon_spawns <= 0)
    {
      spawn_new_balloon(balloons, tri);
      time_until_balloon_spawns = 0.1;
    }
    for (int i = 0; i < MAX_BALLOONS; i++)
    {
      if (balloons[i] != NULL)
      {
        Vector2 delta_position = get_vector_towards_position(balloons[i]->position, tri[balloons[i]->next_pathing_point]);
        delta_position.x *= balloon_speed;
        delta_position.y *= balloon_speed;
        balloons[i]->position.x += delta_position.x * dt;
        balloons[i]->position.y += delta_position.y * dt;
        if (CheckCollisionPointCircle(tri[balloons[i]->next_pathing_point], balloons[i]->position, balloons[i]->pathing_radius))
        {
          if (balloons[i]->next_pathing_point + 1 < arr_length(tri)) {
            balloons[i]->next_pathing_point++;
          } else {
            player_health = player_health - 1;
            free(balloons[i]);
            balloons[i] = NULL;
          }
        }
      }
    }
    for (int i = 0; i < MAX_MONKEYS; i++)
    {
      if (monkeys[i] != NULL)
      {
        monkeys[i]->reload_time -= dt;
        for (int bi = 0; bi < MAX_BALLOONS; bi++)
        {
          if (balloons[bi] != NULL)
          {
            Vector2 monkey_pos = (Vector2){monkeys[i]->rect.x, monkeys[i]->rect.y};
            if (get_distance(monkey_pos.x, monkey_pos.y, balloons[bi]->position.x, balloons[bi]->position.y) <= monkeys[i]->range && monkeys[i]->reload_time <= 0)
            {
              balloons[bi]->health -= 1000000;
              monkeys[i]->reload_time = 1.f;
              Vector2 monkey_center = (Vector2){monkeys[i]->rect.x + towerTex.width / 2, monkeys[i]->rect.y + towerTex.height / 2};
              Vector2 balloon_center = (Vector2){balloons[bi]->position.x, balloons[bi]->position.y};
              monkeys[i]->rotation = rad2degs(get_angle_between_points(monkey_center, balloon_center) + 180);
              // printf("Bang!\n");
            }
          }
        }
      }
    }
    for (int i = 0; i < MAX_BALLOONS; i++)
    {
      if (balloons[i] != NULL && balloons[i]->health <= 0)
      {
        free(balloons[i]);
        balloons[i] = NULL;
      }
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexturePro(backgroundTexture, (Rectangle){0, 0, backgroundTexture.width, backgroundTexture.height}, (Rectangle){0, 0, screenWidth, screenHeight}, (Vector2){0, 0}, 0.f, WHITE);
    for (int i = 0; i < MAX_BALLOONS; i++)
    {
      if (balloons[i] != NULL)
      {
        DrawTexturePro(bruhTex, (Rectangle){0, 0, bruhTex.width, bruhTex.height}, (Rectangle){balloons[i]->position.x, balloons[i]->position.y, 60, 60}, (Vector2){30, 30}, 0.f, WHITE);
        // printf("balloon #%d %f %f\n", i, balloons[i]->position.x, balloons[i]->position.y);
      }
    }
    for (int m = 0; m < MAX_MONKEYS; m++)
    {
      if (monkeys[m] != NULL) 
      {
        DrawTexturePro(towerTex, (Rectangle){.x = 0, .y = 0, .width = towerTex.width, .height = towerTex.height}, (Rectangle){.x = monkeys[m]->rect.x, .y = monkeys[m]->rect.y, .width = towerTex.width, .height = towerTex.height}, (Vector2){towerTex.width / 2, towerTex.height / 2}, monkeys[m]->rotation, WHITE);
      }
    }
    DrawRectangle(0, 0, screenWidth * (player_health / PLAYER_MAX_HEALTH), 50, RED);
    EndDrawing();
  }
  UnloadTexture(bruhTex);
  UnloadTexture(backgroundTexture);
  CloseWindow();

  for (int i = 0; i < MAX_BALLOONS; i++)
  {
    if (balloons[i] != NULL)
      free(balloons[i]);
  }
  for (int i = 0; i < MAX_MONKEYS; i++)
  {
    if (monkeys[i] != NULL)
      free(monkeys[i]);
  }

  return 0;
}
