#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// struct de um nó da árvore de busca binária
struct No {
    int data;
    struct No* esquerda;
    struct No* direita;
};

// Cria um novo nó
struct No* criarNo(int data) {
    struct No* novoNo = (struct No*)malloc(sizeof(struct No));
    if(novoNo == NULL) {
        exit(1);
    }
    novoNo -> data = data;
    novoNo -> esquerda = NULL;
    novoNo -> direita = NULL;
    return novoNo;
}

// Insere um nó
struct No* inserirNo(struct No* raiz, int data) {
    if(raiz == NULL) {
        return criarNo(data);
    }
    if(data < raiz->data) {
        raiz->esquerda = inserirNo(raiz->esquerda, data);
    }
    else if(data > raiz->data) {
        raiz->direita = inserirNo(raiz->direita, data);
    }
    return raiz;
}

// Faz a busca e conta comparações
int busca(struct No* raiz, int data, int* comparacoes) {

    if(raiz == NULL) {
        return 0; // O número não foi encontrado
    }
    (*comparacoes) ++;
    if(data == raiz->data) {
        return 1; // O número foi encontrado
    }
    if(data < raiz -> data) {
        return busca(raiz -> esquerda, data, comparacoes);
    }
    return busca(raiz -> direita, data, comparacoes);
}

int main() {
    struct No* raiz = NULL; // Inicializa a árvore(vazia)
    int numeroProcurado;
    int comparacoes = 0;

    // Abre o arquivo para leitura
    FILE* arquivo = fopen("valores.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo valores.txt.\n");
        return 1;
    }

    // Lê os valores do arquivo e insere na árvore
    while (fscanf(arquivo, "%d", &numeroProcurado) != EOF) {
        raiz = inserirNo(raiz, numeroProcurado);
    }

    // Fecha o arquivo
    fclose(arquivo);

    printf("Digite o numero para buscar: ");
    scanf("%d", &numeroProcurado);

    // Busca na árvore e conta as comparações
    int encontrado = busca(raiz, numeroProcurado, &comparacoes);

    if(encontrado) {
        printf("O numero foi encontrado.\n");
    }
    else {
        printf("O numero não foi encontrado.\n");
    }

    printf("Numero de comparacoes: %d\n", comparacoes);

    // Cria e escreve os dados no arquivo "dataArvore.txt"
    FILE* file = fopen("dataArvoreBalanceada.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo dataArvoreBalanceada.txt.\n");
        return 1;
    }

    int j = 127;
    // Escreve os dados de comparações no arquivo
    for (int i = 1; i <= comparacoes; i++) {
        fprintf(file, "%d %d\n", i, j);
        j = j/2;
        //printf("P = %d", j);
    }

    // Fecha o arquivo
    fclose(file);

    return 0;
}
