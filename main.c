#include <raylib.h>
// https://www.raylib.com/cheatsheet/cheatsheet.html

struct Balloon {
    Vector2 position;
    int health;
};

int main()
{
   const int screenWidth = 800;
   const int screenHeight = 450;
   InitWindow( screenWidth, screenHeight, "tower defense" );
   const Vector2 ballonSpawnPoint = { (float)screenWidth/2, (float)screenHeight/2};
   const Texture2D backgroundTexture = LoadTexture("./background.png");
   SetTargetFPS(60);
   while (!WindowShouldClose())
   {
    BeginDrawing();  
        ClearBackground(RAYWHITE);
        DrawTexture(backgroundTexture, 0, 0, WHITE);
    EndDrawing();

    }
    
    CloseWindow();
    return 0;
}

