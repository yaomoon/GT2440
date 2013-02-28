#include "forks.h"
 
#define LEFT 0
#define RIGHT 1
 
struct location{
    int row;
    int col;
};
 
void screen_refresh(int fd)
{
    int row,col;
    int ch;
    struct location locate[2]={{5,20},{5,40}};
     
    getmaxyx(stdscr,row,col);
    mvaddch(locate[LEFT].row,locate[LEFT].col,'*'); //用*填充以前的字符在屏幕上的位置,等于在屏幕上删除
    mvaddch(locate[RIGHT].row,locate[RIGHT].col,'*');
    mvprintw(row-1,1,"Press F2 to exit.");
    refresh();
     
    if(read(fd,&ch,4) < 0) exit(1);
    while(ch != KEY_F(2)){
        //printw("[%d:%d]",KEY_UP,ch);
        switch(ch){
            case 'w':
            case 'W':
                mvaddch(locate[LEFT].row,locate[LEFT].col,' ');
                locate[LEFT].row--;
                mvaddch(locate[LEFT].row,locate[LEFT].col,'*');
                break;
            case 's':
            case 'S':
                mvaddch(locate[LEFT].row,locate[LEFT].col,' ');
                locate[LEFT].row++;
                mvaddch(locate[LEFT].row,locate[LEFT].col,'*');
                break;
            case 'a':
            case 'A':
                mvaddch(locate[LEFT].row,locate[LEFT].col,' ');
                locate[LEFT].col--;
                mvaddch(locate[LEFT].row,locate[LEFT].col,'*');
                break;
            case 'd':
            case 'D':
                mvaddch(locate[LEFT].row,locate[LEFT].col,' ');
                locate[LEFT].col++;
                mvaddch(locate[LEFT].row,locate[LEFT].col,'*');
                break;
                 
            case KEY_UP:
                mvaddch(locate[RIGHT].row,locate[RIGHT].col,' ');
                locate[RIGHT].row--;
                mvaddch(locate[RIGHT].row,locate[RIGHT].col,'*');
                //printw("%d",ch);
                break;
            case KEY_DOWN:
                mvaddch(locate[RIGHT].row,locate[RIGHT].col,' ');
                locate[RIGHT].row++;
                mvaddch(locate[RIGHT].row,locate[RIGHT].col,'*');
                //printw("%d",ch);
                break;
            case KEY_LEFT:
                mvaddch(locate[RIGHT].row,locate[RIGHT].col,' ');
                locate[RIGHT].col--;
                mvaddch(locate[RIGHT].row,locate[RIGHT].col,'*');
                //printw("%d",ch);
                break;
           case KEY_RIGHT:
                mvaddch(locate[RIGHT].row,locate[RIGHT].col,' ');
                locate[RIGHT].col++;
                mvaddch(locate[RIGHT].row,locate[RIGHT].col,'*');
                //printw("%d",ch);
                break;
            default:
                break; 
        }
        refresh();
        if(read(fd,&ch,4) < 0) exit(1);
    }//end while
}
