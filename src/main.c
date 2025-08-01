#include <raylib.h>
#include <stdint.h>

void DrawBoardGrid(int, int);
void _DrawBoardGridColumns(int);
void _DrawBoardGridRows(int);

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    int width = 10, height = 10;
    uint8_t board[height][width];

    InitWindow(800, 800, "Snake!");
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawBoardGrid(width, height);
        EndDrawing();
    }
    CloseWindow();
}

void DrawBoardGrid(int numberOfColumns, int numberOfRows) {
    _DrawBoardGridColumns(numberOfColumns);
    _DrawBoardGridRows(numberOfRows);
}

void _DrawBoardGridColumns(int numberOfColumns) {
    int columnSpacing = GetScreenWidth() / numberOfColumns;
    for (int column = 1; column < numberOfColumns; column++) {
        DrawLine(
            column * columnSpacing, 0, 
            column * columnSpacing, 
            GetScreenHeight(),
            GRAY
        );
    }
}

void _DrawBoardGridRows(int numberOfRows) {
    int rowSpacing = GetScreenHeight() / numberOfRows;
    for (int row = 1; row < numberOfRows; row++) {
        DrawLine(
            0,
            row * rowSpacing,
            GetScreenWidth(),
            row * rowSpacing,
            GRAY
        );
    }
}
