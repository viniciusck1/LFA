#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "af.h"

AF* criar_af() {
    AF *af = (AF *) malloc(sizeof(AF));
    af->cabeca_lista_estado = NULL;
    af->cabeca_lista_transicao = NULL;
    af->n_estados = 0;
    return af;
}

void adicionar_estado(AF *af, const char *nome, int inicial, int final) {
    ESTADO *novo = (ESTADO *) malloc(sizeof(ESTADO));
    novo->nome = strdup(nome);
    novo->inicial = inicial;
    novo->final = final;
    novo->prox_est = af->cabeca_lista_estado;
    af->cabeca_lista_estado = novo;
    af->n_estados++;
}

void adicionar_transicao(AF *af, const char *origem, char simbolo, const char *destino) {
    TRANSICAO *nova = (TRANSICAO *) malloc(sizeof(TRANSICAO));
    nova->alfabeto = simbolo;
    nova->estado_que_sai = strdup(origem);
    nova->estado_que_chega = strdup(destino);
    nova->prox_trans = af->cabeca_lista_transicao;
    af->cabeca_lista_transicao = nova;
}

ESTADO* encontrar_estado_inicial(AF *af) {
    ESTADO *p = af->cabeca_lista_estado;
    while (p != NULL) {
        if (p->inicial)
            return p;
        p = p->prox_est;
    }
    return NULL;
}

static ESTADO* encontrar_estado_por_nome(AF *af, const char *nome) {
    ESTADO *p = af->cabeca_lista_estado;
    while (p != NULL) {
        if (strcmp(p->nome, nome) == 0)
            return p;
        p = p->prox_est;
    }
    return NULL;
}

// Retorna número de destinos e preenche o array destinos[]
int obter_destinos(AF *af, const char *origem, char simbolo, ESTADO *destinos[], int max_destinos) {
    int count = 0;
    TRANSICAO *t = af->cabeca_lista_transicao;
    while (t != NULL && count < max_destinos) {
        if (strcmp(t->estado_que_sai, origem) == 0 && t->alfabeto == simbolo) {
            ESTADO *e = encontrar_estado_por_nome(af, t->estado_que_chega);
            if (e != NULL) {
                destinos[count++] = e;
            }
        }
        t = t->prox_trans;
    }
    return count;
}

// Função recursiva que tenta todos os caminhos possíveis
static int verificar_cadeia_rec(AF *af, ESTADO *estado_atual, const char *cadeia, int pos) { //A utilização do static é pra evitar conflito
    if (cadeia[pos] == '\0') {
        return estado_atual->final;
    }

    ESTADO *destinos[MAX_ESTADOS];
    int n_destinos = obter_destinos(af, estado_atual->nome, cadeia[pos], destinos, MAX_ESTADOS);

    for (int i = 0; i < n_destinos; i++) {
        if (verificar_cadeia_rec(af, destinos[i], cadeia, pos + 1)) {
            return 1; // Aceita se algum caminho aceitar
        }
    }

    return 0;
}

int verificar_cadeia(AF *af, const char *cadeia) {
    ESTADO *estado_inicial = encontrar_estado_inicial(af);
    if (!estado_inicial) {
        printf("Erro: nenhum estado inicial definido.\n");
        return 0;
    }
    return verificar_cadeia_rec(af, estado_inicial, cadeia, 0);
}

void liberar_af(AF *af) {
    ESTADO *e = af->cabeca_lista_estado;
    while (e != NULL) {
        ESTADO *temp = e;
        e = e->prox_est;
        free(temp->nome);
        free(temp);
    }

    TRANSICAO *t = af->cabeca_lista_transicao;
    while (t != NULL) {
        TRANSICAO *temp = t;
        t = t->prox_trans;
        free(temp->estado_que_sai);
        free(temp->estado_que_chega);
        free(temp);
    }

    free(af);
}
