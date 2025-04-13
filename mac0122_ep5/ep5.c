/******************************************************************************/
/**                                                                          **/
/**   Nome:       Eduardo Gonzalez Silva                                     **/
/**   Professor:  Cristina Gomes Fernandes                                   **/
/**   Turma:      01                                                         **/
/**   NumeroUSP:  15448709                                                   **/
/**   Exercicio-Programa 05 - Escalonamento de tarefas em máquinas idênticas **/
/**                           - A solucao otima                              **/
/**                                                                          **/
/******************************************************************************/


/* ******************************************************/
/*           Parte do EP5 de MAC0122-2024:              */
/* ******************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

FILE *entrada, *saida; /* declaracao da variavel para o arquivo de entrada */

struct melhorSolucao {
  int *quem_fez;
  int duracao;
};

typedef struct melhorSolucao MelhorSolucao;

/* Implementacao das rotinas do ep anterior */
int *sortedGraham(int m, int n, int d[], int *duracao);
void sortInd(int n, int d[], int id[]);/* ordena o vetor iduracao[0..n-1] de modo que duracao[iduracao[0]] <= duracao[iduracao[1]] <= ... <= duracao[iduracao[n-1]] */
int getMostUsed(Maquina maquinas[], int m);
void setId(Maquina maquinas[], int m);
void resetMaquina(Maquina maquinas[], int m);
void heapify(int n, int pai, int d[], int id[]);
void trocaPosEp(int id[], int pos1, int pos2);

/* Implementacao das rotinas desse ep */
int SolucaoOtima(int m, int n, int d[], int escalOtimo[]);
void printSol(int n, int dur, int esclOtimo[]);
int backTracking(int m, int n, int maquinas[], int mediaTar, MelhorSolucao *melhor, int parcial[], int tarefa, int d[], int id[]);
int ehBoa(int m, int maquinas[], int parcial[], MelhorSolucao *melhor, int mediaTar);
int maisCarregada(int maquinas[], int m);
void printSol(int n, int dur, int esclOtimo[]);

int main(int argc, char *argv[]) { 
    int i, m, n, duracao, *d, *id, *escalOtimo;

    /* argc = numero de argumentos na linha de comando */
    /* argv = vetor de apontadores para strings contendo esses argumentos */

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

    /* Leitura de dados */ 
    fscanf(entrada, "%d %d", &m, &n);

    d = malloc(n * sizeof(int));
    id = malloc(n * sizeof(int));
    escalOtimo = malloc(n * sizeof(int));
    
    for(i = 0; i < n; i++){
      fscanf(entrada, "%d", &d[i]);
      id[i] = i;
    }

    /* Impressao inicial de dados */
    fprintf(saida, "Nome do arquivo de entrada: %s\n\n", argv[1]);
    fprintf(saida, "m = %d      n = %d\n\n", m, n);

    fprintf(saida, "Tarefas:   ");
    for(i = 0; i < n; i++)
      fprintf(saida, "%d ", i);

    fprintf(saida, "\nduracao:   ");
    for(i = 0; i < n; i++)
      fprintf(saida, "%d ", d[i]);

    fprintf(saida, "\n\n");

    duracao = SolucaoOtima(m, n, d, escalOtimo);

    printSol(n, duracao, escalOtimo);
        
    fclose(entrada); fclose(saida);
}    

/* Implementacao das rotinas do ep anterior */

int *sortedGraham(int m, int n, int d[], int *duracao){
  int i, *id, *quem_fez;
  Maquina *maquinas;

  id = malloc(n * sizeof(int));
  quem_fez = malloc(n * sizeof(int));
  maquinas = malloc(m * sizeof(Maquina));

  setId(maquinas, m);

  for(i = 0; i < n; i++)
    id[i] = i;

  sortInd(n, d, id);
  
  for(i = 0; i < n; i++){
    quem_fez[id[i]] = min(maquinas); 
    
    add_to_minkey(m, maquinas, d[id[i]]);
  }

  *duracao = getMostUsed(maquinas, m);

  return quem_fez;
}

void sortInd(int n, int d[], int id[]){
  int i;

  for(i = (n/2 - 1); i >= 0; i--)
    heapify(n, i, d, id);

  for(i = (n - 1); i >= 0; i--){/* iduracao se torna decrescente*/
    trocaPosEp(id, 0, i);
    heapify(i, 0, d, id);
  }
}

int getMostUsed(Maquina maquinas[], int m){/* be aware that this functin returns the time of the most used machine ("get" is wrongly used)*/
  int most_used = 0, i = 0;

  for(i = 0; i < m; i++)
    if(maquinas[i].carga > maquinas[most_used].carga)
      most_used = i;
  
  return maquinas[most_used].carga;
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

void heapify(int n, int pai, int d[], int id[]){
  int menor = pai;
  int filho_esquerda = pai*2 + 1;
  int filho_direita = pai*2 + 2;

  /* we get the smallest from the family of 3 (parent and 2 childs)*/
  if(filho_esquerda < n && d[id[filho_esquerda]] < d[id[menor]])
    menor = filho_esquerda;

  if(filho_direita < n && d[id[filho_direita]] < d[id[menor]])
    menor = filho_direita;

  if(menor != pai){
    trocaPosEp(id, pai, menor);
    heapify(n, menor, d, id);/* arrumamos a sub-arvore afetada*/
  }

}

void trocaPosEp(int id[], int pos1, int pos2){
  int t;

  t = id[pos1];
  id[pos1] = id[pos2];
  id[pos2] = t;
}

/* Implementacao das rotinas desse ep */

int SolucaoOtima(int m, int n, int d[], int escalOtimo[]){ 
  int i, *maquinas, *id, *parcial, mediaTar = 0;
  MelhorSolucao *melhor;

  melhor = malloc(sizeof(MelhorSolucao));
  melhor->quem_fez = malloc(n * sizeof(int));
  maquinas = malloc(m * sizeof(int));
  id = malloc(n * sizeof(int));
  parcial = malloc(n * sizeof(int));
  
  // Graham é solucao otima? Mesmo assim vamos guardar seus valores
  melhor->quem_fez = sortedGraham(m, n, d, &melhor->duracao);

  for(i = 0; i < n; i++){
    mediaTar += d[i];
    id[i] = i;
  }

  mediaTar = mediaTar/n;

  if(melhor->duracao == mediaTar){
    for(i = 0; i < n; i++){
      escalOtimo[i] = melhor->quem_fez[i];
      printf("%d\n", melhor->quem_fez[i]);
    }
    printf("Encontramos a solucao otima com Graham!\n");

    return melhor->duracao;
  }

  // Ordenamos o vetor de tarefas
  sortInd(n, d, id);

  // Rodamos realmente o BT

  backTracking(m, n, maquinas, mediaTar, melhor, parcial, 0, d, id);

  for(i = 0; i < n; i++)
    escalOtimo[i] = melhor->quem_fez[i];
  
  return melhor->duracao;
}

int backTracking(int m, int n, int maquinas[], int mediaTar, MelhorSolucao *melhor, int parcial[], int tarefa, int d[], int id[]){
  int i, quem_fazia;

  // Vemos se a solucao dessa folha é melhor, ou ate mesmo otima
  if(tarefa == n){// Uma tarefa depois da ultima (folha da arvore + 1)
    int maximo = maisCarregada(maquinas, m);

    // Solucao otima
    if(maximo == mediaTar){
      melhor->duracao = maximo;

      for(i = 0; i < n; i++)// Passamos a solucao parcial para a melhor
        melhor->quem_fez[id[i]] = parcial[i];

      return 1;// Essa solucao é a otima
    }

    // Melhor solucao ate agora
    if(maximo < melhor->duracao){
      melhor->duracao = maximo;
      
      for(i = 0; i < n; i++)// Passamos a solucao parcial para a melhor
        melhor->quem_fez[id[i]] = parcial[i];

      return 0; 
    }

    else return 0; 
  }

  // Criamos a celula atual e direcionamos os filhos frutiferos
  for(i = 0; i < m; i++){
    maquinas[i] += d[id[tarefa]];
    quem_fazia = parcial[tarefa];
    parcial[tarefa] = i; 

    // Descendo na arvore
    if(ehBoa(m, maquinas, parcial, melhor, mediaTar)) 
      if(backTracking(m, n, maquinas, mediaTar, melhor, parcial, tarefa+1, d, id))
        return 1;

    // Backtracking (subindo na arvore)
    maquinas[i] -= d[id[tarefa]];  
    parcial[tarefa] = quem_fazia;
  }

  return 0; // Nao achamos nada otimo nesse branch 
}

int ehBoa(int m, int maquinas[], int parcial[], MelhorSolucao *melhor, int mediaTar){
  int maximo;

  maximo = maisCarregada(maquinas, m);

  if(maximo < melhor->duracao)// Se for igual o que a gente ja tem nao vale continuar
    return 1;

  return 0;
}

int maisCarregada(int maquinas[], int m){
  int i, maior = 0;

  for(i = 0; i < m; i++)
    if(maior < maquinas[i])
      maior = maquinas[i];

  return maior;
}

void printSol(int n, int dur, int esclOtimo[]){
  int i;
  
  fprintf(saida, "\n\nEscalonamento Ótimo:\n");
  fprintf(saida, "Tarefa  Maquina\n");

  for(i = 0; i < n; i++)
    fprintf(saida, "   %d       %d\n", i, esclOtimo[i]);

   fprintf(saida, "\nDuracao do escalonamento: %d\n", dur);
}
