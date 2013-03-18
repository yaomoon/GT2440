#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

//WINDOW *scene[5];
//WINDOW *bar[2];
//WINDOW *contact;
//WINDOW *input;

//define color of windows
#define PANEL 1
#define BAR 2

//debug
#define debug
#ifdef debug
FILE *moon_log;
#endif
//
//
struct element{
    char key[50];
    char value[100];
    struct element *next;
};

struct window{
    char name[10];
    WINDOW *win;
    struct element *element;
    int size_x;
    int size_y;
    int x;
    int y;
};

struct window contact;
struct window input;
struct window scene[5];
struct window bar[3];

void Create_win(struct window *win, int size_y, int size_x, int y, int x)
{
    win->win = newwin(size_y, size_x, y, x);
    win->size_y = size_y;
    win->size_x = size_x;
    win->y = y;
    win->x = x;
}
struct element *fill_element(struct element *pre, char *key, char *value )
{
    struct element *node;

    node = (struct element *)malloc(sizeof(struct element));
    
    memset(node->key,0,sizeof(node->key));
    memset(node->value,0,sizeof(node->value));
    strcpy(node->key, key);
    strcpy(node->value, value);

    node->next = NULL;
    if(!pre)
    {
#ifdef debug
    fprintf(moon_log,"!pre\n");
#endif
        pre = node;
    }
    else
        pre->next = node;
    
    return node;
}

//fill the contact board
void fill_contact(char *name, char *value)
{
    struct element *p = contact.element;

    if(!contact.element)
        contact.element = fill_element(contact.element, name, value ); 
    else
    {
        while(p->next)
        {
            p = p->next;
        }
        fill_element(p,name,value );
    }
        
}
//fill the input board
void fill_input(char *name, char *value)
{
    struct element *p = input.element;
    if(!input.element)
        input.element = fill_element(input.element,name,value); 
    else
    {
        while(p->next)
        {
            p = p->next;
        }
        fill_element(p,name,value );
    }
        
}

void display_win(struct window *win)
{
    struct element *head = win->element;

    wclear(win->win);

    if(head)
    {
#ifdef debug
    fprintf(moon_log,"head is not NULL\n");
#endif
        while(head)
        {
            wprintw(win->win,"%s  %s\n",head->key,head->value);
            head = head->next;
        }

    }
    wrefresh(win->win); 
}

void refresh_contact(void)
{
    display_win(&contact);
}

void draw_bottom(void)
{
    int y_max,x_max;

    getmaxyx(stdscr, y_max,x_max);

    //聊天窗口
    Create_win(&scene[0],y_max/3*2,x_max/3*2,1,0);
    //scene[0].win = subwin(stdscr,y_max/3*2,x_max/3*2,1,0);

    // 框条窗口
    Create_win(&bar[0],1,x_max/3*2,0,0);
    Create_win(&bar[1],1,x_max/3*2,y_max/3*2+1,0);
    Create_win(&bar[2], 0,2,0,x_max/3*2);

    //联系人窗口 
    Create_win(&contact,0,x_max/3-1,0,x_max/3*2+1);

    //输入窗口
    Create_win(&input,y_max/3-1,x_max/3*2,y_max/3*2+2,0);

    wbkgd(scene[0].win,COLOR_PAIR(PANEL));
    wbkgd(bar[0].win,COLOR_PAIR(BAR));
    wbkgd(bar[1].win,COLOR_PAIR(BAR));
    wbkgd(bar[2].win,COLOR_PAIR(BAR));
    wbkgd(contact.win,COLOR_PAIR(PANEL));
    wbkgd(input.win,COLOR_PAIR(PANEL));

    display_win(&scene[0]);
    display_win(&bar[0]);
    display_win(&bar[1]);
    display_win(&bar[2]);
    display_win(&contact);
    display_win(&input);

}

void init_curses(void)
{
    initscr();
    start_color();
    init_pair(PANEL,COLOR_BLUE,COLOR_WHITE);
    init_pair(BAR,COLOR_WHITE,COLOR_BLUE);
    init_pair(3,COLOR_RED,COLOR_GREEN);
    init_pair(4,COLOR_GREEN,COLOR_RED);
    init_pair(5,COLOR_GREEN,COLOR_BLUE);
    nonl();
    cbreak();
    //noecho();
    leaveok(stdscr, TRUE);
    
#ifdef debug
moon_log = fopen("moon_log","w+");
#endif
}

void tablet_str(char *str)
{

    mvwaddstr(input.win,2,3,"   ");
    mvwscanw(input.win,2,3,"%s",str);
 
    //mvaddstr(2,3,"   ");
    //mvscanw(2,3,"%s",name);
    //mvprintw(2,19,"%s",name);
    fill_contact("name",str);

    //refresh();
    //display_win(input);
    display_win(&contact);
    display_win(&input);

}


/*
int main(int argc, const char *argv[])
{
    int x_max,y_max;
    
    init_curses(); 
    draw_bottom();

#ifdef debug
    fprintf(moon_log,"hello yaomoon\n");
#endif

    fill_contact("name","yaomoonhhh");
    fill_contact("name","nihao");
    fill_contact("name","shijie");
    display_win(&contact);
    fill_input("input","kk");
    fill_input("input","ss");
    fill_input("input","dd");
    display_win(&input);

    //wprintw(contact.win,"%s  %s\n","name","yaomoon");
    //wrefresh(contact.win);
    
    while(1);
    endwin();
    return 0;
}
*/
