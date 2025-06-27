#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ap.h"

// --- Funções para Pilha ---

void pilha_inicializar(PILHA *pilha) {
    pilha->topo = -1;
}

int pilha_esta_vazia(PILHA *pilha) {
    return pilha->topo == -1;
}

int pilha_empilhar(PILHA *pilha, char simbolo) {
    if (pilha->topo >= MAX_PILHA - 1)
        return 0;
    pilha->itens[++pilha->topo] = simbolo;
    return 1;
}

int pilha_desempilhar(PILHA *pilha) {
    if (pilha_esta_vazia(pilha))
        return 0;
    pilha->topo--;
    return 1;
}

char pilha_topo(PILHA *pilha) {
    if (pilha_esta_vazia(pilha))
        return '\0';
    return pilha->itens[pilha->topo];
}

int pilha_topo_bate(PILHA *pilha, const char *seq_desempilhar) {
    int len = strlen(seq_desempilhar);
    if (len == 0) return 1;
    if (pilha->topo + 1 < len) return 0;

    for (int i = 0; i < len; i++) {
        if (pilha->itens[pilha->topo - i] != seq_desempilhar[len - 1 - i])
            return 0;
    }
    return 1;
}

void pilha_desempilhar_sequencia(PILHA *pilha, const char *seq_desempilhar) {
    int len = strlen(seq_desempilhar);
    for (int i = 0; i < len; i++) {
        pilha_desempilhar(pilha);
    }
}

// --- Funções para AP ---

AP* criar_ap() {
    AP *ap = (AP*) malloc(sizeof(AP));
    ap->cabeca_lista_estado = NULL;
    ap->cabeca_lista_transicao = NULL;
    ap->n_estados = 0;
    return ap;
}

void ap_adicionar_estado(AP *ap, const char *nome, int inicial, int final) {
    ESTADO *novo = (ESTADO*) malloc(sizeof(ESTADO));
    novo->nome = strdup(nome);
    novo->inicial = inicial;
    novo->final = final;
    novo->prox_est = ap->cabeca_lista_estado;
    ap->cabeca_lista_estado = novo;
    ap->n_estados++;
}

void ap_adicionar_transicao(AP *ap, const char *origem, char simbolo_entrada, const char *simbolos_para_desempilhar, const char *destino, const char *simbolos_para_empilhar) {
    TRANSICAO_AP *nova = (TRANSICAO_AP*) malloc(sizeof(TRANSICAO_AP));
    nova->estado_que_sai = strdup(origem);
    nova->simbolo_entrada = simbolo_entrada;
    strncpy(nova->simbolos_para_desempilhar, simbolos_para_desempilhar, MAX_SYMBOL_SIZE-1);
    nova->simbolos_para_desempilhar[MAX_SYMBOL_SIZE-1] = '\0';
    nova->estado_que_chega = strdup(destino);
    strncpy(nova->simbolos_para_empilhar, simbolos_para_empilhar, MAX_SYMBOL_SIZE-1);
    nova->simbolos_para_empilhar[MAX_SYMBOL_SIZE-1] = '\0';
    nova->prox_trans = ap->cabeca_lista_transicao;
    ap->cabeca_lista_transicao = nova;
}

static ESTADO* encontrar_estado_por_nome(AP *ap, const char *nome) {
    ESTADO *p = ap->cabeca_lista_estado;
    while (p != NULL) {
        if (strcmp(p->nome, nome) == 0)
            return p;
        p = p->prox_est;
    }
    return NULL;
}

static int verificar_cadeia_rec(AP *ap, ESTADO *estado_atual, const char *cadeia, int pos, PILHA *pilha) {
    if (cadeia[pos] == '\0') {
        if (estado_atual->final && pilha_esta_vazia(pilha))
            return 1;
    }

    TRANSICAO_AP *t = ap->cabeca_lista_transicao;
    while (t != NULL) {
        if (strcmp(t->estado_que_sai, estado_atual->nome) == 0) {
            int simbolo_entrada_bate = (t->simbolo_entrada == '\0' || t->simbolo_entrada == cadeia[pos]);

            if (simbolo_entrada_bate && pilha_topo_bate(pilha, t->simbolos_para_desempilhar)) {
                PILHA pilha_nova = *pilha;
                pilha_desempilhar_sequencia(&pilha_nova, t->simbolos_para_desempilhar);

                int len_empilhar = strlen(t->simbolos_para_empilhar);
                for (int i = len_empilhar - 1; i >= 0; i--) {
                    if (!pilha_empilhar(&pilha_nova, t->simbolos_para_empilhar[i]))
                        goto proximo;
                }

                int pos_nova = (t->simbolo_entrada == '\0') ? pos : pos + 1;

                ESTADO *prox_estado = encontrar_estado_por_nome(ap, t->estado_que_chega);
                if (prox_estado == NULL)
                    goto proximo;

                if (verificar_cadeia_rec(ap, prox_estado, cadeia, pos_nova, &pilha_nova))
                    return 1;
            }
        }
    proximo:
        t = t->prox_trans;
    }

    return 0;
}

int ap_verificar_cadeia(AP *ap, const char *cadeia) {
    ESTADO *estado_inicial = ap->cabeca_lista_estado;
    while (estado_inicial != NULL && !estado_inicial->inicial) {
        estado_inicial = estado_inicial->prox_est;
    }
    if (estado_inicial == NULL) {
        printf("Erro: nenhum estado inicial definido.\n");
        return 0;
    }

    PILHA pilha;
    pilha_inicializar(&pilha);

    // pilha_empilhar(&pilha, 'Z'); // se quiser usar símbolo inicial

    return verificar_cadeia_rec(ap, estado_inicial, cadeia, 0, &pilha);
}

void liberar_ap(AP *ap) {
    ESTADO *e = ap->cabeca_lista_estado;
    while (e != NULL) {
        ESTADO *temp = e;
        e = e->prox_est;
        free(temp->nome);
        free(temp);
    }

    TRANSICAO_AP *t = ap->cabeca_lista_transicao;
    while (t != NULL) {
        TRANSICAO_AP *temp = t;
        t = t->prox_trans;
        free(temp->estado_que_sai);
        free(temp->estado_que_chega);
        free(temp);
    }

    free(ap);
}
