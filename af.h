#ifndef AFD_H
#define AFD_H


#define MAX_ESTADOS 100

typedef struct estado {
    int inicial; // Indica se o estado é inicial (1 para verdadeiro, 0 para falso)
    int final;  // Indica se o estado é final (1 para verdadeiro, 0 para falso)
    char *nome; // Nome ou identificador do estado (ex: "q0", "q1", etc.)

    struct estado *prox_est; // ponteiro para o próximo estado na lista encadeada

} ESTADO;

typedef struct transicao {
    char alfabeto; 
    char *estado_que_sai;
    char *estado_que_chega;
    struct transicao *prox_trans;
} TRANSICAO;

typedef struct af {
    ESTADO *cabeca_lista_estado; 
    TRANSICAO *cabeca_lista_transicao;
    int n_estados; // contador de estados
} AF;

AF* criar_af();
void adicionar_estado(AF *af, const char *nome, int inicial, int final);
void adicionar_transicao(AF *af, const char *origem, char simbolo, const char *destino);
int verificar_cadeia(AF *af, const char *cadeia);
void liberar_af(AF *af);

// NOVO: retorna número de estados destinos para (origem, simbolo) e preenche array destinos[]
int obter_destinos(AF *af, const char *origem, char simbolo, ESTADO *destinos[], int max_destinos);

#endif