/*
 * socket_tcp.c
 *
 *  Created on: 20/5/2016
 *      Author: Nicolas Alejandro Pini
 */
#include "socket_tcp.h"

int create_client(t_client *tClient){

	if(tClient)
	{
		tClient->pSockfd = (int*) malloc(sizeof(int));
		tClient->tDest_addr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
		tClient->h = (struct hostent*) malloc(sizeof(struct hostent));
		*(tClient->pSockfd) = 0;
		tClient->h->h_length = 0;
	}

	if(!(tClient->pSockfd))
		return EXIT_FAILURE;
	if(!(tClient->tDest_addr))
		return EXIT_FAILURE;
	if(!(tClient->h))
		return EXIT_FAILURE;

	// File descriptor del cliente
	if((*(tClient->pSockfd)  = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
	    perror("sockfd");
	    return EXIT_FAILURE;
	}
	tClient->tDest_addr->sin_family = AF_INET;
	tClient->tDest_addr->sin_port = htons(DEST_PORT);  //  short, Ordenacion de red
	tClient->tDest_addr->sin_addr.s_addr = 0;  // Ordenacion de red de la IP
	memset(&(tClient->tDest_addr->sin_zero), '\0', 8);
	return EXIT_SUCCESS;
}

int connect_client(t_client *tClient){

	if(!(tClient->pSockfd) || !(tClient->tDest_addr)){
		return EXIT_FAILURE;
	}
	if((connect(*(tClient->pSockfd), (struct sockaddr *) tClient->tDest_addr, sizeof(struct sockaddr))) == -1)
	{
		perror("connect");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void disconnect_client(t_client *tClient){
    //  Valores de how:
    //                  0 - No se permite recibir mas datos.
    //                  1 - No se permite enviar mas datos.
    //                  2 - No se permite enviar ni recibir mas datos (lo mismo que close)
	shutdown(*(tClient->pSockfd), 2);
}

int setHostByName(t_client *tClient, char *string){

	if (!tClient)
		return EXIT_FAILURE;

	if((tClient->h = gethostbyname(string)) == NULL) {
		herror("gethostbyname");
		return EXIT_FAILURE;
	}

	if(!(tClient->tDest_addr))
		return EXIT_FAILURE;

	tClient->tDest_addr->sin_addr.s_addr = inet_addr(tClient->h->h_addr);
	printf("Host name: %s\n", tClient->h->h_name);
	printf("IP Address: %s\n", inet_ntoa(*((struct in_addr *)tClient->h->h_addr)));
	return EXIT_SUCCESS;
}

int setPortDest(t_client *tClient, char *string){
	if (!tClient)
		return EXIT_FAILURE;

	tClient->tDest_addr->sin_port = htons(atoi(string));
	return EXIT_SUCCESS;
}
