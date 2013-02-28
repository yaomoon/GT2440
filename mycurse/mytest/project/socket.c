
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "curses.h"

#define SERVER_PORT		8100
#define BUFFER_SIZE		256

char buffer[BUFFER_SIZE];

struct my_socket{
    int socket;
    int server_port;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server;
};

void init_socket(struct my_socket *my_socket)
{
    my_socket->server_port = SERVER_PORT; 

	bzero(&my_socket->server, sizeof(my_socket->server));
	my_socket->server.sin_family = AF_INET;
	my_socket->server.sin_port = htons(SERVER_PORT);
	my_socket->server.sin_addr.s_addr = htons(INADDR_ANY);
}

int get_udpsocket(struct my_socket *my_socket)
{
    int server_sock;

	struct sockaddr_in server;

	if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{
        //printf("create socket ok!\n");
	}

/*
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = htons(INADDR_ANY);
*/

	if(bind(server_sock, (struct sockaddr *)&my_socket->server, sizeof(my_socket->server)) < 0)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{
        //printf("bind addr ok!\n");
        //printf("\n");
	}

    my_socket->socket = server_sock;

    return server_sock;
}

int main(int argc, char *argv[])
{
	char c;
	int i,len;
	int server_sock;
	socklen_t client_len;
	struct sockaddr_in server,client;
	struct in_addr in;
	
    struct my_socket my_socket;

    init_curses();
    draw_bottom();

    init_socket(&my_socket);
    server_sock = get_udpsocket(&my_socket);

	// communication with client
	while(1)
	{
		// wait for client request
		client_len = sizeof(client);
		len = recvfrom(server_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client, &client_len);
		if (len < 0)
		{
			close(server_sock);
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		else
		{
            //printf("recvfrom client ok!\n");
			in.s_addr = client.sin_addr.s_addr;
            //printf("client ip  : %s\n", inet_ntoa(in));
            //printf("client port: %d\n", ntohs(client.sin_port));
            //printf("\n");
		}

		// Quit flag
		if(buffer[0] == '.') break;

		// lower to upper	
        /*
		for(i=0; i<len; i++)
		{
			c = buffer[i];
			buffer[i] = toupper(c);
		}
	
        */
		// send back to client
        //printf("%s\n",buffer);
        fill_state("newmessage",buffer);
        refresh_state();
		sendto(server_sock, buffer, len, 0, (struct sockaddr *)&client, client_len);

        memset(buffer,0,sizeof(buffer));
	}

    //printf("Client close the socket\n");
	close(server_sock);

	exit(EXIT_SUCCESS);
}

