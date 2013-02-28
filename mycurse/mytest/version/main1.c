#include <stdio.h>
#include <ncurses.h>

WINDOW *scene[5];
WINDOW *bar[2];
WINDOW *state;
WINDOW *error;
struct window{
    char name[10];
    WINDOW *win;
    int size_x;
    int size_y;
    int x;
    int y;
};
void display_win(struct window *win)
{
    wrefresh(win->win); 
}
void init_curses(void)
{
    initscr();
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    init_pair(2,COLOR_BLUE,COLOR_WHITE);
    init_pair(3,COLOR_RED,COLOR_GREEN);
    init_pair(4,COLOR_GREEN,COLOR_RED);
    init_pair(5,COLOR_GREEN,COLOR_BLUE);
    nonl();
    cbreak();
    noecho();
    leaveok(stdscr, TRUE);
}
int main(int argc, const char *argv[])
{
    int x_max,y_max;
    


    init_curses(); 
    getmaxyx(stdscr, y_max,x_max);
    //printf("y_max=%d\n",y_max);
    //printf("y_max=%d\n",x_max);
    // 场景窗口
    scene[0] = subwin(stdscr,1,x_max/3*2,0,0);
    scene[1] = subwin(stdscr,y_max/3*2,x_max/3*2,1,0);
    // 框条窗口
    bar[0] = subwin(stdscr,1,0,y_max/3*2+1,0);
    bar[1] = subwin(stdscr,y_max/3*2+1,2,0,x_max/3*2);
    //bar[0] = subwin(stdscr,1,x_max/3*2,0,0);

    //状态窗口 
    state = newwin(y_max/3*2+1,x_max/3-1,0,x_max/3*2+1);
    error = newwin(y_max/3-1,0,y_max/3*2+2,0);

    //state = newwin(0,x_max-80,0,80);
    //error = newwin(y_max-30,0,30,0);
    wbkgd(scene[0],COLOR_PAIR(1));
    wbkgd(scene[1],COLOR_PAIR(2));
    wbkgd(bar[0],COLOR_PAIR(5));
    wbkgd(bar[1],COLOR_PAIR(5));
    wbkgd(state,COLOR_PAIR(3));
    wbkgd(error,COLOR_PAIR(4));
    //wbkgd(state,COLOR_PAIR(3));
    //wbkgd(error,COLOR_PAIR(4));
    refresh();
    wrefresh(state);
    wrefresh(error);
    

    while(1);
    endwin();
    return 0;
}
