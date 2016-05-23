/*
 * serializador.c
 *
 *  Created on: 22/5/2016
 *      Author: Nicolas Pini
 */
#include <stdio.h>
#include <stdlib.h>
#include "socket_tcp.h"
#include "serializador.h"

int recibirPaquete(int sockfd, t_paquete *paquete){
	t_header header;
	t_stream *data;
	int size = 0;

	if(recibirHeader(sockfd, &header))
		return EXIT_FAILURE;

	data = malloc(sizeof(header.length));
	size = recv(sockfd, data, header.length, MSG_WAITALL);

	if(size == 0)  // CONNECTION CLOSED
	{
		header.type = CONNECTION_CLOSED;
		header.length = 0;
	}
	memcpy(&(paquete->header), &header, sizeof(t_header));

	if(size == header.length)
		return EXIT_SUCCESS;

	perror("recv_payload en recibirPaquete");
	free(data);
	return EXIT_FAILURE;
}

int recibirHeader(int sockfd, t_header *header){
	int size_header = sizeof(t_header);
	int tmp_size = 0, offset =0;
	t_header *buf_header = malloc(size_header);


	tmp_size = recv(sockfd, buf_header, size_header, MSG_WAITALL);
	if(tmp_size == 0)  // CONNECTION CLOSED
	{
		header->type = CONNECTION_CLOSED;
		header->length = 0;
	}
	if(tmp_size == size_header)
	{
		deserializarHeader(buf_header, &offset, header);  // guardo header
		free(buf_header);
		return EXIT_SUCCESS;
	}
	perror("recv_header");
	free(buf_header);
	return EXIT_FAILURE;
}

int enviarPaquete(int sockfd, t_paquete *paquete){
	int enviados = 0, ret = 0;
	t_header header;

	memcpy(&header, &(paquete->header), sizeof(t_header));
	while(enviados < header.length)
	{
		ret = send(sockfd, paquete->data, header.length, 0);
		if(ret < 0)
			return EXIT_FAILURE;
		enviados += ret;
	}
	return EXIT_SUCCESS;
}


int32_t serializarHeader(const t_header header, t_stream *stream){
	t_header header_net;
	int32_t offset_ = 0, tmp_size = 0;
	header_net.type = htons(header.type);
	header_net.length = htonl(header.length);
	offset_ = 0;
	memcpy(stream, &(header_net.type), tmp_size = sizeof(header_net.type));
	offset_ += (tmp_size / sizeof(t_stream));
	memcpy(stream + offset_, &(header_net.length), tmp_size = sizeof(header_net.length));
	offset_ += (tmp_size / sizeof(t_stream));
	return offset_;
}

void deserializarHeader(const t_header *buf_header, int32_t *offset, t_header *header){
	t_header header_host;
	header_host.type = ntohs(buf_header->type);
	header_host.length = ntohl(buf_header->length);
	memcpy(&header->type, &(header_host.type) , sizeof(header_host.type));
	memcpy(&header->length, &(header_host.length), sizeof(header_host.length));
}

int serializarConfigUMC(t_paquete *paquete, t_UMCConfig *self){
	t_header header;
	int32_t offset = 0;
	int32_t tmp_size = 0;
	int32_t long_data = sizeof(t_UMCConfig);

	paquete->data = (t_stream*) malloc(sizeof(t_header) + long_data);
	if(!paquete->data)
		return 0;
	header.type = CONFIG_UMC;
	header.length = long_data;

	offset = serializarHeader(header, paquete->data);
	memcpy(paquete->data + offset, &(self->paginasXProceso), tmp_size = sizeof(self->paginasXProceso));
	offset += tmp_size;
	memcpy(paquete->data + offset, &(self->tamanioPagina), tmp_size = sizeof(self->tamanioPagina));
	offset += tmp_size;

	memcpy(&(paquete->header), &header, sizeof(t_header));
	return offset;
}

int deserializarConfigUMC(t_UMCConfig *self, t_paquete *paquete){
	int offset = 0;
	int tmp_size = 0;
	memcpy(&(self->paginasXProceso), &(paquete->data)[offset], tmp_size = sizeof(self->paginasXProceso));
	offset += tmp_size;
	memcpy(&(self->tamanioPagina), &(paquete->data)[offset], tmp_size = sizeof(self->tamanioPagina));
	offset += tmp_size;
	return offset;
}



