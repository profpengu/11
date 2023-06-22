#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define new_ptr(ptr) malloc(sizeof(*ptr))
#define arr_length(a) sizeof(a) / sizeof(a[0])
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define PLAYER_MAX_HEALTH 10.f
// https://www.raylib.com/cheatsheet/cheatsheet.html

double rad2degs(double r)
{
    return r * 180 / 3.1416;
}
struct Balloon
{
    Vector2 position;
    int health;
    float pathing_radius;
    int next_pathing_point;
};

struct Monkey
{
    Rectangle rect;
    float range;
    float reload_time;
    double rotation;
};
#define MAX_BALLOONS 10
#define MAX_MONKEYS 15

struct Balloon *balloons[MAX_BALLOONS];
struct Monkey *monkeys[MAX_MONKEYS];

Vector2 tri[] = {(Vector2){0, 400}, (Vector2){220, 400}, (Vector2){220, 200}, (Vector2){500, 200}, (Vector2){500, 450}, (Vector2){850, 450}, (Vector2){850, 330}, (Vector2){1350, 330}};

float get_distance(float x1, float y1, float x2, float y2)
{
    return sqrtf(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}

void spawn_new_monkey(Vector2 position)
{
    for (int i = 0; i < MAX_MONKEYS; i++)
    {
        if (monkeys[i] == NULL)
        {
            struct Monkey *nm = new_ptr(nm);
            nm->range = 200.f;
            nm->rect = (Rectangle){position.x, position.y, 10.f, 10.f};
            nm->reload_time = 1.f;
            nm->rotation = 0.f;
            monkeys[i] = nm;
            return;
        }
    }
}

void spawn_new_balloon()
{
    for (int i = 0; i < MAX_BALLOONS; i++)
    {
        if (balloons[i] == NULL)
        {
            struct Balloon *new_balloon = new_ptr(new_balloon);
            new_balloon->position = tri[0];
            new_balloon->health = 10;
            new_balloon->pathing_radius = 10.f;
            new_balloon->next_pathing_point = 1;
            balloons[i] = new_balloon;
            return;
        }
    }
}

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

int main()
{
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
            spawn_new_monkey(GetMousePosition());
        }
        time_until_balloon_spawns -= dt;
        if (time_until_balloon_spawns <= 0)
        {
            spawn_new_balloon();
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
                // printf("delta %f %f\n", delta_position.x, delta_position.y);
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
