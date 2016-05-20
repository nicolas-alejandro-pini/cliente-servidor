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

typedef struct {
	int *pSockfd;
	struct hostent *h;
	struct sockaddr_in *tDest_addr;
} t_client;

/* primitivas cliente */
int create_client(t_client *tClient);
int connect_client(t_client *tClient);
void disconnect_client(t_client *tClient);
int setHostByName(t_client *tClient, char *string);
int setPortDest(t_client *tClient, char *string);


#endif /* SOCKET_TCP_H_ */
