/*
Funcao copiada do livro Algoritmos em C do Professor Paulo Feofiloff
(Capitulo 6. Pilhas, p. 42) 
*/

/* A função abaixo recebe uma expressão infixa infix e 
* devolve a correspondente expressão posfixa. */ 

char *InfixaParaPosfixa (char infix []) {
    char *posfix, x;
    char *p; int t;
    int n, i, j;
    n = strlen (infix);
    posfix = malloc (n * sizeof (char));
    p = malloc (n * sizeof (char));
    t = 0; p[t++] = infix[0]; /* empilha '(' */
    for (j = 0, i = 1; /*X*/ infix[i] != '\0'; i++) { 
        /* p[0..t-1] é uma pilha de caracteres */ 
        switch (infix[i]) {
            case '(': p[t++] = infix[i]; /* empilha */
                break;
            case ')': while (1) { /* desempilha */
                        x = p[--t];
                        if (x == '(') break; 
                        posfix[j++] = x; }
                break;
            case '+':
            case '-': while (1) {
                        x = p[t-1];
                        if (x == '(') break;
                        --t; /* desempilha */
                        posfix[j++] = x; }
                p[t++] = infix[i]; /* empilha */ 
                break;
            case '*':
            case '/': while (1) {
                        x = p[t-1];
                        if (x == '(' || x == '+' || x == '-')
                            break;
                        --t;
                        posfix[j++] = x; }
                p[t++] = infix[i];
                break;
            default: posfix[j++] = infix[i]; }
    }
    free (p);
    posfix[j] = '\0';
    return posfix;
}