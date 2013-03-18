all:curses.o main.o
	cc -o main curses.o main.o -lncurses
curses.o:curses.c
	cc -c -g curses.c -lncurses

main.o:main.c
	cc -c -g main.c 
