/*
 * serializador.h
 *
 *  Created on: 22/5/2016
 *      Author: Nicolas Pini
 */

#ifndef SERIALIZADOR_H_
#define SERIALIZADOR_H_

#define CONNECTION_CLOSED 0
#define BROKENPIPE 1
#define CONFIG_UMC 2


typedef uint16_t t_buffer;

typedef struct{
	uint16_t type;
	uint32_t length;
} __attribute__ ((__packed__)) t_header;

typedef struct{
	t_header header;
	t_buffer *data;
}  t_paquete;

typedef struct{
	int paginasXProceso;
	int tamanioPagina;
//} __attribute__((packed)) t_UMCConfig;
} t_UMCConfig;

/** Primitivas del cliente **/
void crear_paquete(t_paquete *paquete, int type);
int32_t* serializar_campo(t_paquete *paquete, int32_t *offset, void *campo, int32_t size);
void serializar_header(t_paquete *paquete);
int enviar_paquete(int sockfd, t_paquete *paquete);

/** Primitivas del servidor **/
int recibir_paquete(int sockfd, t_paquete *paquete);
int obtener_paquete_type(t_paquete *paquete);
int32_t deserializar_campo(t_paquete *paquete, int32_t *offset, void *campo, int32_t size);

/** Primitivas comunes **/
void free_paquete(t_paquete *paquete);

/** Estructuras especificas **/
int serializarConfigUMC(t_paquete *paquete, t_UMCConfig *self);
int deserializarConfigUMC(t_UMCConfig *self, t_paquete *paquete);

#endif /* SERIALIZADOR_H_ */
