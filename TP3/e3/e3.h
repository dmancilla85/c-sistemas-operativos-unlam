
/*
== e3.h ==
**************************************
	Ejercicio 3 ( encabezado )
**************************************
Fecha: 11/06/2014
Autor: David A. Mancilla
--------------------------------------
Descripci�n:
Generar un programa que calcule n�meros primos de 1 a 10.000.000, utilizando X
hilos y los imprima por pantalla. Siendo X un n�mero pasado por par�metro.
Y al finalizar informar el tiempo total utilizado para el c�lculo.
En caso de recibir 0, no se utilizar�n hilos.
En caso de recibir 2 (o m�s), se dividir� en 2 (o m�s) intervalos de n�meros
para que cada hilo calcule dentro del intervalo que le toc� y al finalizar todos
los hilos se imprimir� el tiempo total.
Por ejemplo, si recibe 3, un hilo tomar� el intervalo 1 a 3.333.333, el siguiente
de 3.333.334 a 6.666.666 y el �ltimo desde 6.666.666 a 10.000.000. imprimiendo
cada uno la salida por pantalla a medida que los van generando. Y al finalizar
todos los hilos, imprimir el tiempo total.

******************/
#ifndef E3_H_
#define E3_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define DEBUG   0			// Tiene que estar en 1 para depuracion
#define DBG_THREADS	5		// Hilos a generar en DEBUG
#define LS_MIN  1			// Inicio del intervalo de n�meros
#define LS_MAX  100//00	// Final del intervalo de n�meros
#define MAX_THR 1000		// M�xima cantidad de hilos permitidos
#define HELP  "<GENERADOR DE NUMEROS PRIMOS>\n\nDescripci�n:\nGenera un programa que calcula n�meros primos de 1 a 10.000.000, utilizando X hilos y los imprime por pantalla.\n\nSintaxis: \nprompt$ e3 [N]\n\n Donde [N] --> Cantidad de hilos a generar.\nN debe ser un n�mero entero mayor que cero."

int es_primo(int);
void genera_primos (int, int);
void *runnerPrimo(void *);

typedef struct s_lim
{
	int l_inf;
	int l_sup;
} t_intervalo;

#endif
