#ifndef __SNAKE__
#define __SNAKE__

#define _USE_MATH_DEFINES

#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include <ncurses.h>
#include <sys/stat.h>

struct Body
{
    unsigned int x, y;
    struct Body *next, *prev;
};

struct Snake
{
    int8_t dx, dy;
    unsigned int size;
    struct Body *head, *tail;
};

struct Body *make_body(const unsigned int y, const unsigned int x)
{
    struct Body *body = (struct Body *)malloc(sizeof(struct Body));
    body->y = y;
    body->x = x;
    body->next = NULL;
    body->prev = NULL;
    return body;
}

void add_body(struct Snake *snake, const unsigned int y, const unsigned int x)
{
    struct Body *new_body = make_body(y, x);

    if (snake->size == 0)
    {
        snake->head = new_body;
    }
    else if (snake->size == 1)
    {
        snake->tail = new_body;
        snake->head->next = snake->tail;
        snake->tail->prev = snake->head;
    }
    else
    {
        snake->tail->next = new_body;
        new_body->prev = snake->tail;
        snake->tail = new_body;
    }

    snake->size++;
}

void display_snake(const struct Snake *snake)
{
    struct Body *current = snake->head;

    for (uint32_t j = 0; j < snake->size - 1; j++)
    {
        printw("{%u, %u}, ", current->x, current->y);
        current = current->next;
    }
    printw("{%u, %u}\n", current->x, current->y);
    refresh();
}

const bool check_move(struct Snake *snake, const int8_t dx, const int8_t dy)
{
    if (dx > 0 && snake->dx < 0)
    {
        return true;
    }
    else if (dx < 0 && snake->dx > 0)
    {
        return true;
    }
    else if (dy < 0 && snake->dy > 0)
    {
        return true;
    }
    else if (dy > 0 && snake->dy < 0)
    {
        return true;
    }
    return false;
}

void free_snake(struct Snake *snake)
{
    struct Body *current;

    while (snake->head != NULL)
    {
        current = snake->head;
        snake->head = snake->head->next;
        free(current);
    }

    free(snake);
}

#endif
