#include <stdio.h>
#include <ncurses.h>
#include "curses.h"

int main(int argc, const char *argv[])
{
    char name[10] = {0};

    init_curses();
    draw_bottom();

    while(1)
    {
        tablet_str(name);
    }

    endwin();

    
    return 0;
}
