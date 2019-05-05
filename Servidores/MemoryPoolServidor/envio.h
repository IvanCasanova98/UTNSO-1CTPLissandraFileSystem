/*
 * envio.h
 *
 *  Created on: 2 may. 2019
 *      Author: utnso
 */

#ifndef SERVIDORES_MEMORYPOOLSERVIDOR_ENVIO_H_
#define SERVIDORES_MEMORYPOOLSERVIDOR_ENVIO_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>

#include "paquete.h"

t_log* iniciar_logger(void);
t_config* leer_config(void);

void* serializar_paquete_select(t_paquete_select* paquete);
void* serializar_paquete_insert(t_paquete_insert* paquete);

void enviar_paquete_select(t_paquete_select* paquete, int socket_cliente);
void enviar_paquete_insert(t_paquete_insert* paquete, int socket_cliente);





#endif /* SERVIDORES_MEMORYPOOLSERVIDOR_ENVIO_H_ */