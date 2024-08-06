#include <stdlib.h>
#include <stdio.h>
#include "fila.h"

/* 
 * Cria e retorna uma nova fila.
 * Retorna NULL em caso de erro de alocação.
*/
struct fila *fila_cria () {
    struct fila *f = (struct fila *) malloc(sizeof(struct fila));
    if (f == NULL) 
        return NULL;
    
    f->ini = NULL;
    f->fim = NULL;
    f->tamanho = 0;
    
    return f;
}

/* Desaloca toda memoria da fila e faz fila receber NULL. */
void fila_destroi (struct fila **fila) {
    if (fila == NULL || *fila == NULL) 
        return;
    
    struct nodo *atual = (*fila)->ini;
    while (atual != NULL) {
        struct nodo *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    
    free(*fila);
    *fila = NULL;
}

/* 
 * Insere dado na fila (politica FIFO). Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int enqueue (struct fila *fila, int dado) {
    if (fila == NULL) 
        return 0;
    
    struct nodo *no_novo = (struct nodo *) malloc(sizeof(struct nodo));
    if (no_novo == NULL) 
        return 0;
    
    no_novo->chave = dado;
    no_novo->prox = NULL;
    
    if (fila->fim == NULL) {
        fila->ini = no_novo;
    } else
        /* liga o ultimo nodo atual ao nodo novo*/
        fila->fim->prox = no_novo;
    
    fila->fim = no_novo;
    fila->tamanho++;
    
    return 1;
}

/* 
 * Remove dado da fila (politica FIFO) e retorna o elemento 
 * no parametro dado. A funcao retorna 1 em caso de 
 * sucesso e 0 no caso da fila estar vazia.
*/
int dequeue (struct fila *fila, int *dado) {
    if (fila == NULL || fila->ini == NULL) 
        return 0;
    
    struct nodo *temp = fila->ini;  
    *dado = temp->chave;  
    
    /* vai parao o inicio do proximo nodo */
    fila->ini = temp->prox;  
    if (fila->ini == NULL) {
        fila->fim = NULL;  
    }
    
    free(temp);  
    fila->tamanho--;  
    
    return 1;
}

/* Retorna o numero de elementos da pilha, que pode ser 0. */
int fila_tamanho (struct fila *fila) {
    if (fila == NULL) 
        return 0;
    return fila->tamanho;
}

/* Retorna 1 se fila vazia, 0 em caso contrario. */ 
int fila_vazia (struct fila *fila) {
    if (fila == NULL) 
        return 1;
    return (fila->tamanho == 0);
}

/* Imprime a fila no formato "[ e1 e2 ... en ]" 
 * Se a fila for vazia imprime "[ ]" */
void fila_imprime (struct fila *fila) {
    if (fila == NULL || fila->ini == NULL) {
        printf("[ ]\n");
        return;
    }

    struct nodo *atual = fila->ini;
    printf("[");
    while (atual != NULL) {
        if (atual->prox != NULL) {
            printf(" %d", atual->chave);
        } else {
            printf(" %d ", atual->chave);
        }
        atual = atual->prox;
    }
    printf("]\n");
}