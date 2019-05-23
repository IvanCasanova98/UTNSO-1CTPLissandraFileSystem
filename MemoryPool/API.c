#include "API.h"

void selectf(t_paquete_select* paquete, t_config* config, t_log* logger)
{
	if(condicion_select(paquete -> nombre_tabla,paquete -> valor_key))
	{
		t_pagina* pagina_encontrada = buscar_pagina(paquete -> nombre_tabla, paquete -> valor_key);
		printf("%s", pagina_encontrada -> value); //NO PRINTEAR, ENVIAR A KERNEL
	}
	else
	{
		enviar_select_lissandra(paquete, config, logger);
	}
}

void insert(t_paquete_insert* paquete, t_config* config, t_log* logger)
{
	if(existe_tabla_paginas(paquete -> nombre_tabla))
	{
		t_pagina* pagina = crear_pagina(paquete -> valor_key, paquete -> value, paquete -> timestamp);
		t_pagina_completa* pagina_completa = crear_pagina_completa(pagina);
		agregar_pagina(paquete -> nombre_tabla, pagina_completa);
	}
	else
	{
		enviar_insert_lissandra(paquete, config, logger);
	}
}
