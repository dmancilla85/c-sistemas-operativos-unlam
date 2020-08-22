
/*
== e2.h ==
**************************************
	Ejercicio 2 ( encabezado )
**************************************
Fecha: 10/06/2014
Autor: David A. Mancilla
--------------------------------------
Descripción:
Se tienen 4 procesos. El primer proceso genera un número aleatorio del 1 al 10,
lo imprime por pantalla y se lo pasa al segundo proceso por memoria compartida.
El segundo proceso lee el contenido de la memoria compartida y se lo pasa al
tercer proceso por otro segmento de memoria compartida junto con una de las
cuatro operaciones aritméticas básicas (suma, resta, multiplicación o división)
generadas aleatoriamente, imprimiendo por pantalla la operación generada.
El tercer proceso lee el contenido de la memoria compartida, genera un segundo
operando aleatorio también del 1 al 10 imprimiéndolo por pantalla y se lo pasa
por un tercer segmento de memoria compartida al tercer proceso junto con el primer
operando y la operación aritmética. El cuarto proceso lee la operación total desde
la memoria compartida, la resuelve, imprime por pantalla un signo igual (“=“) y
pasa el resultado al primer proceso por otro segmente de memoria compartida.
El primer proceso recibe sólo el resultado de la operación, lo imprime por
pantalla junto con un retorno de carro y vuelve a comenzar el proceso.
Todo se tiene que repetir 10 veces, aplicando un delay de no menos 300 ms (milisegundos)
entre la ejecución de cada proceso para que se pueda ir viendo la ejecución de cada
uno de los procesos.
La idea del ejercicio es generar una ejecución circular entre los diferentes procesos.
En pantalla se debería ver un resultado como el siguiente:
3 + 5 = 8
2 - 10 = -8
5 / 2 = 2.5
2 * 3 = 6

******************/

#ifndef E2_H_
#define E2_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

#define DEBUG   0
#define PROCESOS 4
#define REPITE 	100

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

// Procesos
void proceso1();
void proceso2();
void proceso3();
void proceso4();

// Semaforos
int  obtenerMutex(key_t clave);
int  obtenerSemaforo(key_t clave, int valor);
void pedirSemaforo(int IdSemaforo);
void devolverSemaforo(int IdSemaforo);
void eliminarSemaforo(int IdSemaforo);
void eliminarMutex(int IdSemaforo);

// Señales
void handleSignal(int, siginfo_t*, void*);
void  inicializarStrSignal(struct sigaction, int, void (*)(), int);
#endif
