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

const bool collide(const struct Board *board, const struct Body *head)
{
    return ((board->width - 1 < head->x) || (board->height - 1 < head->y) || (0 > head->x) || (0 > head->y));
}

void set_snake(const struct Board *board, const struct Snake *snake)
{
    unsigned int y, x;
    struct Body *current = snake->head;

    for (unsigned int j = 0; j < snake->size; j++)
    {
        y = current->y;
        x = current->x;
        board->screen[y][x] = 'O';
        current = current->next;
    }
}

void grow(struct Board *board, struct Snake *head)
{
}

void move_snake(struct Board *board, struct Snake *snake, const int8_t dx, const int8_t dy)
{
    unsigned int px = snake->head->x, py = snake->head->y;
    snake->head->x += dx;
    snake->head->y += dy;

    struct Body *head = snake->head;
    if (snake->size > 1)
    {
        head = head->next;

        struct Body *current;
        while (head->next != NULL)
        {
            board->screen[py][px] = '.';
            current = head;
            current->x = px;
            current->y = py;
            head = head->next;
            px = head->x;
            py = head->y;
        }
    }
    else
    {
        board->screen[py][px] = '.';
    }
}

struct Board *set_board(const unsigned int height, const unsigned int width)
{

    struct Board *board = (struct Board *)malloc(sizeof(struct Board));
    board->width = width;
    board->height = height;
    board->screen = (char **)calloc(height, sizeof(char *));
    board->food = (struct Origin *)malloc(sizeof(struct Origin));

    for (unsigned int row = 0; row < height; row++)
    {
        board->screen[row] = (char *)calloc(width, sizeof(char));

        for (unsigned int col = 0; col < width; col++)
        {
            board->screen[row][col] = '.';
        }
    }

    return board;
}

void free_board(struct Board *board, struct Snake *snake)
{
    for (unsigned int row = 0; row < board->height; row++)
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

    for (unsigned int row = 0; row < board->height; row++)
    {
        for (unsigned int col = 0; col < board->width; col++)
        {
            printw("%c ", board->screen[row][col]);
        }
        printw("\n");
    }
}

#endif