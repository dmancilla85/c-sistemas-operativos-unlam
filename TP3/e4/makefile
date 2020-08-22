.PHONY: clean

EJERCICIO = e4

all: $(EJERCICIO)

$(EJERCICIO): $(EJERCICIO).o
	gcc -Iheaders -pthread -ggdb -o $(EJERCICIO) $(EJERCICIO).o

$(EJERCICIO).o: $(EJERCICIO).c
	gcc -Iheaders -ggdb -c $(EJERCICIO).c
	
clean:
	rm -f *.o $(EJERCICIO)
