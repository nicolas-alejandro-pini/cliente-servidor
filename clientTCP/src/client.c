#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define DEST_PORT 80
int main(int argc, char* argv[])
{
	int sockfd;
  struct hostent *h;
  struct sockaddr_in dest_addr;
  if(argc != 2) {
    fprintf(stderr, "usage: client [address]");
    exit(1);
  }
  if((h=gethostbyname(argv[1])) == NULL) {
    herror("gethostbyname");
    exit(1);
  }
  printf("Connecting to host name [%s] ...\n", h->h_name);
  // File descriptor del cliente
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
    perror("sockfd");
    exit(1);
  }
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(DEST_PORT);  //  short, Ordenacion de red
	dest_addr.sin_addr.s_addr = inet_addr(h->h_addr);  // Ordenacion de red de la IP
	memset(&(dest_addr.sin_zero), '\0', 8);
	if((connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr))) < 0 ) {
    perror("connect");
    exit(1);
	}
	close(sockfd);
	return 0;
}

