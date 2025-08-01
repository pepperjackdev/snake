#include <raylib.h>

int main() {
    SetTargetFPS(60);

    InitWindow(600, 600, "This is Raylib!");
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
        EndDrawing();
    }
    CloseWindow();
}
