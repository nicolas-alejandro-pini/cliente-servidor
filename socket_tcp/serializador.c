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

void crear_paquete(t_paquete *paquete, int type){
	t_header header;
	header.type = type;
	header.length = 0;

	// Copio Header
	memcpy(&(paquete->header), &header, sizeof(t_header));
	// Reservo espacio para header
	paquete->data= malloc(sizeof(t_header));
}

void free_paquete(t_paquete *paquete){
	free(paquete->data);
}

int obtener_paquete_type(t_paquete *paquete){
	return paquete->header.type;
}

int recibir_paquete(int sockfd, t_paquete *paquete){
	uint32_t size = 0;

	// recibo header
	if(recibir_header(sockfd, &(paquete->header)))
		return EXIT_FAILURE;

	// obtengo payload
	paquete->data = malloc(paquete->header.length);
	size = recv(sockfd, paquete->data, paquete->header.length, MSG_WAITALL);

	if(size == 0)  // CONNECTION CLOSED
	{
		paquete->header.type = CONNECTION_CLOSED;
		paquete->header.length = 0;
	}
	if(size == paquete->header.length)
		return EXIT_SUCCESS;

	paquete->header.type = BROKENPIPE;
	paquete->header.length = 0;
	perror("recv_payload en recibirPaquete");
	free(paquete->data);
	return EXIT_FAILURE;
}

int recibir_header(int sockfd, t_header *header){
	int32_t size_header = sizeof(t_header);
	int32_t tmp_size = 0, offset =0;
	t_header *buf_header = malloc(sizeof(t_header));

	tmp_size = recv(sockfd, buf_header, size_header, MSG_WAITALL);
	if(tmp_size == 0)  // CONNECTION CLOSED
	{
		header->type = CONNECTION_CLOSED;
		header->length = 0;
	}
	if(tmp_size == size_header)
	{
		deserializar_header(buf_header, &offset, header);  // guardo header
		free(buf_header);
		return EXIT_SUCCESS;
	}
	perror("recv_header");
	free(buf_header);
	return EXIT_FAILURE;
}

int enviar_paquete(int sockfd, t_paquete *paquete){
	int32_t enviados = 0, ret = 0, size_paquete = 0;

	size_paquete = sizeof(t_header) + paquete->header.length;
	while(enviados < size_paquete)
	{
		ret = send(sockfd, paquete->data, size_paquete , 0);
		if(ret < 0)
			return EXIT_FAILURE;
		enviados += ret;
	}
	return EXIT_SUCCESS;
}

void serializar_header(t_paquete *paquete){
	t_header header_net;
	int32_t offset_ = 0, tmp_size = 0;

	// Ordenacion de bytes de red
	header_net.type = paquete->header.type;   // htons(paquete->header.type); // para uint16_t
	header_net.length = htons(paquete->header.length);  //htonl(paquete->header.length); // para uint32_t

	// Copio header en buffer
	offset_ = 0;
	memcpy(paquete->data, &(header_net.type), tmp_size = sizeof(header_net.type));
	offset_ += (tmp_size / sizeof(t_buffer));
	memcpy(paquete->data + offset_, &(header_net.length), sizeof(header_net.length));
}

void deserializar_header(t_header *buf_header, int32_t *offset, t_header *header){
	t_header header_host;
	header_host.type = buf_header->type; //ntohs(buf_header->type); // para uint16_t
	header_host.length = ntohs(buf_header->length); //ntohl(buf_header->length); // para uint32_t
	memcpy(&(header->type), &(header_host.type) , sizeof(header_host.type));
	memcpy(&(header->length), &(header_host.length), sizeof(header_host.length));
}

int32_t* serializar_campo(t_paquete *paquete, int32_t *offset, void *campo, int32_t size){
	int32_t size_header = sizeof(t_header) / sizeof(t_buffer);
	uint16_t buffer_net, buffer_host;
	uint32_t buffer_net32, buffer_host32;

	// reservo memoria para el campo
	printf("tamaño memoria [%d]", sizeof(t_header) +(*offset)*sizeof(t_buffer) + size);
	paquete->data = realloc(paquete->data, sizeof(t_header) +(*offset)*sizeof(t_buffer) + size);

	// Actualizo longitud en el header
	paquete->header.length += size;
	printf("header length [%d]", paquete->header.length);

	switch(size){
		case sizeof(uint16_t):
			memcpy(&buffer_host, campo, size);
			buffer_net = htons(buffer_host);
			memcpy(paquete->data + size_header + (*offset), &buffer_net, size);
			*offset += size / sizeof(t_buffer);
			break;
		case sizeof(uint32_t):
			memcpy(&buffer_host32, campo, size);
			buffer_net32 = htonl(buffer_host32);
			memcpy(paquete->data + size_header + (*offset), &buffer_net32, size);
			*offset += size / sizeof(t_buffer);
			break;
		default:
			// si es una estructura debe llevar el __attribute__((packed))
			memcpy(paquete->data + size_header + (*offset), campo, size);
			*offset += size / sizeof(t_buffer);
			break;
	}
	return offset;
}

int32_t deserializar_campo(t_paquete *paquete, int32_t *offset, void *campo, int32_t size){
	uint16_t buffer_net, buffer_host;
	uint32_t buffer_net32, buffer_host32;

	switch(size){
		case sizeof(uint16_t):
			memcpy(&buffer_net, paquete->data + (*offset), size);
			buffer_host = ntohs(buffer_net);
			memcpy(campo, &buffer_host, size);
			*offset += size / sizeof(t_buffer);
			break;
		case sizeof(uint32_t):
			memcpy(&buffer_net32, paquete->data + (*offset), size);
			buffer_host32 = ntohl(buffer_net32);
			memcpy(campo, &buffer_host32, size);
			*offset += size / sizeof(t_buffer);
			break;
		default:
			// si es una estructura debe llevar el __attribute__((packed))
			memcpy(campo, paquete->data + (*offset), size);
			*offset += size / sizeof(t_buffer);
	}
	// offset restante por deserializar
	return ((paquete->header.length/sizeof(t_buffer)) - (*offset)*sizeof(t_buffer));
}


int32_t deserializar_lista(t_paquete *paquete, int32_t *offset, t_list *lista, int32_t size_struct){
	int32_t i,bytes_left, max_list;
	void *elem = malloc(size_struct);

	// Calculo bytes restantes del buffer. LA LISTA DEBE IR AL FINAL DEL PAQUETE.
	// para saber cuantos nodos tengo en la lista.
	bytes_left = ((paquete->header.length/sizeof(t_buffer)) - (*offset)*sizeof(t_buffer));
	max_list = bytes_left / size_struct;

	for(i=0; i < max_list; i++)
	{
		deserializar_campo(paquete, offset, elem, size_struct);
		list_add(lista, elem);
	}

	// este valor debe dar 0 bytes left
	return ((paquete->header.length/sizeof(t_buffer)) - (*offset)*sizeof(t_buffer));
}

int32_t* serializar_lista(t_paquete *paquete, int32_t *offset, t_list *lista, int32_t size_struct)
{
	int32_t i,max_list;
	void *elem = NULL;

	max_list = list_size(lista);
	for(i=0; i < max_list; i++)
	{
		elem = list_get(lista,i);
		serializar_campo(paquete, offset, elem, size_struct);
	}
	return offset;
}



/************* funciones especificas ********************/

int serializar_ejemplo(t_paquete *paquete, t_UMCConfig *self){
	uint32_t numero32 = 777;
	uint16_t numero16 = 243;
	t_list *lista = list_create();

	// lista cargada con 10 veces la estructura t_UMCConfig
	int i;
	for(i=0; i < 10; i++)
		list_add(lista, self);


	// Iniciar en 0
	int32_t offset = 0;

	// Serializo int de 8 bytes y 4 bytes
	serializar_campo(paquete, &offset, &numero32, sizeof(numero32));
	serializar_campo(paquete, &offset, &numero16, sizeof(numero16));
	serializar_campo(paquete, &offset, &(self->paginasXProceso), sizeof(self->paginasXProceso));
	serializar_campo(paquete, &offset, &(self->tamanioPagina), sizeof(self->tamanioPagina));

	// Serializo la estructura t_UMCConfig , debe tener el __attribute__((packed))
	serializar_campo(paquete, &offset, self, sizeof(t_UMCConfig));

	// Serializo lista
	serializar_lista(paquete, &offset, lista, sizeof(t_UMCConfig));

	// Serializar el header despues de agregar todos los campos (porque actualiza header.length)
	serializar_header(paquete);

	list_destroy(lista);

	return offset;
}

int deserializar_ejemplo(t_UMCConfig *self, t_paquete *paquete){
	int offset = 0;
	t_UMCConfig self2;
	uint32_t numero32;
	uint16_t numero16;
	t_list *lista = list_create();

	deserializar_campo(paquete, &offset, &numero32, sizeof(numero32));
	deserializar_campo(paquete, &offset, &numero16, sizeof(numero16));
	deserializar_campo(paquete, &offset, &(self->paginasXProceso), sizeof(self->paginasXProceso));
	deserializar_campo(paquete, &offset, &(self->tamanioPagina), sizeof(self->tamanioPagina));
	deserializar_campo(paquete, &offset, &self2, sizeof(t_UMCConfig));

	deserializar_lista(paquete, &offset, lista, sizeof(t_UMCConfig));

	// chequeo que esten todos los nodos
	int32_t cant = list_size(lista);
	printf("cant %d", cant);
	self = list_get(lista,7);
	list_destroy(lista);

	return EXIT_SUCCESS;
}



