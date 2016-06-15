/*
 * archivo.h
 *
 *  Created on: 15/6/2016
 *      Author: utnso
 */
#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
int archivo_contar(char* path, char c);
char* archivo_leer(char* path);
int archivo_obtenerTamanio(char* path) ;

#endif

