/*
--------------------------------------------------------------------
 Ejercicio 2
--------------------------------------------------------------------
Escribir un programa que durante un ciclo de S segundos lance un hijo 
y luego espere E segundos para seguir iterando. S y E deben ser pasados 
por par�metro. Al finalizar el tiempo, los procesos que sean hojas del 
�rbol de parentesco, deber�n imprimir por pantalla su PID y el m�ximo 
nivel de �profundidad� alcanzado. Es decir, el padre es nivel 1, los 
hijos nivel 2, los nietos nivel 3, biznietos 4 y así sucesivamente.
Si en el tiempo S se lleg� hasta generar biznietos, estos imprimirán 
que se llegó a nivel 4 y su PID.
Los procesos cuyo tiempo de ejecuci�n restante sea menor que el par�metro 
E deber�n esperar 10 segundos adicionales antes de finalizar (para 
permitir �verlos� con ps o top). Realice varias ejecuciones con los 
mismos par�metros, redirigiendo la salida a un archivo para poder analizar 
el resultado. Se recomienda generar carga de procesamiento en algunas de 
las corridas (en otra terminal, find es un buen comando para esto, o bien 
un grep recursivo, ambos sobre directorios con bastantes archivos). 
Compare las ejecuciones, verificando cantidad de procesos generados 
en el �ltimo nivel, y a qu� nivel lleg� en cada corrida.
---------------------------------------------------
Conteste entonces:
a. 		Es consistente el resultado entre las distintas ejecuciones? 
		Es decir, �siempre se llega al mismo nivel?

Rta: 	No siempre se llega al mismo nivel. Depende del tiempo S y E 
		que ingrese por parametros.
---------------------------------------------------
b. 		La cantidad de procesos de un nivel dado es siempre la misma
Rta: 	Si. Siempre es la misma.
---------------------------------------------------
c. 		Indique sus conclusiones al respecto, del porqu� del 
		comportamiento observado.
Rta: 	Los niveles pueden variar segun los tiempos ingresados por parametro.
		Si el tiempo S es mayor y el tiempo E es bajo, la cantidad
		de niveles va a aumentar considerablemente.
		Si el tiempo E es mayor, el proceso padre va a dormir mas tiempo
		y por lo tanto va a haber menor cantidad de niveles creados.
--------------------------------------------------------------------
*/

#ifndef _TP2_E2_H_
#define _TP2_E2_H_
	#include<stdio.h>
	#include<stdlib.h>
	#include <sys/types.h>	// Se define el tipo de dato pid_t
	#include <unistd.h> 	// Contiene las definiciones de fork() y sleep().
	#include <sys/wait.h> 	// Contiene la definici�n de wait().
	#include <errno.h>		// Contiene librerias de error.
	#include <time.h>

	#define DEBUG 0 		// 1 Modo DEBUG para pruebas
	#define HELP  "Sintaxis: \nprompt$tp2_e2 [cicloSeg] [cicloEspera]\n\nDonde [cicloSeg] --> Intervalo de generaci�n de procesos hijos.\n\t[cicloEsp] --> Tiempo de espera.\nAmbos deben ser n�meros enteros mayor que cero."

	void ciclar(int E, int S, int nivel);
#endif
