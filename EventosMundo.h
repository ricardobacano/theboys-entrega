#ifndef _EVENTOS_MUNDO_H
#define _EVENTOS_MUNDO_H

#include "conjunto.h" 
#include "fila.h"     
#include "lef.h"     

/* defines de tempo e tamanho do mundo */ 
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600 /* tempo em que o mundo acaba */
#define N_TAMANHO_MUNDO 20000 /* tamanho do mundo em coordenadas */

/* defines do mundo */
#define N_HABILIDADES 10 /* numero de habilidades possiveis */
#define N_HEROIS (N_HABILIDADES * 5) /* numero de heróis no mundo */
#define N_BASES (N_HEROIS / 6) /* numero de bases no mundo */
#define N_MISSOES (T_FIM_DO_MUNDO / 100) /* numero de missões no mundo */

/* defines para tipos de eventos */
#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MISSAO 8
#define FIM 9
#define TRES_DIAS 4320  

/* struct coordenadas no mundo */
struct coord {
    int x; 
    int y; 
};

/* struct de distancia ate uma base */
struct distancia_t {
    int id_base; 
    int dist;    
};

/* struct para representar um herói */
struct heroi_t { 
    int id;               
    struct conjunto *habilidade; /* conjunto de habilidades do herói */
    int paciencia;        
    int vel;              
    int exp;              
    int id_base;          
};

/* struct de uma base */
struct base_t {
    int id;               
    int lot;              
    struct conjunto *presentes; /* conjunto de heróis presentes na base */
    struct fila *espera;  /* fila de espera dos heróis na base */
    struct coord *local;  /* localização da base */
};

/* struct de uma missão */
struct missao_t {
    int id;               
    struct conjunto *habilidades; /* conjunto de habilidades necessárias para a missão */
    struct coord *local;  /* localização da missão */
};

/* struct para representar o mundo */
struct mundo_t {
    int total_herois;      
    struct heroi_t **herois; /* vetor des para heróis */
    int total_bases;      
    struct base_t **bases; /* vetor des para bases */
    int total_missoes;    
    struct missao_t **missoes; /* vetor des para missões */
    int total_habilidades; 
    struct coord *tamanho; /* tamanho do mundo */
    int relogio;          /* relógio do mundo (tempo atual) */
    int tentativas;       /* numero de tentativas para cumprir missões */
};

/* Funções para inicialização dos heróis, bases e missões */

/**
 * Inicia os heróis no mundo
 *  n_heroi número de heróis a serem iniciados
 * retorna vetor des para heróis
 */
struct heroi_t **iniciar_herois(int n_heroi);

/**
 * Cria e inicializa o mundo
 * retorna para a estrutura do mundo criada
 */
struct mundo_t *criar_mundo();

/**
 * Destrói o mundo e libera a memória alocada
 *  *w para o mundo para ser destruído
 * retorna NULL
 */
struct mundo_t *destroi_mundo(struct mundo_t *w);

/**
 * Inicia as bases no mundo
 *  n_base Número de bases a serem iniciadas
 * retorna vetor des para bases
 */
struct base_t **iniciar_bases(int n_base);

/**
 * Inicia as missões no mundo
 *  n_missao Número de missões a serem iniciadas
 * retorna vetor des para missões
 */
struct missao_t **iniciar_missao(int n_missao);

/**
 * Inicia o mundo com heróis, bases e missões
 *  *w para o mundo a ser iniciado
 * retorna para o mundo iniciado
 */
struct mundo_t *iniciar_mundo(struct mundo_t *w);

/* Função para simular o mundo */

/**
 * Simula o mundo processando os eventos na lef
 *  *w para o mundo a ser simulado
 *  *l para a lef
 *  ev para o evento atual
 */
void simula_mundo(struct mundo_t *w, struct lef_t *l, struct evento_t *ev);

/* Funções auxiliares */

/**
 * Gera um número aleatório entre min e max
 *  min Valor mínimo
 *  max Valor máximo
 * retorna Número aleatório entre min e max
 */
int aleat(int min, int max);

/**
 * Obtém o valor do relógio do mundo
 *  *w para o mundo
 * retorna Valor do relógio do mundo
 */
int relogio_mundo(struct mundo_t *w);

/**
 * Atualiza o relógio do mundo com base no tempo do evento
 *  *w para o mundo
 *  *ev para o evento
 * retorna 1 se bem-sucedido, -1 caso contrário
 */
int atualiza_relogio_mundo(struct mundo_t *w, struct evento_t *ev);

/* Funções para manipulação de distancias */ 

/**
 * Ordena as distancias usando o algoritmo insertion sort
 *  *d vetor de distancias a ser ordenado
 *  n Tamanho do vetor
 */
void insertionSort(struct distancia_t *d, int n);

/**
 * Calcula a distancia entre a missão e as bases
 *  *w para o mundo
 *  *m para a missão
 *  *d vetor de distancias a ser preenchido
 */
void calcula_distancia_missao(struct mundo_t *w, struct missao_t *m, struct distancia_t *d);

/* Funções para tratamento de eventos */

/**
 * Trata o evento de chegada de um herói a uma base
 *  *l para a lef
 *  tempo Tempo do evento
 *  *h para o herói
 *  *b para a base
 */
void trata_evento_chega(struct lef_t *l, int tempo, struct heroi_t *h, struct base_t *b);

/**
 * Trata o evento de espera de um herói em uma base
 *  *l para a lef
 *  tempo Tempo do evento
 *  *h para o herói
 *  *b onteiro para a base
 */
void trata_evento_espera(struct lef_t *l, int tempo, struct heroi_t *h, struct base_t *b);

/**
 * Trata o evento de desistência de um herói em uma base
 *  *l para a lef
 *  tempo Tempo do evento
 *  *h para o herói
 *  *b para a base
 */
void trata_evento_desiste(struct lef_t *l, int tempo, struct heroi_t *h, struct base_t *b);

/**
 * Trata o evento de aviso em uma base
 *  *l para a lef
 *  tempo Tempo do evento
 *  *b para a base
 */
void trata_evento_avisa(struct lef_t *l, int tempo, struct base_t *b);

/**
 * Trata o evento de saída de um herói de uma base
 *  *l para a lef
 *  tempo Tempo do evento
 *  *h para o herói
 *  *b para a base
 */
void trata_evento_sai(struct lef_t *l, int tempo, struct heroi_t *h, struct base_t *b);

/**
 * Trata o evento de entrada de um herói em uma base
 *  *l para a lef
 *  tempo Tempo do evento
 *  *h para o herói
 *  *b para a base
 */
void trata_evento_entra(struct lef_t *l, int tempo, struct heroi_t *h, struct base_t *b);

/**
 * Trata o evento de viagem de um herói entre bases
 *  *l para a lef
 *  tempo Tempo do evento
 *  *h para o herói
 *  *b para a base de destino
 *  *w para o mundo
 */
void trata_evento_viaja(struct lef_t *l, int tempo, struct heroi_t *h, struct base_t *b, struct mundo_t *w);

/**
 * Trata o evento de missão
 *  *l para a lef
 *  tempo Tempo do evento
 *  *m para a missão
 *  *w para o mundo
 */
void trata_evento_missao(struct lef_t *l, int tempo, struct missao_t *m, struct mundo_t *w);

/**
 * Trata o evento de fim de simulação
 *  tempo Tempo do evento
 *  *w para o mundo
 *  *l para a lef
 */
void trata_evento_fim(int tempo, struct mundo_t *w, struct lef_t *l);

/* Função para criar eventos iniciais */

/**
 * Cria os eventos iniciais no mundo
 *  *w para o mundo
 *  *l para a lef
 * retorna para a lef com os eventos iniciais inseridos
 */
struct lef_t *cria_eventos_iniciais(struct mundo_t *w, struct lef_t *l);

#endif
