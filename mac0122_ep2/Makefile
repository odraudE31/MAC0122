# Ajuste os diretorios abaixo conforme o seu sistema 
LIBDIR = /home/Documents/mac0122_ep2
CFLAGS = -g -Wall -std=c99 -pedantic -Wno-unused-result
###########################################################################

all: testa-EP2

testa-EP2: testa-EP2.o polinomios.o 
	gcc -o testa-EP2 testa-EP2.o polinomios.o

testa-EP2.o: testa-EP2.c polinomios.h
	gcc $(CFLAGS) testa-EP2.c -c 

polinomios: polinomios.o polinomios.o
	gcc -o polinomios polinomios.o polinomios.o

polinomios.o: polinomios.c polinomios.h
	gcc $(CFLAGS) polinomios.c -c 

clean:
	rm -rf *.o *~ client
