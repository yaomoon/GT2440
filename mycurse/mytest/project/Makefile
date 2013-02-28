all:curses.o socket.o
	cc -o main curses.o socket.o -lncurses
curses.o:curses.c
	cc -c curses.c -lncurses
socket.o:socket.c
	cc -c socket.c
