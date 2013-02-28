#include <stdio.h>
#include <ncurses.h>


int main(int argc, const char *argv[])
{
    WINDOW *w1, *w2;
    initscr();
    w1=newwin(10,40,8,36);
    box(w1,'|','-');
    w2=newwin(8,14,5,5);
    box(w2,'|','-');
    waddstr(w1,"bulls");
    wnoutrefresh(w1);
    waddstr(w2,"eye");
    wnoutrefresh(w2);
    doupdate();
    while(1);
    endwin();

    
    return 0;
}
/*
main()

{
    WINDOW *w1，  *w2 ；

        initscr()；

        w1=newwin(10， 40 ， 8 ， 36) ；

        box(w1， ' ┃ '， ' ━ ')；

        w2=newwin(8， 14 ， 5 ， 5) ；

        box(w2， ' ┃ '， ' ━ ')；  waddstr(w1 ， ”bulls”) ；

        wnoutrefresh(w1)；

        waddstr(w2， ”eye”) ；

        wnoutrefresh(w2)；

        doupdate()；

        endwin()；

} 
*/
