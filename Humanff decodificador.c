#include <stdio.h>
#include <stdlib.h>

typedef struct lista{
	unsigned char caractere;
	struct lista *proximo;
}lista;

typedef struct arvore{
    unsigned char caractere;
    struct arvore *esquerda;
    struct arvore *direita;
}arvore;


typedef struct hash_table{
    int h;
    unsigned short codificacao;
}hash_table;


hash_table *caracteres[256];


//---------------------VOID COMPLETA---------------------
//inicializa um vetor de ponteiros para a tabela de hash
//utilizada na compressão.
void cria_hash_table(){
    int i;
    for(i = 0; i < 256; i++){
        caracteres[i] = NULL;
    }
}

//extrair essa informação do cabeçalho do arquivo compactado
//durante a descompactação, você precisa saber quantos desses bits extras foram adicionados, para que possam ser ignorados
void* lixo(unsigned short c) {
    c = c >> 5;
    unsigned short d;
    d = (unsigned short)c;
    return (void*)(intptr_t)d;
}


//realiza operações de deslocamento de bits para criar um valor de tamanho de árvore, que é uma combinação dos bits de a e b
void *arv_tam(unsigned char a,unsigned char b){
    a = a << 3;
    a = a >> 3;

    unsigned short d = (unsigned short)a;
    d = d << 7;
    d = d|b;
    unsigned short *c = &d;
    return c;
}

//armazena os bytes lido do arquivo compactado
//cria uma lista de nós e adiciona ao final
void *add_node(lista *cauda,unsigned char c){
    lista *new = malloc(sizeof(lista));
    new->caractere = c;
    new->proximo = NULL;
    if(cauda != NULL) {
        cauda -> proximo = new;
    }
    return (lista*)new;
}


void *create_node(unsigned char c){
    arvore *new_node = malloc(sizeof(arvore));
    new_node->caractere = c;
    new_node->esquerda = NULL;
    new_node->direita = NULL;
    return (arvore*)new_node;
}


//constroi a arvore de huffman a apartir do dados lidos do arquivo compactado
//cont controla a quantidade de bits a serem lidos do arquivo
//retorna ponteiro pra no raiz
void *add_arv(FILE *file,unsigned short *cont){
    unsigned short a = *cont;
    if(a == 0) {
        return (arvore*)NULL;
    }
    arvore *node;
    unsigned char c;
    fscanf(file,"%c",&c);
    if(c != '*'){
        if(c == '\\'){
            fscanf(file,"%c",&c);
            a--;
            cont = &a;
        }
        node = create_node(c);
        a--;
        cont = &a;
    }
    else {
        node = create_node(c);
        a--;
        cont = &a;
        node->esquerda = add_arv(file,cont);
        node->direita = add_arv(file,cont);
    }
    return (arvore*)node;
}


//verifica se o um bit i é 1
void *bit_is_set(unsigned short c, int i) {
    void *comp = (void*)1;
    c = c >> i;
    return (void *)((intptr_t)comp & (intptr_t)c);
}


//faz a descompactação de huffman
//recebe a lista de bytes, arvore de huffman e a quantidade de bytes de lixo
//lê cada byte da lista encadeada, segue o caminho na árvore de Huffman correspondente aos bits do byte e escreve os caracteres correspondentes no arquivo de saída
void codificacao(lista *cabeca,arvore *raiz,short lixo){
    int cont = 8 - lixo,i;
    FILE *file;
    char c[100];
    printf("Digite o nome do novo arquivo descompactado: \n");
    scanf("%s",c);
    file = fopen(c,"wb");

    arvore *aux = raiz;
    while(cabeca != NULL){

        for(i=7;i>=0;i--){
            if(bit_is_set(cabeca->caractere,i)){
                aux = aux->direita;
            } else {
                aux = aux->esquerda;
            }
            if(aux->esquerda == NULL && aux->direita == NULL){
                fprintf(file,"%c",aux->caractere);
                aux = raiz;
            }
            if(cabeca->proximo == NULL){
                cont --;
                if(cont == 0) break;
            }
        }
        cabeca = cabeca->proximo;
    }
    fclose(file);
}



void *main(){
    FILE *file;
    char arquivoCompactado[100];
    unsigned char a,b,c;
    printf("Digite o nome do arquivo compactado:\n");
    scanf(" %s",arquivoCompactado);
    unsigned short trash, *arvore_tamanho;
    lista *cauda = NULL, *byte_cabeca = NULL;
    arvore *raiz = NULL;

    file = fopen(arquivoCompactado,"rb");

    if (file == NULL){
		system("cls || clear");
	}
	else{
        fscanf(file,"%c%c",&a,&b);
        trash = (unsigned short)(intptr_t)lixo(a);
        arvore_tamanho = arv_tam(a,b);
    }
    raiz = add_arv(file,arvore_tamanho);
    while(fscanf(file,"%c",&c) != EOF){
        cauda = add_node(cauda,c);
        if(byte_cabeca == NULL) {
            byte_cabeca = cauda;
        }
    }

    fclose(file);
    codificacao(byte_cabeca,raiz,trash);
    system("cls || clear");
	printf("\nArquivo descompactado com sucesso\n");
    return 0;
}
