/*
 * socket_tcp.c
 *
 *  Created on: 20/5/2016
 *      Author: Nicolas Alejandro Pini
 */

#ifndef SOCKET_TCP_H_
#define SOCKET_TCP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEST_PORT 8080
#define DEFAULT_BACKLOG 10

typedef struct{
	int paginasXProceso;
	int tamanioPagina;
} __attribute__((packed))stUMCConfig2;

typedef struct {
	int *pSockfd;
	struct hostent *h;
	struct sockaddr_in *tDest_addr;
} t_client;

typedef struct {
	int *pSockfdListener;
	struct sockaddr_in *tOrig_addr;
	fd_set master;
	fd_set readfds;
	fd_set writefds;
	fd_set exceptfds;
	int numfds;
}t_server;



/* primitivas cliente */
int create_client(t_client *tClient);
int connect_client(t_client *tClient);
void disconnect_client(t_client *tClient);
int setHostByName(t_client *tClient, char *string);
int setPortDest(t_client *tClient, char *string);

/* primitivas servidor */
int create_server(t_server *tServer, int port, void* addr);
int listen_server(t_server *tServer, int backlog);
int accept_server(t_server *tServer); // devuelve socket
void disconnect_server(t_server *tServer);





#endif /* SOCKET_TCP_H_ */
