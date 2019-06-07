/*
 * CREO ESTE .c para generalizar todos los logs aca
 *
 *
 *
 */
#include "logs.h"

void LogearInsert(long long timestamp,uint16_t key,char* value,char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_info(logger,"INSERTANDO %lli;%d;%s en %s\n",timestamp,key,value,nombreTabla);
	log_destroy(logger);
}

void LogearDrop(char* nombreTabla)
{
	t_log* logger=iniciar_logger();
	log_info(logger,"BORRANDO %s\n",nombreTabla);
	log_destroy(logger);
}

void LogearCreate(char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_info(logger,"CREANDO TABLA %s\n",nombreTabla);
	log_destroy(logger);
}
void LogearSelect(long long timestamp,uint16_t key,char* value,char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_info(logger,"BUSCANDO  %lli;%d;%s en %s\n",timestamp,key,value,nombreTabla);
	log_destroy(logger);
}

void valueExiste(char*nombreTabla,uint16_t key,char* value){

	t_log* logger=iniciar_logger();
	log_info(logger,"EL VALOR DE LA KEY %d en la TABLA %s ES %s",key,nombreTabla,value);
	log_destroy(logger);
}


// WARNINGS DE ERRORES

void LaTablaNoExisteSelect(char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR AL BUSCAR KEY LA TABLA %s NO EXISTE",nombreTabla );
	log_destroy(logger);
}


void LaTablaNoExiste(long long timestamp,uint16_t key,char* value,char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR AL INSERTAR %lli;%d;%s en %s LA TABLA NO EXISTE",timestamp,key,value,nombreTabla );
	log_destroy(logger);
}

void LaTablaYaExiste(char*nombreTabla){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR AL CREAR %s LA TABLA YA EXISTE",nombreTabla);
	log_destroy(logger);
}

void valueNoExiste(char* nombreTabla,int key){

	t_log* logger=iniciar_logger();
	log_warning(logger, "ERROR AL BUSCAR KEY %d EN %s NO EXISTE REGISTRO",key,nombreTabla);
	log_destroy(logger);
}


void LogDumpeo(){
	t_log* logger=iniciar_logger();
	log_info(logger,"REALIZANDO DUMP");
	log_destroy(logger);
}