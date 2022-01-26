#include "board.h"

int main(int argc, char **argv)
{
    assert(argc == 3);
    unsigned int height, width;

    height = atoi(argv[1]);
    width = atoi(argv[2]);

    /*
        W = 119 S = 115
        A = 97 D = 100
    */

    struct Board *board = set_board(height, width);
    struct Snake *snake = (struct Snake *)malloc(sizeof(struct Snake));

    snake->size = 0;
    snake->dx = 0, snake->dy = 0;

    add_body(snake, height / 2, width / 2);
    add_body(snake, height / 2, width / 2 + 1);

    int ch;
    bool stop;

    int8_t dx = 0, dy = 1;

    initscr();
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);

    display(board, snake);
    while (true)
    {
        ch = getch();

        if (ch == 119)
        {
            printw("UP\n");
            dx = 0;
            dy = -1;
        }
        else if (ch == 115)
        {
            printw("DOWN\n");
            dx = 0;
            dy = 1;
        }
        else if (ch == 100)
        {
            printw("RIGHT\n");
            dx = 1;
            dy = 0;
        }
        else if (ch == 97)
        {
            printw("LEFT\n");
            dx = -1;
            dy = 0;
        }

        move_snake(board, snake, dx, dy);

        usleep(200000);
        if (collide(board, snake->head))
        {
            stop = true;
            break;
        }

        if (ate(board->food, snake->head))
        {
            add_body(snake, snake->tail->y, snake->tail->x);
            regen_food(board);
        }

        display(board, snake);
    }

    endwin();

    // printf("%f\n", angle(make_coord(snake->head->y, snake->head->x), make_coord(snake->tail->y, snake->tail->x)));
    // printf("%s\n", collide(board, snake->tail) ? "True" : "False");

    if (stop)
    {
        printf("Score: %u\n", snake->size - 3);
    }

    fflush(stdout);
    free_board(board, snake);
}
