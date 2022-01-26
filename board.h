#ifndef __BOARD__
#define __BOARD__

#include "snake.h"

struct Origin
{
    unsigned int x, y;
};

struct Board
{
    char **screen;
    struct Origin *food;
    unsigned int height, width, food_size;
};

struct Origin make_coord(const unsigned int y, const unsigned int x)
{
    struct Origin coord = {x, y};
    return coord;
}

const int32_t randint(const int32_t min, const int32_t max)
{
    return rand() % (max - min) + min;
}

const float angle(const struct Origin pair1, const struct Origin pair2)
{
    const float A = pair1.y - pair2.y;
    const float B = pair1.x - pair2.x;
    return atan(A / B) * 180 / M_PI;
}

const bool ate(const struct Origin *food, const struct Body *head)
{
    return ((food->x == head->x) && (food->y == head->y));
}

const bool body_collision(const struct Body *head)
{
    struct Body *current = head->next;

    if (current->x == head->x && current->y == head->y)
        return true;

    while (current->next != NULL)
    {
        if (current->x == head->x && current->y == head->y)
            return true;

        current = current->next;
    }

    return false;
}

const bool collide(const struct Board *board, const struct Body *head)
{
    return (body_collision(head) || (board->width - 1 < head->x) || (board->height - 1 < head->y) || (0 > head->x) || (0 > head->y));
}

void set_snake(const struct Board *board, const struct Snake *snake)
{
    unsigned int y, x;
    struct Body *current = snake->head;

    for (uint32_t j = 0; j < snake->size; j++)
    {
        y = current->y;
        x = current->x;
        board->screen[y][x] = 'O';
        current = current->next;
    }
}

void move_snake(struct Board *board, struct Snake *snake, const int8_t dx, const int8_t dy)
{
    if (!(snake->size > 1 && check_move(snake, dx, dy)))
    {
        snake->dx = dx;
        snake->dy = dy;
    }

    unsigned int px = snake->head->x, py = snake->head->y;
    snake->head->x += snake->dx;
    snake->head->y += snake->dy;
    board->screen[py][px] = '.';

    struct Body *head = snake->head;

    if (snake->size > 1)
    {
        head = head->next;
        struct Body *current;
        unsigned int nx, ny;

        for (uint32_t j = 1; j < snake->size; j++)
        {
            current = head;
            nx = px, ny = py;

            px = current->x;
            py = current->y;

            current->x = nx;
            current->y = ny;

            board->screen[py][px] = '.';

            if (j < snake->size - 1)
                head = head->next;
        }
    }
}

void regen_food(struct Board *board)
{
    int32_t nx = randint(0, board->width), ny = randint(0, board->height);
    board->screen[board->food->y][board->food->x] = '.';
    board->food->x = nx, board->food->y = ny;
    board->screen[ny][nx] = 'X';
}

struct Board *set_board(const unsigned int height, const unsigned int width)
{

    struct Board *board = (struct Board *)malloc(sizeof(struct Board));
    board->width = width;
    board->height = height;
    board->screen = (char **)calloc(height, sizeof(char *));
    board->food = (struct Origin *)malloc(sizeof(struct Origin));
    board->food->x = width / 2, board->food->y = height / 2 + 2;

    for (uint32_t row = 0; row < height; row++)
    {
        board->screen[row] = (char *)calloc(width, sizeof(char));

        for (uint32_t col = 0; col < width; col++)
        {
            board->screen[row][col] = '.';
        }
    }

    board->screen[board->food->y][board->food->x] = 'X';

    return board;
}

void free_board(struct Board *board, struct Snake *snake)
{
    for (uint32_t row = 0; row < board->height; row++)
    {
        free(board->screen[row]);
    }

    free(board->food);
    free(board->screen);
    free(board);

    if (snake->size > 0)
    {
        free_snake(snake);
    }
}

void display(const struct Board *board, struct Snake *snake)
{
    clear();
    set_snake(board, snake);

    for (uint32_t row = 0; row < board->height; row++)
    {
        for (uint32_t col = 0; col < board->width; col++)
        {
            printw("%c ", board->screen[row][col]);
        }
        printw("\n");
    }
}

#endif
