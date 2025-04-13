/******************************************************************************/
/**                                                                          **/
/**   Nome:       Eduardo Gonzalez Silva                                     **/
/**   Professor:  Cristina Gomes Fernandes                                   **/
/**   Turma:      01                                                         **/
/**   NumeroUSP:  15448709                                                   **/
/**   Exercicio-Programa 04 - Escalonamento de tarefas em máquinas idênticas **/
/**                                                                          **/
/******************************************************************************/


/* ************************************************************ */
/*           Parte do EP4 de MAC0122-2024: heap.c               */
/*            Implementacao de um heap de mi­nimo                */
/* ************************************************************ */

#include "heap.h"
#include <stdio.h>

/* Prototipos das rotinas auxiliares */

void trocaPos(Maquina maquinas[], int pos1, int pos2);

/* Implementacao da biblioteca */

int min(Maquina v[]) {/* devolve o id da maquina com a menor carga do heap v */
    return v[0].id;
}

void add_to_minkey(int m, Maquina v[], int delta) { /* reorganiza o heap v[0..m-1] depois de aumentar a carga da maquina de carga mi­nima de delta */
    int f = 1;

    v[0].carga += delta;

    while(f < m){/* f <= m / 2 -> unicos que tem fihos??*/
        if(v[f].carga > v[f + 1].carga) f++; /* pegamos o menor entre os dois filhos*/

        if(v[(f - 1)/2].carga <= v[f].carga) break; /* se f for maior que f/2 -> para*/

        trocaPos(v, f, (f - 1)/2); /* f/2 fica menor*/

        f = (f*2) + 1;
    }
}

/* Implementacao das rotinas auxiliares */

void trocaPos(Maquina maquinas[], int pos1, int pos2){
    Maquina t;

    t = maquinas[pos1];
    maquinas[pos1] = maquinas[pos2];
    maquinas[pos2] = t;
}