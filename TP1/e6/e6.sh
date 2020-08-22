#!/bin/bash

# Ejercicio #6
# --------------------------------------------------------------------------------
# ( Mancilla, David - Ferreyra, Jonathan - Muñoz, Juan Pablo - Miceli, Domingo )
# --------------------------------------------------------------------------------
# Genere un script que reciba por parámetro un archivo de texto con información de los 
# goleadores del torneo Inicial de fútbol de Primera Nacional, cada línea del archivo 
# de texto recibido contendrá la fecha, nombre de jugador y cantidad de goles. El script 
#debe sumar los goles de cada fecha por cada jugador y generar en un archivo de salida 
# ordenado de mayor a menor según la cantidad de goles de cada jugador. Si dos jugadores 
# tienen igual cantidad de goles entonces se ordenarán en segundo orden alfabéticamente.

MODO_DE_USO(){
	echo
	echo "$NOMBRE_ARCHIVO(1)                 MODO DE USO DE EJERCICIO 6 - TP 1                 $NOMBRE_ARCHIVO(1)"
	echo 
	echo "SYNOPSIS:"
	echo "       $0 file_name"
	echo
	echo "       $0 path/file_name"
	echo
	echo "DESCRIPTION:"
	echo "file_name:     Nombre del archivo de entrada de cantidad de goles del torneo final"
	echo "               El formato del archivo será:"
	echo "                  FECHA|NOMBRE_JUGADOR|CANTIDAD_GOLES"
	echo
	echo "               Las columnas deben ser separadas por el caracter | "
	echo
	echo "EXAMPLE OF FILE_NAME:"
	echo "4/3/2014|Scocco, Ignacio|2"
	echo "5/3/2014|Gigliotti, Emanuel|1"
	echo "8/3/2014|Gigliotti, Emanuel|2"
	echo
	echo "EXAMPLE OF USE OF SCRIPT:"
	echo "       $0 Entrada.txt"
	echo "       $0 ./Entrada.txt"
	echo
}
SYNOPSIS(){
	echo 
	echo "SYNOPSIS:"
	echo "       $0 file_name"
	echo
	echo "       $0 path/file_name"
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

VALIDA_PARAMETROS(){
	VALIDA_CANTIDAD_PARAMETROS
}
VALIDA_CANTIDAD_PARAMETROS(){
	#Valido que la cantidad sea 1
	if ( test $CANT_PARAM -ne 1 )
	then
		ERROR_CANTIDAD_PARAMETRO
	fi
}


CANT_PARAM=$#
ENTRADA=$@
IFS='Â¬'
NOMBRE_ARCHIVO=`echo $0 | sed 's_.sh__' | sed 's_./__' `

if (test $# -eq 1 && ( test $1 = "-?" || test $1 = "-h")) then
	MODO_DE_USO
	exit 1
fi

VALIDA_PARAMETROS

#Valido si el archivo existe
ARCHIVO_ENTRADA=$1
if(test ! -d $ARCHIVO_ENTRADA ) then # Valido si el parametro d es un directorio valido. 
	cat $ARCHIVO_ENTRADA | awk -F"|" 'BEING{ FLAG=0; JUGADOR="$2"; i=0; }
									 { 
										 VECTOR[$2]=VECTOR[$2]+$3;
									 }
									 END{
										  for (indice in VECTOR)
											printf("%d|%s\n",VECTOR[indice],indice); 
										}' | sort -t '|' -nrk1 > SALI_TMP
fi

cat SALI_TMP 
#~ | awk -F"|" 'BEING{i=0;cont=0;flag=0;c=0;}
					#~ {
						#~ GOLES[NR]=$1;
						#~ JUGADOR[NR]=$2;
						#~ 
						#~ if(flag==0 || ant!=$1 ){
							#~ CORTE[c]=i;
							#~ ant=$1;
							#~ c++;
						#~ }
					#~ }
					#~ END{
						#~ CONTADOR=0;
						#~ for(i=1; i<=NR; i++){
							#~ if(GOLES[i]==GOLES[i+1]){
								#~ CONTADOR++;
							#~ }else{
								#~ for(j=i-CONTADOR;j<CONTADOR;j++){
									#~ printf("%d\t%s\n",GOLES[j],JUGADOR[j]);
								#~ }
								#~ CONTADOR=0;
							#~ }
						#~ }
					#~ }'
