#include <raylib.h>
#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define FINISHED TRUE
#define UNFINISHED FALSE

typedef struct {
    uint8_t width;
    uint8_t height;
} Size;

typedef struct {
    int row;
    int col;
} Point;

typedef int8_t Entity;
typedef Entity** Map;

typedef enum {
    UP,
    RIGHT,
    LEFT,
    DOWN,
} Direction;

typedef gboolean State;

typedef struct {
    Size size;
    Map map;
    Direction direction;
    State status;
} Board;

typedef enum {
    MOVE,
    EAT,
    HIT,
} Event;

Board* NewBoard(Size size);
Map NewMap(Size size);
void FreeMap(Map map, Size size);

void UpdateBoard(Board *board);
Event UpdateSnakeHead(Size size, Map old, Map new, Direction direction);

Point GetSnakeHeadPoint(Size size, Map map);


void DrawBoard(Board *board);
void DrawEntities(Board *board);
void DrawGuides(Board *board);
void DrawGuidesColumns(uint8_t);
void DrawGuidesRows(uint8_t);

Size GetGuideSpacing(Size size);
Point MovePoint(Point point, Direction direction);
bool IsPointValid(Size size, Point point);

int main() {
    srand(time(NULL));
    SetTargetFPS(60);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    Board *board = NewBoard((Size){10, 10});

    InitWindow(900, 900, "Snake!");
    double start = GetTime();

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_D) && board->direction != LEFT) {
            board->direction = RIGHT;
        } else if (IsKeyPressed(KEY_S) && board->direction != UP) {
            board->direction = DOWN;
        } else if (IsKeyPressed(KEY_A) && board->direction != RIGHT) {
            board->direction = LEFT;
        } else if (IsKeyPressed(KEY_W) && board->direction != DOWN) {
            board->direction = UP;
        }

        if (GetTime() - start > 0.1) {
            start = GetTime();
            UpdateBoard(board);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawBoard(board);
        EndDrawing();
    }
    CloseWindow();
}

Board* NewBoard(Size size) {
    Board *board = (Board*)malloc(sizeof(Board));
    Map map = NewMap(size);

    map[0][0] = 3; // Adding the snake!

    *board = (Board){
        size,
        map,
        RIGHT,
        UNFINISHED,
    };

    return board;
}

Map NewMap(Size size) {
    Entity **entity = (Entity**)malloc(sizeof(Entity*) * size.height);
    for (int row = 0; row < size.height; row++) {
        entity[row] = (Entity*)calloc(size.width, sizeof(Entity));
    }
    return (Map)entity;
}

void FreeMap(Map map, Size size) {
    for (int row = 0; row < size.height; row++) {
        free(map[row]);
    }
    free(map);
}

void UpdateBoard(Board *board) {
    if (board->status == FINISHED) return;

    Map new = NewMap(board->size);
    Event event = UpdateSnakeHead(board->size, board->map, new, board->direction);

    if (event == HIT) {
        board->status = FINISHED;
        return;
    }

    for (int row = 0; row < board->size.height; row++) {
        for (int column = 0; column < board->size.width; column++) {
            if (new[row][column] != 0) continue; // Do not override head...
            Entity entity = board->map[row][column];
            if (entity > 0) {
                if (event == MOVE | event == EAT) {
                    new[row][column] = entity - 1;
                }
            } else if (entity < 0){
                new[row][column] = entity;
            } else {
                if (rand() % 1000 == 0) {
                    new[row][column] = -1;
                }
            }
        }
    }

    // Replacing the board's map with the new map
    FreeMap(board->map, board->size);
    board->map = new;
}

Event UpdateSnakeHead(Size size, Map old, Map new, Direction direction) {
    Point oldHeadPoint = GetSnakeHeadPoint(size, old);
    Point newHeadPoint = MovePoint(oldHeadPoint, direction);
    if ((!IsPointValid(size, newHeadPoint)) || old[newHeadPoint.row][newHeadPoint.col] > 0) {
        return HIT;
    } else {
        if (old[newHeadPoint.row][newHeadPoint.col] < 0) {
            new[newHeadPoint.row][newHeadPoint.col] = old[oldHeadPoint.row][oldHeadPoint.col] + 1;
            return EAT;
        } else {
            new[newHeadPoint.row][newHeadPoint.col] = old[oldHeadPoint.row][oldHeadPoint.col];
            return MOVE;
        }
    }
}

Point GetSnakeHeadPoint(Size size, Map map) {
    Point max = (Point){0, 0};
    for (int row = 0; row < size.height; row++) {
        for (int col = 0; col < size.width; col++) {
            if (map[row][col] > map[max.row][max.col]) {
                max = (Point){row, col};
            }
        }
    }
    return max;
}

Point MovePoint(Point point, Direction direction) {
    Point new;
    switch (direction) {
        case UP:     { new = (Point){point.row - 1, point.col}; break; }
        case RIGHT:  { new = (Point){point.row, point.col + 1}; break; }
        case DOWN:   { new = (Point){point.row + 1, point.col}; break; }
        case LEFT:   { new = (Point){point.row, point.col - 1}; break; }
    }
    return new;
}

bool IsPointValid(Size size, Point point) {
    return  point.row >= 0 &&
            point.row < size.height &&
            point.col >= 0 &&
            point.col < size.width;
}

void DrawBoard(Board *board) {
    DrawEntities(board);
    DrawGuides(board);
}

void DrawEntities(Board *board) {
    Size spacing = GetGuideSpacing(board->size); // Will be used later
    for (int row = 0; row < board->size.height; row++) {
        for (int column = 0; column < board->size.width; column++) {
            Entity entity = board->map[row][column];

            Color color;
            if (entity > 0) {
                // The entity is a snake part
                color = GREEN;
            } else if (entity < 0) {
                // The entity is food
                color = RED;
            } else {
                color = WHITE;
            }

            DrawRectangle(
                column * spacing.width,
                row * spacing.height,
                spacing.width,
                spacing.height,
                color
            );

            // // Debug info!
            DrawText(
                TextFormat("%d", entity), 
                column * spacing.width, 
                row * spacing.height, 
                20, 
                BLACK
            );
        }
    }
}

void DrawGuides(Board *board) {
    DrawGuidesColumns(board->size.width);
    DrawGuidesRows(board->size.height);
}

void DrawGuidesColumns(uint8_t width) {
    uint8_t columnSpacing = GetScreenWidth() / width;
    for (uint8_t column = 1; column < width; column++) {
        DrawLine(
            column * columnSpacing, 
            0,
            column * columnSpacing, 
            GetScreenHeight(),
            GRAY
        );
    }
}

void DrawGuidesRows(uint8_t height) {
    uint8_t rowSpacing = GetScreenHeight() / height;
    for (uint8_t row = 1; row < height; row++) {
        DrawLine(
            0,
            row * rowSpacing,
            GetScreenWidth(),
            row * rowSpacing,
            GRAY
        );
    }
}

Size GetGuideSpacing(Size size) {
    return (Size){
        GetScreenWidth() / size.width,
        GetScreenHeight() / size.height
    };
}