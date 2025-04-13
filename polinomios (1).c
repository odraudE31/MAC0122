/*    
 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO-PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome: Karolina Vasconcellos Barreto
  NUSP:15508831
  MAC0122 - Princípios de Desenvolvimento de Algoritmos 
  Professora: Cristina Gomes Fernandes            
  Exercicio-Programa 02 

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma referência, liste-as abaixo
  para que o seu programa não seja considerada plágio.

 \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
 */

#include <stdio.h>
#include <stdlib.h>
#include "polinomios.h"

/*Declaracao da celula das listas encadeadas */
typedef struct polinomio_celula {
  float coeficiente;
  int expoente;
  struct polinomio_celula *prox;
} Celula;

/*Eventuais funcoes auxiliares*/
void *mallocSafe (size_t tamanho) {
  void *p = malloc(tamanho);
  if (p) return p;
  else { /*Em caso de erro ao alocar memória, termina o programa*/
    printf("\nErro ao alocar memoria.");
    exit(EXIT_FAILURE);
  }
}

float modulo (float i) { /*Obtem o módulo do número*/
  if (i < 0.0) return -i;
  else return i;
}

polinomio ordena (polinomio p) { /*Ordena em ordem decrescente*/
  Celula *ini = (Celula *) p, 
  *aux_atual = ini, *aux_prox, *aux_anterior = NULL;
  int trocou = 1;
  if (ini == NULL || ini -> prox == NULL) return p; /*Lista vazia ou com um elemento não precisa fazer nada*/
  while (trocou == 1) {
    trocou = 0; 
    aux_atual = ini;
    aux_anterior = NULL;
    while (aux_atual != NULL && aux_atual -> prox != NULL) {
      aux_prox = aux_atual -> prox;
      if (aux_atual -> expoente < aux_prox -> expoente) { /*Se os expoentes estão em ordem crescente, troca*/
        if (aux_anterior == NULL) ini = aux_prox; 
        else aux_anterior -> prox = aux_prox;
        aux_atual -> prox = aux_prox -> prox;
        aux_prox -> prox = aux_atual;
        trocou = 1;
        aux_anterior = aux_prox; 
      } else { /*Caso não, mantém a ordem atual e preoara os ponteiros para a próxima análise*/
        aux_anterior = aux_atual;
        aux_atual = aux_atual -> prox; 
      }
    }
  } 
  return (polinomio) ini; 
}

polinomio limpa (polinomio p) { /*Apaga termos com coeficiente nulo e junta os com mesmos expoentes*/
  Celula *ini = (Celula *) p, 
  *aux = ini, *aux_anterior = NULL, *aux_prox;
   while (aux) { /*Limpa zeros*/
    if (aux -> coeficiente == 0) {
      if (ini != aux) {
        if (aux -> prox) aux_anterior -> prox = aux -> prox;
        else aux_anterior -> prox = NULL;
        free (aux); 
        if (aux_anterior -> prox) aux = aux_anterior -> prox;
        else aux = NULL;
      } else {
        ini = (aux -> prox);
        free (aux);
        aux = ini;
      } 
    } else {
      aux_anterior = aux; 
      aux = aux -> prox;
    } 
  }

  for (aux = ini; aux != NULL ; aux = aux -> prox) { /*Limpa duplicata de coeficientes*/
    aux_anterior = aux;
    aux_prox = aux -> prox;
    while (aux_prox != NULL) {
      if (aux_prox -> expoente == aux -> expoente) {
        aux -> coeficiente += aux_prox -> coeficiente;
        aux_anterior -> prox = aux_prox -> prox;
        free (aux_prox); 
        aux_prox = aux_anterior -> prox;
      } else {
        aux_anterior = aux_prox;
        aux_prox = aux_prox -> prox;
      }
    }
  }
  return (polinomio) ini;
}

/* implementacao das funcoes da biblioteca polinomios.h */

polinomio cria() {
  Celula *nula = (Celula *) mallocSafe (sizeof(Celula));
  nula -> coeficiente = 0;
  nula -> expoente = 0;
  nula -> prox = NULL;
  return (polinomio) nula;
}

polinomio leia() {
    Celula *anterior = NULL, *nova = cria (), *ini = NULL;
    scanf("%f", &(nova->coeficiente));
    while (nova -> coeficiente != 0) {
      scanf("%d", &(nova -> expoente));
      nova -> prox = NULL;
      if (anterior) { /*Caso não seja o primeiro nó*/
        anterior -> prox = nova;    
      } else { /*Caso seja o primeiro nó*/
        ini = nova;  
      }
      anterior = nova;
      nova = cria();
      scanf("%f", &(nova -> coeficiente));
    }
    free (nova); /*Limpa a célula que guardou o zero que acabou o while*/
    ini = limpa (ini);
    ini = ordena (ini);
    return (polinomio) ini;  
}

polinomio copia(polinomio p) {
  Celula *aux = (Celula *) p, 
  *ini = NULL, *anterior = NULL;
  while (aux) {
    Celula *nova = (Celula *) mallocSafe(sizeof(Celula));
    nova -> expoente = aux -> expoente;
    nova -> coeficiente = aux -> coeficiente;
    nova -> prox = NULL;
    if (anterior) {
      anterior -> prox = nova;
    } else {
      ini = nova;  /*Primeiro nó da lista*/
    }
    anterior = nova;
    aux = aux -> prox; 
  }
  return (polinomio) ini;
} 

void impr(char c, polinomio p) {
  Celula *aux = (Celula *) p;
  if (aux) {
    printf("\n %c(x) =", c); 
    if (aux -> coeficiente < 0) printf(" -"); /*Se o primeiro termo é negativo*/
    if (aux -> coeficiente == 0.0) { /*Polinomio de cria()*/
      printf(" 0");
    } else {
      while (aux) {
        printf(" %.2f", modulo (aux -> coeficiente) );
        if (aux -> expoente != 0) printf(" x^%d", aux -> expoente); /*Não imprime expoente p/ termos independentes*/
        aux = aux -> prox;
        if (aux){ /*Coloca o sinal entre termos*/
          if (aux -> coeficiente >= 0) printf(" +");
          else printf(" -");
        }
      }
    }
    printf("\n");
  }
}

polinomio soma(polinomio p, polinomio q) { /*Assume que os polinômios saiam da leia() em ordem decrescente*/
  Celula *aux_p = (Celula *) p, *aux_q = (Celula *) q, 
  *ini = NULL, *anterior = NULL;
  while (aux_p != NULL || aux_q != NULL) {
     Celula *nova = (Celula *) mallocSafe(sizeof(Celula));
     nova -> prox = NULL;
      if ((aux_p != NULL && aux_q != NULL) && aux_p -> expoente == aux_q -> expoente) { /*Mesmo expoente - soma coeficientes*/
        nova -> expoente = aux_p -> expoente;
        nova -> coeficiente = (aux_p -> coeficiente) + (aux_q -> coeficiente);
        aux_p = aux_p -> prox; aux_q = aux_q -> prox;
      } else if ((aux_q == NULL && aux_p != NULL) || ((aux_p != NULL && aux_q != NULL) && (aux_p -> expoente > aux_q -> expoente))) { 
        /*Adiciona na soma termos cujo o expoente só está presente em p*/
        nova -> expoente = aux_p -> expoente;
        nova -> coeficiente = aux_p -> coeficiente;
        aux_p = aux_p -> prox;
      } else {
        if ((aux_p == NULL && aux_q != NULL) || ((aux_p != NULL && aux_q != NULL) && (aux_p -> expoente < aux_q -> expoente))) { 
          /*Adiciona na soma termos cujo o expoente só está presente em q*/
          nova -> expoente = aux_q -> expoente;
          nova -> coeficiente = aux_q -> coeficiente;
          aux_q = aux_q -> prox;
        }
      }
      if (anterior == NULL) {
        ini = nova; 
      } else {
        anterior -> prox = nova;
      }
      anterior = nova;
    }
    ini = limpa (ini);
    if (ini) ini = ordena (ini);
    else ini = cria (); /*Resultado da soma é zero*/
    return (polinomio) ini;
}

polinomio subt(polinomio p, polinomio q) {
  Celula *aux_p = (Celula *) p, *aux_q = (Celula *) q, 
  *ini = NULL, *anterior = NULL;
  while (aux_p != NULL || aux_q != NULL) {
     Celula *nova = (Celula *) mallocSafe(sizeof(Celula));
     nova -> prox = NULL;
      if ((aux_p != NULL && aux_q != NULL) && aux_p -> expoente == aux_q -> expoente) {
        nova -> expoente = aux_p -> expoente;
        nova -> coeficiente = (aux_p -> coeficiente) - (aux_q -> coeficiente);
        aux_p = aux_p -> prox; aux_q = aux_q -> prox;
      } else if ((aux_q == NULL && aux_p != NULL) || ((aux_p != NULL && aux_q != NULL) && (aux_p -> expoente > aux_q -> expoente))) { /*Assume que lista tem expoentes em ordem decrescente*/
        nova -> expoente = aux_p -> expoente;
        nova -> coeficiente = aux_p -> coeficiente;
        aux_p = aux_p -> prox;
      } else {
        if ((aux_p == NULL && aux_q != NULL) || ((aux_p != NULL && aux_q != NULL) && (aux_p -> expoente < aux_q -> expoente))) {
          nova -> expoente = aux_q -> expoente;
          nova -> coeficiente = - (aux_q -> coeficiente);
          aux_q = aux_q -> prox;
        }
      }
      if (anterior == NULL) {
        ini = nova; 
      } else {
        anterior -> prox = nova;
      }
      anterior = nova;
    }
    ini = limpa (ini);
    if (ini) ini = ordena (ini);
    else ini = cria (); /*Resultado da subtração é zero*/
    return (polinomio) ini;
}

polinomio nega(polinomio p) { /*Troca o sinal dos coeficientes*/
  Celula *aux = (Celula *) p;
  while (aux) {
    aux -> coeficiente = - (aux -> coeficiente);
    aux = aux -> prox;
  }
  return (polinomio) aux; 
}

polinomio mult(polinomio p, polinomio q) {
  Celula *aux_p = (Celula *) p , *aux_q = NULL, 
  *ini = (Celula *) mallocSafe(sizeof(Celula)), *anterior = NULL;
  while (aux_p) {
    for (aux_q = (Celula *) q; aux_q; aux_q = aux_q -> prox) {
      if (anterior) {
        Celula *nova = (Celula *) mallocSafe(sizeof(Celula));
        nova -> expoente = (aux_p -> expoente) + (aux_q -> expoente);
        nova -> coeficiente = (aux_p -> coeficiente) * (aux_q -> coeficiente);
        nova -> prox = NULL;
        anterior -> prox = nova;
        anterior = nova;
      } else {
        ini -> expoente = (aux_p -> expoente) + (aux_q -> expoente);
        ini -> coeficiente = (aux_p -> coeficiente) * (aux_q -> coeficiente);
        ini -> prox = NULL;
        anterior = ini;
      }
    }
    aux_p = aux_p -> prox;
  }
  ini = limpa (ini);
  ini = ordena (ini);
  return (polinomio) ini;
}

polinomio quoc(polinomio p, polinomio q) {
  Celula *aux_p = (Celula *) p, *aux_q = (Celula *) q, *ini = NULL, *anterior = NULL;
  if (aux_p -> expoente < aux_q -> expoente) { /*Retorna um polinomio nulo caso p seja menor que q*/
    ini = cria ();
  } else {
    while ((aux_p && aux_q) && (aux_p -> expoente >= aux_q -> expoente)) { /*Acha o primeiro termo do quociente*/
      Celula *nova = (Celula *) mallocSafe(sizeof(Celula));
      nova -> expoente = (aux_p -> expoente) - (aux_q -> expoente);
      nova -> coeficiente = (aux_p -> coeficiente) / (aux_q -> coeficiente);
      nova -> prox = NULL;
      if (anterior) {
        anterior->prox = nova;
      } else {
        ini = nova;  
      }
        anterior = nova;
        aux_p = subt(aux_p, mult(nova, aux_q)); /* Sobrescreve aux_p com um novo polinômio */
    }
    ini = limpa (ini);
    ini = ordena (ini);
  }
  return (polinomio) ini;
}

polinomio rest(polinomio p, polinomio q) {
  Celula *aux_p = (Celula *) p, *aux_q = (Celula *) q, *ini = NULL, *anterior = NULL;
  if (aux_p -> expoente < aux_q -> expoente) {
    ini = aux_p; 
  } else {
    while ((aux_p && aux_q) && (aux_p -> expoente >= aux_q -> expoente)) {
      Celula *nova = (Celula *) mallocSafe(sizeof(Celula));
      nova -> expoente = (aux_p -> expoente) - (aux_q -> expoente);
      nova -> coeficiente = (aux_p -> coeficiente) / (aux_q -> coeficiente);
      nova->prox = NULL;
      if (anterior) {
        anterior->prox = nova;
      } else {
        ini = nova;  
      }
        anterior = nova;
        aux_p = subt (aux_p, (mult (nova, aux_q)));
    }
    ini = aux_p;
    ini = limpa (ini);
    if (ini) ini = ordena (ini);
    else ini = cria (); 
  }
  return (polinomio) ini;
}

void libera(polinomio p) { /*Libera polinomio*/
  Celula *aux1 = (Celula *) p, *aux2;
  while (aux1) {
    aux2 = aux1 -> prox;
    free(aux1);
    aux1 = aux2;
  }
}

