#include "forks.h"
 
void llfork(void);
void leftfork(void);
void rightfork(void);
void screenfork(void);
 
// just as the name: initial. more in lib ncurses's HOWTO
void init(void)
{
    initscr();
    cbreak();// ¿¿¿¿¿¿¿¿¿¿
    keypad(stdscr,TRUE);
    noecho();
}
 
int fd[3][2];
 
int main(int argc,char** argv)
{
    int pid_ll;
    int pid_left,pid_right;
    int pid4;
     
    pipe(fd[0]);
    pipe(fd[1]);
    pipe(fd[2]);
     
    init();
    if((pid_ll=fork())==0)
        llfork();   
    else if((pid_left=fork())==0)
        leftfork();
    else if((pid_right=fork())==0)
        rightfork();    
    else if((pid4=fork())==0)
        screenfork();
 
    //close pipe descriptor,wait for children process exit
    for(int i=0;i<3;i++)
        for(int j=0;j<2;j++)
            close(fd[i][j]);
    int status; 
    waitpid(pid4, &status,0);
    waitpid(pid_ll, &status,0);
    waitpid(pid_left, &status,0);
    waitpid(pid_right,  &status,0);
     
    endwin();
    return 0;
      
}
 
//get the keyboard press action
void llfork(void)//µ×²ã¼üÅÌ´¦Àí
{
    for(int i=0;i<3;i++)
        for(int j=0;j<2;j++){
            if(i==1 && j==1) continue;
            if(i==2 && j==1) continue;
            close(fd[i][j]);
        }
         
    int ch;
    while(ch=getch()){
        if(ch==KEY_F(2)){//end condition
            write(fd[1][1],&ch,4);
            write(fd[2][1],&ch,4);
             
            close(fd[1][1]);
            close(fd[2][1]);
            exit(0);
        }
         
        if(isalpha(ch))
            write(fd[1][1],&ch,4);
        else
            write(fd[2][1],&ch,4);
    }
    exit(1);
}
void leftfork(void)//×ó±ß ¡°WSAD¡±
{
    for(int i=0;i<3;i++)
        for(int j=0;j<2;j++){
            if(i==1 && j==0) continue;
            if(i==0 && j==1) continue;
            close(fd[i][j]);
        }
    while(1){
        int ch;
        read(fd[1][0],&ch,4);
        if(ch==KEY_F(2)){//end condition
            close(fd[1][0]);
            close(fd[0][1]);
            exit(0);
        }
        write(fd[0][1],&ch,4);
    }
}
void rightfork(void)//ÓÒ±ß·½Ïò¼ü
{
    for(int i=0;i<3;i++)
        for(int j=0;j<2;j++){
            if(i==2 && j==0) continue;
            if(i==0 && j==1) continue;
            close(fd[i][j]);
        }
    while(1){
        int ch;
        read(fd[2][0],&ch,4);
        write(fd[0][1],&ch,4);
         
        if(ch==KEY_F(2)){
            close(fd[2][0]);
            close(fd[0][1]);
            exit(0);
        }
    }
}
 
void screenfork(void)//ÆÁÄ»Ë¢ÐÂ
{
    for(int i=0;i<3;i++)
        for(int j=0;j<2;j++){
            if(i==0 && j==0) continue;
            close(fd[i][j]);
        }
    screen_refresh(fd[0][0]);
    close(fd[0][0]);
    exit(0);
}
