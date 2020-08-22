
/*
== e2.c ==
**************************************
	Ejercicio 2 ( código )
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

#include "e2.h"

// id. Semáforos
int   idMutSeg1,
      idMutSeg2,
      idMutSeg3,
      idMutSeg4;

pid_t PPID;
pid_t hijoPID[PROCESOS];

int main(int argc, char *argv[])
{
    key_t 	claveSeg1, // Claves para semáforos
            claveSeg2,
            claveSeg3,
            claveSeg4;

    int  i = 0, cantHijos;

		struct sigaction 	st_sgnAct;
  
  /* Inicialización de estructuras de señales */
  inicializarStrSignal(st_sgnAct, SIGUSR1, handleSignal, 1);

    PPID = getpid();

    // Genero clave para semáforos
    claveSeg1 = ftok("/", 6000);
    claveSeg2 = ftok("/", 6001);
    claveSeg3 = ftok("/", 6002);
    claveSeg4 = ftok("/", 6003);

	 printf("\n.....................\n"); // Comienzo nuevo renglón
    while (i++ < REPITE)
    {
        // Inicializo semáforos
	idMutSeg1 = obtenerSemaforo(claveSeg1, 1);
        idMutSeg2 = obtenerSemaforo(claveSeg2, 0);
        idMutSeg3 = obtenerSemaforo(claveSeg3, 0);
        idMutSeg4 = obtenerSemaforo(claveSeg4, 0);

        if( (hijoPID[0] = fork()) != 0 )
        {
            if( (hijoPID[1] = fork()) != 0 )
            {
                if( (hijoPID[2] = fork()) != 0 )
                {
                    if( (hijoPID[3] = fork()) != 0 )
                    {
                      // Espero a que los procesos finalicen
                      // Esperar a que terminen los hijos
                      cantHijos = PROCESOS;
                      
							 while(cantHijos-- > 0)
							 {
								  wait(NULL);
							 }
               
                    }
                    else
                    {
                        proceso4();
                    }
                }
                else
                {
                    proceso3();
                }
            }
            else
            {
                proceso1();
            }
        }
        else
        {
            proceso2();
        }

        // Elimino los semaforos para recomenzar
        if(getpid() == PPID)
        {
            eliminarSemaforo(idMutSeg1);
            eliminarSemaforo(idMutSeg2);
            eliminarSemaforo(idMutSeg3);
            eliminarSemaforo(idMutSeg4);
            printf("\n.....................\n"); // Comienzo nuevo renglón
        }
    }

    return EXIT_SUCCESS;
}

void proceso1()
{
    key_t 	clave1 = 10, 
				clave4 = 40;
    int 		shmid1, 
				shmid4, 
				nro1;
    char 	*resultado;
    char  	*buffer;
    float	res = 0;
	
	// Genero número aleatorio
   nro1 = 1 + rand() % 10;
	
	
    // Primer segmento
	if(DEBUG) puts("Proceso1 pide A");
   pedirSemaforo(idMutSeg1);
	if(DEBUG) puts("Proceso1 tiene A");
   
  
	shmid1 = shmget(clave1, 1024, IPC_CREAT | 0660);
   buffer = (char *)shmat(shmid1, NULL, 0);
   sprintf(buffer, "%2d", nro1); // Primer operando
	// Mostrar el primer número
	printf("%d", nro1);
	fflush(stdout);
	shmdt(buffer);
		
	if(DEBUG) puts("Proceso1 suelta B");    
	devolverSemaforo(idMutSeg2);
	
	if(DEBUG) puts("Proceso1 suelta A");    
	devolverSemaforo(idMutSeg1);

    // Cuarto segmento
	if(DEBUG) puts("Proceso1 pide D");
	pedirSemaforo(idMutSeg4);
	pedirSemaforo(idMutSeg4);
	
	if(DEBUG) puts("Proceso1 tiene D");
   shmid4 = shmget(clave4, 1024, IPC_CREAT | 0660);
   
   resultado = (char *)shmat(shmid4, NULL, 0);

	// Mostrar el resultado
	res = atof(resultado);
   printf("%3.2f", res);
	fflush(stdout);

	// Desasocia las variables
   shmdt(resultado);
   
   if(DEBUG) puts("Proceso1 devuelve A");
   devolverSemaforo(idMutSeg1);
   if(DEBUG) puts("Proceso1 devuelve D");
	devolverSemaforo(idMutSeg4);
	sleep(1);
	// Elimino área de memoria compartida
   shmctl(shmid4, IPC_RMID, 0);
   exit(EXIT_SUCCESS);
}

void proceso2()
{
    key_t 	clave1 = 10, 
			clave2 = 20;
    int 	shmid1, 
			shmid2, 
			nro1, 
			aux;
    char  	*buffer1, 
			*buffer2, 
			signo;
	
	// Semilla para números aleatorios
    srand(time(NULL));
	
	// Genero operando aleatorio
    aux = 1 + rand() % 4;
	
	// Determina el signo
    switch(aux)
    {
      case 1: signo = '+'; break;
      case 2: signo = '-'; break;
      case 3: signo = '*'; break;
      case 4: signo = '/'; break;
    }

    // Primer segmento
	if(DEBUG) puts("Proceso2 pide B");
	pedirSemaforo(idMutSeg2);
  	if(DEBUG) puts("Proceso2 tiene B");
   shmid1 = shmget(clave1, 1024, IPC_CREAT | 0660);
   buffer1 = (char *)shmat(shmid1, NULL, 0);
   
   //if(DEBUG) printf("\n2.1--- %s ---- ", buffer1);
   
   nro1 = atoi(buffer1);

   // 2do. segmento
   shmid2 = shmget(clave2, 1024, IPC_CREAT | 0660);
   buffer2 = (char *)shmat(shmid2, NULL, 0);
    
	// Genero mensaje para el próximo proceso
	sprintf(buffer2,"%2d%c", nro1, signo);
   
   // Mostrar el signo
  	printf(" %s ", buffer2+2);
  	fflush(stdout);
   
   // Desasocia las variables
	shmdt(buffer1);
  	shmdt(buffer2);
	if(DEBUG) puts("Proceso2 suelta C");
	if(DEBUG) puts("Proceso2 suelta B");
	
	devolverSemaforo(idMutSeg3);
	devolverSemaforo(idMutSeg2);
	
	sleep(1);
	
	// Elimino área de memoria compartida
   shmctl(shmid1, IPC_RMID, 0);
   exit(EXIT_SUCCESS);
}

void proceso3()
{
    key_t 	clave2 = 20, 
				clave3 = 30;
    int 		shmid3, 
				shmid2, 
				nro2;
    char  	*buffer3, 
				*buffer2, num[4];
    
	// Semilla para números aleatorios
	srand(time(NULL));
	
	// Genero número aleatorio
   nro2 = 1 + rand() % 10;
   sprintf(num, "%2d", nro2);

    // 2do. segmento
  	if(DEBUG) puts("Proceso3 pide C");
	pedirSemaforo(idMutSeg3);
	
	if(DEBUG) puts("Proceso3 tiene C");
		
   shmid2 = shmget(clave2, 1024, IPC_CREAT | 0660);
   buffer2 = (char *)shmat(shmid2, NULL, 0);
	
   // 3r. segmento
   shmid3 = shmget(clave3, 1024, IPC_CREAT | 0660);
   buffer3 = (char *)shmat(shmid3, NULL, 0);
  	
	strcpy(buffer3, buffer2);
	strcat(buffer3, num);
	// Mostrar el segundo número
  	printf("%s", num);
	fflush(stdout);
	
 	// Desasocia las variables
   shmdt(buffer3);
   shmdt(buffer2);
   
   if(DEBUG) puts("Proceso3 suelta C");
	devolverSemaforo(idMutSeg3); 
	
	if(DEBUG) puts("Proceso3 suelta 2D");
	devolverSemaforo(idMutSeg4);
	devolverSemaforo(idMutSeg4);
	
	sleep(1);
	
	// Elimino área de memoria compartida
   shmctl(shmid2, IPC_RMID, 0);
   exit(EXIT_SUCCESS);
}

void proceso4()
{
    key_t 	clave4 = 40, 
			clave3 = 30;
    int 	shmid3, 
			shmid4; 
		float res, nro1, nro2;
    char 	*buffer3, 
			op1[2], 
			op2[2], 
			signo;
    char 	*resultado;

    // 3r. segmento
 	if(DEBUG) puts("Proceso4 pide D");
	pedirSemaforo(idMutSeg4);
  
  	if(DEBUG) puts("Proceso4 tiene D");
   shmid3 = shmget(clave3, 1024, IPC_CREAT | 0660);
   buffer3 = (char *)shmat(shmid3, NULL, 0);
   
   // 4to. segmento
   shmid4 = shmget(clave4, 1024, IPC_CREAT | 0660);
   resultado = (char *)shmat(shmid4, NULL, 0);
	res = atoi(resultado);
	
	// Se desglosa el mensaje guardado en el buffer
   strncpy(op1, buffer3, 2);
   nro1 = atoi(op1);

   strncpy(op2, buffer3 + 3, 2);
   nro2 = atoi(op2);
   signo = *(buffer3+2);
	
	// Realiza la operación
    switch(signo)
    {
    case '+':
        res = nro1 + nro2;
        break;
    case '-':
        res = nro1 - nro2;
        break;
    case '*':
        res = nro1 * nro2;
        break;
    case '/':
        res = nro1 / nro2;
        break;
    }
    
   printf(" = ");
   fflush(stdout);
	sprintf(resultado, "%f", res);
	
	// Desasocia las variables
  	shmdt(buffer3);
  	shmdt(resultado);
  
  	if(DEBUG) puts("Proceso4 devuelve D");
	
	devolverSemaforo(idMutSeg4);
	
	sleep(1);
	
	// Elimino área de memoria compartida
    shmctl(shmid3, IPC_RMID, 0);
    exit(EXIT_SUCCESS);
}

// Semaforos
int obtenerMutex(key_t clave)
{
    int IdSemaforo;
    union semun CtlSem;
    IdSemaforo = semget(clave, 1, IPC_CREAT | 0600);
    CtlSem.val = 1;
    semctl(IdSemaforo, 0, SETVAL, CtlSem);
    return IdSemaforo;
}

int obtenerSemaforo(key_t clave, int valor)
{
    int IdSemaforo;
    union semun CtlSem;
    IdSemaforo = semget(clave, 1, IPC_CREAT | 0600);
    CtlSem.val = valor;
    semctl(IdSemaforo, 0, SETVAL, CtlSem);
    return IdSemaforo;
}

void pedirSemaforo(int IdSemaforo)
{
    struct sembuf OpSem;
    //if(!DEBUG) printf("(%d Pide semaforo %d)\n", getpid(), IdSemaforo);
    OpSem.sem_num = 0;
    OpSem.sem_op = -1;
    OpSem.sem_flg = 0;
    semop(IdSemaforo, &OpSem, 1);
}

void devolverSemaforo(int IdSemaforo)
{
    struct sembuf OpSem;
    //if(!DEBUG) printf("(%d Devuelve semaforo %d)\n", getpid(), IdSemaforo);
    OpSem.sem_num = 0;
    OpSem.sem_op = 1;
    OpSem.sem_flg = 0;
    semop(IdSemaforo, &OpSem, 1);
}
void eliminarSemaforo(int IdSemaforo)
{
    semctl(IdSemaforo, 0, IPC_RMID);
}

void eliminarMutex(int IdSemaforo)
{
    semctl(IdSemaforo, 0, IPC_RMID);
}

// Señales
// Inicialización 
void  inicializarStrSignal( struct sigaction strSignal, 
							int sigNum, void (*ptrHandler)(), 
							int enmascara)
{
  /* Inicialización de estructuras de señales */
  strSignal.sa_sigaction = ptrHandler;

  // Bloqueamos todas las señales mientras se ejecuta la rutina.
  if(enmascara > 0)
    sigfillset(&strSignal.sa_mask);

  strSignal.sa_flags = SA_SIGINFO; // Es para que la estructura llegue instanciada a la rutina.
  sigaction(sigNum, &strSignal, NULL); // Establecemos la captura de la señal.
}
// Señal recibida por los hijos o el padre para finalizar el proceso
void handleSignal(int sig_num, siginfo_t *info, void *ni)
{
  int i;
	puts("Se recibio una señal...");
  // El proceso padre indica que el hijo debe terminar
  if(getpid() != PPID )
  {
		if(sig_num == SIGKILL)
		{
			puts("Abortando proceso...");
   		exit(EXIT_FAILURE);
		}
  }
  else // El proceso padre recibe la indicación de terminarse
  {
	
	// Enviar señal de fin a todos los procesos de la lista
	for(i = 0; i < PROCESOS; i++);
		kill(hijoPID[i], SIGKILL);
	
	 eliminarSemaforo(idMutSeg1);
   eliminarSemaforo(idMutSeg2);
   eliminarSemaforo(idMutSeg3);
   eliminarSemaforo(idMutSeg4);

	 exit(EXIT_FAILURE);	
  }
}
