#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>

typedef struct {
    int numberOfColumns;
    int numberOfRows;
} Size;

typedef struct {
    Size size;
} Board;

Board* NewBoard(Size size);

void DrawBoard(Board *board);
void DrawGuides(Board *board);
void DrawGuidesColumns(int);
void DrawGuidesRows(int);

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    Board *board = NewBoard((Size){10, 10});

    InitWindow(900, 900, "Snake!");
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawBoard(board);
        EndDrawing();
    }
    CloseWindow();
}

Board* NewBoard(Size size) {
    Board *board = (Board*)malloc(sizeof(Board));
    *board = (Board){size};
    return board;
}

void DrawBoard(Board *board) {
    DrawGuides(board);
}

void DrawGuides(Board *board) {
    DrawGuidesColumns(board->size.numberOfColumns);
    DrawGuidesRows(board->size.numberOfRows);
}

void DrawGuidesColumns(int numberOfColumns) {
    int columnSpacing = GetScreenWidth() / numberOfColumns;
    for (int column = 1; column < numberOfColumns; column++) {
        DrawLine(
            column * columnSpacing, 
            0,
            column * columnSpacing, 
            GetScreenHeight(),
            GRAY
        );
    }
}

void DrawGuidesRows(int numberOfRows) {
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
