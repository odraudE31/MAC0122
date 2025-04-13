# Ajuste os diretorios abaixo conforme o seu sistema 
LIBDIR = /home/eduardo/Documents/algoritmos/mac0122_ep4
CFLAGS = -g -Wall -std=c99 -pedantic -Wno-unused-result
###########################################################################

all: ep4

ep4: ep4.o heap.o 
	gcc -o ep4 ep4.o heap.o

ep4.o: ep4.c heap.h
	gcc $(CFLAGS) ep4.c -c 

heap.o: heap.c heap.h
	gcc $(CFLAGS) heap.c -c

clean:
	rm -rf *.o *~ ep4
