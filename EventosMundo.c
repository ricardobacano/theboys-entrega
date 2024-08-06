#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "fila.h"
#include "lef.h"
#include "EventosMundo.h"

int aleat(int min, int max) {
    return rand() % (max - min + 1) + min;
}

/* funcao para criar e inicializar um heroi */
struct heroi_t *criar_heroi(int id_heroi) {
    struct heroi_t *h = malloc(sizeof(struct heroi_t));
    if (!h)
        return NULL;

    h->id = id_heroi; /* atribui ID do heroi */
    h->exp = 0; /* inicializa o xp do herói */
    h->paciencia = aleat(0, 100); /* paciência do herói */
    h->vel = aleat(50, 5000); /* velocidade do herói */
    h->habilidade = cria_cjt(N_HABILIDADES); /* cria conjunto de habilidades do herói */ 
    h->id_base = -1; /* inicializa o *id_base do herói (supondo que esse campo exista e precisa ser inicializado) */
    
    if (h->habilidade == NULL) {
        free(h);
        return NULL;
    }

    /* atribui habilidades aleatórias ao herói */
    int tam = aleat(1, 3);
    while (cardinalidade_cjt(h->habilidade) < tam)
        insere_cjt(h->habilidade, aleat(0, N_HABILIDADES - 1));
    return h;
}


/* funcao para criar e inicializar uma base */
struct base_t *criar_base(int id_base) {
    struct base_t *b = malloc(sizeof(struct base_t));
    b->local = malloc(sizeof(struct coord));
    if (!b || !b->local)
        return NULL;

    b->id = id_base; /* id da base */
    b->local->x = aleat(0, N_TAMANHO_MUNDO); /* coordenada x aletoria */
    b->local->y = aleat(0, N_TAMANHO_MUNDO); /* coordenada y aletoria */ 
    b->lot = aleat(3, 10); /* define a lotacao da base */ 
    b->presentes = cria_cjt(N_HEROIS); /* herois na base */
    b->espera = fila_cria(); /* cria fila de espera da base */

    if (b->presentes == NULL || b->espera == NULL) {
        free(b->local);
        free(b);
        return NULL;
    }

    return b;
}

/* Função para criar e inicializar uma missão */
struct missao_t *criar_missao(int id_missao) {
    struct missao_t *m = malloc(sizeof(struct missao_t));
    m->local = malloc(sizeof(struct coord));
    if (!m || !m->local)
        return NULL;

    m->id = id_missao; /* id da missão */
    m->local->x = aleat(0, N_TAMANHO_MUNDO); /* coordenada x aletoria */
    m->local->y = aleat(0, N_TAMANHO_MUNDO); /* coordenada y aletoria */
    m->habilidades = cria_cjt(N_HABILIDADES); /* cria conjunto de habilidades necessarias pela missão */

    if (m->habilidades == NULL) {
        free(m->local);
        free(m);
        return NULL;
    }

    /* atribui habilidades necessarias para a missço 
        habilidades aleatorias */
    int tam = aleat(6, 10);
    while (cardinalidade_cjt(m->habilidades) < tam)
        insere_cjt(m->habilidades, aleat(0, N_HABILIDADES - 1));
    return m;
}

/* Função para criar e inicializar o mundo */
struct mundo_t *criar_mundo() {
    struct mundo_t *w = malloc(sizeof(struct mundo_t));
    w->tamanho = malloc(sizeof(struct coord));
    if (!w || !w->tamanho)
        return NULL;

    w->total_herois = N_HEROIS; /* número total de heróis */
    w->herois = NULL;
    w->total_bases = N_BASES; /* número total de bases */
    w->bases = NULL;
    w->total_missoes = N_MISSOES; /* número total de missões */
    w->missoes = NULL;
    w->total_habilidades = N_HABILIDADES; /* número total de habilidades */
    w->tamanho->x = N_TAMANHO_MUNDO; /* tamanho do mundo por x */
    w->tamanho->y = N_TAMANHO_MUNDO; 
    w->relogio = 0; /* inicializa o relógio do mundo */
    w->tentativas = 0; /* inicializa o número de tentativas das missões */
    return w;
}


/* Função para iniciar heróis */
struct heroi_t **iniciar_herois(int n_heroi) {
    struct heroi_t **h = malloc(n_heroi * sizeof(struct heroi_t *));
    if (!h)
        return NULL;
    for (int i = 0; i < n_heroi; i++) {
        h[i] = criar_heroi(i); /* cria cada herói e o armazena no vetor */
    }
    return h;
}

/* Função para iniciar bases */
struct base_t **iniciar_bases(int n_base) {
    struct base_t **b = malloc(n_base * sizeof(struct base_t *));
    if (!b)
        return NULL;
    for (int i = 0; i < n_base; i++) {
        b[i] = criar_base(i); /* cria as bases e a armazena no vetor */
    }
    return b;
}

/* Função para iniciar missões */
struct missao_t **iniciar_missoes(int n_missao) {
    struct missao_t **m = malloc(n_missao * sizeof(struct missao_t *));
    if (!m)
        return NULL;
    for (int i = 0; i < n_missao; i++) {
        m[i] = criar_missao(i); /* cria as missões e a armazena no vetor */
    }
    return m;
}

/* Função para imprimir o estado do mundo */
void imprime_mundo(struct mundo_t *w) {
    if (!w)
        return;
    for (int i = 0; i < w->total_herois; i++) {
        printf("Heroi id: %d\n", w->herois[i]->id);
        imprime_cjt(w->herois[i]->habilidade);
    }
    for (int i = 0; i < w->total_bases; i++) {
        printf("Base id: %d\n", w->bases[i]->id);
        imprime_cjt(w->bases[i]->presentes);
    }
    for (int i = 0; i < w->total_missoes; i++) {
        printf("Missao id: %d\n", w->missoes[i]->id);
        imprime_cjt(w->missoes[i]->habilidades);
    }
}

/* Função para iniciar o mundo com heróis, bases e missões */
struct mundo_t *iniciar_mundo(struct mundo_t *w) {
    if (!w)
        return NULL;
    w->herois = iniciar_herois(w->total_herois); 
    w->bases = iniciar_bases(w->total_bases); 
    w->missoes = iniciar_missoes(w->total_missoes); 
    return w;
}

/* Função para destruir o mundo e liberar memória */
struct mundo_t *destroi_mundo(struct mundo_t *w) {
    if (!w)
        return NULL;
    for (int i = 0; i < w->total_herois; i++) {
        destroi_cjt(w->herois[i]->habilidade); /* destroi habilidades do herói */
        free(w->herois[i]); 
    }
    for (int i = 0; i < w->total_bases; i++) {
        destroi_cjt(w->bases[i]->presentes); /* destroi o conjunto de heróis na base */
        fila_destroi(&w->bases[i]->espera); /* destroi fila de espera da base */
        free(w->bases[i]->local); /* coordenadas da base */
        free(w->bases[i]); 
    }
    for (int i = 0; i < w->total_missoes; i++) {
        destroi_cjt(w->missoes[i]->habilidades); /* destroi as hab da missão */
        free(w->missoes[i]->local); /* coordenadas da missão */
        free(w->missoes[i]); 
    }
    free(w->missoes); 
    free(w->herois); 
    free(w->bases); 
    free(w->tamanho); 
    free(w); 
    return NULL;
}

/* Função para obter o valor do relógio do mundo */
int relogio_mundo(struct mundo_t *w) {
    if (!w)
        return -1;
    return w->relogio; 
}

/* Função para atualizar o relógio do mundo */
int atualiza_relogio_mundo(struct mundo_t *w, struct evento_t *ev) {
    if (!w || !ev)
        return -1;
    w->relogio = ev->tempo; /* atualiza o relógio com o tempo do evento */
    return 1;
}

/* Função para ordenar as distâncias */
void insertionSort(struct distancia_t *d, int n) {
    for (int i = 1; i < n; i++) {
        struct distancia_t tmp = d[i];
        int j = i - 1;
        while (j >= 0 && d[j].dist > tmp.dist) {
            d[j + 1] = d[j];
            j--;
        }
        d[j + 1] = tmp;
    }
}

/* Função para calcular a distância entre a missão e as bases */
void calcula_distancia_missao(struct mundo_t *w, struct missao_t *m, struct distancia_t *d) {
    if (!w || !d)
        return;
    for (int i = 0; i < w->total_bases; i++) {
        /* calcula a distância euclidiana entre a missão e cada base */
        d[i].dist = sqrt(pow(m->local->x - w->bases[i]->local->x, 2) 
                    + pow(m->local->y - w->bases[i]->local->y, 2));
        d[i].id_base = i;
    }
}

/* Função de evento de missão */
void trata_evento_missao(struct lef_t *l, int tempo, struct missao_t *m, struct mundo_t *w) {
    if (!l || !m || !w)
        return;

    struct evento_t *ev;
    struct conjunto *hab;
    
    /* vetor de dist, com tamanho igual aas bases no mundo */
    struct distancia_t *dist = malloc(w->total_bases * sizeof(struct distancia_t));
    if (!dist) {
        fprintf(stderr, "Erro de alocação de memória\n");
        return;
    }
    
    calcula_distancia_missao(w, m, dist);

    int bmp = -1; /* base mais próxima */
    
    printf("%6d: MISSAO %d HAB REQ:", tempo, m->id);
    imprime_cjt(m->habilidades);
    
    insertionSort(dist, w->total_bases);

    for (int i = 0; i < w->total_bases; i++) {
        hab = cria_cjt(N_HABILIDADES);
        if (!hab) {
            fprintf(stderr, "Erro de alocação de memória\n");
            free(dist);
            return;
        }
        
        printf("%6d: MISSAO %d HAB BASE %d:", tempo, m->id, w->bases[dist[i].id_base]->id);
        
        for (int j = 0; j < w->total_herois; j++) {
            /* verifica se o herói está na base  */
            if (pertence_cjt(w->bases[dist[i].id_base]->presentes, w->herois[j]->id)) {
                /* atualiza o conjunto de habilidades dos heróis na base */
                struct conjunto *novo_hab = uniao_cjt(hab, w->herois[j]->habilidade);
                destroi_cjt(hab);
                hab = novo_hab;
            }
        }
        
        imprime_cjt(hab);
        
        /* verifica o conjunto de habilidades da missão ⊂ no conjunto de habilidades acumuladas na base */
        if (contido_cjt(m->habilidades, hab)) {
            bmp = dist[i].id_base; 
            
            /* aumenta a experiência dos heróis na base */
            for (int i = 0; i < w->total_herois; i++) {
                if (w->herois[i]->id_base == bmp)
                    w->herois[i]->exp++;
            }
            
            printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROIS:", tempo, m->id, bmp);
            imprime_cjt(w->bases[bmp]->presentes);
            
            destroi_cjt(hab);
            free(dist);
            return;
        }
        
        destroi_cjt(hab);
    }
    
    if (bmp == -1) {
        /* evento novo para a missão depois de 24 horas */
        ev = cria_evento(tempo + 24 * 60, MISSAO, m->id, 0);
        insere_lef(l, ev);
        
        printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, m->id);
        
        /* incrementa as tentativas de cumprir missões no mundo */
        w->tentativas++;
    }
    
    free(dist);
}


/* Função de evento de viagem */
void trata_evento_viaja(struct lef_t *l, int tempo, struct heroi_t *h, struct base_t *b, struct mundo_t *w) {
    if (!l || !h || !b)
        return;
    struct evento_t *ev;
    /* calcula a distância euclidiana entre a base atual do herói e a base de destino */
    int dist = sqrt(pow(w->bases[h->id_base]->local->x - b->local->x, 2) + pow(w->bases[h->id_base]->local->y - b->local->y, 2));
    int duracao = dist / h->vel; /* calculo da duração usando distancia pela velocidade do heroi */
    ev = cria_evento(tempo + duracao, CHEGA, h->id, b->id);
    insere_lef(l, ev);
    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", 
        tempo, 
        h->id, 
        w->bases[h->id_base]->id,
        b->id, 
        dist,
        h->vel, 
        duracao + tempo);
}

/* Função de evento de aviso */
void trata_evento_avisa(struct lef_t *l, int tempo, struct base_t *b) {
    if (!l || !b)
        return;

    int id_heroi;
    struct evento_t *ev;
    
    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA", 
        tempo, 
        b->id, 
        cardinalidade_cjt(b->presentes), 
        b->lot);
    
    fila_imprime(b->espera);

    /* laço pra entrada de heróis na base 
    (disponibilidades: espaço na base e heróis na fila de espera) */
    while ((cardinalidade_cjt(b->presentes) < b->lot) && (!fila_vazia(b->espera))) {

        /* remove um herói da fila de espera */
        dequeue(b->espera, &id_heroi);
        
        /* insere o herói no conjunto de heróis presentes na base */
        insere_cjt(b->presentes, id_heroi);
        
        /* evento novo pra entrada do herói na base admitida */
        ev = cria_evento(tempo, ENTRA, id_heroi, b->id);
        
        /* insere o evento de entrada na lef */
        insere_lef(l, ev);
        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", tempo, b->id, id_heroi);
    }
}


/* Função para o evento de entrada */
void trata_evento_entra(struct lef_t *l, int tempo, struct heroi_t *h, struct base_t *b) {
    if (!h || !b)
        return;
    int tpb; 
    struct evento_t *ev;
    insere_cjt(b->presentes, h->id); /* adiciona o herói na lista de presentes na base */
    tpb = 15 + h->paciencia * aleat(1, 20); /* calcula o tempo de permanência na base */
    ev = cria_evento(tempo + tpb, SAI, h->id, b->id); /* evento de saída para o herói na base */
    insere_lef(l, ev);
    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", 
        tempo, 
        h->id, 
        b->id, 
        cardinalidade_cjt(b->presentes), 
        b->lot, 
        tpb + tempo);
}

/* Função para o evento de saída */
void trata_evento_sai(struct lef_t *l, int tempo, struct heroi_t *h, struct base_t *b) {
    if (!h || !b)
        return;
    struct evento_t *ev;
    int dest_aleat;
    retira_cjt(b->presentes, h->id); /* remove o herói da lista de presentes na base */
    dest_aleat = aleat(0, N_BASES - 1); /* define aleatoriamente a próxima base de destino */
    ev = cria_evento(tempo, VIAJA, h->id, dest_aleat); /* evento de viagem pro herói para a base de destino */
    insere_lef(l, ev);
    ev = cria_evento(tempo, AVISA, 0, 0); /* evento de avisa para a base*/
    insere_lef(l, ev);
    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", 
        tempo, 
        h->id, 
        b->id, 
        cardinalidade_cjt(b->presentes), 
        b->lot);
}

/* função de evento de espera */
void trata_evento_espera(struct lef_t *l, int tempo, struct heroi_t *h, struct base_t *b) {
    if (!h || !b)
        return;
    struct evento_t *ev;
    enqueue(b->espera, h->id); /* adiciona o herói na fila de espera da base */
    ev = cria_evento(tempo, AVISA, 0, b->id);
    insere_lef(l, ev);
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", 
        tempo, 
        h->id, 
        b->id, 
        fila_tamanho(b->espera) - 1);
}

/* Função de evento de desistência */
void trata_evento_desiste(struct lef_t *l, int tempo, struct heroi_t *h, struct base_t *b) {
    if (!h || !b)
        return;
    struct evento_t *ev;
    int baseRand = aleat(0, N_BASES - 1); /* randomiza a próxima base de destino */
    ev = cria_evento(tempo, VIAJA, h->id, baseRand); 
    insere_lef(l, ev);
    printf("%6d: DESIST HEROI %2d BASE %d\n", tempo, h->id, b->id);
}

/* Função para tratar o evento de chegada */
void trata_evento_chega(struct lef_t *l, int tempo, struct heroi_t *h, struct base_t *b) {
    if (!h || !b)
        return;
    int espera;
    struct evento_t *ev;
    h->id_base = b->id; /* atualiza a base atual do herói */

    /* verifica se o herói deve esperar ou desistir */
    if (cardinalidade_cjt(b->presentes) < b->lot && fila_vazia(b->espera))
        espera = 1;
    else
        /* se a paciência do herói é maior que 10 vezes o tamanho da fila, ele espera */
        espera = (h->paciencia) > (10 * fila_tamanho(b->espera));

    if (espera) {
        ev = cria_evento(tempo, ESPERA, h->id, b->id); /* evento de espera do heroi*/
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", 
            tempo,
            h->id, 
            b->id, 
            cardinalidade_cjt(b->presentes), 
            b->lot);
    } else {
        ev = cria_evento(tempo, DESISTE, h->id, b->id); /* evento de desistencia do heroi*/
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", 
            tempo, 
            h->id, 
            b->id, 
            cardinalidade_cjt(b->presentes), 
            b->lot);
    }
    insere_lef(l, ev);
}

/* Função para criar eventos iniciais */
struct lef_t *cria_eventos_iniciais(struct mundo_t *w, struct lef_t *l) {
    if (!l)
        return NULL;

    int base, tempo; 
    struct evento_t *ev; 

    for (int i = 0; i < N_HEROIS; i++) {
        base = aleat(0, N_BASES - 1); /* escolhe uma base aleatória para o herói */
        tempo = aleat(0, TRES_DIAS); /* define um tempo aleatório para o evento de chegada */
        ev = cria_evento(tempo, CHEGA, w->herois[i]->id, w->bases[base]->id); /* cria evento de chegada do herói */
        insere_lef(l, ev); 
    }

    for (int i = 0; i < N_MISSOES; i++) {
        tempo = aleat(0, T_FIM_DO_MUNDO); /* define um tempo aleatório para o evento de missão */
        ev = cria_evento(tempo, MISSAO, w->missoes[i]->id, 0); /* cria evento de missão */
        insere_lef(l, ev); 
    }

    ev = cria_evento(T_FIM_DO_MUNDO, FIM, 0, 0); /* evento de fim da simulação */
    insere_lef(l, ev); 

    return l;
}


/* Função para o evento de fim da simulação */
void trata_evento_fim(int tempo, struct mundo_t *w, struct lef_t *l) {
    printf("%6d: FIM\n", tempo);
    int m_completas;
    float media, cont,tentativas;
    struct evento_t *ev;

    cont = 0; /* contador de missões */

    /* laço de impressão as info de cada heroi */
    for (int i = 0; i < N_HEROIS; i++) {
        printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ", 
            w->herois[i]->id, 
            w->herois[i]->paciencia, 
            w->herois[i]->vel, 
            w->herois[i]->exp);
        imprime_cjt(w->herois[i]->habilidade);
    }

    /* laço para os eventos restantes na lef*/
    while (l->primeiro) {
        ev = retira_lef(l);
        if (ev->tipo == MISSAO)
            cont++; /* incremente se o evento for missão*/
        destroi_evento(ev);
    }

    m_completas = w->total_missoes - cont;
    media = (float)m_completas / w->total_missoes * 100; /* calculo em percentual */
    tentativas = (float)w->tentativas / w->total_missoes;
    printf("MISSOES CUMPRIDAS: %d/%d (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n", 
       m_completas, 
       w->total_missoes, 
       media, 
       tentativas);
}

/* Função para simular o mundo */
void simula_mundo(struct mundo_t *w, struct lef_t *l, struct evento_t *ev) {
    int fim_simulacao = 0;
    
    while (!fim_simulacao) {
        ev = retira_lef(l);
        if (!ev) {
            fprintf(stderr, "Erro ao retirar evento da lista de eventos futuros\n");
            break;
        }
        w->relogio = ev->tempo; /* atualiza o relógio do mundo com o tempo do evento */
        switch (ev->tipo) {
        case CHEGA:
            trata_evento_chega(l, relogio_mundo(w), w->herois[ev->dado1], w->bases[ev->dado2]);
            break;
        case ESPERA:
            trata_evento_espera(l, relogio_mundo(w), w->herois[ev->dado1], w->bases[ev->dado2]);
            break;
        case DESISTE:
            trata_evento_desiste(l, relogio_mundo(w), w->herois[ev->dado1], w->bases[ev->dado2]);
            break;
        case AVISA:
            trata_evento_avisa(l, relogio_mundo(w), w->bases[ev->dado2]);
            break;
        case ENTRA:
            trata_evento_entra(l, relogio_mundo(w), w->herois[ev->dado1], w->bases[ev->dado2]);
            break;
        case SAI:
            trata_evento_sai(l, relogio_mundo(w), w->herois[ev->dado1], w->bases[ev->dado2]);
            break;
        case VIAJA:
            trata_evento_viaja(l, relogio_mundo(w), w->herois[ev->dado1], w->bases[ev->dado2], w);
            break;
        case MISSAO:
            trata_evento_missao(l, relogio_mundo(w), w->missoes[ev->dado1], w);
            break;
        case FIM:
            trata_evento_fim(relogio_mundo(w), w, l);
            fim_simulacao = 1;
            break;
        default:
            fprintf(stderr, "Tipo de evento desconhecido: %d\n", ev->tipo);
            break;
        }
        destroi_evento(ev); 
    }
}
