/* 
--------------------------------------------------------------------
 Ejercicio 5
--------------------------------------------------------------------
Una empresa tiene un servidor donde se generan en un único directorio 
miles de archivos de log y salidas de sus procesos. No están 
parametrizados, así que para modificarlos y que cada proceso guarde 
los archivos en un directorio separado deber recompilar cada programa 
y es muy riesgoso. Como se hace muy complicada la administración de 
este directorio por la gran cantidad de archivos, necesitan generar 
una herramienta que le permita al administrador trabajar con los 
archivos pero paralelizando la ejecución de los comandos, y sin que 
esto afecte la performance del server. Para esto, ustedes deben 
generar un proceso que tome la lista de archivos de un directorio, 
y ejecute el comando que se le indique pasándole de a N archivos como 
parámetro de entrada al comando a ejecutar; que se deberá ejecutar 
varias veces concurrentemente. Se le debe poder indicar la cantidad 
máxima (-c) de archivos que le debe pasar al comando, y cuántos 
procesos (-p) como máximo pueden estar ejecutando a la vez.
Por ejemplo, si tengo un directorio con 100 archivos, y ejecuto así:
$ concurrent -c 20 -p 2 rm -f
Se ejecutarían 5 procesos en total (secuencialmente), cada uno 
borrando 10 archivos, pero sólo puede ejecutar de a 2 “rm -f” a la vez.
--------------------------------------------------------------------
*/

#include "tp2_e5.h"

int main(int argc, char *argv[])
{
	///PARAMETRO DE ENTRADA
	char 	directorio[256] = "../ELI/";
	char 	comando[256] 	= "sort";
	///VARIABLES A USAR
	char 	vector_archivos[FIL][COL];
	int 	cantidad_archivos = 17,
			max_archs_por_comando = -1,
			cantidad_procesos = -1,
			archivos_proceso,
			i, 
			nro_hijo, 
			recorrer_hasta, 
			paso = 0,
			option = 0;
			pid_t hijo_pid;
	DIR 	*dir_dest = NULL;		

	// Verifica si se solicita ayuda
	if(argc == 2 && !DEBUG)
	{
		if(strcmp(argv[1], "-h") == 0)
		{
			mostrar_ayuda();
			return EXIT_FAILURE;
		}
	}

	// Tiene todos los parámetros requeridos?
	if(argc != 7 && !DEBUG)
	{
		printf("No se ingresaron todos los parametros obligatorios\n");
		sintaxis();
		return EXIT_FAILURE;
	}
	
	// Determino las variables ingresadas
	while ((option = getopt(argc, argv, "p:c:")) != -1) 
	{
		switch (tolower(option)) 
		{
			case 'p': 
				cantidad_procesos = atoi(optarg);
				break;
			case 'c': 
				max_archs_por_comando = atoi(optarg);
				break;
			default: 
				printf("Opciones incorrectas\n");
				exit(EXIT_FAILURE);
		}
	}

	/** Verifica el directorio */
	if ((dir_dest = opendir ( argv[6] )) == NULL && !DEBUG)
		{
			perror("- Directorio no válido.");
			sintaxis();
			closedir(dir_dest);
			return EXIT_FAILURE;
		}
	
	closedir(dir_dest);
	
	strcpy(directorio,argv[6]);
	/** Obtengo el comando */
	strcpy(comando,argv[5]);

	if( 	strcmp(comando,"grep") != 0 && 
			strcmp(comando,"sort") != 0 && 
			strcmp(comando,"cat") != 0  && 
			strcmp(comando,"cut") != 0  && 
			strcmp(comando,"uniq") != 0  && 
			strcmp(comando,"rm") != 0  && 
			strcmp(comando,"ls") != 0  && 
			strcmp(comando,"mv") != 0  && 
			strcmp(comando,"cp") != 0 &&
			strcmp(comando,"more")!=0 && 
			strcmp(comando,"less") != 0 && !DEBUG)
	{
	printf("El comando %s no es encontrado o no esta bien utilizado\n", comando);
	return EXIT_SUCCESS;
	}

	/** Valido que cantidad de procesos sea mayor a cero*/
	if(cantidad_procesos < 0 || max_archs_por_comando < 0)
	{
	printf("Opciones incorrectas\n");
	exit(EXIT_FAILURE);
	}

	///DECLARO SENIALES
	signal(SIGCHLD, esperar_hijo);

	///PROGRAMA
	/*Guardo lista de archivos e un vector.*/
	cantidad_archivos = buscar_archivos_directorio(directorio, vector_archivos);

	/*printf( "\n***********************\nESTADISTICAS\n***********************\nCant. Procesos:\t%5d\nArch. xComando:\t%5d\nCant. Archivos:\t%5d\n***********************\n", cantidad_procesos, max_archs_por_comando, cantidad_archivos);*/

	if(cantidad_archivos != 0)
	{	
	archivos_proceso = cantidad_archivos / cantidad_procesos;

	for(i = 0; i < cantidad_procesos; i++)
	{ //Creo los procesos
	if((hijo_pid = fork()) == 0)
	{
	nro_hijo = i;
	break; //Si es hijo corto ciclo for.
	}
	}

	if(hijo_pid == 0)
	{ //Si es hijo, entonces recibo la archivos a trabajar por proceso.
	if((nro_hijo + 1) != cantidad_procesos)
	recorrer_hasta = archivos_proceso;
	else
	recorrer_hasta = archivos_proceso + cantidad_archivos % cantidad_procesos;

	for(i=0; i<recorrer_hasta; i+=max_archs_por_comando)
	{	
	crear_proceso_comando(	comando, vector_archivos, i, 
						paso, max_archs_por_comando, recorrer_hasta,
						nro_hijo, archivos_proceso, directorio);
	}
	}

	if(hijo_pid != 0)
	{//Si es proceso padre entonces esperara por la terminacion de sus hijos.	
	for(i = 0; i < cantidad_procesos; i++)
	wait(NULL);
	}
	}
	else
	printf("El directorio %s no presenta archivos\n",directorio);
	return EXIT_SUCCESS;
}

void esperar_hijo(int iNumSen) 
{
	while (waitpid (-1, NULL, WNOHANG) > 0);
}

int crear_proceso_comando( char* comando, char vector_archivos[][COL],int i, int paso,
						  int max_archs_por_comando, int recorrer_hasta, int nro_hijo, 
						  int archivos_proceso, char* directorio )
{			
	FILE 	*pipein_fp;
	int 	j, cant=0;
	char 	readbuf[80];
	char 	comando_ejecutar[2000]=" ";
	char 	archivo[100];
	
	if( directorio[0] == '.' )
		directorio++;
		
	if( directorio[0] == '/' )
		directorio++;
	
	if( directorio[strlen(directorio) - 1] != '/')
	 strcat(directorio, "/"); 

	for(j = i; j < i+ max_archs_por_comando && j < recorrer_hasta; j++)
	{
		strcat(comando_ejecutar, comando);
		strcat(comando_ejecutar, " ");
		strcat(comando_ejecutar, directorio);
			
		strcat(comando_ejecutar, vector_archivos[j + nro_hijo * archivos_proceso]);
		
		strcat(comando_ejecutar, " | ");
		cant++;
	}
	
	if( strcmp(comando_ejecutar + (strlen(comando_ejecutar) - 3)," | ") == 0)
		comando_ejecutar[(strlen(comando_ejecutar) - 3)] = '\0';
	
	/* Crea una tuberia de un sentido llamando a popen() */
	if(DEBUG) printf("Comando: %s\n", comando_ejecutar);
	
	if (( pipein_fp = popen(comando_ejecutar, "r")) == NULL)
	{
		perror("popen");
		exit(EXIT_SUCCESS);
	}
	
	while(fgets(readbuf, 80, pipein_fp))
		printf("%s",readbuf);
		
	/* Cierre de las tuberias */
	pclose(pipein_fp);
	
	return 0;
}

int buscar_archivos_directorio(const char* directorio, char vector_archivos[][COL])
{
	int 	cantidad_archivos;
	DIR 	*dir;		// Puntero a directorio a recorrer.
	struct 	dirent *entrada;		// Puntero a estructura que contiene 
											// informacion sobre una entrada del directorio.
	struct 	stat datosFichero;
	char aux[256];
	
	/* busca el posible acceso al directorio */
	if ((dir = opendir(directorio)) == NULL) 
	{
		//(void) fprintf(stderr, "No existe el directorio <%s>\n", directorio); 
		printf("No existe el directorio <%s>\n", directorio);
		exit(EXIT_FAILURE);
	}
	
	cantidad_archivos=0;
	
	/* lectura de las entradas del directorio */ 
	while ((entrada=readdir(dir)) != NULL)
	{	
		strcpy(aux, "");
		strcat(aux, directorio);
		
		if( directorio[strlen(directorio) - 1] != '/')
			strcat(aux, "/"); 
		strcat(aux, entrada->d_name);
		if(DEBUG) printf("Analizando %s...\n", aux);
		
		if (stat (aux, &datosFichero) == -1)
			printf ("%s no existe\n", aux);
		else
			if (!S_ISDIR(datosFichero.st_mode))
			{
				strcpy(vector_archivos[cantidad_archivos++], entrada->d_name);
			}
	}
	
	closedir(dir);
	
	return cantidad_archivos;
}

void mostrar_ayuda()
{
	printf("<< MODO DE USO DE EJERCICIO 5 - TP 2 >>\n\n"
	"Sintaxis:\n\t./e5 -p numberP -c numberC Comand -f path\n"
	"OPCIONES:\n"
	"\t-p numberP\tCambiar 'numberP' cantidad de procesos maximos que ejecutaran el comando.\n"
	"\t-c numberC\tCambiar 'numberC' cantidad de archivos maximos que ejecutaran el comando.\n"
	"\t-f path\tCambiar 'path' por el directorio donde se encontran los archivos a tratar.\n"
	"\tComand\tReemplazar 'Comand' con el comando que se quiere usar para administrar los archivos."
	"\nEjemplo:\n\t./tp2_e5 -p 3 -c 3 rm ./ELI/\n");
}

void sintaxis()
{
	printf( "Ejemplo de uso:\n\t./tp2_e5 -p 3 -c 3 rm ./TEST/\nAyuda:\n\t./tp2_e5 -h\n");
}
