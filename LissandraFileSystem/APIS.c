

#include "APIS.h"

void APIdrop(t_paquete_drop* paquete_drop){

	char nombreTablaMayuscula [strlen(paquete_drop->nombre_tabla)+1];
	strcpy(nombreTablaMayuscula,paquete_drop->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);

	if(existeTabla(nombreTablaMayuscula))
	{
		LogearDropCorrecto(nombreTablaMayuscula);

		RemoverDeLaMemtable(nombreTablaMayuscula);
		RemoverParticionesDeTablaYSusBloques(nombreTablaMayuscula);
		RemoverTemporalesDeTablaYSusBloques(nombreTablaMayuscula);
		RemoverMetadataDeTabla(nombreTablaMayuscula);
		RemoverCarpetaVaciaDeTabla(nombreTablaMayuscula); //ESTA FUNCION SIRVE SOLO SI LA CARPETA ESTA VACIA.

		printf("%s BORRADA.\n",nombreTablaMayuscula);

	} else
	{
		LogearDropFallido(nombreTablaMayuscula);
		error_show("No existe esa tabla.\n");
	}


}


void APIcreate(t_paquete_create* paquete_create){

	char nombreTablaMayuscula [strlen(paquete_create->nombre_tabla)+1];
	string_to_upper(nombreTablaMayuscula);
	strcpy(nombreTablaMayuscula,paquete_create->nombre_tabla);

	if(!existeTabla(nombreTablaMayuscula)){
		crearTabla(nombreTablaMayuscula);
		crearMetadataConfig(nombreTablaMayuscula,paquete_create->metadata.consistencia,paquete_create->metadata.particiones,paquete_create->metadata.tiempo_de_compactacion);
		//crearParticiones(nombreTablaMayuscula,paquete_create->metadata.particiones);
		//int particiones=paquete_create->metadata.particiones;
		for(int particiones=paquete_create->metadata.particiones;particiones;particiones--)
		crearParticion(nombreTablaMayuscula,particiones);

	} else if (EEXIST==errno) error_show("YA EXISTE LA TABLA");

}


void APIinsert(t_paquete_insert* paquete_insert){
	/*
		 *1. Verificar que la tabla exista en el file system. En caso que no exista, informa el error y continúa su ejecución.
		 *2. Obtener la metadata asociada a dicha tabla.
		 *3. Verificar si existe en memoria una lista de datos a dumpear. De no existir, alocar dicha memoria.
		 *4. El parámetro Timestamp es opcional. En caso que un request no lo provea (por ejemplo insertando un valor desde la consola), se usará el valor actual del Epoch UNIX.
		 *5. Insertar en la memoria temporal del punto anterior una nueva entrada que contenga los datos enviados en la request.

		 * En los request solo se utilizarán las comillas (“”) para enmascarar el Value que se envíe. No se proveerán request con comillas en otros puntos.
	 */

	char nombreTablaMayuscula[strlen(paquete_insert->nombre_tabla)+1];
	strcpy(nombreTablaMayuscula,paquete_insert->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);
	LogearInsert(paquete_insert->timestamp,paquete_insert->valor_key,paquete_insert->value,nombreTablaMayuscula);
	if(existeTabla(nombreTablaMayuscula)){
	t_metadata* metadataDeTabla=obtenerMetadataTabla(nombreTablaMayuscula);
	agregarTabla(paquete_insert);
//	agregarTabla(paquete_insert);
//	agregarTabla(paquete_insert);
//	agregarTabla(paquete_insert);
//	agregarTabla(paquete_insert);
//	agregarTabla(paquete_insert);
	imprimirListaTablas();
	//dump();
	//crearTemporal("TABLA_B",dictionary_get(memTable, "TABLA_B"));
	free(metadataDeTabla);

	} else LaTablaNoExiste(paquete_insert->timestamp,paquete_insert->valor_key,paquete_insert->value,paquete_insert->nombre_tabla);


}

char* APIselect(t_paquete_select* paquete_select){
	/*
	 *1. Verificar que la tabla exista en el file system.
	 *2. Obtener la metadata asociada a dicha tabla.
	 *3. Calcular cual es la partición que contiene dicho KEY.
	 *4. Escanear la partición objetivo, todos los archivos temporales y la memoria temporal de dicha tabla (si existe) buscando la key deseada.
	 *5. Encontradas las entradas para dicha Key, se retorna el valor con el Timestamp más grande.
	 *
	 */


	char nombreTablaMayuscula[strlen(paquete_select->nombre_tabla)+1];
	strcpy(nombreTablaMayuscula,paquete_select->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);

		if(existeTabla(nombreTablaMayuscula)){
			t_metadata* metadataDeTabla=obtenerMetadataTabla(nombreTablaMayuscula);
			//int particionKey;
			//particionKey =	particionDeKey(paquete_select->valor_key,metadataDeTabla->particiones);
			t_list* RegistrosEncontrados=list_create();
			list_add(RegistrosEncontrados,buscarMemTable(nombreTablaMayuscula,paquete_select->valor_key));
			//list_add(RegistrosEncontrados,buscarParticiones(nombreTablaMayuscula,paquete_select->valor_key));
			//list_add(RegistrosEncontrados,buscarTemporal(nombreTablaMayuscula,paquete_select->valor_key));
			char* valueEncontrado = elegirMayorTimeStamp(RegistrosEncontrados);
			printf("El Value encontrado fue %s\n",valueEncontrado);
			free(metadataDeTabla);
			list_clean(RegistrosEncontrados);
			return valueEncontrado;

		}else return NULL;

}

t_metadata* APIdescribe (t_paquete_describe* paquete_describe){
	char nombreTablaMayuscula [strlen(paquete_describe->nombre_tabla)+1];
	strcpy(nombreTablaMayuscula,paquete_describe->nombre_tabla);
	string_to_upper(nombreTablaMayuscula);

	if(existeTabla(nombreTablaMayuscula)){
		t_metadata* metadataDeTabla = obtenerMetadataTabla(nombreTablaMayuscula);
		return metadataDeTabla;
		free(metadataDeTabla);
	} else {
		error_show("NO EXISTE %s\n",nombreTablaMayuscula);
		return NULL;
	}

}

void APIdescribeTodasLasTablas(){
	t_list* listaDeTablas = listarTablasExistentes();
	if(listaDeTablas != NULL){
		list_iterate(listaDeTablas,imprimirMetadataDeTabla);
	}
	list_destroy(listaDeTablas);
	//DESPUES VAMOS A TENER QUE CAMBIAR A TIPO T_LIST LA FUNCION ASI RETORNAMOS LA LISTA
	//Y VEMOS COMO LA ENVIAMOS.
}

void listarTablas(){
	DIR *d;
	  struct dirent *dir;
	  d = opendir("/home/utnso/LISSANDRA_FS/Tables");
	  if (d)
	  {
		while ((dir = readdir(d)) != NULL) {
			if (!string_contains(dir->d_name,".")) printf("%s ", dir->d_name);

		}
		closedir(d);
	  }
}

void imprimirMetadata(t_metadata* metadataDeTablaPedida){
	printf("%s%s ",BLUE,pasarAConsistenciaChar(metadataDeTablaPedida->consistencia));
	printf(" %d ",metadataDeTablaPedida->particiones);
	printf(" %d\n",metadataDeTablaPedida->tiempo_de_compactacion);
	printf("%s",NORMAL_COLOR);
}



t_list* listarTablasExistentes() {
  DIR *d;
  struct dirent *dir;
  d = opendir("/home/utnso/LISSANDRA_FS/Tables");
  t_list *listaDeTablas = list_create();
  if (d)
  {
    while ((dir = readdir(d)) != NULL) {
    	if (!string_contains(dir->d_name,".")){
    	list_add(listaDeTablas,dir->d_name);
    	}

    }
    return listaDeTablas;
    closedir(d);
  } else return listaDeTablas;

  	list_destroy(listaDeTablas);
}

char* elegirMayorTimeStamp(t_list* RegistrosEncontrados){
	list_sort(RegistrosEncontrados,mayorTimeStamp);
	t_registro* registroConMayorTimeStamp = (t_registro*)list_remove(RegistrosEncontrados,0);
	return registroConMayorTimeStamp->value;
}

t_registro* crearRegistro (char*value,uint16_t key,long long timestamp){
	t_registro* registroNuevo=malloc(sizeof(t_registro));
	registroNuevo->value=malloc(strlen(value));
	strcpy(registroNuevo->value,value);
	registroNuevo->key=key;
	registroNuevo->timestamp=timestamp;
	return registroNuevo;
}

int particionDeKey(int key,int particiones){
	return particiones%key;

}

t_metadata* obtenerMetadataTabla (char* nombreTabla ){

	char* ubicacionMetaData = DirectorioDeMetadataTabla(nombreTabla);
	t_config* config = config_create(ubicacionMetaData);
	char* consistencia = config_get_string_value(config,"CONSISTENCY");
	int particiones = atoi(config_get_string_value(config,"PARTITIONS"));
	int compactacion = atoi(config_get_string_value(config,"COMPACTION_TIME"));
	t_metadata* metadataObtenida=crearMetadata(pasarAConsistenciaINT(consistencia),particiones,compactacion);
	free(ubicacionMetaData);
	config_destroy(config);
	return metadataObtenida;

}


int existeTabla(char* TablaBuscada){

	char* directorioTablas = DirectorioDeTabla(TablaBuscada);

	DIR* dir = opendir(directorioTablas);
	if (dir)
	{
	    closedir(dir);
	    free(directorioTablas);
	    return 1;
	}
	else if (ENOENT == errno)
	{
	    free(directorioTablas);
	    return 0;
	}
	else
	{

		printf("Fallo desconocido :(\n");
		free(directorioTablas);
	    return 0;
	}
}





t_metadata* crearMetadata(consistency consistencia, int particiones,int tiempo_compactacion){
	t_metadata* metadata = malloc(sizeof(metadata));

	metadata->consistencia=consistencia;
	metadata->particiones=particiones;
	metadata->tiempo_de_compactacion=tiempo_compactacion;

	return(metadata);

}

//PASAR CONSISTENCIAS
int pasarAConsistenciaINT(char* consistencia){

	if (strcmp(consistencia, "SC")==0) {
		return 0;
	}
	else if (strcmp(consistencia, "SHC")==0) {
		return 1;
	}
	else if (strcmp(consistencia, "EC")==0) {
		return 2;

	}
	return 0;

}

char* pasarAConsistenciaChar(int consistencia){

	if (consistencia==0) {
		return "SC";
	}
	else if (consistencia==1) {
		return "SHC";
	}
	else if (consistencia==2) {
		return "EC";
	}
	return "SC";

}

void logLaTablaYaExiste(){
	t_log* logger = iniciar_logger();
	log_info(logger, "ERROR LA TABLA YA EXISTE");
    log_destroy(logger);
}

void crearTabla(char* nombreTabla){
	char* DirectorioTablas = DirectorioDeTabla(nombreTabla);
	mkdir(DirectorioTablas,0777);
	free(DirectorioTablas);
}


char* DirectorioDeTabla(char* tabla){
	t_config* config = config_create("Lissandra.config");
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioTablas = malloc(strlen(Montaje) + strlen(tabla) +strlen("Tables/") +1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,tabla);
	return directorioTablas;
}


char* DirectorioDeMetadataTabla(char* tabla){
	t_config* config = config_create("Lissandra.config");
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioTablas = malloc(strlen(Montaje) + strlen(tabla) +strlen("Tables/") + strlen("/Metadata.config") +1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,tabla);
	strcat(directorioTablas,"/Metadata.config");
	return directorioTablas;

}
char* DirectorioDeMetadata(){
	t_config* config = config_create("Lissandra.config");
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioMetadata = malloc(strlen(Montaje)  +strlen("Metadata/Metadata.bin")  +1);
	strcpy(directorioMetadata,Montaje);
	strcat(directorioMetadata,"Metadata/Metadata.bin");
	return directorioMetadata;

}
char* DirectorioDeParticion(char* nombretabla,int numeroParticion){
	t_config* config = config_create("Lissandra.config");
	char buffer [3];
	sprintf(buffer,"/%d",numeroParticion);
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioTablas = malloc(strlen(Montaje) + strlen(".bin") +strlen("Tables/") +sizeof(buffer)+ strlen(nombretabla)+1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,nombretabla);
	strcat(directorioTablas,buffer);
	strcat(directorioTablas,".bin");
	return directorioTablas;


}

int existe_temporal(char* path){
	struct stat buffer;
	return(stat(path,&buffer)==0);
}

void RemoverTemporalesDeTablaYSusBloques(char* nombretabla){
	t_config* config = config_create("Lissandra.config");
	char* NombreTmp = string_substring_until(string_reverse(nombretabla), 1);

	char buffer [3];
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char* directorioAux= malloc(strlen(Montaje) + strlen(".tmp") +strlen("Tables/") +sizeof(buffer)+ strlen(nombretabla)+strlen(NombreTmp)+1);
	char* directorioTablas = malloc(strlen(Montaje) + strlen(".tmp") +strlen("Tables/") +sizeof(buffer)+ strlen(nombretabla)+strlen(NombreTmp)+1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,nombretabla);
	strcat(directorioTablas,"/");
	strcat(directorioTablas,NombreTmp);
	int i =1;
	strcpy(directorioAux,directorioTablas);
	sprintf(buffer,"%d",i);
	strcat(directorioAux,buffer);
	strcat(directorioAux,".tmp");
	while(existe_temporal(directorioAux)){

		t_config* directorioTemporal= config_create(directorioAux);
		char **arrayBloques = string_get_string_as_array(config_get_string_value(directorioTemporal,"BLOCKS"));
		string_iterate_lines(arrayBloques,removerBloque);
		remove(directorioAux);
		i++;
		strcpy(directorioAux,directorioTablas);
		sprintf(buffer,"%d",i);
		strcat(directorioAux,buffer);
		strcat(directorioAux,".tmp");
	}

	strcat(directorioTablas,buffer);
	strcat(directorioTablas,".tmp");
	free(directorioAux);

}


char* DirectorioDeTemporal(char* nombretabla){
	t_config* config = config_create("Lissandra.config");
	char* NombreTmp = string_substring_until(string_reverse(nombretabla), 1);

	char buffer [3];
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char* directorioAux= malloc(strlen(Montaje) + strlen(".tmp") +strlen("Tables/") +sizeof(buffer)+ strlen(nombretabla)+strlen(NombreTmp)+1);
	char* directorioTablas = malloc(strlen(Montaje) + strlen(".tmp") +strlen("Tables/") +sizeof(buffer)+ strlen(nombretabla)+strlen(NombreTmp)+1);
	strcpy(directorioTablas,Montaje);
	strcat(directorioTablas,"Tables/");
	strcat(directorioTablas,nombretabla);
	strcat(directorioTablas,"/");
	strcat(directorioTablas,NombreTmp);
	int i =1;
	strcpy(directorioAux,directorioTablas);
	sprintf(buffer,"%d",i);
	strcat(directorioAux,buffer);
	strcat(directorioAux,".tmp");
	while(existe_temporal(directorioAux)){
		i++;
		strcpy(directorioAux,directorioTablas);
		sprintf(buffer,"%d",i);
		strcat(directorioAux,buffer);
		strcat(directorioAux,".tmp");
	}

	strcat(directorioTablas,buffer);
	strcat(directorioTablas,".tmp");
	free(directorioAux);
	return directorioTablas;


}



char* DirectorioDeBloque(int numeroBloque){
	t_config* config = config_create("Lissandra.config");
	char* buffer=malloc(sizeof(int));
	sprintf(buffer,"%d",numeroBloque);
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioBloque = malloc(strlen(Montaje) + strlen(".bin") +strlen("Bloques/") +sizeof(buffer)+1);
	strcpy(directorioBloque,Montaje);
	strcat(directorioBloque,"Bloques/");
	strcat(directorioBloque,buffer);
	strcat(directorioBloque,".bin");
	free(buffer);
	return directorioBloque;


}

char* DirectorioBitMap(){
	t_config* config = config_create("Lissandra.config");
	char* Montaje= config_get_string_value(config,"PUNTO_MONTAJE");
	char *directorioMetadata = malloc(strlen(Montaje)  +strlen("Metadata/bitmap.bin")  +1);
	strcpy(directorioMetadata,Montaje);
	strcat(directorioMetadata,"Metadata/bitmap.bin");
	return directorioMetadata;

}

void crearMetadataConfig(char*nombreTablaMayuscula, consistency consistencia, int particiones,int tiempo_compactacion){
	char *directorioTablaNueva = DirectorioDeMetadataTabla(nombreTablaMayuscula);
	FILE *fp=NULL;
	fp=fopen(directorioTablaNueva,"a");
	fprintf(fp,"CONSISTENCY=%s\nPARTITIONS=%d\nCOMPACTION_TIME=%d",pasarAConsistenciaChar(consistencia),particiones,tiempo_compactacion);
	fclose(fp);
	free(directorioTablaNueva);
}



void crearParticiones(char*nombreTabla ,int nroParticiones){ // no anda pero la dejo x las dudas
	char* directorioMetadata=DirectorioDeMetadata();
	char* directorioBitmap=DirectorioBitMap();
	t_config* config = config_create(directorioMetadata);
	int blockNum = atoi(config_get_string_value(config,"BLOCKS"));
	while(blockNum%8!=0){
			blockNum++; //BASICAMENTE AUMENTO BLOCK NUM HASTA QUE SEA DIVISIBLE POR 8 PARA TRABAJAR CON BYTES
	}
	FILE *particionBin;


	int i=0;
	while(nroParticiones){

	if(!bitarray_test_bit(bitmap,i)){
		particionBin=fopen(DirectorioDeParticion(nombreTabla,nroParticiones),"w");
		fprintf(particionBin,"SIZE=0\nBLOCKS=[%d]",i);
		nroParticiones--;
		bitarray_set_bit(bitmap,i);
		fclose(particionBin);
	}
	i++;
	}
	free(directorioMetadata);
	free(directorioBitmap);
	fclose(particionBin);
	config_destroy(config);

}

void crearParticion(char*nombreTabla ,int nroParticion){
	char* directorioMetadata=DirectorioDeMetadata();
	char* directorioBitmap=DirectorioBitMap();
	t_config* config = config_create(directorioMetadata);
	int blockNum = atoi(config_get_string_value(config,"BLOCKS"));
	while(blockNum%8!=0){
			blockNum++; //BASICAMENTE AUMENTO BLOCK NUM HASTA QUE SEA DIVISIBLE POR 8 PARA TRABAJAR CON BYTES
	}
	FILE *particionBin;


	int i=0;
	while(i<blockNum){
	if(!bitarray_test_bit(bitmap,i)){
		particionBin=fopen(DirectorioDeParticion(nombreTabla,nroParticion),"w");
		fprintf(particionBin,"SIZE=0\nBLOCKS=[%d]",i);
		bitarray_set_bit(bitmap,i);
		crearBloque(i);
		//pruebasSet();
		//ActualizarBitmap();
		break;//bitarray_set_bit(bitmap, i);
	}
	i++;
	}
	free(directorioMetadata);
	free(directorioBitmap);
	fclose(particionBin);
	config_destroy(config);


}
//free(nombreTablaMayus);
void RemoverCarpetaVaciaDeTabla(char *nombreTabla){
	char *directorioDeTablaABorrar= DirectorioDeTabla(nombreTabla);
	remove(directorioDeTablaABorrar);
	free(directorioDeTablaABorrar);
}

void RemoverMetadataDeTabla(char *nombreTabla){
	char *directorioDeTablaABorrar= DirectorioDeMetadataTabla(nombreTabla);
	remove(directorioDeTablaABorrar);
	free(directorioDeTablaABorrar);

}

void RemoverDeLaMemtable(char *nombreTabla){
	if(memTable != NULL)
	{
		if(dictionary_has_key(memTable,nombreTabla))
		{ //BORRAR DE LA MEMTABLE.
		dictionary_remove_and_destroy(memTable,nombreTabla,liberarNodo);
		printf("Se eliminaron los registros de %s de la memTable.\n",nombreTabla);
		} else {printf("No se hallaron registros en la memTable de %s\n",nombreTabla);}

	}
}

void RemoverParticionesDeTablaYSusBloques(char* nombreTabla){
	t_metadata* metadataDeTabla=obtenerMetadataTabla(nombreTabla);
	int particiones = metadataDeTabla->particiones;
	free(metadataDeTabla);

	for(int i=1;i<=particiones;i++){

		char *directorioDeTablaABorrar= DirectorioDeParticion(nombreTabla,i);
		t_config* directorio = config_create(directorioDeTablaABorrar);
		char **arrayBloques = string_get_string_as_array(config_get_string_value(directorio,"BLOCKS"));
		string_iterate_lines(arrayBloques,removerBloque);
		//ESTA FUNCION REMUEVE CADA BLOQUE ASIGNADO A ESA PARTICION.

		remove(directorioDeTablaABorrar); //ESTE REMOVE REMUEVE LA PARTICION.

		free(arrayBloques);
		config_destroy(directorio);
		free(directorioDeTablaABorrar);
	}
}

void removerBloque(char* nroBloque){
	char *directorioDeBloque = DirectorioDeBloque(atoi(nroBloque));
	bitarray_clean_bit(bitmap,atoi(nroBloque));
	remove(directorioDeBloque);
	free(directorioDeBloque);
}





