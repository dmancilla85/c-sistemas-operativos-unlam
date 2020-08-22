
/*
== e3.c ==
**************************************
	Ejercicio 3 ( c�digo )
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

#include "e3.h"

pthread_t tid[MAX_THR]; 

int main(int argc, char *argv[])
{
    int 	N,					// Hilos a generar
            tiempo,				// Tiempo transcurrido
            hilosGen = 0,		// Hijos generados
            fraccion;			// Fraccion para cada hilo
    time_t 	t_inicio,			// Hora inicial
            t_final;			// Hora final
    t_intervalo inter;			// Intervalo (para hilos);

    time(&t_inicio);

  if(argc > 1 && strcmp(argv[1], "--h") == 0 && !DEBUG)
		{
			puts(HELP);
			exit(EXIT_SUCCESS);
		}
    
    if(argc != 2 && !DEBUG)
    {
      fprintf(stderr, "Error: Faltan par�metros.\n");
      puts("Utilice --h para ayuda");
      return EXIT_FAILURE;
    }
	 
	 if ((atoi(argv[1])) == 0  && (argv[1][0] != '0')  && !DEBUG)
    {
      fprintf(stderr, "Error: Argumento debe ser un numero valido y menor a %d.\n", MAX_THR);
      puts("Utilice --h para ayuda");
      return EXIT_FAILURE;
    }
	 
	 
    if ((atoi(argv[1]) < 0 || atoi(argv[1]) >= MAX_THR) && !DEBUG)
    {
      fprintf(stderr, "Error: Argumento debe ser un numero valido y menor a %d.\n", MAX_THR);
      puts("Utilice --h para ayuda");
      return EXIT_FAILURE;
    }

    if(!DEBUG)
      N = atoi( argv[1] );
    else
      N = DBG_THREADS;

    if( N < 0)
    {
        perror("Parametros incorrectos");
        puts("Utilice --h para ayuda");
        return EXIT_FAILURE;
    }

    // Si no se solicitan hilos
    switch( N )
    {
    case 0: // Sin hilos
        genera_primos(LS_MIN, LS_MAX);
        break;
    default: // Un solo hilo

        inter.l_inf = LS_MIN;
        fraccion = (LS_MAX / N);
        inter.l_sup = fraccion;

        // Comienzo a generar los hilos
        while(hilosGen < N && inter.l_sup <= LS_MAX)
        {
            // Genera un hilo para el intervalo predefinido
            pthread_create(&tid[hilosGen], NULL, runnerPrimo, &inter);
            sleep(1);
            // Comienza con el nro contiguo al max. anterior
            inter.l_inf += (fraccion + 1);
            inter.l_sup += (inter.l_inf + fraccion);

            if(inter.l_sup > LS_MAX)
              inter.l_sup = LS_MAX;

            hilosGen++;
        }
        break;
    }

    // Obtiene el tiempo final
    int i = 0;
    while(i++ < N)
        pthread_join(tid[i], NULL);

    time(&t_final);
    tiempo = difftime(t_final, t_inicio);

    if(tiempo >= 60)
        printf("Tiempo total de proceso: %3.2f minutos\n", ((float)tiempo)/60);
    else
        printf("Tiempo total de proceso: %d segundos\n", tiempo);

    return EXIT_SUCCESS;
}

// Comprobamos si un n�mero es primo o no
int es_primo(int numero)
{
    int 	res  = 0,
            divs = 2,
            mitad;

    if(numero > 2)    // Si el n�mero es mayor a 2, comprobamos si es primo.
    {
        /* Para ahorrar s�lo comprobamos si un n�mero es divisible hasta su mitad,
         ya que 7/2 = 3.xxx, 7/3=2.xxx, 7/4=0.xxx cuando empezamos a obtener
         valores menores que 0, ya no encontraremos m�s divisores enteros */
        mitad = floor((double)numero/2);

        do
        {
            res=(numero%divs!=0);  /* � El resto es 0 ? */
            //      printf("Divido %d entre %d. Resto %d. Res: %d\n", numero, divs, numero%divs, res);
            divs++;
        }
        while ((res!=0) && (divs<=mitad));
    }
    else if (numero==2) /* || (numero==1) */ /* �1 lo consideramos primo? */
        res=1;

    return res;
}

void *runnerPrimo(void *st_int)
{
    void *ret = NULL;
    t_intervalo *aux;

    aux = (t_intervalo*) st_int;
    genera_primos(aux->l_inf, aux->l_sup);

    pthread_exit(EXIT_SUCCESS);

    return ret;
}

// Genera primos, los almacenaremos en p, empezamos
// desde _inicial_ hasta el final
void genera_primos (int inicial, int final)
{
    //int primos = 0;
    int i = inicial;

    printf("\nHilo %d empieza en:\n%d\n", inicial, getpid());

    while (i < final)
    {
        // Vamos comprobando uno a uno si es primo
        if (es_primo(i))
        {
            //primos
	    		printf("%d\n", i++);
				}
			i++;
    }	
}
