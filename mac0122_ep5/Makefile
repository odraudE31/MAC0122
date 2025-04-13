# Ajuste os diretorios abaixo conforme o seu sistema 
LIBDIR = /home/eduardo/Documents/algoritmos/mac0122_ep5
CFLAGS = -g -Wall -std=c99 -pedantic -Wno-unused-result
###########################################################################

all: ep5

ep5: ep5.o heap.o 
	gcc -o ep5 ep5.o heap.o

ep5.o: ep5.c heap.h
	gcc $(CFLAGS) ep5.c -c 

heap.o: heap.c heap.h
	gcc $(CFLAGS) heap.c -c

clean:
	rm -rf *.o *~ ep5
