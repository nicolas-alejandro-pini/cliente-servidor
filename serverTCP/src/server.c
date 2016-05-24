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
	int reintento = 1;
	int sockfd = 0;

	t_server server;
	create_server(&server, 5001, 0);
	listen_server(&server, 0);
	sockfd = accept_server(&server);

	//recv(sockfd, &UMCConfig, sizeof(t_UMCConfig), 0);

	while(reintento)
	{
		int type = 0;
		recibir_paquete(sockfd, &paquete);
		type = obtener_paquete_type(&paquete);

		switch(type){
		case CONFIG_UMC:
						deserializar_ejemplo(&UMCConfig, &paquete);
						reintento = 0;
						break;
		case CONNECTION_CLOSED:

						reintento = 1;
						break;
		}
	}

	free_paquete(&paquete);
	disconnect_server(&server);
	return EXIT_SUCCESS;
}
