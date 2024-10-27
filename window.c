#include "raylib.h"

const int screen_width = 800;
const int screen_height = 800;

Camera2D camera = { 0 };
Vector2 origin = { screen_width / 2.0f, screen_height / 2.0f };

bool is_moving = false;
Vector2 mouse_drag = { 0 };

void DrawFirstCoordinateSystem() {
    DrawLine(-5000, 50, +5000, 50, RED);
    DrawLine(30, -5000, 30, +5000, RED);
}

void DrawSecondCoordinateSystem() {
    DrawLine(-5000, 0, +5000, 0, BLUE);
    DrawLine(0, -5000, 0, +5000, BLUE);
}

int main(void) {

    InitWindow(screen_width, screen_height, "relativização");

    camera.target = (Vector2){ 0.0f, 0.0f };
    camera.offset = (Vector2){ screen_width / 2.0f, screen_height / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
        {
            is_moving = true;
            mouse_drag = GetMousePosition();
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) 
        {
            is_moving = false;
        }

        if (is_moving) {
            Vector2 mousePosition = GetMousePosition();
            Vector2 dragDelta = { mouse_drag.x - mousePosition.x, mouse_drag.y - mousePosition.y };
            
            camera.target.x += dragDelta.x;
            camera.target.y += dragDelta.y;
            
            mouse_drag = mousePosition;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode2D(camera); 

            DrawFirstCoordinateSystem();

            DrawSecondCoordinateSystem();

            EndMode2D(); 

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
