#!/bin/bash

if test $# -lt 2; then
	echo "La cantidad de argumentos es menor a 2"
	exit 1;
fi

if ! test -e $1; then
	echo "El argumento ($1) pasado por parametro no es un archivo existente"
	exit 1;
fi

FILE=$1
shift 1
COUNTER=0
while test $COUNTER -le $# ; do
	SET[$COUNTER]=$1
	shift 1
	((COUNTER++))
done

for X in "${SET[@]}" ;do
	CANT=0
	echo "contenido por parametro $X"
	for WORD in `cat $FILE` ;do
	    echo "contenido del archivo $WORD"
		if [[ "$WORD" == "$X" ]]; then
			((CANT++))
		fi
	done
echo "$X : $CANT"
done

exit 0