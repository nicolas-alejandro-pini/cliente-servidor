/*
 * serializador.h
 *
 *  Created on: 22/5/2016
 *      Author: Nicolas Pini
 */

#ifndef SERIALIZADOR_H_
#define SERIALIZADOR_H_

#define CONNECTION_CLOSED 0
#define CONFIG_UMC 1



typedef int16_t t_stream;

typedef struct{
	t_stream type;
	t_stream length;
} __attribute__ ((__packed__)) t_header;

typedef struct{
	t_header header;
	t_stream *data;
}  t_paquete;

typedef struct{
	int paginasXProceso;
	int tamanioPagina;
//} __attribute__((packed)) t_UMCConfig;
} t_UMCConfig;

/** Primitivas de la lib **/
void serializarHeader(const t_header header, int32_t *offset, t_stream *stream);
void deserializarHeader(const t_stream *datos, int32_t *offset, t_header *header);
int enviarPaquete(int sockfd, t_paquete *paquete);
int recibirPaquete(int sockfd, t_paquete *paquete);
int recibirHeader(int sockfd, t_header *header);

/** Estructuras especificas **/
int serializarConfigUMC(t_paquete *paquete, t_UMCConfig *self);
int deserializarConfigUMC(t_UMCConfig *self, t_paquete *paquete);

#endif /* SERIALIZADOR_H_ */
