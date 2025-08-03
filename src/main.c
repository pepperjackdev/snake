#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>

typedef struct {
    int numberOfColumns;
    int numberOfRows;
} Size;

typedef GHashTable Map;

typedef Vector2 Point;

typedef enum {
    FOOD,
    SNAKE,
} EntityType;

typedef struct {
    void *data;
    EntityType type;
} Entity;

typedef struct {
    Size size;
    Map *map;
} Board;

Board* NewBoard(Size size);
Entity* GetBoardMapEntityAt(Board *board, Point point);
void SetBoardMapEntityAt(Board *board, Point point, Entity *enity);

void DrawBoard(Board *board);
void DrawGuides(Board *board);
void DrawGuidesColumns(int);
void DrawGuidesRows(int);
void DrawFood(Board *board);

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    Board *board = NewBoard((Size){10, 10});

    InitWindow(800, 800, "Snake!");
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
    Map *map = g_hash_table_new(NULL, NULL);

    *board = (Board){
        size,
        map,
    };

    // TEMP
    Entity *entity = (Entity*)malloc(sizeof(Entity));
    *entity = (Entity){
        NULL,
        SNAKE,
    };
    SetBoardMapEntityAt(board, (Point){0, 0}, entity);
    // TEMP


    return board;
}

Entity* GetBoardMapEntityAt(Board *board, Point point) {
    Entity *entity;
    g_hash_table_lookup_extended(
        board->map, 
        &point,
        NULL,
        (void**)&entity
    );
    return entity;
}

void SetBoardMapEntityAt(Board *board, Point point, Entity *entity) {
    Point *ptrPoint = (Point*)malloc(sizeof(Point));
    *ptrPoint = point;
    g_hash_table_insert(
        board->map,
        ptrPoint,
        entity
    );
}

void DrawBoard(Board *board) {
    DrawGuides(board);
    DrawFood(board);
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

void DrawFood(Board *board) {
    GHashTableIter iterator;
    Point *point;
    Entity *entity;
    g_hash_table_iter_init(&iterator, board->map);
    while (g_hash_table_iter_next(&iterator, (gpointer*)point, (gpointer*)entity)) {
        if (entity->type == FOOD) {
            DrawRectangle(0, 0, 100, 100, RAYWHITE);
        }
    }
}
