/* 
--------------------------------------------------------------------
 Ejercicio 5
--------------------------------------------------------------------
Una empresa tiene un servidor donde se generan en un único directorio 
miles de archivos de log y salidas de sus procesos. No están 
parametrizados, así que para modificarlos y que cada proceso guarde 
los archivos en un directorio separado deber recompilar cada programa 
y es muy riesgoso. Como se hace muy complicada la administración de 
este directorio por la gran cantidad de archivos, necesitan generar 
una herramienta que le permita al administrador trabajar con los 
archivos pero paralelizando la ejecución de los comandos, y sin que 
esto afecte la performance del server. Para esto, ustedes deben 
generar un proceso que tome la lista de archivos de un directorio, 
y ejecute el comando que se le indique pasándole de a N archivos como 
parámetro de entrada al comando a ejecutar; que se deberá ejecutar 
varias veces concurrentemente. Se le debe poder indicar la cantidad 
máxima (-c) de archivos que le debe pasar al comando, y cuántos 
procesos (-p) como máximo pueden estar ejecutando a la vez.
Por ejemplo, si tengo un directorio con 100 archivos, y ejecuto así:
$ concurrent -c 20 -p 2 rm -f
Se ejecutarían 5 procesos en total (secuencialmente), cada uno 
borrando 10 archivos, pero sólo puede ejecutar de a 2 “rm -f” a la vez.
--------------------------------------------------------------------
*/
#ifndef _TP2_E5_H_
#define  _TP2_E5_H_ 
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <dirent.h>			// Contiene manejo de directorios.
	#include <getopt.h>
	#include <sys/stat.h>
	#include <sys/types.h>		// Se define el tipo de dato pid_t
	#include <unistd.h> 		// Contiene las definiciones de fork() y sleep().
	#include <sys/wait.h> 		// Contiene la definición de wait().
	#include <errno.h>			// Contiene librerias de error.

	#define FIL 256
	#define COL 256
	#define DEBUG 		0 		// Modo DEBUG 

	void esperar_hijo(int iNumSen);
	int	 buscar_archivos_directorio(const char*, char[][COL]);
	int  crear_proceso_comando(char*, char[][COL], int, int, int, int, int, int, char*);
	void mostrar_ayuda();
	void sintaxis();
#endif	
