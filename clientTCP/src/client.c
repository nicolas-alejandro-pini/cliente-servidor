#include <stdio.h>
#include <stdlib.h>
#include <socket_tcp.h>

int main(int argc, char* argv[])
{
  t_client client;

  if(create_client(&client)){
	  perror("Al crear cliente");
	  return EXIT_FAILURE;
  }

  if(argc >= 2) {
	  setHostByName(&client, argv[1]);
  }

  if(argc == 3) {
	  setPortDest(&client, argv[2]);
  }

  if(connect_client(&client)){
	  perror("Al conectar cliente");
	  return EXIT_FAILURE;
  }
  printf("Connected");

  disconnect_client(&client);
  //printf("Connecting to host name [%s] ...\n", h->h_name);
  return 0;
}







