#include <stdlib.h>
#include <stdio.h>
#include "lef.h"

/* 
 * Cria um evento contendo os parâmetros informados.
 * Retorna um ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *cria_evento (int tempo, int tipo, int dado1, int dado2) {
    struct evento_t *e = (struct evento_t *)malloc(sizeof(struct evento_t));
    if (e == NULL) 
        return NULL;

    e->tempo = tempo;
    e->tipo = tipo;
    e->dado1 = dado1;
    e->dado2 = dado2;

    return e;
}
 
/* 
 * Destroi um evento e retorna NULL.
 */
struct evento_t *destroi_evento (struct evento_t *e) {
    if (e != NULL) {
        free(e);
    }
    return NULL;
}

/* 
 * Cria uma LEF vazia 
 * Retorna um ponteiro para a LEF ou NULL se falhar.
 */
struct lef_t *cria_lef () {
    struct lef_t *l = (struct lef_t *)malloc(sizeof(struct lef_t));
    if (l == NULL) 
        return NULL;

    l->primeiro = NULL;
    return l;
}

/* 
 * Destroi a LEF, liberando a memória ocupada por ela, inclusive os eventos.
 * Retorna NULL.
 */ 
struct lef_t *destroi_lef (struct lef_t *l) {
    if (l != NULL) {
        struct nodo_lef_t *atual = l->primeiro;
        while (atual != NULL) {
            struct nodo_lef_t *temp = atual;
            atual = atual->prox;
            destroi_evento(temp->evento);
            free(temp);
        }
        free(l);
    }
    return NULL;
}

/*
 * Insere o evento apontado na LEF na ordem de tempos crescentes.
 * Eventos com o mesmo tempo devem respeitar a politica FIFO.
 * Retorna 1 em caso de sucesso ou 0 caso contrario.
*/
int insere_lef (struct lef_t *l, struct evento_t *e) {
    if (l == NULL || e == NULL) 
        return 0;

    struct nodo_lef_t *no_novo = (struct nodo_lef_t *)malloc(sizeof(struct nodo_lef_t));
    if (no_novo == NULL) 
        return 0;

    no_novo->evento = e;
    no_novo->prox = NULL;

    /* insere no inicio ou em uma lista vazia */
    if (l->primeiro == NULL || l->primeiro->evento->tempo > e->tempo) {
        no_novo->prox = l->primeiro;
        l->primeiro = no_novo;
    } else {
        struct nodo_lef_t *atual = l->primeiro;
        /* procura a posicao correta para inserir */
        while (atual->prox != NULL && atual->prox->evento->tempo <= e->tempo) {
            atual = atual->prox;
        }
        no_novo->prox = atual->prox;
        atual->prox = no_novo;
    }

    return 1;
}

/* 
 * Retira o primeiro evento da LEF.
 * Retorna ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *retira_lef (struct lef_t *l) {
    if (l == NULL || l->primeiro == NULL) 
        return NULL;

    struct nodo_lef_t *temp = l->primeiro;
    struct evento_t *e = temp->evento;

    l->primeiro = temp->prox;
    free(temp);

    return e;
}

/* 
 * Informa se a LEF está vazia.
 * Retorna 1 se vazia e 0 senao ou se falhar.
 */
int vazia_lef (struct lef_t *l) {
    if (l == NULL) 
        return 1;
    return (l->primeiro == NULL);
}

/* 
 * Imprime a LEF. Não faz parte do TAD, mas serve para depuração.
 * Formato de saída:
 * tempo tipo dado1 dado2
 * tempo tipo dado1 dado2
 * ...
 * total XX eventos
 */
void imprime_lef(struct lef_t *l) {
    if (!l) return;

    struct nodo_lef_t *atual = l->primeiro;
    int contador = 0;
    while (atual) {
        printf("tempo %d tipo %d dado1 %d dado2 %d\n", atual->evento->tempo, atual->evento->tipo, atual->evento->dado1, atual->evento->dado2);
        atual = atual->prox;
        contador++;
    }
    printf("total %d eventos\n", contador);
}
