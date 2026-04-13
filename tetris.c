#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CAP_FILA 5
#define CAP_PILHA 3

typedef struct {
    char tipo;
    int id;
} Peca;

typedef struct {
    Peca itens[CAP_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

typedef struct {
    Peca itens[CAP_PILHA];
    int topo;
} Pilha;

// -------------------- Funções utilitárias --------------------

Peca gerarPeca() {
    static int proximoId = 1;
    char tipos[] = {'I', 'O', 'T', 'S', 'Z', 'J', 'L'};

    Peca p;
    p.tipo = tipos[rand() % 7];
    p.id = proximoId++;
    return p;
}

void mostrarPeca(Peca p) {
    printf("[%c-%d]", p.tipo, p.id);
}

// -------------------- Fila circular --------------------

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

int filaCheia(Fila *f) {
    return f->tamanho == CAP_FILA;
}

int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) {
        return 0;
    }

    f->tras = (f->tras + 1) % CAP_FILA;
    f->itens[f->tras] = p;
    f->tamanho++;
    return 1;
}

int dequeue(Fila *f, Peca *p) {
    if (filaVazia(f)) {
        return 0;
    }

    *p = f->itens[f->frente];
    f->frente = (f->frente + 1) % CAP_FILA;
    f->tamanho--;
    return 1;
}

void mostrarFila(Fila *f) {
    int i, indice;

    printf("\nFila de pecas futuras: ");
    if (filaVazia(f)) {
        printf("vazia");
        return;
    }

    for (i = 0; i < f->tamanho; i++) {
        indice = (f->frente + i) % CAP_FILA;
        mostrarPeca(f->itens[indice]);
        printf(" ");
    }
}

// -------------------- Pilha linear --------------------

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == CAP_PILHA - 1;
}

int push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        return 0;
    }

    p->topo++;
    p->itens[p->topo] = x;
    return 1;
}

int pop(Pilha *p, Peca *x) {
    if (pilhaVazia(p)) {
        return 0;
    }

    *x = p->itens[p->topo];
    p->topo--;
    return 1;
}

void mostrarPilha(Pilha *p) {
    int i;

    printf("\nPilha de reserva: ");
    if (pilhaVazia(p)) {
        printf("vazia");
        return;
    }

    for (i = p->topo; i >= 0; i--) {
        mostrarPeca(p->itens[i]);
        printf(" ");
    }
}

// -------------------- Operações especiais --------------------

void reporFila(Fila *f) {
    while (!filaCheia(f)) {
        enqueue(f, gerarPeca());
    }
}

void trocarFrenteComTopo(Fila *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("\nErro: a fila esta vazia.");
        return;
    }

    if (pilhaVazia(p)) {
        printf("\nErro: a pilha de reserva esta vazia.");
        return;
    }

    Peca temp = f->itens[f->frente];
    f->itens[f->frente] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("\nTroca da frente da fila com o topo da pilha realizada com sucesso.");
}

void trocarTresPrimeiros(Fila *f, Pilha *p) {
    if (f->tamanho < 3) {
        printf("\nErro: a fila nao possui pelo menos 3 pecas.");
        return;
    }

    if (p->topo != 2) {
        printf("\nErro: a pilha precisa ter exatamente 3 pecas.");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int indiceFila = (f->frente + i) % CAP_FILA;
        Peca temp = f->itens[indiceFila];
        f->itens[indiceFila] = p->itens[2 - i];
        p->itens[2 - i] = temp;
    }

    printf("\nTroca dos 3 primeiros da fila com as 3 pecas da pilha realizada com sucesso.");
}

// -------------------- Exibição geral --------------------

void mostrarEstado(Fila *f, Pilha *p) {
    printf("\n\n==============================");
    mostrarFila(f);
    mostrarPilha(p);
    printf("\n==============================\n");
}

// -------------------- Programa principal --------------------

int main() {
    Fila fila;
    Pilha pilha;
    Peca p;
    int opcao;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < CAP_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    do {
        mostrarEstado(&fila, &pilha);

        printf("Menu:\n");
        printf("1 - Jogar peca (remover da frente da fila)\n");
        printf("2 - Enviar peca da fila para a reserva (pilha)\n");
        printf("3 - Usar peca da reserva (remover do topo da pilha)\n");
        printf("4 - Trocar peca da frente com topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com os 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (dequeue(&fila, &p)) {
                    printf("\nVoce jogou a peca ");
                    mostrarPeca(p);
                    reporFila(&fila);
                } else {
                    printf("\nErro: fila vazia.");
                }
                break;

            case 2:
                if (pilhaCheia(&pilha)) {
                    printf("\nErro: a pilha de reserva esta cheia.");
                } else if (dequeue(&fila, &p)) {
                    push(&pilha, p);
                    printf("\nPeca enviada para a reserva: ");
                    mostrarPeca(p);
                    reporFila(&fila);
                } else {
                    printf("\nErro: fila vazia.");
                }
                break;

            case 3:
                if (pop(&pilha, &p)) {
                    printf("\nVoce usou a peca da reserva: ");
                    mostrarPeca(p);
                } else {
                    printf("\nErro: a pilha de reserva esta vazia.");
                }
                break;

            case 4:
                trocarFrenteComTopo(&fila, &pilha);
                break;

            case 5:
                trocarTresPrimeiros(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando o programa...\n");
                break;

            default:
                printf("\nOpcao invalida.");
        }

    } while (opcao != 0);

    return 0;
}