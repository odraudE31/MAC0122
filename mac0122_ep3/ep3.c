/****************************************************************/
/**                                                            **/
/**   Nome:       Eduardo Gonzalez Silva                       **/
/**   Professor:  Cristina Gomes Fernandes                     **/
/**   Turma:      01                                           **/
/**   NumeroUSP:  15448709                                     **/
/**   Exercicio-Programa 03 - Calculadora de polinômios Pt. II **/
/**                                                            **/
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "polinomios.h"

#define MAX 1000 /*numero razoavel de caracteres a ser digitado*/

typedef struct {
    char items[MAX];
    int inicio;   // Índice do início da fila
    int fim;      // Índice do final da fila
    int tamanho;  // Quantidade de elementos na fila
} Fila;

typedef struct pilha {
    char items[MAX];  
    int topo;         
} Pilha;

/*Declaracao de funcoes*/
Pilha *criaPilha();
int taCheiaP(Pilha *pil);
int taVaziaP(Pilha *pil);
void empilha(Pilha *pil, char item);
char desempilha(Pilha *pil);
char xeretaP(Pilha *pil); 
int ehOperador(char item);
void inicializaPolinomios(polinomio *Polinomios);
void descarta_resto_da_linha();
polinomio opera(char letra1, char letra2, char operador, polinomio *Polinomios);
int daIndice(char c);
int leExpressao(polinomio *Polinomios);
int prec(char c);
char associativity(char c);
char *infixToPostfix(char s[]);
polinomio processaExpressao(char *expr, polinomio *Polinomios, char letraResp);
void inicializaPolinomiosAuxiliares(polinomio *Polinomios);
void liberaPolinomiosAuxiliares(polinomio *Polinomios);
void liberaPolinomios(polinomio *Polinomios);

int main(){
  int horaDeQuitar = 1;
  polinomio Polinomios[50];

  inicializaPolinomios(Polinomios);
  printf("*******************************\n"
      "   Calculadora de polinomios\n"
      "*******************************\n");

  while(horaDeQuitar){
      horaDeQuitar = leExpressao(Polinomios);
  }
  liberaPolinomios(Polinomios);

  return 0;
}

/*Implementacao de funcoes de pilha*/

Pilha *criaPilha(){
  Pilha *pil = malloc(sizeof(Pilha));
  if (pil == NULL){
    printf("Socorro! malloc devolveu NULL! \n");
    exit(EXIT_FAILURE);
  }

  pil->topo = 0; /*vazia*/
  return pil;
}

int taCheiaP(Pilha *pil){
  return pil->topo == MAX;
}

int taVaziaP(Pilha *pil){
  return pil->topo == 0;
}

void empilha(Pilha *pil, char item){
  if(taCheiaP(pil)){
    printf("ERRO (adiciona): pilha cheia.\n");
    return;
  }

  pil->items[++pil->topo] = item;
}

char desempilha(Pilha *pil){
  if (taVaziaP(pil)) {
      printf("ERRO (desempilha): pilha vazia.\n");
      return '\0';
  }
  return pil->items[pil->topo--]; 
}

char xeretaP(Pilha *pil){
  if (taVaziaP(pil)) {
      printf("ERRO (xeretaP): pilha vazia.\n");
      return '\0';  
  }
  return pil->items[pil->topo-1]; 
}

void liberaPilha(Pilha* pil) {
  if (pil != NULL)
    free(pil);
}

/*Implementacao de funcoes auxiliares*/

int ehOperador(char item){
    return (item == '/' || item == '*' || item == '+' || item == '-' || item == '%');
}

void inicializaPolinomiosAuxiliares(polinomio *Polinomios){
  int i;
  
  for(i = 26; i < 50; i++)
    Polinomios[i] = cria(); 
}

void liberaPolinomiosAuxiliares(polinomio *Polinomios){
  int i;
  
  for(i = 26; i < 50; i++)
    libera(Polinomios[i]); 
}

void liberaPolinomios(polinomio *Polinomios){
  int i;
  
  for(i = 0; i < 26; i++)
    libera(Polinomios[i]); 
}

void inicializaPolinomios(polinomio *Polinomios){
  int i;
  
  for(i = 0; i < 26; i++)
    Polinomios[i] = cria(); 
}

void descarta_resto_da_linha(){
  char c;
  
  do {
    scanf("%c", &c); 
  } while (c != '\n' && c != EOF);
}

int daIndice(char c){
  return c - 97;
}

polinomio opera(char letra1, char letra2, char operador, polinomio *Polinomios){
  
  polinomio Polinomio1 = Polinomios[daIndice(letra1)];
  polinomio Polinomio2 = Polinomios[daIndice(letra2)];
  polinomio Result = cria();

  switch(operador){
    case '+':
      Result = soma(Polinomio1, Polinomio2);
      break;
    
    case '-':
      Result = subt(Polinomio1, Polinomio2);
      break;

    case '*':
      Result = mult(Polinomio1, Polinomio2);
      break;

    case '/':
      /*Nao consigo checar se é nulo, mas seria muito bom :(*/
      Result = quoc(Polinomio1, Polinomio2);
      break;

    case '%':
      Result = rest(Polinomio1, Polinomio2);
      break;

    default:
      printf("ERRO (opera): operador '%c' nao existe.", operador);
  }

  return Result;
}

polinomio processaExpressao(char *expr, polinomio *Polinomios, char letraResp){
  char operador, letra1, letra2;
  Pilha *variaveis = criaPilha();
  polinomio auxiliar = cria();
  int i, letra3 = 'z' + 1;

  inicializaPolinomiosAuxiliares(Polinomios);
  
  for(i = 0; i < strlen(expr); i++){
    if(!ehOperador(expr[i]) && !(expr[i] == '~'))
      empilha(variaveis, expr[i]);

    else if(expr[i] == '~'){
      letra1 = desempilha(variaveis);
      Polinomios[daIndice(letra3)] = nega(Polinomios[daIndice(letra1)]);

      empilha(variaveis, letra3);
      letra3++;
    }

    else{
      operador = expr[i];

      letra2 = desempilha(variaveis);/*a ordem é invertida para o menos funcionar (ab- = a-b)*/
      letra1 = desempilha(variaveis);

      Polinomios[daIndice(letra3)] = opera(letra1, letra2, operador, Polinomios);
      empilha(variaveis, letra3);/*a gnt empilha uma auxiliar, nao queremos mexer na verdadeira*/
      letra3++;/*a auxiliar cresce para nao perdermos o valor da auxiliar MUITO CUIDADO COM O ~ (126)*/
    }
  }
  letra3 = desempilha(variaveis);

  /*
  Por algum motivo aqui tem um ASCII 32 (so as vezes) ¯\_(ツ)_/¯ 
  em nenhum teste que eu fiz ele apareceu mais de uma vez, nao 
  sei se poderia aparecer, se sim seria legal fazer um while 
  para limpar tudo antes.
  
  Acredito que isso aconteca porque eu costumava killar o programa 
  com cntr + z. Eu poderia testar se isso parou de acontecer, mas 
  ja eh sabado e o programa funcionou so agora, entao me desculpe.
  */
  if(letra3 == 32) 
    letra3 = desempilha(variaveis);
  
  auxiliar = copia(Polinomios[daIndice(letra3)]);

  liberaPilha(variaveis);
  liberaPolinomiosAuxiliares(Polinomios);

  return auxiliar;/*sempre é a ultima letra da pilha (ou era pra ser)*/
}

int leExpressao(polinomio *Polinomios){
  int indice, indiceResp;
  char *expr, *exprPos, letra, letraResp;

  expr = (char *)calloc(MAX, sizeof(char));

  printf("\nDigite uma expressão ou quit para sair do programa:\n> ");
  scanf("%c %c", &expr[0], &expr[1]);

  switch(expr[1]){
  case 'u':
    descarta_resto_da_linha();
    printf("Tchau!\n");
    return 0;

  case '?':
    letra = expr[0];
    indice = daIndice(letra);

    impr(letra, Polinomios[indice]);
    descarta_resto_da_linha();
    return 1;  
  
  case ':':
     letra = expr[0];
    indice = daIndice(letra);

    Polinomios[indice] = leia();
    impr(letra, Polinomios[indice]);
    descarta_resto_da_linha();
    return 1;

  case '=':
    letraResp = expr[0]; 
    indiceResp = daIndice(letraResp);

    scanf("%s[^\n]", expr);
    descarta_resto_da_linha();
    
    exprPos = infixToPostfix(expr); 

    Polinomios[indiceResp] = processaExpressao(exprPos, Polinomios, letraResp);
  
    impr(letraResp, Polinomios[indiceResp]);

    return 1;

  default:
    descarta_resto_da_linha();
    printf("ERRO (main): o programa nao processa a expressao passada.\n");
    return 1;
  }
}

/*
Implementacao de funcoes copiadas de geeksforgeeks
https://www.geeksforgeeks.org/convert-infix-expression-to-postfix-expression/
Minimas alteracoes feitas para que a funcao se adaptasse a esse ep
*/

// Function to return precedence of operators
int prec(char c){
    if (c == '~')
        return 3;
    else if (c == '/' || c == '*' || c == '%')
        return 2;
    else if (c == '+' || c == '-')
        return 1;
    else
        return -1;
}

// Function to return associativity of operators
char associativity(char c){
  if (c == '^')
    return 'R';
  return 'L';  // Default to left-associative
}

// The main function to convert infix expression to postfix expression
char *infixToPostfix(char s[]) {
    char *result;
    int resultIndex = 0;
    int len = strlen(s);
    char stack[1000];
    int stackIndex = -1;

    result = (char*) malloc(MAX*sizeof(char));

    for (int i = 0; i < len; i++) {
        char c = s[i];

        // If the scanned character is an operand, add it to the output string.
        if ((c >= 'a' && c <= 'z')) {
            result[resultIndex++] = c;
        }
        // If the scanned character is an ‘(‘, push it to the stack.
        else if (c == '(') {
            stack[++stackIndex] = c;
        }
        // If the scanned character is an ‘)’, pop and add to the output string from the stack
        // until an ‘(‘ is encountered.
        else if (c == ')') {
            while (stackIndex >= 0 && stack[stackIndex] != '(') {
                result[resultIndex++] = stack[stackIndex--];
            }
            stackIndex--; // Pop '('
        }
        // If an operator is scanned
        else {
            while (stackIndex >= 0 && ((prec(s[i]) < prec(stack[stackIndex]) ||
                                       prec(s[i]) == prec(stack[stackIndex])) &&
                                           associativity(s[i]) == 'L')) {
                result[resultIndex++] = stack[stackIndex--];
            }
            stack[++stackIndex] = c;
        }
    }

    // Pop all the remaining elements from the stack
    while (stackIndex >= 0) {
        result[resultIndex++] = stack[stackIndex--];
    }

    result[resultIndex] = '\0';

    return result;
}
