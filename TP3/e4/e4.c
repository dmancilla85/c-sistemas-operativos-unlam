
/*
== e4.c ==
**************************************
	Ejercicio 4 ( c�digo )
**************************************
Fecha: 11/06/2014
Autor: David A. Mancilla
--------------------------------------
Descripci�n:
Se solicita confeccionar una soluci�n cliente/servidor en tcp (sockstream).
El servidor ser� un demonio que ir� logueando los clientes conectados,
tiempo de conexi�n, mensajes, etc.
Generando un archivo con el siguiente formato:
Cliente #, IP, Hora de conexi�n, Tiempo total conectado, Cantidad de mensajes
Este archivo deber� mantenerse actualizado.
El cliente podr� enviar mensajes al servidor (cadenas de texto); �ste los recibir�,
contabilizara, pero no har� nada m�s. Salvo en los siguientes casos:
- Si recibe TIME, debe devolverle al cliente el tiempo de conexi�n hasta ese mensaje.
- Si recibe MSJ, debe devolverle al cliente la cantidad de mensajes recibidos de �l.
- Si recibe INI, la hora de conexi�n.
- Si recibe DIP, la direcci�n IP.
- Si recibe LOG, el log completo.
Al recibir la respuesta el cliente deber� mostrarla por pantalla.
Al finalizar un cliente, el servidor deber� mostrar el log de dicho cliente.
Y al finalizar todas las conexiones, deber� mostrar el archivo completo por pantalla,
teniendo en cuenta que dicha salida deber� estar formateada, para una correcta legibilidad.
Para finalizar el servidor, deber� recibir una se�al a su elecci�n, la cual deber�
aparecer en la ayuda, junto a todos los par�metros de cada llamada y los distintos
mensajes que puede enviarse para obtener informaci�n.

******************/

#include "e4.h"

int  	conectados; 		// Contador de clientes conectados
char 	idProceso[8];	// Identificador de proceso
int	activo = 1; 
int 	nro = 0;	
pthread_t tid[MAXQ];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int main(int argc, char *argv[])
{
	char opc;
	char paramIp[15];

	if(argc > 1 && strcmp(argv[1], "-h") == 0)
	{
		puts(HELP);
		exit(EXIT_SUCCESS);
	}

	puts("Seleccione iniciar proceso como: ");
	puts("1  --> Cliente");
	puts("2  --> Servidor");
	puts("3  --> Salir");
	scanf ("%c", &opc);

	switch( opc)
	{
		case '1':
				  puts("Iniciando cliente...");
				  if(!DEBUG)
				  {
				  	if(argc == 1)
				  	{
				  		puts("Error: No se indico la IP por parámetro.\nDebe ingresar la dirección IP:" );
				  		scanf("%s", paramIp);
				  	}
				  	else
				  		strcpy(paramIp, argv[1]);
					runnerCliente(argc, paramIp);
				  }
				  else 
					runnerCliente(0, LOCALHOST);
				  break;
		case '2':
				  puts("Iniciando servidor...");
				  mainServidor(0, NULL);
		
				  break;
		default:	  puts("Saliendo del programa...");
	}

	return EXIT_SUCCESS;
}

int mainServidor(int argc, const char *argv)
{

	int 								listen_socket = 0, ret, 
											comm_socket = 0, c;
	FILE 								*log_servidor = NULL;  // Log del servidor
	unsigned short int 	listen_port = 0;
	unsigned long int  	listen_ip_address = 0;
	struct sockaddr_in 	listen_address, 
											con_address;
	char 						mystring[64];
	struct sigaction 	st_sgnAct;										
	socklen_t 					con_addr_len;
	pthread_t 				Timer;
	
	// Contador de conectados
	conectados = 0;
	/* Inicialización de estructuras de señales */
   inicializarStrSignal(st_sgnAct, SIGUSR1, handleSignal, 1);
	
	system("clear");
	puts("<< SERVIDOR INICIADO >>");
	
	if(DEBUG) puts("Inicializo archivo de log...");
	
	sprintf(idProceso, "%d", getpid());
	strcat(idProceso,ARCHIVO_LOG);
	
	log_servidor = fopen(idProceso, "w");
	fprintf(log_servidor, "#    IP\t\tFecha de conexi�n Tiempo conectado\tMensajes\n");
	fclose( log_servidor );
	log_servidor = NULL;
	
	// Hilo TIMER
	pthread_create(&Timer, NULL, timer, NULL);
	
	// Creación del socket
	listen_socket = prepararSocketOyente(&listen_address);
	printf("Enlace listo.");
	printf(" Esperando conexiones...\n");
	
	ret = listen(listen_socket, MAXQ);
	
	if(ret < 0)
	{
		perror("Error al enlazar");
		exit(EXIT_FAILURE);
	}

	bzero(&con_address, sizeof(con_address));
	c = sizeof(struct sockaddr_in);
	
	// Si transcurre un tiempo y no hay clientes conectados se desconectara
	while( (comm_socket = accept(listen_socket, (struct sockaddr *)(&con_address), (socklen_t*)&c) ) >= 0
				&& activo)
	{
		t_conn conexion;
		
		conexion.comm_socket = comm_socket;
		conexion.con_address = con_address;
		printf("Conexion recibida (%d)\n", comm_socket);
		// Genera un hilo para el intervalo predefinido
      pthread_create(&tid[conectados], NULL, runnerServidor, (void*) &conexion);
      
      p(&mutex);
      conectados++;
      v(&mutex);
      
      comm_socket = 0;
	}

	 if (comm_socket < 0 && activo)
	{	
		perror("Error. No se pudo aceptar la conexion");
		exit(EXIT_FAILURE);
	}

	puts("No hay clientes conectados y finalizo el tiempo de espera.");
	
	p(&mutex);
	log_servidor = fopen(idProceso, "r");
	
	printf("\n**********************************\nRegistro total de conexiones:\n**********************************\n");
	
	while( fgets(mystring , 64 , log_servidor) != NULL )
   	puts (mystring);
	
	fclose( log_servidor );
	log_servidor = NULL;
	v(&mutex);
	
	pthread_mutex_destroy(&mutex);
	
	return EXIT_SUCCESS;
}

int runnerCliente(int argc, const char *argv)
{
	int 				caller_socket = 0, 
						ipValida = 1, 
						ret;
	unsigned short int listen_port = 0;
	unsigned long int listen_ip_address = 0;
	struct sockaddr_in listen_address;
	char buffer[TAMBUF], buffer_in[TAMBUF], ip[15];

	(DEBUG)? strcpy(ip, LOCALHOST): strcpy(ip, argv);
	
	ipValida = inet_pton(AF_INET, ip, &listen_address); 

	if(ipValida < 0)
	{
		puts("Error - Dirección IP invalida.");
		puts(HELP);
		exit(EXIT_FAILURE);
	}

	// Creaci�n del socket
	if((caller_socket = configurarSocketLlamador(&listen_address)) < 0)
	{
		perror("No se pudo crear socket");
		exit(EXIT_FAILURE);
	}

	// Se conecta con el servidor
	puts("Conectando con el servidor...");
	ret = connect(caller_socket, (struct sockaddr*)&listen_address, sizeof(struct sockaddr));
	if(ret < 0)
	{
		puts("Fallo - No hay conexión con el servidor.");
		close(caller_socket);
		exit(EXIT_FAILURE);
	}
	else
		printf(" Conexion exitosa.\n");
	
	do
	{
		// Enviamos el mensaje
		printf("Ingrese un mensaje para el servidor ('fin' para salir): ");
		scanf("%s",buffer);
		
		//puts("Enviando mensaje...");
		if(strlen(buffer) > 0)
			send(caller_socket, buffer, TAMBUF, 0);

		bzero(buffer_in, TAMBUF);
		
		//puts("Recibiendo mensaje...");
		recv(caller_socket, buffer_in, TAMBUF, 0);
		
		if(strlen(buffer_in) > 0)
			printf("<<SERVIDOR>>: %s\n", /*inet_ntoa(con_address.sin_addr),*/ buffer_in);
		
	} while( strcmp(buffer, "fin") != 0);
	
	// Cerramos el socket como corresponde.
	puts("Finaliza la conexion con el servidor");
	close(caller_socket);

	exit(EXIT_SUCCESS);
}

void *runnerServidor(void *client)
{
	t_cliente	aux;
	int 		read_size, ret;	
	t_conn 		*conn;
	char 			fecha[20];
	char 		buffer[TAMBUF], 
				buffer_out[TAMBUF], 
				log_cte[MAXQ][100];
	FILE 		*log_servidor = NULL;  // Log del servidor
	
	conn = (t_conn*) client;

	// Llenar estructura t_cliente
	time(&aux.t_inicio);
	aux.cant_msj = 0;
	aux.estado 	 = ACTIVO;
	strcpy(aux.ip, inet_ntoa(conn->con_address.sin_addr));
	
	bzero(buffer, TAMBUF);

	if(DEBUG) puts("Hilo de servidor atendiendo cliente...");
	
	while( (read_size = recv(conn->comm_socket, buffer, TAMBUF, 0)) > 0 )
	{
		aux.cant_msj++;
		buffer[read_size] = '\0';
		strcpy(buffer_out,"");

		//printf("%s envio: %s\n", inet_ntoa(conn->con_address.sin_addr), buffer);
		
		// Tiempo de conexi�n
		if( strcasecmp(buffer, "TIME") == 0 )
			sprintf(buffer_out, "Tiempo total de conexi�n: %3.2f segundos",
					difftime(time(NULL), aux.t_inicio));

		// Cantidad de mensajes enviados
		if( strcasecmp(buffer, "MSJ") == 0 )
			sprintf(buffer_out, "Cantidad de mensajes enviados: %d", aux.cant_msj);

		// Hora de inicio de la conexi�n
		if( strcasecmp(buffer, "INI") == 0 )
			sprintf(buffer_out, "Hora de inicio de conexi�n: %s", ctime(&aux.t_inicio));

		// Direcci�n IP
		if( strcasecmp(buffer, "DIP") == 0 )
			sprintf(buffer_out, "Direcci�n IP: %s", aux.ip);
		
		write(conn->comm_socket, buffer_out, TAMBUF);
		
		memset(buffer, 0, TAMBUF);
	}
	
	close(conn->comm_socket);
	time(&aux.t_fin);
	strncpy(fecha, ctime(&aux.t_inicio), 20);
	fecha[20]='\0';
	
	// �rea cr�tica
	p(&mutex);
	sprintf(log_cte[conectados - 1], "%2d %15s %10s %3.1f s.  %4d\n", 
			++nro, aux.ip, fecha, difftime(aux.t_fin, aux.t_inicio), aux.cant_msj);
	
	log_servidor = fopen(idProceso, "a");
	fputs(log_cte[conectados - 1], log_servidor);
	fclose( log_servidor );
	log_servidor = NULL;
	conectados--;
	v(&mutex);
	
	puts("Cliente desconectado");
	puts(log_cte[conectados]);
	pthread_exit(EXIT_SUCCESS);
}


void *timer(void *n)
{
	time_t start;
	int    check = 1;
	
	if(DEBUG) puts("Timer iniciado");
	time(&start);
	
	while(1)
	{ 
		if(conectados == 0)
			check = 1;
		else
		{
			check = 0;
			time(&start);	
		}
		
		if(difftime(time(NULL), start) > ESPERA && check)
		{	
			puts("Se agoto el tiempo de espera de conexión");
			sleep(2);
			kill(getpid(), SIGUSR1);
			pthread_exit(EXIT_SUCCESS);
		}
		
		sleep(2);	
	}
	
	pthread_exit(EXIT_SUCCESS);
}

/* ------------------------------------- *
 * MANEJO DE SOCKETS					 		  *
 * ------------------------------------- */

int prepararSocketOyente(struct sockaddr_in *stListenAddr)
{
  int nroSocket = socket(AF_INET, SOCK_STREAM, 0);

  // Asignandole una dirección a éste
  bzero(stListenAddr, sizeof(*stListenAddr));

  // Configuración de listen_Address
  stListenAddr->sin_family       = AF_INET;
  stListenAddr->sin_port         = htons(PORT);// Un puerto cualquiera
  stListenAddr->sin_addr.s_addr  = INADDR_ANY; //htonl(INADDR_ANY); // Cualq. direcciòn

  // Enlazo el socket con la direcci�n
	if( bind(nroSocket, (struct sockaddr *)stListenAddr, sizeof(*stListenAddr)) < 0)
	{
			perror("El enlace falló. Error");
			exit(EXIT_FAILURE);
	}
  listen(nroSocket, MAXQ);

  return nroSocket;
}

int configurarSocketLlamador(struct sockaddr_in* listen_address)
{
  int caller_socket = socket(AF_INET, SOCK_STREAM, 0);
  listen_address->sin_family       = AF_INET;
  listen_address->sin_port         = htons(PORT);// Un puerto cualquiera
  listen_address->sin_addr.s_addr  = inet_addr(LOCALHOST); // Direccion
  bzero(&(listen_address->sin_zero), 8);

  return caller_socket;
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

// Señal recibida por los hijos o el padre para finalizar el proceso
void handleSignal(int sig_num, siginfo_t *info, void *ni)
{

   /* // El proceso padre indica que el hijo debe terminar
    if(getpid() != PPID )
    {
        printf("\n%d: (HIJO) Padre (%d) indica que debo terminar (SIG %d)",
               getpid(), info->si_pid, sig_num);

        // Le aviso al padre que termino el proceso
        exit(EXIT_SUCCESS);
    }*/
 	switch(sig_num)
 	{
 		case SIGUSR1:
 				//if(DEBUG) puts("recibio SIGUSR1");
 				puts("No se recibiran mas conexiones.");
 				activo = 0;   
 	}  
}
