#include <stdio.h>
#include <stdlib.h>
#include <socket_tcp.h>
#include <serializador.h>
#include "test_client.h"

int main(int argc, char* argv[])
{
  t_UMCConfig umcConfig;
  t_client client;
  t_paquete paquete;

  umcConfig.paginasXProceso = 4;
  umcConfig.tamanioPagina = 6000;

  if(strcmp(argv[1], "--cunit") == 0){
	  test_unit();
  }

  if(create_client(&client)){
	  perror("Al crear cliente");
	  return EXIT_FAILURE;
  }

  if(argc >= 2) {
	  setPortDest(&client, argv[1]);
  }

  if(argc == 3) {
	  setHostByName(&client, argv[2]);
  }

  if(connect_client(&client)){
	  perror("Al conectar cliente");
	  return EXIT_FAILURE;
  }
  printf("socket %d", *(client.pSockfd));
  //send(*(client.pSockfd), &umcConfig, sizeof(t_UMCConfig), 0);

	// Reserva memoria para header + datos

  crear_paquete(&paquete, CONFIG_UMC);
  serializar_ejemplo(&paquete, &umcConfig);
  enviar_paquete(*(client.pSockfd), &paquete);
  free_paquete(&paquete);
  printf("Connected");

  disconnect_client(&client);
  return EXIT_SUCCESS;
}







