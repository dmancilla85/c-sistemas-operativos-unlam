EJERCICIO = e3

all: $(EJERCICIO)

$(EJERCICIO): $(EJERCICIO).o
	gcc -Iheaders -ggdb -pthread -o $(EJERCICIO) $(EJERCICIO).o -lm

$(EJERCICIO).o: $(EJERCICIO).c
	gcc -Iheaders -ggdb -c $(EJERCICIO).c

﻿.PHONY: clean
	
clean:
	rm -f *.o $(EJERCICIO)
