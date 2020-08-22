#!/bin/bash

# Ejercicio #3
# --------------------------------------------------------------------------------
# ( Mancilla, David - Ferreyra, Jonathan - Muñoz, Juan Pablo - Miceli, Domingo )
# --------------------------------------------------------------------------------
# Crear un script que se llame interlineado, que permita la modificación del interlineado
# de un archivo de texto cuyo path debe recibirse por parámetro y dejar la salida en un 
# nuevo archivo que se llame igual al archivo de entrada, más el prefijo out_.
# El archivo de salida debe crearse en el mismo path del archivo de entrada y en caso de no 
# contar con los permisos requeridos, en el directorio temporal.
# El script debe contemplar las siguientes opciones:
# -i n Modificar el interlineado a un valor n que se especifique por parámetro.
# -i Modificar el interlineado a 1, es decir, agregar una línea vacía a continuación de cada 
# 	 línea del archivo que contenga texto. Debe ser el mismo comportamiento que para la opción -i 1
# -d Eliminar todas las líneas vacías de un archivo.
# Para indicar el archivo a procesar, utilizar la opción -f.

MODO_DE_USO(){
	echo
	echo "$NOMBRE_ARCHIVO(1)                 MODO DE USO DE EJERCICIO 3 - TP 1                 $NOMBRE_ARCHIVO(1)"
	echo 
	echo "SYNOPSIS:"
	echo "       $0 -i [number] -f name"
	echo
	echo "       $0 -d -f name"
	echo
	echo "OPTIONS:"
	echo "     -f name      Cambiar 'name' por el nombre del archivo que se realizara el interlineado."
	echo "     -i 	        Realiza el interlineado de 1 por default."
	echo "     -i [number]  Reemplazar 'number' por la cantidad de lineas para el interlineado."
	echo "     -d           Elimina todas las líneas vacías de un archivo."
	echo
	echo "EXAMPLE:"
	echo "       $0 -i -f archivo"
	echo "       $0 -i 4 -f archivo"
	echo "       $0 -d -f archivo"
}
SYNOPSIS(){
	echo 
	echo "SYNOPSIS:"
	echo "       $0 -i [number] -f name"
	echo
	echo "       $0 -d -f name"
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
ERROR_FORMATO_NUMERO(){
	echo "Error - El numero ingresado tiene formato incorrecto. Tiene que ser mayor a cero y entero"
	echo
	echo "PARA OBTENER AYUDA: "$0" -h"
	echo
	exit 1
}
ERROR_NUMERO_NEGATIVO(){
	echo "Error - El numero ingresado es un numero negativo. Tiene que ser mayor a cero y entero."
	echo
	echo "PARA OBTENER AYUDA: "$0" -h"
	echo
	exit 1
}
ERROR_OPCION_REPETIDA(){
	echo
	echo "Error - Se ingresaron opciones de forma repetida."
	SYNOPSIS
}
ERROR_OPCION_INCORRECTA(){
	echo
	echo "Error - Se ingreso alguna opción incorrecta."
	SYNOPSIS
}
ERROR_FALTA_OPCION_OBLIGATORIA(){
	echo
	echo "Error - No se ingreso las opciones obligatoria."
	SYNOPSIS
}
ERROR_USO_DE_OPCION_INCORRECTA(){
	echo
	echo "Error - No se usaron las opciones de forma correcta."
	SYNOPSIS
}
ERROR_EL_NOMBRE_ARCHIVO_OBLIGATORIO(){
	echo
	echo "Error - No se ingreso el nombre del archivo obligatorio."
	SYNOPSIS
}
ERROR_ARCHIVO_NO_VALIDO(){
	echo
	echo "Error - El archivo ingresado no es valido. Verifique su nombre o ubicación."
	SYNOPSIS
}

VALIDA_PARAMETROS(){
	VALIDA_CANTIDAD_PARAMETROS
	VALIDA_OPCION_NO_REPETIDA
	VALIDA_OPCION_CORRECTA
	VALIDA_OPCIONES_OBLIGATORIAS
}
VALIDA_CANTIDAD_PARAMETROS(){	
	#Valido que la cantidad de parametros sea mayor a 1 y menor a 3
	if ( test $CANT_PARAM -lt 1 || test $CANT_PARAM -gt 4 )
	then
		ERROR_CANTIDAD_PARAMETRO
	fi
}
VALIDA_OPCION_NO_REPETIDA(){
	RESULT=`echo $LISTA | awk '	BEGIN{CONT_I=0; CONT_F=0; CONT_D=0; }
					{	if($1=="i") CONT_I++;
						if($1=="f") CONT_F++;
						if($1=="d") CONT_D++;
					}
					END{ if(CONT_I > 1 || CONT_F > 1 || CONT_D > 1) 
							print 1
						 else
							print 0 }'`
	if(test $RESULT -eq 1 ) then
		ERROR_OPCION_REPETIDA
	fi
}
VALIDA_OPCION_CORRECTA(){
	RESULT=`echo $LISTA | awk '	BEGIN{CONT=0; }
					{	if($1!="i" && $1!="f" && $1!="d") CONT++;
					}
					END{ printf CONT }'`
	if(test $RESULT -ge 1 ) then
		ERROR_OPCION_INCORRECTA
	fi
}
VALIDA_OPCIONES_OBLIGATORIAS(){
	RESULT=`echo $LISTA | awk '	BEGIN{CONT_F=0;CONT_I=0;CONT_D=0;CONT_I_D=0;}
					{	if($1=="f") CONT_F++;
						if($1=="i"){ CONT_I_D++;}
						if($1=="d"){ CONT_I_D++;}
					}
					END{ if(CONT_F!=1 || CONT_I_D!=1)
							print CONT_I_D
						 else
							print 0 }'`
	if(test $RESULT -eq 1 ) then
		ERROR_FALTA_OPCION_OBLIGATORIA
	fi
	if(test $RESULT -eq 2 ) then
		ERROR_USO_DE_OPCION_INCORRECTA
	fi
}

# -----------------------------	Programa principal ----------------------------- #
#Si la cantidad de parametros es 1, validamos si es comando de ayuda.
NOMBRE_ARCHIVO=`echo $0 | sed 's_.sh__' | sed 's_./__' `

if (test $# -eq 1 && ( test $1 = "-?" || test $1 = "-h")) then
	MODO_DE_USO
	exit 0
fi

#Inicializo variables del programa
CANT_PARAM=$#
ENTRADA=$@
IFS='Â¬'
LISTA=`echo $ENTRADA | sed 's_-_\n_g' | sed '1d'`

#VALIDA_PARAMETROS

ARCHIVO=`echo $LISTA | awk '/^f/{for(i=2;i<=NF;i++) printf("%s ",$i);}' | sed 's_ $__g'`
AUX=`echo "" | awk -v comodin=$ARCHIVO '{ print comodin }' | sed 's_^*_COMODIN_g' `
ES_COMODIN=`echo $AUX | grep ^COMODIN | awk 'END{print NR}'`

if( test $ES_COMODIN ) then
	ARCHIVO=`echo $AUX`
fi

#Valido datos Obligatorios
if(test -z $ARCHIVO ) then
	ERROR_EL_NOMBRE_ARCHIVO_OBLIGATORIO
fi

if(test ! -f $ARCHIVO ) then
	ERROR_ARCHIVO_NO_VALIDO
fi

OPCION_SACAR_INTERLINEADO=`echo $LISTA | awk '/^d/{ if(NF==1) print 1; else print 0;}' `
CONT=0

if(test -w . ) then
	FILE_OUT=`echo "" | awk -v file_name=$NOMBRE_ARCHIVO '{ printf("out_%s\n",file_name); }'`
	TEMPORAL=0
else
	FILE_OUT=`echo "" | awk -v file_name=$NOMBRE_ARCHIVO '{ printf("/tmp/out_%s\n",file_name); }'`
	TEMPORAL=1
fi

rm /tmp/$ARCHIVO 2>/dev/null
cp $ARCHIVO /tmp/$ARCHIVO
ARCHIVO=`echo "" | awk -v file_in=$ARCHIVO '{ printf("/tmp/%s",file_in); }'`
rm $FILE_OUT 2>/dev/null

if(test $OPCION_SACAR_INTERLINEADO ) then #Si la OPCION_PONER_INTERLINEADO es distinto de nulo
	CONT=1
	
	if(test $OPCION_SACAR_INTERLINEADO -eq 0 ) then
		ERROR_USO_DE_OPCION_INCORRECTA
	fi
	
	#Si paso las validaciones del numero entonces comienzo a agregarle el interlineado y lo guardo
	cat $ARCHIVO | sed '/^$/d' >> $FILE_OUT
	DIRECTORIO=`pwd | awk '{print $0}'`
	
	echo "Se borraron las lineas vacías del archivo " $DIRECTORIO"/"$ARCHIVO
	
	if(test $TEMPORAL -ne 0)then
		echo "No se pudo escribir en el directorio" $DIRECTORIO
	fi
	
	echo "Salida: " $FILE_OUT
fi

OPCION_PONER_INTERLINEADO=`echo $LISTA | awk '/^i/{ if(NF>1) for(i=2;i<=NF;i++) printf("%s ",$i); else	printf("1");}' | sed 's_ $__g'`

if(test $OPCION_PONER_INTERLINEADO ) then #Si la OPCION_PONER_INTERLINEADO es distinto de nulo

	NUMERO=$OPCION_PONER_INTERLINEADO
	CONT=1
	#Validamos que el parametro de interlineado sea un numero
	SIN_SIGNO=`echo $NUMERO | sed 's/^[+-]//g'`	#saco el signo a la variable numero ingresado
	VALOR=`echo $SIN_SIGNO | sed 's/[0-9]//g'`	#Con sed reemplazo saco los numero, quedaran las letras (si hay).

	if(test $VALOR ) then #Pregunto si el valor quedo con info, como una letra, es porque no es numero. 
		ERROR_FORMATO_NUMERO
	fi

	#Si es un numero, entonces pregunto si el numero de interlineado es un numero positivo.
	ES_NEGATIVO=`echo $NUMERO | awk '{ if($1<0) print 1
									   else print 0
									 }'`

	if(test $ES_NEGATIVO -eq 1) then
		ERROR_NUMERO_NEGATIVO
	fi
	
	INTERLINEADO=`echo $NUMERO | sed 's/^+//g'`
	#Si paso las validaciones del numero entonces comienzo a agregarle el interlineado
	cat $ARCHIVO | sed '/^$/d' | awk -v interlineado=$INTERLINEADO '{ 
		printf("%s\n",$0); if(length($0)!=0){ for(i=0;i<interlineado;i++) printf("\n");} }' >> $FILE_OUT
	DIRECTORIO=`pwd | awk '{print $0}'`
	
	echo "Se modificó el interlineado del archivo " $DIRECTORIO"/"$ARCHIVO
	
	if(test $TEMPORAL -ne 0)then
		echo "No se pudo escribir en el directorio" $DIRECTORIO
	fi
	
	echo "Salida: " $FILE_OUT
fi

if(test $CONT -eq 0 )then
	ERROR_FALTA_OPCION_OBLIGATORIA
fi

rm $ARCHIVO 2>/dev/null
