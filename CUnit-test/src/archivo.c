/*
 * archivo.c
 *
 *  Created on: 15/6/2016
 *      Author: utnso
 */
#include "archivo.h"

int archivo_obtenerTamanio(char* path) {
	struct stat stat_file;
	stat(path, &stat_file);
	return stat_file.st_size;
}

char* archivo_leer(char* path) {
	FILE* archivo = fopen(path, "r");
	if (archivo == NULL) return NULL;
	int tamanio = archivo_obtenerTamanio(path);

	char* texto = malloc(tamanio + 1);
	fread(texto, tamanio, sizeof(char), archivo);
	fclose(archivo);
	texto[tamanio] = '\0';

	return texto;
}

int archivo_contar(char* path, char c) {
	char* contenido = archivo_leer(path);
	if (contenido == NULL) return -1;

	int cantidad = 0, i;
	for(i = 0; i < strlen(contenido); i++)
		if (contenido[i] == c) cantidad++;

	return cantidad;
}


