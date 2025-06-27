#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ap.h"
#include "af.h"

int main() {
    AP *ap = criar_ap();

    // Estados: q0 (inicial), q1 (intermediário), qf (final)
    ap_adicionar_estado(ap, "q0", 1, 0);
    ap_adicionar_estado(ap, "q1", 0, 0);
    ap_adicionar_estado(ap, "qf", 0, 1);

    // Transições:
    // q0 --a, ε/A--> q0  (para cada 'a', empilha 'A' na pilha; nada para desempilhar)
    ap_adicionar_transicao(ap, "q0", 'a', "", "q0", "A");

    // q0 --ε, ε/ε--> q1 (epsilon transição: muda para q1 sem alterar pilha)
    ap_adicionar_transicao(ap, "q0", '\0', "", "q1", "");

    // q1 --b, A/ε--> q1 (para cada 'b', desempilha um 'A' da pilha)
    ap_adicionar_transicao(ap, "q1", 'b', "A", "q1", "");

    // q1 --ε, ε/ε--> qf (aceita se pilha vazia e fim da cadeia)
    ap_adicionar_transicao(ap, "q1", '\0', "", "qf", "");

    // Abrir arquivo de palavras para testar
    FILE *arquivo = fopen("palavras.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de palavras");
        liberar_ap(ap);
        return 1;
    }

    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\r\n")] = 0; // remove \n ou \r\n
        //if (strlen(linha) == 0) continue;

        // Se linha for "ε", considerar cadeia vazia:
        if (strcmp(linha, "ε") == 0) {
        linha[0] = '\0';  // transforma em string vazia
        }

        printf("Palavra \"%s\": %s\n", linha,
               ap_verificar_cadeia(ap, linha) ? "ACEITA" : "REJEITADA");
    }

    fclose(arquivo);
    liberar_ap(ap);
    return 0;
}
