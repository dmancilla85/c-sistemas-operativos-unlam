#!/bin/bash

# Ejercicio #5
# --------------------------------------------------------------------------------
# ( Mancilla, David - Ferreyra, Jonathan - Muñoz, Juan Pablo - Miceli, Domingo )
# --------------------------------------------------------------------------------
# Implemente un script llamado dependencias que reciba un nombre de directorio, 
# correspondiente al directorio de un proyecto Java, y admita opciones para los siguientes reportes:
# -a Para toda clase/interface importada en el proyecto, indicar la cantidad de veces que se 
# importa considerando los archivos de código fuente.
# -c clase/interface Para una clase/interface pasada por parámetro, indicar cuáles son las 
# clases/interfaces que la importan considerando los archivos de código fuente.
# Además de mostrar el reporte por la salida estándar, se puede guardar en un archivo si el mismo 
# es pasado por parámetro, con la opción -f.
# El directorio a procesar se debe indicar con la opción -d.

MODO_DE_USO(){
	echo
	echo "$NOMBRE_ARCHIVO(1)                 MODO DE USO DE EJERCICIO 5 - TP 1                 $NOMBRE_ARCHIVO(1)"
	echo 
	echo "SYNOPSIS:"
	echo "       $0 -a -d path_proyect_java -f name_file_out"
	echo
	echo "       $0 -c name_class_interface -d path_proyect_java -f name_file_out"
	echo
	echo "OPTIONS:"
	echo "   -a                       Realiza el conteo de todas las clases/interfaces que se importan dentro del proyecto java."
	echo "   -c name_class_interface  Cambiar 'name_class_interface' por el nombre de la Clase/Interface a contar."
	echo "                            Cuenta la cantidad de veces que se importa dentro del proyeto name."
	echo "   -d path_proyect_java     Cambiar 'path_proyect_java' por el directorio del proyecto java a analizar."
	echo "   -f name_file_out         Cambiar 'name_file_out' por el directorio y nombre del archivo de salida."
	echo
	echo "EXAMPLE:"
	echo "       $0 -a -d . -f ./reporte_proyecto"
	echo "       $0 -c org.junit.Assert.* -d ./java/ -f reporte_proyecto"
	echo
}
SYNOPSIS(){
	echo 
	echo "SYNOPSIS:"
	echo "       $0 -a -d path_proyect_java -f name_file_out"
	echo
	echo "       $0 -c name_class_interface -d path_proyect_java -f name_file_out"
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
ERROR_DIRECTORIO_NO_ES_PROYECTO_JAVA(){
	echo
	echo "Error - El directorio ingresado no es un proyecto java"
	SYNOPSIS
}
ERROR_DIRECTORIO_NO_VALIDO(){
	echo
	echo "Error - El directorio ingresado es no valido. Verifique su nombre o ubicación. "
	SYNOPSIS
}

VALIDA_PARAMETROS(){
	VALIDA_CANTIDAD_PARAMETROS
	VALIDA_OPCION_NO_REPETIDA
	VALIDA_OPCION_CORRECTA
	VALIDA_OPCIONES_OBLIGATORIAS
}
VALIDA_CANTIDAD_PARAMETROS(){	
	if(test ! $DIREC_PROYECTO )then
		MIN=4
	else
		MIN=5
	fi
	#Valido que la cantidad de parametros sea mayor a 5 y menor a 6
	if ( test $CANT_PARAM -lt $MIN || test $CANT_PARAM -gt 6 )
	then
		ERROR_CANTIDAD_PARAMETRO
	fi
}
VALIDA_OPCION_NO_REPETIDA(){
	RESULT=`echo $LISTA | awk '	BEGIN{CONT_A=0; CONT_C=0; CONT_D=0; CONT_F=0; }
								{	if($1=="a") CONT_A++;
									if($1=="c") CONT_C++;
									if($1=="d") CONT_D++;
									if($1=="f") CONT_F++;
								}
								END{ if(CONT_A > 1 || CONT_C > 1 || CONT_D > 1 || CONT_F > 1) 
										print 1
									 else
										print 0 }'`
	if(test $RESULT -eq 1 ) then
		ERROR_OPCION_REPETIDA
	fi
}
VALIDA_OPCION_CORRECTA(){
	RESULT=`echo $LISTA | awk '	BEGIN{CONT=0; }
								{ if($1!="a" && $1!="c" && $1!="d" && $1!="f") CONT++; }
								END{ printf CONT }'`
	if(test $RESULT -ge 1 ) then
		ERROR_OPCION_INCORRECTA
	fi
}
VALIDA_OPCIONES_OBLIGATORIAS(){
	RESULT=`echo $LISTA | awk '	BEGIN{CONT_A_C=0;CONT_D=0;CONT_F=0;}
								{	if($1=="a") CONT_A_C++;
									if($1=="c") CONT_A_C++;
									if($1=="d") CONT_D++;
									if($1=="f") CONT_F++;
								}
								END{ if(CONT_A_C!=1 || CONT_D!=1 || CONT_F!=1)
										print CONT_A_C
									 else
										print 0 }'`
	if(test $RESULT -eq 1 ) then
		ERROR_FALTA_OPCION_OBLIGATORIA
	fi
	if(test $RESULT -eq 2 ) then
		ERROR_USO_DE_OPCION_INCORRECTA
	fi
}
VALIDA_SIN_CARACTERES_ESPECIALES(){
	VALI_ARCHIVO_SALIDA=`echo $ARCHIVO_SALIDA | awk '{ if(NF>1) print 1 }'`
	VALI_DIREC_PROYECTO=`echo $DIREC_PROYECTO | awk '{ if(NF>1) print 1 }'`
	VALI_CLASE_A_CONTAR=`echo $CLASE_A_CONTAR | awk '{ if(NF>1) print 1 }'`
	VALI_CONTAR_TODO=`echo $CONTAR_TODO | awk '{ if(NF>1) print 1 }'`
	
	if(test $VALI_ARCHIVO_SALIDA )then
		echo
		echo "Error - No se puede ingresar caracter * como parametro de archivo de salida."
		SYNOPSIS
	fi
	if(test $VALI_DIREC_PROYECTO )then
		echo
		echo "Error - No se puede ingresar caracter * como parametro de direccion del proyecto."
		SYNOPSIS
	fi
	if(test $VALI_CONTAR_TODO )then
		echo
		echo "Error - No se puede ingresar caracter * luego del parametro -a."
		SYNOPSIS
	fi
	if(test $VALI_CLASE_A_CONTAR )then
		echo
		echo "Error - No se puede ingresar caracter * como parametro de clase/interface a buscar."
		SYNOPSIS
	fi
}

# -----------------------------	Programa principal ----------------------------- #
#Si la cantidad de parametros es 1, validamos si es comando de ayuda.
#Inicializo variables del programa
CANT_PARAM=$#
ENTRADA=$@
IFS='Â¬'
NOMBRE_ARCHIVO=`echo $0 | sed 's_.sh__' | sed 's_./__' `

if (test $# -eq 1 && ( test $1 = "-?" || test $1 = "-h")) then
	MODO_DE_USO
	exit 1
fi

LISTA=`echo $ENTRADA | sed 's_-_\n_g' | sed '1d'`

BUSCAR_ARCHIVO_JAVA=.java

ARCHIVO_SALIDA=`echo $LISTA | awk '/^f/{for(i=2;i<=NF;i++) printf("%s ",$i);}' | sed 's_ $__g'`
DIREC_PROYECTO=`echo $LISTA | awk '/^d/{for(i=2;i<=NF;i++) printf("%s ",$i);}' | sed 's_ $__g'`
CLASE_A_CONTAR=`echo $LISTA | awk '/^c/{for(i=2;i<=NF;i++) printf("%s ",$i);}' | sed 's_ $__g'`
PARAMETRO_CONT=`echo $LISTA | awk '/^a/{for(i=2;i<=NF;i++) printf("%s ",$i);}' | sed 's_ $__g'`

CONTAR_TODO=`echo $PARAMETRO_CONT | awk '{print NF}'`

PARAMETRO_CLASE=`echo $LISTA | awk '/^c/{for(i=2;i<=NF;i++) printf("%s ",$i);}' | sed 's_ $__g'`
PASARON_CLASE=`echo $PARAMETRO_CLASE | awk '{print NF}'`

# --------	Validacion - Inicio -------- #
VALIDA_SIN_CARACTERES_ESPECIALES
VALIDA_PARAMETROS

if(test ! -d $DIREC_PROYECTO ) then # Valido si el parametro d es un directorio valido.
	ERROR_DIRECTORIO_NO_VALIDO
fi

if(test $CONTAR_TODO -eq 1 )then
	ERROR_USO_DE_OPCION_INCORRECTA
fi

# --------	Validacion - Fin -------- #
IFS='
'
ARCHIVO_DE_IMPORT_DESORDENADOS="/tmp/archDeImportsDesordenado"
ARCHIVO_DE_IMPORT_ORDENADOS="/tmp/archDeImporstOrdenado"
ARCHIVO_DE_IMPORT_ORDENADOS_SIN_REPETIR="/tmp/archDeImportsOrdenadosSinRepetir"
rm $ARCHIVO_DE_IMPORT_DESORDENADOS 2>/dev/null
rm $ARCHIVO_DE_IMPORT_ORDENADOS 2>/dev/null
rm $ARCHIVO_DE_IMPORT_ORDENADOS_SIN_REPETIR 2>/dev/null
LISTA_ARCHIVOS=`find $DIREC_PROYECTO -name '*'${BUSCAR_ARCHIVO_JAVA}'' -type f `
CONTINUAR=`echo $LISTA_ARCHIVOS | awk '{ if(NF>0) print 1; else print 0 }'` # Valido si el directorio es un proyecto java o tiene archivos java

if(test $CONTINUAR -eq 0 ) then
	ERROR_DIRECTORIO_NO_ES_PROYECTO_JAVA
fi

for ARCHI in $LISTA_ARCHIVOS
do
	cat $ARCHI | awk '/^import/{ if($0) printf("%s\n",$0); }' | sed 's_^ __' >> $ARCHIVO_DE_IMPORT_DESORDENADOS
done

cat $ARCHIVO_DE_IMPORT_DESORDENADOS | sort | awk '/^import/{ if($2 == "static" ) print $3; else if($0) print $2; }' | sed 's_;__g' >> $ARCHIVO_DE_IMPORT_ORDENADOS

cat $ARCHIVO_DE_IMPORT_ORDENADOS | sort | uniq >> $ARCHIVO_DE_IMPORT_ORDENADOS_SIN_REPETIR


if(test -w $ARCHIVO_SALIDA ) then
	FILE_OUT=`echo "" | awk -v file_name=$ARCHIVO_SALIDA '{ printf("%s\n",file_name); }'`
	TEMPORAL=0
else
	FILE_OUT=`echo "" | awk -v file_name=$ARCHIVO_SALIDA '{ printf("%s\n",file_name); }'`
	TEMPORAL=1
fi

rm $FILE_OUT 2>/dev/null
LISTA_IMPORT=`cat $ARCHIVO_DE_IMPORT_ORDENADOS_SIN_REPETIR`

if(test $CONTAR_TODO -eq 0  && test $PASARON_CLASE -ne 1 ) then #Entro por falso porque CONTAR_TODO tiene que estar vacio, porque parametro a no tiene

	echo "" | awk '{printf("Cantidad\tClase/Interfaz\n");}'

	for IMPORT in $LISTA_IMPORT
	do
		cat $ARCHIVO_DE_IMPORT_ORDENADOS | awk -v import=$IMPORT 'BEING{cant=0;}
																  {
																	if(import==$0) cant++;
																  }
																  END{ printf("CANTIDAD: %d\tIMPORT: %s\n",cant,import);}' >> $FILE_OUT
	done
	
else
	if(test $DIREC_PROYECTO ) then
		echo "" | awk '{printf("Cantidad\tClase/Interfaz\n");}'
		
		IMPORT=$CLASE_A_CONTAR
		
		cat $ARCHIVO_DE_IMPORT_ORDENADOS | sed 's/^M$//' | awk -v import=$IMPORT 'BEING{cant=0;}
																				  {
																					  long=length($0)-1;
																					  actual=substr($0, 1, long);
																					  
																					  if(import==actual) cant++;
																				  }
																				  END{ printf("CANTIDAD: %d\tIMPORT: %s\n",cant,import);}' >> $FILE_OUT
	fi
fi

echo "Se proceso el proyecto " $DIREC_PROYECTO
#~ if(test $TEMPORAL -ne 0)then
	#~ echo "No se pudo escribir en el directorio" $ARCHIVO_SALIDA
#~ fi

echo "Salida: " $FILE_OUT
rm $ARCHIVO_DE_IMPORT_DESORDENADOS
rm $ARCHIVO_DE_IMPORT_ORDENADOS
rm $ARCHIVO_DE_IMPORT_ORDENADOS_SIN_REPETIR