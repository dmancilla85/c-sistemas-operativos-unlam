/* 
--------------------------------------------------------------------
 Ejercicio 4
--------------------------------------------------------------------
Se quiere simular el funcionamiento del comando grep, por lo que se 
pide realizar un proceso que reciba como parámetros el patrón a 
buscar, un filtro para los archivos que tiene que evaluar y un flag 
que indique si la búsqueda debe incluir subdirectorios. Ej.:
$ c_grep /patron/ *.txt
$ c_grep /patron/ ./*.dat [-r]
El proceso principal debe generar un proceso por cada archivo a 
evaluar y estos 
procesos deben generar un archivo donde se indique el archivo leído, 
el número de línea en que se encontró coincidencia y el contenido de 
la misma, para corroborar que sea correcto. Si no se encuentran 
coincidencias no se deben generar los archivos.
El nombre de los archivos generados debe ser igual al pid del proceso 
que los genera.

$ c_grep TP *.txt
diccionario.txt
línea 15: Sistemas Operativos TP 2.
--------------------------------------------------------------------
*/

#ifndef  _C_GREP_H
#define  _C_GREP_H
	#include <stdio.h>
	#include <signal.h>
	#include <string.h>
	#include <sys/wait.h>
	#include <sys/types.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <sys/stat.h>
	#include <dirent.h>
	#include <errno.h>

	#define DEBUG 0 // 1 Modo DEBUG para pruebas
	#define HELP  "<< C_GREP >> - Comando de búsqueda estilo GREP\nSintaxis: \nprompt$/c_grep <patron> <filtro> [-r]\nDonde <patron> 	--> Patrón a buscar.\n     <filtro> 	--> Filtro de archivos a evaluar (archivos de texto).\n      [-r]	--> Incluir subdirectorios."

	void buscarEnArchivo(const char*, const char*, const char*);
	void scanDirectorio(const char*, const char*, const char*, char);
#endif
/**************************************/
