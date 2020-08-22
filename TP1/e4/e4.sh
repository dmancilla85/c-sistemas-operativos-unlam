#! /bin/sh

# Ejercicio #4
# --------------------------------------------------------------------------------
# ( Mancilla, David - Ferreyra, Jonathan - Muñoz, Juan Pablo - Miceli, Domingo )
# --------------------------------------------------------------------------------
# Realizar un shell script con AWK, que analice matrices. Dicho script deberá 
# analizar la estructura de la matriz y determinar el tipo al que corresponde y, 
# opcionalmente, si recibe un parámetro que lo indique, realizar operaciones sobre la matriz.
# La matriz a analizar será cargada desde un archivo de texto plano, en el que las columnas 
# estarán separadas por un carácter recibido por parámetro y las filas por un salto de línea.
# Los tipos de matrices a analizar serán los siguientes:
# Matriz Cuadrada, Matriz Rectangular, Matriz Columna, Matriz Fila, 
# Matriz Diagonal, Matriz Identidad, Matriz Nula
# Cabe destacar que una matriz puede pertenecer a más de un tipo, por lo que se deberán informar 
# todos aquellos en los que coincida. En el caso de ser una matriz cuadrada, indicar el orden de la misma.
# Parámetros:
# -c Separador de columnas. Obligatorio.
# -o Operación a realizar: (Opcional)
# trasp: indica trasponer la matriz
# prod: realizar el producto de la matriz por un escalar, indicado en el parámetro siguiente

####FUNCIONES#####
Error()
{			# $0 es el nombre del script
	echo "Error. Verifique sintaxis: $0 [ARCHIVO] -c [SEPARADOR]"
	echo "Comandos adicionales:"
	echo "-o [trasp] / [prod] NRO   "
	echo "El archivo debe ser una matriz numérica válida."
	echo "AYUDA: $0 -h" 
	exit 0
}
Ayuda()	#AYUDA
{
	echo "\nVersión 1.0"
	echo "\nUSO: $0 [ARCHIVO] -c [SEPARADOR]" 
	echo "\nEl comando $0 recibe como primer parametro un archivo que contiene una matriz válida, e informa el tipo de matriz, como segundo parametro se indica el separador y puede indicarse una orden opcional."
	echo "\nOpciones: \n\t -o [OPERACION]:"
	echo "\nOperaciones:
	\n\ttrasp: Trasponer matriz (en la salida por consola)
	\n\tprod: Producto por un escalar. \n\tExige un número como parámetro extra. Ej: -o prod [NUMERO]"
	echo "\nFormato de Archivo:\n\nCada fila debe indicarse en una nueva línea, y cada columna será separada por un separador que debera indicarse a la hora de invocar al comando." 
 exit 0
}
TipoMatriz() # Determina el tipo de matriz
{
cat $1 | 
awk -v param_1=$2 '	#paso parámetros a AWK
BEGIN{  
	sep = param_1; #variable con el separador
	i = 0;
	col = 0;
	while(( getline temp ) > 0 )
	{
		#corto el contenido de temp a un array temp2
		AUX = split(temp, temp2, sep);
	
		#recorro, cargo a matriz e imprimo	
		col = 0;				
	
		for(j = 0;  j < AUX;  j++)	
		{		
			#Verifico que sea un número el valor a cargar	
			if(  temp2[j + 1] + 0 == temp2[j + 1]    )
			{	
				m[i, col] = temp2[j + 1];
				printf m[i, col++] " ";
			}
		}
		
		printf "\n";
		i++;
	}
	
	COL = col;
	FIL = i;
}

END{
	FLAG = 1;
	
	for( f = 0; f < FIL; f++)	#recorro todas las posiciones
	{
		if( m[f, f] != 0 )
			FLAG = 0;
		
		for( c = f + 1; c < FIL; c++ )
			if( m[f, c] != 0 || m[f, c] !=0)	
				#si las posiciones son distinas de 0, cambio el flag
				FLAG= 0;
	}

	if(FLAG == 1)	#entonces es matriz nula
		print "Matriz Nula";
	else
	{							
		if(FIL == COL)	#si las dimensiones coinciden, es cuadrada
		{
			print "Matriz cuadrada de orden " COL;
			
			FLAG = 1;
			
			for( f = 0; f < FIL; f++)	#recorro todas las posiciones
			{
				if( m[f, f] != 1 )	
				#si está en la diagonal y es distinto de 1, cambio el flag
					FLAG = 0;
				
				for( c = f + 1; c < FIL; c++ )
					if( m[f, c] || m[f, c] )	
					#si no están en la diagonal y son distintas de 0, cambio el flag
						FLAG= 0;
			}
			
			if(FLAG == 1)	#entonces es matriz identidad
				print "Matriz Identidad";
				
			FLAG = 1;
			
			for( f = 0; f < FIL; f++) #recorro todas las posiciones
			{
				if( m[f, f] == 0 )	#si está en la diagonal y es 0, cambio el flag
					FLAG = 0;
				
				for( c = f +1; c < FIL; c++ )
					if( m[f, c] || m[f, c] )	
					#si no estás en la diagonal y son distintas de 0, cambio el flag 
						FLAG = 0;
			}

			if(FLAG == 1)	#entonces es matriz diagonal
				print "Matriz Diagonal";
		} 
		else
		{
			if(FIL == 1 && COL > 1)	#una fila, varias columnas
				print "Matriz Fila";
			else
			{
				if(COL == 1 && FIL > 1)	
					#una columna, varias filas
					print "Matriz Columna";
				else	
					#si no cumple, es matriz rectangular
					print "Matriz Rectangular";
			}
		}
	}		
}
	'
}

Operacion() #genera la operación seleccionada
{
cat $1 | 
awk -v param_1=$2 -v param_2=$3 -v param_3=$4 '
BEGIN{
	sep=param_1;	#variable con el separador
	i=0;
	col=0;
	
	while(( getline temp ) > 0 )	#recorro línea a línea
	{
		#corto el contenido de temp a un array temp2
		AUX=split(temp, temp2,sep);	

		col = 0;
		
		#recorro y cargo a matriz	
		for(j=0; j<AUX; j++)	
		{		
			#Verifico que sea un número el valor a cargar	
			if(  temp2[j+1] + 0 ==temp2[j+1]    )		
				m[i,col++]=temp2[j+1];
		}
	
		i++;
	}
	FIL=i;	#guardo dimensiones de fila y columna
	COL=col;
}

END{
	if(param_2=="trasp")	#Matriz Traspuesta
	{
		print "Matriz Traspuesta:";

		for(i=0; i<FIL; i++)	#recorro toda la matriz
		{
			for(j=0; j<COL; j++)
				TRASP[j,i]=m[i,j]; #guardo en la posición traspuesta, dentro de una matriz auxiliar
		}
		
		for(i=0; i<COL; i++)	#recorro toda la matriz traspuesta
		{
			for(j=0; j<FIL; j++)
				printf TRASP[i,j] " ";	#imprimo el resultado
			
			printf "\n";
		}

	}
	else	#como hay validación previa si no es "trasp" es PRODUCTO ESCALAR "prod"
	{
		NUM=param_3;	#guardo el parámetro en la variable NUM
		print "Producto Escalar por " NUM ":";

		for(i=0; i<FIL; i++)	#recorro toda la matriz
		{
			for(j=0; j<COL; j++)
			{
				PROD[i,j]=m[i,j]*NUM; #multiplico por NUM
				printf PROD[i,j] " "; #imprimo la matriz resultado
			}
			
			printf "\n";
		}
	}
}
'
}



##### FIN FUNCIONES #####


if test $# -lt 1
then
	Error
fi
# Valido que este el archivo
if test $# -eq 1
then
	if test $1 = "-h" #AYUDA
	then
		Ayuda
	else
		Error
	fi
fi

# Mostrar tipo de matriz
if test $# -ge 3
then
	if test $2 = '-c'
	then # Verifico que solo contenga números y el separador
		if grep -Eq [^0-9,.,$3] $1 > /dev/null ; then
			echo - La matriz contiene caracteres no numericos.			
			Error
		else
			TipoMatriz $1 $3
		fi
	else
		Error
	fi
else
	Error
fi

if test $# -eq 5
then
	if test $2 = "-c" && test $4 = "-o" && test $5 = "trasp"
	then
		Operacion $1 $3 $5
	else
		Error
	fi
fi

if test $# -eq 6
then
	if test $2 = "-c" && test $4 = "-o" && test $5 = "prod" && expr $6 + 0 > /dev/null 2>&1 #Verifico que sea un nro.
	then
		Operacion $1 $3 $5 $6
	else
		Error
	fi
fi

#FIN DE ARCHIVO
