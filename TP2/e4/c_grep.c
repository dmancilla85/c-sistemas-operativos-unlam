/* 
--------------------------------------------------------------------
 Ejercicio 4
--------------------------------------------------------------------
Se quiere simular el funcionamiento del comando grep, por lo que se 
pide realizar un proceso que reciba como parámetros el patrón a 
buscar, un filtro para los archivos que tiene que evaluar y un flag 
que indique si la búsqueda debe incluir subdirectorios. Ej.:
$ c_grep /patron/ *.txt
$ c_grep /patron/ ./*.dat [-r]
El proceso principal debe generar un proceso por cada archivo a 
evaluar y estos 
procesos deben generar un archivo donde se indique el archivo leído, 
el número de línea en que se encontró coincidencia y el contenido de 
la misma, para corroborar que sea correcto. Si no se encuentran 
coincidencias no se deben generar los archivos.
El nombre de los archivos generados debe ser igual al pid del proceso 
que los genera.

$ c_grep TP *.txt
diccionario.txt
línea 15: Sistemas Operativos TP 2.
--------------------------------------------------------------------
*/

#include "c_grep.h"

int main (int argc, char **argv) 
{
	pid_t 	id_padre = getpid (),	// PID del proceso padre
			id_hijo;				// PID del proceso hijo
	int   	estado, 
			i = 2;					// Estado de salida
	struct 	stat buf;
	char 	subdir = 'N';

	if(DEBUG) printf ("Inicio proceso padre. PID=%d\n",id_padre);

   // Validación de parámetros mínimos
	if (argc < 3 && !DEBUG)
	{
		fprintf(stderr, "Error: Faltan parámetros.\n");
		puts(HELP);
		return EXIT_FAILURE;
	}

	// Verificación tipo de archivo
	if (( strstr(argv[2], ".") == NULL || strstr(argv[2], ".o") != NULL 
		 || strstr(argv[2], ".lib") != NULL) && !DEBUG)
	{
		fprintf(stderr, "Error: Debe indicar un archivo de texto.\n");
		puts(HELP);
		return EXIT_FAILURE;
	}

	// Verificación parámetro correcto de directorios
	if ( strstr(argv[argc - 1], "-") != 0 && !DEBUG)
	{		
		if( strcmp(argv[argc - 1], "-r") != 0 )
		{
			fprintf(stderr, "Error: Argumentos inválido.\n");
			puts(HELP);
			return EXIT_FAILURE;
		}
		else	
			// Incluye subdirectorios
			subdir = 'S';
	}
	
	// Validación de archivos ingresados
	while(i < argc && strcmp(argv[i], "-r") != 0)
	{			
		// Verifico existencia del archivo
		if( stat( argv[i], &buf ) == -1 && !DEBUG)
		{
			fprintf( stderr, "No se puede leer el archivo %s.\nError: %s\n", argv[2], strerror(errno) );
			puts(HELP);
			return EXIT_FAILURE;
		} // es archivo valido
		else
		{	
			// Genero procesos hijos
			if ((id_hijo = fork ()) == 0)
			{
				// Proceso hijo
				if(DEBUG) printf ("Inicio proceso hijo. PID=%d, PPID=%d\n", getpid (), id_padre);
				scanDirectorio(".", argv[i], argv[1], subdir);
				if(DEBUG) printf ("Salida proceso hijo. PID=%d\n", getpid ());
				exit (getpid () > id_padre); // 1, si PID > PPID
			}
			else 
			{
				// Ignorar CTRL-C
				signal (SIGINT, SIG_IGN); 

				// Esperar procesos hijos
				while (waitpid (id_hijo, &estado, 0) != id_hijo);
					
				if (WIFSIGNALED (estado) && DEBUG)
					printf ("El proceso hijo ha recibido la señal %d\n", WTERMSIG (estado));
					
				if (WIFEXITED (estado) && DEBUG)
				{
					printf ("Estado de salida del proceso hijo: %d\n", WEXITSTATUS (estado));
					
					if (WEXITSTATUS (estado) == 1)
						printf ("PID hijo > PID padre.\n");
					else
						printf ("PID padre > PID hijo.\n");
				}
			}
		}
		i++;
	}

	if(DEBUG) printf("Fin del proceso %d\n", getpid ());
	return EXIT_SUCCESS;
}

void buscarEnArchivo(const char *dir, const char *fname, const char *str) 
{
	FILE 	*fp;
	int 	line_num = 1;
	int 	find_result = 0;
	char 	temp[50], 
			path[100];
	
	// Ensambla string "path/archivo"
	strcpy(path, dir);
	strcat(path,"/");
	strcat(path, fname);
	
	// Abrir archivo
	if((fp = fopen(path, "r")) == NULL)
		printf("\nError. No se pudo leer el archivo indicado: %s\n", fname);
	
	// Recorre el archivo para buscar coincidencias
	while(fp!=NULL && fgets(temp, sizeof(temp), fp)!= NULL) 
	{
		if((strstr(temp, str)) != NULL) 
		{
			if( strcmp(dir, ".") == 0 )
				printf("%s:%s\n", fname, temp);
			else
				printf("%s/%s:%s\n", dir, fname, temp);
		}
		line_num++;
	}
	
	fclose(fp);
	fp = NULL;
}

void scanDirectorio(const char *dire, const char *fileName, const char *searchTxt, char subdir) 
{
	struct dirent	**namelist;
	char 			*buffer = NULL, 
					fileSearch[50];
	int 			n, 
					pos;
	
	// Obtener la lista en la carpeta actual de archivos y subdirectorios
	if( (n = scandir(dire, &namelist, NULL, alphasort)) < 0)
	{
		perror("No se pudo leer el directorio indicado.");
		return;
	}  
	else 
	{
		while (n--) 
		{		  		
			if(DEBUG) printf("\n%d %s/%s:%s",getpid(),dire,namelist[n]->d_name, searchTxt);
			
			// Si es un archivo busca el patrón en su contenido
			if(namelist[n]->d_type == 8 && strcmp(namelist[n]->d_name, fileName) == 0 )
			{
				buscarEnArchivo(dire, namelist[n]->d_name, searchTxt);
			}
			
			// Si es un directorio llama a la función de forma recursiva
			if(namelist[n]->d_type == 4)// es un directorio
			{ 
				if(strcmp(namelist[n]->d_name, ".") != 0 && strcmp(namelist[n]->d_name, "..") != 0 
					&& subdir == 'S')
					scanDirectorio(namelist[n]->d_name, fileName, searchTxt, subdir);
			}
			// Libera la estructura
			free(namelist[n]);
		}
		// Libera la estructura completa
		free(namelist);
	}
}

/**************************************/
