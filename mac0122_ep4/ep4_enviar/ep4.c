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
/*            ImplementaÃ§Ã£o de um heap de mÃ­nimo                */
/* ************************************************************ */

#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

/* Declaracao das rotinas principais*/
int *Graham(int m, int n, int duracao[], Maquina maquinas[]);
int *sortedGraham(int m, int n, Maquina iduracao[], Maquina maquinas[]);
void sortInd(int n, int duracao[], Maquina iduracao[]);/* ordena o vetor iduracao[0..n-1] de modo que duracao[iduracao[0]] <= duracao[iduracao[1]] <= ... <= duracao[iduracao[n-1]] */

/* Declaracao das rotinas auxiliares*/
int getLeastUsed(Maquina maquinas[], int m);
int getMostUsed(Maquina maquinas[], int m);
void printGrah(FILE *saida, int m, int n, int duracao[], int quem_fez[], Maquina maquinas[]);
void printSortedGrah(FILE *saida, int m, int n, Maquina iduracao[], int duracao[], int quem_fez[], Maquina maquinas[]);
void setId(Maquina maquinas[], int m);
void resetMaquina(Maquina maquinas[], int m);
void heapify(int n, Maquina v[], int pai);
void trocaPosEp(Maquina v[], int pos1, int pos2);

int main(int argc, char *argv[]) { 
    int i, m, n, *duracao, *quem_fez;
    Maquina *maquinas, *iduracao;

    /* argc = numero de argumentos na linha de comando */
    /* argv = vetor de apontadores para strings contendo esses argumentos */
    
    FILE *entrada, *saida; /* declaracao da variavel para o arquivo de entrada */

    if (argc == 1 || argc == 2) {
        printf("Uso: %s <arquivo de entrada> <arquivo de saida>\n", argv[0]);
        return -1;
    }          

    if ((entrada = fopen(argv[1],"r")) == NULL) {
        printf("%s: arquivo de entrada %s nao pode ser aberto.\n", argv[0], argv[1]);
        return -1;
    }

    if ((saida = fopen(argv[2],"w")) == NULL) {
        printf("%s: arquivo de saida %s nao pode ser aberto.\n", argv[0], argv[2]);
        return -1;
    }

    /* Leitura de dados */ /*void le(FILE *entrada, int *tarefas, int *m, int *n)*/
    fscanf(entrada, "%d %d", &m, &n);

    duracao = malloc(n * sizeof(int));
    iduracao = malloc(n * sizeof(Maquina));
    maquinas = malloc(m * sizeof(Maquina));

    setId(maquinas, m);
    setId(iduracao, n);
    
    for(i = 0; i < n; i++){
      fscanf(entrada, "%d", &duracao[i]);
      iduracao[i].carga = duracao[i];
    }

    /* Impressao inicial de dados*/ /*void imprimeIni(FILE *entrada, FILE *saida, int *tarefas, int *m. int *n)*/
    fprintf(saida, "Nome do arquivo de entrada: %s\n\n", argv[1]);
    fprintf(saida, "m = %d      n = %d\n\n", m, n);

    fprintf(saida, "Tarefas:   ");
    for(i = 0; i < n; i++)
      fprintf(saida, "%d ", i);

    fprintf(saida, "\nDuração:   ");
    for(i = 0; i < n; i++)
      fprintf(saida, "%d ", duracao[i]);

    fprintf(saida, "\n\n");

    /* Graham normal*/

    quem_fez = Graham(m, n, duracao, maquinas);
    printGrah(saida, m, n, duracao, quem_fez, maquinas);

    /* Graham ordenado*/ 

    setId(maquinas, m);
    resetMaquina(maquinas, m);

    sortInd(n, duracao, iduracao);
    
    quem_fez = sortedGraham(m, n, iduracao, maquinas);
    printSortedGrah(saida, m, n, iduracao, duracao, quem_fez, maquinas);
        
    fclose(entrada); fclose(saida);
}    

/* Implementacao das rotinas principais*/

int *Graham(int m, int n, int duracao[], Maquina maquinas[]){
  int i, *quem_fez;

  quem_fez = malloc(n * sizeof(int));
  
  for(i = 0; i < n; i++){
    quem_fez[i] = min(maquinas);
    maquinas[0].carga += duracao[i];

    add_to_minkey(m, maquinas, 0);
  }

  return quem_fez;
}

int *sortedGraham(int m, int n, Maquina iduracao[], Maquina maquinas[]){
  int i, id_tarefa, *quem_fez;

  quem_fez = malloc(n * sizeof(int));
  
  for(i = 0; i < n; i++){
    id_tarefa = iduracao[i].id;

    quem_fez[id_tarefa] = min(maquinas); 
    maquinas[0].carga += iduracao[i].carga;

    add_to_minkey(m, maquinas, 0);
  }

  return quem_fez;
}

void sortInd(int n, int duracao[], Maquina iduracao[]){
  int i;

  for(i = (n/2 - 1); i >= 0; i--)
    heapify(n, iduracao, i);

  for(i = (n - 1); i >= 0; i--){/* iduracao se torna decrescente*/
    trocaPosEp(iduracao, 0, i);
    heapify(i, iduracao, 0);
  }
}

/* Implementacao das rotinas auxiliares */

int getLeastUsed(Maquina maquinas[], int m){
  int least_used = 0, i = 0;

  for(i = 0; i < m; i++)
    if(maquinas[i].carga < maquinas[least_used].carga)
      least_used = i;
  
  return least_used;
}

int getMostUsed(Maquina maquinas[], int m){/* be aware that this functin returns the time of the most used machine ("get" is wrongly used)*/
  int most_used = 0, i = 0;

  for(i = 0; i < m; i++)
    if(maquinas[i].carga > maquinas[most_used].carga)
      most_used = i;
  
  return maquinas[most_used].carga;
}

void printGrah(FILE *saida, int m, int n, int duracao[], int quem_fez[], Maquina maquinas[]){
  int i;
  
  fprintf(saida, "Algoritmo de Graham:\n");
  fprintf(saida, "Tarefa  Maquina\n");

  for(i = 0; i < n; i++)
    fprintf(saida, "   %d       %d\n", i, quem_fez[i]);

   fprintf(saida, "\nDuracao do escalonamento: %d\n", getMostUsed(maquinas, m));
}

void printSortedGrah(FILE *saida, int m, int n, Maquina iduracao[], int duracao[], int quem_fez[], Maquina maquinas[]){
  int i;

  fprintf(saida, "\nTarefas em ordem decrescente de duracao:\n");
  for(i = 0; i < n; i++)
    fprintf(saida, "%d ", iduracao[i].carga);
  
  fprintf(saida, "\n\nAlgoritmo de Graham com ordenacao:\n");
  fprintf(saida, "Tarefa  Maquina\n");

  for(i = 0; i < n; i++)
    fprintf(saida, "   %d       %d\n", i, quem_fez[i]);

   fprintf(saida, "\nDuracao do escalonamento: %d\n", getMostUsed(maquinas, m));
}

void setId(Maquina maquinas[], int m){
  int i;

  for(i = 0; i < m; i++)
    maquinas[i].id = i;
}

void resetMaquina(Maquina maquinas[], int m){
  int i;

  for(i = 0; i < m; i++)
    maquinas[i].carga = 0;
}

void heapify(int n, Maquina v[], int pai){
  int menor = pai;
  int filho_esquerda = pai*2 + 1;
  int filho_direita = pai*2 + 2;

  /* we get the smallest from the family of 3 (parent and 2 childs)*/
  if(filho_esquerda < n && v[filho_esquerda].carga < v[menor].carga)
    menor = filho_esquerda;

  if(filho_direita < n && v[filho_direita].carga < v[menor].carga)
    menor = filho_direita;

  if(menor != pai){
    trocaPosEp(v, pai, menor);
    heapify(n, v, menor);/* arrumamos a sub-arvore afetada*/
  }

}

void trocaPosEp(Maquina v[], int pos1, int pos2){
  Maquina t;

  t = v[pos1];
  v[pos1] = v[pos2];
  v[pos2] = t;
}
