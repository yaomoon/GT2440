#include <stdio.h>
#include <ncurses.h>
#include "curses.h"

int main(int argc, const char *argv[])
{
    char name[10] = {0};
    char info[100] = {2,0,0,0,'m','m',4,0,0,0,'m','o','o','n'};

    init_curses();
    draw_bottom();

#ifdef debug
    fprintf(moon_log,"this is main file\n");
#endif

    moon_test(info);
    while(1)
    {
        tablet_str(name);
    }

    endwin();

    
    return 0;
}
