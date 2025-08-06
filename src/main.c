#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define UNFINISHED 0
#define FINISHED 1
#define Bool int

typedef int State;

typedef struct {
    int width;
    int height;
} Size;

typedef struct {
    int row;
    int col;
} Point;

typedef int Entity;
typedef Entity** Map;

typedef enum {
    UP,
    RIGHT,
    LEFT,
    DOWN,
} Direction;

typedef struct {
    int length;
    Point point;
    Direction direction;
} Snake;

typedef struct {
    State state;
    Size size;
    Map map;
    Snake *snake;
} Board;

Point MovePointToDirection(Point point, Direction direction);
Bool IsPointIntoSizeBounds(Point point, Size size);

Bool IsSnake(Entity entity);
Bool IsFood(Entity entity);
Bool IsNothing(Entity entity);

Entity GetEntityAtPointFromMap(Point point, Map map);
void SetEntityAtPointFromMap(Entity entity, Point point, Map map);

Board* NewBoard(Size size);
void FreeBoard(Board *board);
void UpdateBoard(Board *board);

Map NewMap(Size size);
void FreeMap(Map map, Size size);

Snake* NewSnake();
void FreeSnake(Snake *snake);
void UpdateSnakeDirection(Snake *snake);

Size GetGuideSpacing(Size size);

void DrawBoard(Board *board);
void DrawEntities(Board *board);
void DrawGuides(Board *board);
void DrawGuidesColumns(int);
void DrawGuidesRows(int);

int main() {
    srand(time(NULL));
    SetTargetFPS(60);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    Board *board = NewBoard((Size){10, 10});

    InitWindow(900, 900, "Snake!");
    double start = GetTime();

    while (!WindowShouldClose()) {
        UpdateSnakeDirection(board->snake);
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

Point MovePointToDirection(Point point, Direction direction) {
    Point new;
    switch (direction) {
        case UP:
            new = (Point){point.row - 1, point.col};
            break;
        case DOWN:
            new = (Point){point.row + 1, point.col};
            break;
        case LEFT:
            new = (Point){point.row, point.col - 1};
            break;
        case RIGHT:
            new = (Point){point.row, point.col + 1};
            break;      
    }
    return new;
}

Bool IsPointIntoSizeBounds(Point point, Size size) {
    return (point.row >= 0 && point.row < size.height) &&
        (point.col >= 0 && point.col < size.width);
}

Bool IsSnake(Entity entity) {
    return entity > 0;
}

Bool IsFood(Entity entity) {
    return entity < 0;
}

Bool IsNothing(Entity entity) {
    return entity == 0;
}

Entity GetEntityAtPointFromMap(Point point, Map map) {
    return map[point.row][point.col];
}

void SetEntityAtPointFromMap(Entity entity, Point point, Map map) {
    map[point.row][point.col] = entity;
}

Board* NewBoard(Size size) {
    Board *board = (Board*)malloc(sizeof(Board));
    Map map = NewMap(size);
    Snake *snake = NewSnake();

    *board = (Board){
        UNFINISHED,
        size,
        map,
        snake
    };

    return board;
}

void FreeBoard(Board *board) {
    FreeMap(board->map, board->size);
    FreeSnake(board->snake);
    free(board);
}

void UpdateBoard(Board *board) {
    if (board->state == FINISHED) return;
    // Updating map

    Point newHeadPoint = MovePointToDirection(board->snake->point, board->snake->direction);
    if (!IsPointIntoSizeBounds(newHeadPoint, board->size) || 
        IsSnake(GetEntityAtPointFromMap(newHeadPoint, board->map))) {
            board->state = FINISHED;
            return;
    } else {
        board->snake->point = newHeadPoint;
        if (IsFood(GetEntityAtPointFromMap(newHeadPoint, board->map))) {
            board->snake->length++;
        }
        SetEntityAtPointFromMap(board->snake->length + 1, board->snake->point, board->map);
    }

    for (int row = 0; row < board->size.height; row++) {
        for (int col = 0; col < board->size.width; col++) {
            Entity entity = board->map[row][col];
            if (IsSnake(entity)) {
                board->map[row][col] = entity - 1;
            } else if (IsNothing(entity)) {
                if (rand() % 1000 == 1) {
                    board->map[row][col] = -1;
                }
            }
        }
    }
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

Snake* NewSnake() {
    Snake *snake = (Snake*)malloc(sizeof(Snake));
    *snake = (Snake){
        3,              // Default snake length
        (Point){0, 0},
        RIGHT
    };
    return snake;
}

void FreeSnake(Snake *snake) {
    free(snake);
}

void UpdateSnakeDirection(Snake *snake) {
    switch (snake->direction) {
        case UP:
        case DOWN:
            if (IsKeyPressed(KEY_A)) snake->direction = LEFT;
            if (IsKeyPressed(KEY_D)) snake->direction = RIGHT;
            break;
        case RIGHT:
        case LEFT:
            if (IsKeyPressed(KEY_W)) snake->direction = UP;
            if (IsKeyPressed(KEY_S)) snake->direction = DOWN;
            break;
    }
}

Size GetGuideSpacing(Size size) {
    return (Size){
        GetScreenWidth() / size.width,
        GetScreenHeight() / size.height
    };
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

            // Debug info!
            // DrawText(
            //     TextFormat("%d", entity), 
            //     column * spacing.width, 
            //     row * spacing.height, 
            //     20, 
            //     BLACK
            // );
        }
    }
}

void DrawGuides(Board *board) {
    DrawGuidesColumns(board->size.width);
    DrawGuidesRows(board->size.height);
}

void DrawGuidesColumns(int width) {
    int columnSpacing = GetScreenWidth() / width;
    for (int column = 1; column < width; column++) {
        DrawLine(
            column * columnSpacing, 
            0,
            column * columnSpacing, 
            GetScreenHeight(),
            BLACK
        );
    }
}

void DrawGuidesRows(int height) {
    int rowSpacing = GetScreenHeight() / height;
    for (int row = 1; row < height; row++) {
        DrawLine(
            0,
            row * rowSpacing,
            GetScreenWidth(),
            row * rowSpacing,
            BLACK
        );
    }
}
