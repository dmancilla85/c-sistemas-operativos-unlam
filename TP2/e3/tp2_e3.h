
/*
--------------------------------------------------------------------
 Ejercicio 3
--------------------------------------------------------------------
Dada una carpeta indicada por parámetro, se desea generar un hash
del contenido de la misma. El algoritmo de hash a utilizar será
simplemente una sumatoria de todos los bytes de los archivos.
Para realizar esta tarea de manera eficiente, el programa a
desarrollar disparará un hijo por cada archivo dentro de la carpeta,
encargado de calcular el hash. Una vez calculado, guardará el resultado
en un archivo cuyo nombre será el PID del proceso que lo calculó, y
luego terminará.
El proceso padre, a medida que vaya terminando cada uno de los hijos
deberá informar por pantalla la sumatoria parcial de los hashes y una
vez finalizados todos, generar un archivo de salida conteniendo el
hash total.
En caso de recibir una señal que interrumpa al padre, éste deberá
manejar la señal informando a todos los hijos mediante una señal a
elección que deben finalizar su ejecución, y esperar a que terminen
todos antes de salir.
--------------------------------------------------------------------
*/

#ifndef TP2_e3_H
#define TP2_e3_H

	#include <dirent.h>		// Manejo de directorios
	#include <errno.h>		//
	#include <stdio.h>		// Bibliotecas estándar
	#include <stdlib.h>		//
	#include <string.h>		// Manejo de strings
	#include <sys/wait.h>	// Comando WAIT
	#include <ctype.h>		//
	#include <unistd.h>		// Biblioteca UNIX
	#include <sys/types.h> 	//
	#include <signal.h> 	// Señales

	#define HELP "<<TP2_E3>>\nDada una carpeta indicada por parámetro, se generará un hash del contenido de la misma.\nSintaxis: \nprompt$/tp2_e3 <dir_destino>\nDonde <dir_destino> 	--> Carpeta donde se calculará el hash.\n."
	#define TST "//home//david//Escritorio//Práctica"

	#define DEBUG 0
	#define DIRE        "."
	#define EXT   ".hash" 	// Extensión de logs

	// 		Validación
	void	validaParametros(int argc, char *argv);

	// 		Directorios y hash
	int 	hashArchivo(int); // Retorna el hash
	int 	scanDirectorio(const char*) ; // Analiza directorios

	// 		Handle de SEÑALES
	void  	killProc(const pid_t);
	void  	inicializarStrSignal(struct sigaction, int, void (*)(), int);
	void  	hijo_finalizado(int, siginfo_t*, void*);
	void  	handleSignal(int, siginfo_t*, void*);
#endif
