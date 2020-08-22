
/*
== e4.h ==
**************************************
	Ejercicio 4 ( encabezado )
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
#ifndef _E4_H_
#define _E4_H_
#include <sys/types.h> // socket(), bind()
#include <sys/socket.h> // socket(), bind(), inet_addr()
#include <netinet/in.h> // inet_addr()
#include <arpa/inet.h> // inet_addr()
#include <string.h> // bzero()
#include <pthread.h>
#include <stdio.h>
#include <sys/wait.h>	// Comando WAIT
#include <ctype.h>		//
#include <unistd.h>		// Biblioteca UNIX
#include <signal.h> 	// Señales
#include <time.h>
#include <stdlib.h>

// Macros para semaforos
#define p(a) pthread_mutex_lock(a)
#define v(a) pthread_mutex_unlock(a)

#define ESPERA			60 // Tiempo de espera
#define DEBUG      	0
#define TAMBUF 		1024
#define MAXQ 		3
#define PORT 		8888
#define LOCALHOST 	"127.0.0.1" 
//#define SERVERHOST 	"192.168.0.100"
#define ACTIVO		1
#define INACTIVO 	0
#define ARCHIVO_LOG "SERVIDOR.log"
#define HELP  "<SISTEMA CLIENTE-SERVIDOR>\n\nDescripci�n:\n\nEl servidor genera un archivo con el siguiente formato:\nCliente #, IP, Hora de conexion, Tiempo total conectado, Cantidad de mensajes\nEl cliente podra enviar mensajes al servidor (cadenas de texto); este los recibira,\ncontabilizara, pero no hara nada mas. Salvo en los siguientes casos:\n- Si recibe TIME, debe devolverle al cliente el tiempo de conexion hasta ese mensaje.\n- Si recibe MSJ, debe devolverle al cliente la cantidad de mensajes recibidos de el.\n- Si recibe INI, la hora de conexion.\n- Si recibe DIP, la direccion IP.\n- Si recibe LOG, el log completo.Seleccione si desea ejecutar el cliente o el servidor.\n\nSintaxis: \nprompt$/e4 [ip]\n\n Donde [ip] --> Direcci�n IP, si se ejecuta como Cliente.\n Para terminar la ejecución del servidor puede enviar la señal SIGUSR2. De todas formas,\nsi el servidor registra un periodo de inactividad (sin conexiones) por 1 minuto terminará automaticamente."

typedef struct s_conn
{
	int			comm_socket;
	struct sockaddr_in con_address;
} t_conn;

typedef struct s_client
{
	time_t 	t_inicio;
	time_t 	t_fin;
	int 	cant_msj;
	int		estado;
	int		id;
	char 	ip[15];
} t_cliente;

int runnerCliente(int, const char*);
int mainServidor(int,const char*);
void *runnerServidor(void *client);
void *timer(void*);

//	   Redes
int   prepararSocketOyente(struct sockaddr_in*);
int   configurarSocketLlamador(struct sockaddr_in*);

// 	Handle de SEÑALES
void  inicializarStrSignal(struct sigaction, int, void (*)(), int);
void  handleSignal(int, siginfo_t*, void*);
#endif
