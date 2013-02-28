#ifndef FORKS_H
#define FORKS_H
#include <ncurses.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
 
void screen_refresh(int fd);
 
#endif
