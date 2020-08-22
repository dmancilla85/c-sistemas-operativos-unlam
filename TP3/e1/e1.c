
#include "e1.h"

pthread_t 		tid[PROCESOS];
pthread_attr_t 	attr[PROCESOS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
t_stock 		stock;
int  PPID;

int main(int argc, char *argv[])
{
  int i;
  char mesas[7];
  struct sigaction 	st_sgnChld;
  struct sigaction 	st_sgnAct;
  PPID = getpid ();
  
  /* Inicialización de estructuras de señales */
  inicializarStrSignal(st_sgnAct, SIGUSR1, handleSignal, 1);
  inicializarStrSignal(st_sgnChld, SIGCHLD, hijo_finalizado, 0);

  // Inicializo contador de stock
  stock.cajones = 0;
  stock.marcos = 0;
  stock.vidrio = 0;
  stock.patas = 0;
  stock.finales = 0;
	
   // Validación de parámetros
  if (argc != 2 && !DEBUG)
  {
    fprintf(stderr, "Error: Faltan parámetros.\n");
    puts(HELP);
    return EXIT_FAILURE;
  }

  if ((atoi(argv[1]) <= 0 || atoi(argv[1]) >= MAX_MESAS) && !DEBUG)
  {
    fprintf(stderr, "Error: Argumento debe ser un número válido y menor a 10000000.\n");
    puts(HELP);
    return EXIT_FAILURE;
  }

  // Cargo string con parámetros para los hilos 
  if(!DEBUG)
    sprintf(mesas, "%s", argv[1]);
  else
    sprintf(mesas, DEFAULT_PROD);

  // Inicializar los atributos de los subprocesos
  pthread_attr_init(&attr[MARCOS]);
  pthread_attr_init(&attr[FINAL]);
  pthread_attr_init(&attr[CAJONES]);
  pthread_attr_init(&attr[VIDRIO]);
  pthread_attr_init(&attr[PATAS]);

  // Inicio subprocesos
  pthread_create(&tid[MARCOS], &attr[MARCOS], fabricaMarcos, mesas);
  pthread_create(&tid[PATAS], &attr[PATAS], fabricaPatas, mesas);
  pthread_create(&tid[VIDRIO], &attr[VIDRIO], ensamblaVidrio, mesas);
  pthread_create(&tid[CAJONES], &attr[CAJONES], ensamblaCajones, mesas);
  pthread_create(&tid[FINAL], &attr[FINAL], finalizaMesa, mesas);

  // Espero subprocesos
  for(i = 0; i < FINAL + 1; i++)
    pthread_join(tid[i], NULL);

  pthread_mutex_destroy(&mutex);
  puts("Proceso terminado.");
  return EXIT_SUCCESS;
}


/* *************** */
/*  SUB-PROCESOS   */
/* *************** */

void *fabricaMarcos(void *param)
{
  void *ret = NULL;
  int mesasPedidas = atoi(param);

  while(1)
  { // Finaliza el proceso si se completa el pedido de las mesas
	// o el stock de marcos es suficiente.
    if( stock.finales >= mesasPedidas
        || (mesasPedidas - stock.finales) < stock.marcos)
      pthread_exit(EXIT_SUCCESS);

	// Fabricación de marcos
    while(stock.marcos < MAX_STOCK_MARCOS)
    {
      p(&mutex);
      
	  printf("Fabricación del marco %d\n", stock.marcos);
      stock.marcos++;
      
	  v(&mutex);
      printf("Total de marcos en stock: %d\n", stock.marcos);
      sleep(1);
    }

  }

  return ret;
}

void *fabricaPatas(void *param)
{
  void *ret = NULL;
  int mesasPedidas = atoi(param);

  while(1)
  {	// Finaliza el proceso si se completa el pedido de las mesas, 
	// o el stock de patas es suficiente.
    if( stock.finales == mesasPedidas
        || (mesasPedidas - stock.finales) * 4 < stock.patas)
      pthread_exit(EXIT_SUCCESS);
	
	// Fabricación de patas
    while(stock.patas < MAX_STOCK_PATAS)
    {
      p(&mutex);
	  
      printf("Fabricación de la pata %d\n", stock.patas);
      stock.patas++;
      printf("Total de patas en stock: %d\n", stock.patas);
      
	  v(&mutex);
      sleep(1);
    }
  }

  return ret;
}

void *ensamblaVidrio(void *param )
{
  void *ret = NULL;
  int mesasPedidas = atoi(param);

  while(1)
  {
	// Finaliza el proceso si se completa el pedido de las mesas, 
	// o el stock de marcos vidriados es suficiente.
    if( stock.finales == mesasPedidas
        || (mesasPedidas - stock.finales) < stock.vidrio)
      pthread_exit(EXIT_SUCCESS);

    p(&mutex);
	
	// Verifica existencia de materias primas
    if( stock.patas < 4 )
      puts("No hay suficiente stock de patas para continuar.");

    if( stock.marcos < 1 )
      puts("No hay suficiente stock de marcos para continuar.");

    if( stock.vidrio >= 2 )
      puts("No hay suficiente espacio para colocar marcos vidriados.");

    if( stock.patas < 4
        || stock.marcos < 1
        || stock.vidrio >= 2 )
      puts("Operacion cancelada.");

    else
    { // Producción de marcos vidriados
      stock.patas -= 4;
      stock.marcos -= 1;
      stock.vidrio += 1;

      printf("Terminación de ensamblaje de vidrio y patas de la mesa número %d\n", stock.finales + 1);
      printf("Total de marcos en stock: %d\n", stock.marcos);
      printf("Total de patas en stock: %d\n", stock.patas);
    }
    v(&mutex);
    sleep(2);
  }

  return ret;
}

void *ensamblaCajones(void *param)
{
  void *ret = NULL;
  int mesasPedidas = atoi(param);

  while(1)
  {
	// Finaliza el proceso si se completa el pedido de las mesas, 
	// o el stock de mesas con cajones es suficiente para completar el pedido.
  if( stock.finales == mesasPedidas
      || (mesasPedidas - stock.finales) < stock.cajones)
    pthread_exit(EXIT_SUCCESS);

  p(&mutex);
  
  // Verifica existencia de materias primas
  if(stock.vidrio < 1)
    puts("No hay ningún marco vidriado para ensamblar");
  else
  { // Producción de mesas encajonadas 
    stock.vidrio -= 1;
    stock.cajones += 1;
    printf("Terminación de ensamblaje de los cajones de la mesa número %d\n", stock.finales + 1);
  }

  v(&mutex);
  sleep(2);

  }
  return ret;
}

void *finalizaMesa(void *param)
{
  void *ret = NULL;
  int mesasPedidas = atoi(param);

  while(1)
  {
  // Finaliza el proceso si se completó el pedido de las mesas. 
  if( stock.finales == mesasPedidas)
    pthread_exit(EXIT_SUCCESS);

  p(&mutex);
  
  // Verifica existencia de materias primas
  if( stock.cajones < 1 )
    puts("No hay ninguna mesa para finalizar.");
  else
  { // Finalización de mesas 
    stock.cajones -= 1;
    stock.finales += 1;
    printf("Terminación final de la mesa número %d\n", stock.finales);
  }
  v(&mutex);
  sleep(9);
  }

  return ret;
}

/* ------------------------------------- *
 * MANEJO DE SEÑALES					 *
 * ------------------------------------- */
 
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

// Handle para hijos finalizados
void hijo_finalizado(int sig_num, siginfo_t *info, void *ni)
{
  /* Cuando estamos aquí sabemos que hay un proceso zombie esperando */
  int childStatus;

  printf("%d: Se recibio SIGCHLD. El proceso que envió la señal fue: %d.", getpid(), info->si_pid);

  switch (info->si_code)
  {
    case CLD_EXITED:
                    printf("%d: Terminó normalmente\n", getpid());
                    break;
    case CLD_KILLED:
                    printf("%d: Fue terminado\n", getpid());
                    break;
    case CLD_DUMPED:
                    printf("%d: Terminó anormalmente\n", getpid());
      
                    break;
    case CLD_STOPPED:
                    printf("%d: Fue parado\n", getpid());
                    break;
    case CLD_CONTINUED:
                    printf("%d: Estaba parado y ahora continua\n", getpid());
                    break;
  }
}

// Señal recibida por los hijos o el padre para finalizar el proceso
void handleSignal(int sig_num, siginfo_t *info, void *ni)
{
  int i;
  long spid;

  // El proceso padre indica que el hijo debe terminar
  if(getpid() != PPID )
  {
    printf("%d: (HIJO) Padre (%d) indica que debo terminar (SIG %d)\n",
           getpid(), info->si_pid, sig_num);
  }
  else // El proceso padre recibe la indicación de terminarse
  {
	// Enviar señal de fin a todos los procesos de la lista
	for(i = 0; i < FINAL + 1; i++);
		pthread_cancel(tid[i]);
	
	pthread_mutex_destroy(&mutex);
	
  }
}

// Matar procesos
void killProc(const int pid)
{ kill(pid, SIGKILL); }
