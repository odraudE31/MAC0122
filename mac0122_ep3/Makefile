# Ajuste os diretorios abaixo conforme o seu sistema 
LIBDIR = /home/Documents/mac0122_ep2
CFLAGS = -g -Wall -std=c99 -pedantic -Wno-unused-result
###########################################################################

all: ep3

ep3: ep3.o polinomios.o 
	gcc -o ep3 ep3.o polinomios.o

ep3.o: ep3.c polinomios.h
	gcc $(CFLAGS) ep3.c -c 

polinomios: polinomios.o polinomios.o
	gcc -o polinomios polinomios.o polinomios.o

polinomios.o: polinomios.c polinomios.h
	gcc $(CFLAGS) polinomios.c -c 

clean:
	rm -rf *.o *~ client
