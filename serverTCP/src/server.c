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

int main(void) {

	int sockfd = 0;
	stUMCConfig2 UMCConfig;
	t_server server;
	create_server(&server, 5001, 0);
	listen_server(&server, 0);
	sockfd = accept_server(&server);

	printf("sockfd %d\n", sockfd);
	recv(sockfd, &UMCConfig, sizeof(stUMCConfig2), 0);
	disconnect_server(&server);
	return EXIT_SUCCESS;
}
