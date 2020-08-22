

#ifndef E1_1_H
#define E1_1_H

#include <stdio.h>
#include <ctype.h>
#include <sys/wait.h>	// Comando WAIT
#include <stdlib.h>
#include <unistd.h>
#include <signal.h> 	// Señales
#include <pthread.h>

#define DEBUG             0 // Tiene que estar en 1 para depuracion
#define DEFAULT_PROD      "50" // Mesas a producir por default
#define MAX_MESAS 		  10000000

// Restricciones de stock
#define PROCESOS		  5
#define MAX_STOCK_MARCOS  2
#define MAX_STOCK_PATAS   6
#define MAX_STOCK_VIDRIOS 2
#define MAX_STOCK_CAJONES 1

// Macros para semaforos
#define p(a) pthread_mutex_lock(a)
#define v(a) pthread_mutex_unlock(a)

#define HELP  "Sintaxis: \nprompt$e1 [cant]\n\nDonde [cant] --> Cantidad de mesas a producir.\nDebe ser un número entero mayor que cero."

// Defino un tipo enumerado para mejorar la legibilidad del código
enum Subproceso
{
  MARCOS,
  PATAS,
  VIDRIO,
  CAJONES,
  FINAL
};

// Estructura de variables de produccion
typedef struct s_stock
{
  int patas;
  int marcos;
  int vidrio;
  int cajones;
  int finales;
} t_stock;

void *fabricaMarcos(void*);
void *fabricaPatas(void*);
void *ensamblaVidrio(void*);
void *ensamblaCajones(void*);
void *finalizaMesa(void*);

// Handle de SEÑALES
void  killProc(const int);
void  inicializarStrSignal(struct sigaction, int, void (*)(), int);
void  hijo_finalizado(int, siginfo_t*, void*);
void handleSignal(int, siginfo_t*, void*);

#endif
