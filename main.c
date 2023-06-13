#include <raylib.h>
// https://www.raylib.com/cheatsheet/cheatsheet.html

struct Balloon {
    Vector2 position;
    int health;
};

int main()
{
   const int screenWidth = 1280;
   const int screenHeight = 720;
   InitWindow( screenWidth, screenHeight, "tower defense" );
   const Vector2 ballonSpawnPoint = { (float)screenWidth/2, (float)screenHeight/2};
   const Texture2D backgroundTexture = LoadTexture("./background.png");
   const Texture2D balloonimage = LoadTexture("./balloon.png");
   SetTargetFPS(60);
   while (!WindowShouldClose())
   {
    BeginDrawing();  
        ClearBackground(RAYWHITE);
        // DrawTexturePro(backgroundTexture, (Rectangle){0, 0, backgroundTexture.width, backgroundTexture.height}, (Rectangle){0, 0, screenWidth, screenHeight}, (Vector2){0, 0}, 0.f, WHITE);
        DrawTexture(balloonimage, 20, 20, RED);
       EndDrawing();
  
    }
    
    CloseWindow();
    return 0;
}

