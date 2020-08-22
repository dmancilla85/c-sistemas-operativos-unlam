.PHONY: clean

EJERCICIO = tp2_e2

all: $(EJERCICIO)

$(EJERCICIO): $(EJERCICIO).o
	gcc -Iheaders -ggdb -o $(EJERCICIO) $(EJERCICIO).o

$(EJERCICIO).o: $(EJERCICIO).c
	gcc -Iheaders -ggdb -c $(EJERCICIO).c
	
clean:
	rm -f *.o $(EJERCICIO)
