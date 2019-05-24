/*
 * APIS.h
 *
 *  Created on: 8 may. 2019
 *      Author: utnso
 */

#ifndef APIS_H_
#define APIS_H_




#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include <stdbool.h>
#include<curses.h>
#include<sys/socket.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<commons/log.h>
#include<commons/error.h>
#include<commons/string.h>
#include<commons/config.h>
#include<commons/temporal.h>
#include<commons/bitarray.h>
#include<commons/collections/dictionary.h>
#include <commons/collections/node.h>
#include <commons/txt.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include"recibir.h"
#include"memTable.h"
#include "bloque.h"
#include "Lissandra.h"
#include "bitmap.h"
#include"logs.h"
t_config config;



int existeTabla(char* TablaBuscada);

//**********- FUNCIONES API -**********
void  APIdrop(t_paquete_drop*);
char* APIselect(t_paquete_select*);
void  APIinsert(t_paquete_insert*);
void  APIcreate(t_paquete_create*);
//*************************************

char* DirectorioDeTemporal(char* nombretabla);
int existe_temporal(char* path);
char* DirectorioDeBloque(int numeroBloque);
void crearParticiones(char*nombreTabla ,int nroParticiones);
char* elegirMayorTimeStamp(t_list* RegistrosEncontrados);
t_registro* crearRegistro (char*value,uint16_t key,long long timestamp);
int particionDeKey(int key,int particiones);
t_metadata* obtenerMetadataTabla (char* nombreTabla );
char* DirectorioDeTabla(char* tabla);
t_metadata* crearMetadata(consistency consistencia, int particiones,int tiempo_compactacion);
int pasarAConsistenciaINT(char* consistencia);
char* pasarAConsistenciaChar(int consistencia);
void logLaTablaYaExiste();
void crearTabla(char* tabla);
char* pasarMayuscula(char*);
char* DirectorioDeMetadataTabla(char* tabla);
void crearMetadataConfig(char*nombreTablaMayuscula, consistency consistencia, int particiones,int tiempo_compactacion);
char* DirectorioDeMetadata();
char* DirectorioBitMap();
void crearParticiones(char*nombreTabla ,int particiones);
char* DirectorioDeParticion(char* ,int numeroParticion);
void crearParticion(char*nombreTabla ,int particiones);
void removerBloque(char* nroBloque);
void RemoverTemporalesDeTablaYSusBloques(char* nombretabla);
void RemoverMetadataDeTabla(char *nombreTabla);
void RemoverCarpetaVaciaDeTabla(char *nombreTabla);
void RemoverDeLaMemtable(char *nombreTabla);
void RemoverParticionesDeTablaYSusBloques(char* nombreTabla);
#endif /* APIS_H_ */
