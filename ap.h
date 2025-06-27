#ifndef AP_H
#define AP_H

#include "af.h" // Reaproveita ESTADO e TRANSICAO lá da af.h

#define MAX_PILHA 100
#define MAX_SYMBOL_SIZE 3 // até 2 símbolos + '\0'

// Transição específica para AP (não conflita com TRANSICAO do AF pois tem nome distinto)
typedef struct transicao_ap {
    char *estado_que_sai;
    char simbolo_entrada; // '\0' para epsilon
    char simbolos_para_desempilhar[MAX_SYMBOL_SIZE];
    char *estado_que_chega;
    char simbolos_para_empilhar[MAX_SYMBOL_SIZE];
    struct transicao_ap *prox_trans;
} TRANSICAO_AP;

typedef struct pilha {
    char itens[MAX_PILHA];
    int topo;
} PILHA;

typedef struct ap {
    ESTADO *cabeca_lista_estado;
    TRANSICAO_AP *cabeca_lista_transicao;
    int n_estados;
} AP;

// Funções para AP - EVITAR CONFLITO com AFD/AFN FEITO PRIMEIRAMENTE
AP* criar_ap();
void ap_adicionar_estado(AP *ap, const char *nome, int inicial, int final);
void ap_adicionar_transicao(AP *ap, const char *origem, char simbolo_entrada, const char *simbolos_para_desempilhar, const char *destino, const char *simbolos_para_empilhar);
int ap_verificar_cadeia(AP *ap, const char *cadeia);
void liberar_ap(AP *ap);

// Pilha
void pilha_inicializar(PILHA *pilha);
int pilha_esta_vazia(PILHA *pilha);
int pilha_empilhar(PILHA *pilha, char simbolo);
int pilha_desempilhar(PILHA *pilha);
char pilha_topo(PILHA *pilha);

#endif
