#!/bin/bash

# Ejercicio #2
# --------------------------------------------------------------------------------
# ( Mancilla, David - Ferreyra, Jonathan - Muñoz, Juan Pablo - Miceli, Domingo )
# --------------------------------------------------------------------------------
# Genere un script que reciba por parámetro un número entero a, un número entero b 
# y el path del archivo de salida en el que se guardaran los resultados. 
# El script deberá realizar la suma, resta, multiplicación y el promedio de estos 
# dos números enteros guardando en el archivo de salida cada operación realizada.

MODO_DE_USO(){
	echo
	echo "$NOMBRE_ARCHIVO(1)        MODO DE USO DE EJERCICIO 2 - TP 1        $NOMBRE_ARCHIVO(1)"
	echo 
	echo "SYNOPSIS:"
	echo "       $0 numberA numberB fileName"
	echo
	echo "OPTIONS:"
	echo "     numberA     Cambiar 'numberA' por un numero entero."
	echo "     numberB     Cambiar 'numberB' por un numero entero."
	echo "     fileName    Cambiar 'fileName' por el nombre o path para archivo de salida."
	echo
	echo "EXAMPLE:"
	echo "       $0 2      62 /tmp/salida"
	echo "       $0 -4     -78 /tmp/salida"
	echo "       $0 -100   15 /tmp/salida"
	echo "       $0 0      80 /tmp/salida"
	echo "       $0 -97    0 /tmp/salida"
}
SYNOPSIS(){
	echo 
	echo "SYNOPSIS:"
	echo "       $0 numberA numberB fileName"
	echo
	echo "PARA OBTENER MÁS AYUDA: "$0" -h"
	echo
	exit 1
}

ERROR_CANTIDAD_PARAMETRO(){
	echo
	echo "Error - Numero de parámetro inválido."
	echo
	echo "PARA OBTENER AYUDA: "$0" -h"
	echo
	exit 1
}

VALIDA_CANTIDAD_PARAMETROS(){
	#Valido que la cantidad de parametros sea 3
	if ( test $CANT_PARAM -ne 3 )
	then
		ERROR_CANTIDAD_PARAMETRO
	fi
}
VALIDA_INGRESO_PARAMETROS(){
	if(test ! $NUMBER_A ) then
		echo "Error - No se ingreso numero A."
		SYNOPSIS
	fi

	if(test ! $NUMBER_B ) then
		echo "Error - No se ingreso numero B."
		SYNOPSIS
	fi
	
	if(test ! $ARCHIVO_SALIDA ) then
		echo "Error - No se ingreso el archivo de salida."
		SYNOPSIS
	fi
}

NOMBRE_ARCHIVO=`echo $0 | sed 's_.sh__' | sed 's_./__' `

if (test $# -eq 1 && ( test $1 = "-?" || test $1 = "-h")) then
	MODO_DE_USO
	exit 1
fi

CANT_PARAM=$#
NUMBER_A=$1
NUMBER_B=$2
ARCHIVO_SALIDA=$3

#~ VALIDA_CANTIDAD_PARAMETROS
VALIDA_INGRESO_PARAMETROS

#Validamos que el parametro numberA sea un numero
SIN_SIGNO=`echo $NUMBER_A | sed 's/^[+-]//g'`	#saco el signo a la variable numero ingresado
VALOR=`echo $SIN_SIGNO | sed 's/[0-9]//g'`	#Con sed reemplazo saco los numero, quedaran las letras (si hay).

if(test $VALOR ) then #Pregunto si el valor quedo con algun caracter, como una letra, es porque no es numero. 
	echo "Error - El 'numberA' ingresado tiene que ser numero entero."
	SYNOPSIS
fi

#Validamos que el parametro numberB sea un numero
SIN_SIGNO=`echo $NUMBER_B | sed 's/^[+-]//g'`	#saco el signo a la variable numero ingresado
VALOR=`echo $SIN_SIGNO | sed 's/[0-9]//g'`	#Con sed reemplazo saco los numero, quedaran las letras (si hay).

if(test $VALOR ) then #Pregunto si el valor quedo con algun caracter, como una letra, es porque no es numero. 
	echo "Error - El 'numberB' ingresado tiene que ser numero entero."
	SYNOPSIS
fi

#TODO: Validar que se puede crear el archivo en ese directorio.
if(test -z $ARCHIVO_SALIDA ) then
	#~ ERROR_ARCHIVO_NO_VALIDO
	echo "Error - Nombre del archivo no valido"
fi

#~ 999999999999999
rm $ARCHIVO_SALIDA 2>/dev/null

echo "" | awk -v nroA=$NUMBER_A -v nroB=$NUMBER_B 'END{
	printf("%.0f  +  %.0f    =  %0.f\n",nroA,nroB,nroA+nroB);
	printf("%.0f  -  %.0f    =  %0.f\n",nroA,nroB,nroA-nroB);
	printf("%.0f  *  %.0f    =  %0.f\n",nroA,nroB,nroA*nroB);
	printf("(%.0f +  %.0f)/2 =  %0.2f\n",nroA,nroB,(nroA+nroB)/2);
	}'


echo "" | awk -v nroA=$NUMBER_A -v nroB=$NUMBER_B 'END{
	printf("%.0f  +  %.0f    =  %0.f\n",nroA,nroB,nroA+nroB);
	printf("%.0f  -  %.0f    =  %0.f\n",nroA,nroB,nroA-nroB);
	printf("%.0f  *  %.0f    =  %0.f\n",nroA,nroB,nroA*nroB);
	printf("(%.0f +  %.0f)/2 =  %0.2f\n",nroA,nroB,(nroA+nroB)/2);
	}'>> $ARCHIVO_SALIDA
	
echo
echo "Se genero un documento de salida, ubicada en: $ARCHIVO_SALIDA"
