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

#include "tp2_e2.h"

time_t 	ti; 		// Tiempo de inicio
int 				tiempo;

int main(int argc, char *argv[])
{
	
	time(&ti);
	tiempo = 0;
	
	if(argc < 3 && !DEBUG)
    {
      fprintf(stderr, "Error: Faltan parametros.\n");
      puts(HELP);
      return EXIT_FAILURE;
    }

    if ((atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0) && !DEBUG)
    {
      fprintf(stderr, "Error: Los parámetros ingresados no son numéricos.\n");
      puts(HELP);
      return EXIT_FAILURE;
    }

	// Si E es mayor que S no va a ejecutarse
	if( atoi(argv[1]) > atoi(argv[2]) )
	{
		fprintf(stderr, "Error: El tiempo de ejecución debe ser mayor que el tiempo de generación de hijos.\n");
		puts(HELP);
		return EXIT_FAILURE;
	}
	
	// Comienzo proceso
	printf("%d - Comienza proceso...\n", getpid());
	ciclar(atoi(argv[1]), atoi(argv[2]), 1);
	
	wait(NULL);
	
	return EXIT_SUCCESS;
}

void ciclar(int E, int S, int nivel)
{	// hijos cada E segundos durante S segundos
	char 	esHoja = 'S';
	time_t 	tf;
	
	if(DEBUG) printf("Cicla Proceso: %d -- Nivel: %d\n", getpid(), nivel);
	
	time(&tf);

	
	while( (tiempo = difftime(tf, ti)) < S )
	{	
		printf("Tiempo transcurrido: %d\n", tiempo);
		
		if(DEBUG) printf("Tiempo inicio: %s", asctime(localtime ( &ti )) );
		if(DEBUG) printf("Tiempo actual: %s\n", asctime(localtime ( &tf )) );
		
		if((S - tiempo) < E)
		{
			printf("(%d) Dormir 10s: tiempo %d\n", getpid(), tiempo);
			sleep(10);
		}
		
		if( fork() == 0)
		{
			if(DEBUG) printf("Cicla Proceso2: %d -- Nivel: %d\n", getpid(), nivel);
			ciclar(E, tiempo, nivel + 1);
			exit(EXIT_SUCCESS);
		}
		else
		{
			sleep(E);
			esHoja = 'N';
		}
		
		time(&tf);
	}
	
	if(esHoja == 'S')
		printf("(*) Proceso hoja: %d -- Nivel: %d\n", getpid(), nivel);
}
