/*
    Dada una carpeta indicada por parámetro, se desea generar un hash del contenido de la misma.
    El algoritmo de hash a utilizar será simplemente una sumatoria de todos
    los bytes de los archivos.
    Para realizar esta tarea de manera eficiente, el programa a desarrollar disparará un
    hijo por cada archivo dentro de la carpeta, encargado de calcular el hash.
    Una vez calculado, guardará el resultado en un archivo cuyo nombre será el
    pid del proceso que lo calculó, y luego terminará.
    El proceso padre, a medida que vaya terminando cada uno de los hijos deberá informar por pantalla
	la sumatoria parcial de los hashes y una vez finalizados todos, generar un archivo de salida
	conteniendo el hash total. En caso de recibir una señal que interrumpa al padre, éste deberá
	manejar la señal informando a todos los hijos mediante una señal a elección que deben finalizar
	su ejecución, y esperar a que terminen todos antes de salir.
-----------------
verificar todo
 */

#include "tp2_e3.h"

pid_t 	hijos[256];
pid_t 	PPID;
int 	cantHijos;
int 	hashTotal = 0;

int main(int argc, char **argv)
{
    struct sigaction 	st_sgnChld;
    struct sigaction 	st_sgnAct;
    FILE 				*total = NULL;
    char 				spid[50];
    PPID = getpid();
    cantHijos = 0;

    /* Inicialización de estructuras de señales */
    inicializarStrSignal(st_sgnAct, SIGUSR1, handleSignal, 1);
    inicializarStrSignal(st_sgnChld, SIGCHLD, hijo_finalizado, 1);

    if(DEBUG) puts("Valida parametros");
    if(!DEBUG) validaParametros(argc, argv[1]);

    if(DEBUG)puts("Inicio proceso de análisis");

    !DEBUG ? scanDirectorio( argv[1]) : scanDirectorio(DIRE);

    if(getpid() != PPID) exit(0);

    // Esperar a que terminen los hijos
    while(cantHijos-- > 0)
    {
        wait(NULL);
    }

    printf("\nResultado total: %d\n\n", hashTotal);

    sprintf(spid, "TOTAL_%d%s", getpid(), EXT);

    // Crea un archivo(1) con el nombre del proceso actual
    if( (total = fopen( spid, "w+") ) == NULL )
    {
        printf("\nNo se pudo generar el registro final del proceso: %s", spid);
        exit(EXIT_FAILURE);
    }

    fprintf(total, "%d", hashTotal);
    fclose(total);
    total = NULL;

    return EXIT_SUCCESS;
}

int hashArchivo(int length)
{
    int ret 		= 0;
    char str_pid [sizeof(long)*8+1];
    FILE *hashFile 	= NULL;

    sprintf(str_pid, "%d", getpid());
    strcat(str_pid, EXT);

    // Crea un archivo(1) con el nombre del proceso actual
    if( (hashFile = fopen( str_pid, "w+") ) == NULL )
    {
        perror("No se pudo generar el registro del hash de archivo");
        exit(EXIT_FAILURE);
    }

    // Grabo el hash en el archivo(1)
    ret = fprintf(hashFile, "%d", length);

    if(ret <= 0)
        printf("\n%d: Error-No se pudo calcular el hash para el archivo\n", getpid());

    fclose(hashFile);
    hashFile = NULL;

    return ret;
}

int scanDirectorio(const char *dir)
{
    FILE            *fp = NULL;
    DIR             *dir_dest = NULL;
    char            aux[100];
    struct dirent	**namelist;
    pid_t 			spid,
                    n;			// Hash
    int             sz;

    // Obtener la lista en la carpeta actual de archivos y subdirectorios
    if( (n = scandir(dir, &namelist, NULL, alphasort)) < 0)
    {
        perror("No se pudo leer el directorio indicado.");
        exit(EXIT_FAILURE);
    }
    else
    {
        while (n--)
        {
            // Si es un archivo genera el hash de su contenido

            if(namelist[n]->d_type == 8 && strstr(namelist[n]->d_name, EXT) == 0)
            {
                // Genera el hash
                if( (spid = fork()) == 0 )
                {
                    //if(DEBUG) printf("\nSe genero el proceso %d.. padre %d", getpid(), getppid());

                    if(getpid()!= PPID)
                    {
                        strcpy(aux,dir);
                        strcat(aux,"/");
                        strcat(aux, namelist[n]->d_name);
                    }


                    if( (fp = fopen( aux, "r") ) == NULL )
                    {
                        printf("\n%d Error -scanDirectorio: No se pudo abrir el archivo %s", getpid(),aux);
                        exit(EXIT_FAILURE);
                    }

                    fseek(fp, 0L, SEEK_END);
                    sz = ftell(fp);
                    fclose(fp);
                    fp = NULL;

                    printf("\n%d: %s/%s (%d bytes)\n",getpid(),dir, namelist[n]->d_name, sz);

                    hashArchivo(sz);
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    hijos[cantHijos++] = spid;
                    //if(DEBUG) printf("\n%d tiene ahora %d hijos",getpid(), cantHijos);
                }
            }

            // Si es un directorio llama a la función de forma recursiva
            if(namelist[n]->d_type == 4 && strcmp(namelist[n]->d_name, "..") != 0
                && strcmp(namelist[n]->d_name, ".") != 0 )// es un directorio
            {
                strcpy(aux,"");

                strcat(aux,dir);
                strcat(aux,"/");
                strcat(aux, namelist[n]->d_name);

                if ((dir_dest = opendir ( aux )) == NULL)
                    closedir(dir_dest);
                else
                {
                    if(strstr(aux, "../") == 0)
                    {
                        closedir(dir_dest);
                        scanDirectorio(aux);
                    }
                }
            }
            // Libera la estructura
            free(namelist[n]);
        }
        // Libera la estructura completa
        free(namelist);
    }

    return 1;
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
    int 		hashNum = 0;
    int         childStatus;
    char 		strPid[10];
    FILE   	*hashFile = NULL;

    if(DEBUG)
    {
        printf("\nSe recibio SIGCHLD. El proceso que envió la señal fue: %d.", info->si_pid);

        switch (info->si_code)
        {
        case CLD_EXITED:
            printf(" Terminó normalmente");
            break;
        case CLD_KILLED:
            printf(" Fue terminado");
            break;
        case CLD_DUMPED:
            printf(" Terminó anormalmente");

            break;
        case CLD_STOPPED:
            printf(" Fue parado");
            break;
        case CLD_CONTINUED:
            printf(" Estaba parado y ahora continua");
            break;
        }
    }
    // Aqui deberia obtener el numero a sumar y agregarlo al hash
    sprintf(strPid, "%d%s", info->si_pid, EXT);

    // Abre el archivo del ultimo proceso terminado
    if( (hashFile = fopen(strPid, "r") ) == NULL )
    {
        printf("\nNo se encuentra el registro del proceso: %s", strPid);
        //exit(EXIT_FAILURE);
    }
    else
    // Obtengo el número de hash
    {
        fscanf(hashFile, "%d", &hashNum);
        hashTotal += hashNum;
        fclose(hashFile);
    }
	//sleep(1);
    printf("\n(%d): Sumatoria parcial %d...",getpid(), hashTotal);
    hashFile = NULL;

    waitpid (-1, &childStatus, WNOHANG);
}

// Señal recibida por los hijos o el padre para finalizar el proceso
void handleSignal(int sig_num, siginfo_t *info, void *ni)
{

    // El proceso padre indica que el hijo debe terminar
    if(getpid() != PPID )
    {
        printf("\n%d: (HIJO) Padre (%d) indica que debo terminar (SIG %d)",
               getpid(), info->si_pid, sig_num);

        // Le aviso al padre que termino el proceso
        exit(EXIT_SUCCESS);
    }
    else // El proceso padre recibe la indicación de terminarse
    {
        // Enviar señal de fin a todos los procesos de la lista
        printf("\n%d: (X) SOY Padre (%d) indica que debo terminar (SIG %d)",
               getpid(), info->si_pid, sig_num);

        while(cantHijos > 0)
            kill(hijos[cantHijos--], SIGKILL);
    }
}

// Matar procesos
void killProc(const pid_t pid)
{
    kill(pid, SIGKILL);
}

// Validaciones
void	validaParametros(int argc, char *argv)
{
    DIR		*dir_dest = NULL;

    // Verifica cantidad de parámetros
    if( argc == 1 && !DEBUG)
    {
        perror("- Directorio no especificado.");
        puts(HELP);
        exit(EXIT_FAILURE);
    }
    else
    {
        // Validación de directorio
        if ((dir_dest = opendir ( argv )) == NULL  && !DEBUG)
        {
            perror("- Directorio no válido.");
            puts(HELP);
            closedir(dir_dest);
            exit(EXIT_FAILURE);
        }
        closedir(dir_dest);
    }
}
