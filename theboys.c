#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fila.h"
#include "lef.h"
#include "EventosMundo.h"

int main() {

    srand(0); 

    /* cria e inicializa o mundo */
    struct mundo_t *mundo = criar_mundo();
    if (!mundo) {
        fprintf(stderr, "Erro ao criar o mundo.\n");
        return 1;
    }

    /* cria a lef */
    struct lef_t *lista_eventos = cria_lef();
    if (!lista_eventos) {
        fprintf(stderr, "Erro ao criar a lista de eventos.\n");
        destroi_mundo(mundo); // libera a memoria do mundo
        return 1;
    }

    /* inicia o mundo com herois, bases e missões */
    mundo = iniciar_mundo(mundo);
    cria_eventos_iniciais(mundo, lista_eventos); 
    simula_mundo(mundo, lista_eventos, NULL); 

    destroi_lef(lista_eventos); //lef
    destroi_mundo(mundo); 

    return 0;
}
