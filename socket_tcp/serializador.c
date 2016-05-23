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

	// Obtengo payload
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
	t_stream size_header = sizeof(t_header);
	t_stream *stream = malloc(size_header);
	int tmp_size = 0, offset =0;

	tmp_size = recv(sockfd, stream, size_header, MSG_WAITALL);
	if(tmp_size == 0)  // CONNECTION CLOSED
	{
		header->type = CONNECTION_CLOSED;
		header->length = 0;
	}
	if(tmp_size == size_header)
	{
		deserializarHeader(stream, &offset, header);  // guardo header
		free(stream);
		return EXIT_SUCCESS;
	}
	perror("recv_header");
	free(stream);
	return EXIT_FAILURE;
}

int enviarPaquete(int sockfd, t_paquete *paquete){
	int16_t enviados = 0, ret = 0;
	t_header header = paquete->header;

	while(enviados < header.length)
	{
		ret = send(sockfd, paquete->data, header.length, 0);
		if(ret < 0)
			return EXIT_FAILURE;
		enviados += ret;
	}
	return EXIT_SUCCESS;
}


void serializarHeader(const t_header header, int32_t *offset, t_stream *stream){
	*offset = 0;
	memcpy(stream, &(header.type), sizeof(header.type));
	*offset = sizeof(header.type);

	memcpy(stream + (*offset), &(header.length), sizeof(header.length));
	*offset = *offset + sizeof(header.length);
}

void deserializarHeader(const t_stream *datos, int32_t *offset, t_header *header){
	int size = sizeof(t_stream);
	memcpy(&header->type, datos, size);
	memcpy(&header->length, &datos[size], size);
	*offset = sizeof(t_header);
}

int serializarConfigUMC(t_paquete *paquete, t_UMCConfig *self){
	t_header header;
	int32_t offset = 0;
	int32_t tmp_size = 0;
	int32_t long_data = sizeof(t_UMCConfig);

	paquete->data = malloc(sizeof(t_header) + long_data);
	if(!paquete->data)
		return 0;
	header.type = CONFIG_UMC;
	header.length = long_data;

	serializarHeader(header, &offset, paquete->data);
	memcpy(paquete->data + offset, &(self->paginasXProceso), tmp_size = sizeof(self->paginasXProceso));
	offset += tmp_size;
	memcpy(paquete->data + offset, &(self->tamanioPagina), tmp_size = sizeof(self->tamanioPagina));
	offset += tmp_size;

	memcpy(&(paquete->header), &header, sizeof(t_header));
	return offset;
}

int deserializarConfigUMC(t_UMCConfig *self, t_paquete *paquete){
	int32_t offset = 0;
	int32_t tmp_size = 0;
	deserializarHeader(paquete->data, &offset, &(paquete->header));
	memcpy(&(self->paginasXProceso), &(paquete->data)[offset], tmp_size = sizeof(self->paginasXProceso));
	offset += tmp_size;
	memcpy(&(self->tamanioPagina), &(paquete->data)[offset], tmp_size = sizeof(self->tamanioPagina));
	offset += tmp_size;
	return offset;
}



