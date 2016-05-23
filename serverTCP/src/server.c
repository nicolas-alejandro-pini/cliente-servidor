/*
 ============================================================================
 Name        : ServerTCP.c
 Author      : Nicolas Pini
 Version     :
 Copyright   : http://www.gnu.org/licenses/
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <socket_tcp.h>
#include <serializador.h>

int main(void) {

	t_UMCConfig UMCConfig;
	t_paquete paquete;
	int sockfd = 0;

	t_server server;
	create_server(&server, 5001, 0);
	listen_server(&server, 0);
	sockfd = accept_server(&server);

	printf("sockfd %d\n", sockfd);
	//recv(sockfd, &UMCConfig, sizeof(t_UMCConfig), 0);

	recibirPaquete(sockfd, &paquete);
	deserializarConfigUMC(&UMCConfig, &paquete);
	disconnect_server(&server);
	return EXIT_SUCCESS;
}
