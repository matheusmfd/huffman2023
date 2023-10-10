#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct lista_huffman{
	unsigned char caractere;
	long int frequencia;
	struct lista_huffman *proximo;
	struct lista_huffman *esquerda;
	struct lista_huffman *direita;
}no;

typedef struct hash_table{
	long h;
	unsigned short codificacao;
}hash_table;

struct novo_codificacao{
    unsigned char byte;
    struct novo_codificacao *proximo;
};
typedef struct novo_codificacao codificacao;


hash_table *caracteres[256];



//---------------------VOID COMPLETA---------------------
// percorre a lista
// compara se ja existe na lista
// se existe adiciona +1 a frequencia
// se não existe um nó é criado com o caractere e frequencia =1
void *add(no *cabeca, unsigned char caractere){
	no *auxiliar = cabeca, *novo;
	while(auxiliar != NULL && auxiliar -> caractere != caractere){
		auxiliar = auxiliar -> proximo;
	}
	if(auxiliar == NULL){
		novo = malloc(sizeof(no));
		novo -> caractere = caractere;
		novo -> frequencia = 1;
		novo -> proximo = cabeca;
		novo -> esquerda = NULL;
		novo -> direita = NULL;
		return (no*)novo;
	}
	else {
		auxiliar -> frequencia ++;
		return (no*)cabeca;
	}
}



//---------------------VOID COMPLETA---------------------
// cria um novo nó a partir de dois nós com as menores frequencias
// o novo nó é inserido na lista encadeada em ordem crescente de frequencia( a soma das duas menores frequencias)
void *add_arvore(no *cabeca){
	no *novo_cabeca = cabeca -> proximo -> proximo, *novo;
	novo = malloc(sizeof(no));
	novo -> frequencia = cabeca -> frequencia + cabeca -> proximo -> frequencia;
	novo -> esquerda = cabeca;
	novo -> direita = cabeca->proximo;
	novo -> esquerda -> proximo = NULL;
	novo -> direita -> proximo = NULL;
	novo -> caractere = '*';
	novo -> proximo = NULL;

	if(novo_cabeca == NULL) {
        return (no*)novo;
	}
	else{
		no *auxiliar = novo_cabeca, *anterior = NULL;
		while(auxiliar != NULL && auxiliar->frequencia < novo->frequencia){
			anterior = auxiliar;
			auxiliar = auxiliar->proximo;
		}
		if(anterior == NULL){
			novo->proximo = novo_cabeca;
			return (no*)novo;
		}
		else {
			anterior->proximo = novo;
			novo->proximo = auxiliar;
			return (no*)novo_cabeca;
		}
	}
}



//---------------------VOID COMPLETA-----------------
// função para ordenar a lista encadeada usando Bubble Sort
void *ordenar(no* cabeca) {
    int swap;
    no* a;
    no* b = NULL;
    if(cabeca == NULL || cabeca -> proximo == NULL) {
        return (no*)cabeca;
    }
    do{
        swap = 0;
        a = cabeca;
        while (a -> proximo != b) {
            if (a -> frequencia > a -> proximo -> frequencia) {
                long int temp = a->frequencia;
                unsigned char tempChar = a -> caractere;
                a -> frequencia = a -> proximo -> frequencia;
                a -> caractere = a -> proximo -> caractere;
                a -> proximo -> frequencia = temp;
                a -> proximo -> caractere = tempChar;
                swap = 1;
            }
            a = a -> proximo;
        }
        b = a;
    } while (swap);
    return (no*)cabeca;
}



//---------------------VOID COMPLETA---------------------
//percorre a lista
//imprime a lsita em um arquivo de saída
//quando o caractere é um asterisco ou uma barra invertida, a função imprime uma barra invertida antes dele. */
void printar(no *cabeca, FILE *novo){
	if(cabeca != NULL){
		if(cabeca->direita == NULL && cabeca->esquerda == NULL && (cabeca->caractere == '*' || cabeca->caractere == '\\')){
			fprintf(novo, "\\");
		}
		fprintf(novo,"%c",cabeca->caractere);
		printar(cabeca->esquerda,novo);
		printar(cabeca->direita,novo);
	}
}



//---------------------VOID COMPLETA---------------------
//inicializa um vetor de ponteiros para a tabela de hash
//utilizada na compressão.
void cria_hash_table(){
	int i;
	for(i = 0; i < 256; i++){
		caracteres[i] = NULL;
	}
}



//---------------------VOID COMPLETA---------------------
//incrementa o contador toda vez que encontra um nó folha com o caractere '\\' ou '*'. Essa parte parece estar correta
//função preenche a tabela de hash caracteres com informações sobre os caracteres encontrados na árvore.
//Para cada nó folha, ela cria uma entrada na tabela de hash com a chave sendo o valor do caractere (interpretado como um número inteiro) e os valores sendo a altura do nó na árvore (alturaAtual) e a direção em que o nó foi encontrado (direction).
int codificacao_arvore(no *cabeca, int alturaAtual, unsigned short direction, int cont){
    if(cabeca == NULL){
        return 0;
    }
	if(cabeca != NULL){
		if(cabeca -> direita == NULL && cabeca -> esquerda == NULL && (cabeca -> caractere == '\\' || cabeca -> caractere == '*')) {
            cont  ++;
		}
		if(cabeca -> direita == NULL && cabeca -> esquerda == NULL){
			int i = (int)cabeca->caractere;
			caracteres[i] = malloc(sizeof(hash_table));
			caracteres[i]->h = alturaAtual;
			caracteres[i]->codificacao = direction;
		}
		cont = 1 + codificacao_arvore(cabeca -> esquerda, alturaAtual +1, direction << 1, cont);
		cont = cont + codificacao_arvore(cabeca -> direita, alturaAtual +1, (direction << 1) +1, cont);
	}
    return cont;
}



//---------------------VOID COMPLETA---------------------
// cria um arquivo de saída contendo a tabela de hash(caractere, altura e codigo)
void create_file_hash_table() {
    FILE *file = fopen("hash.txt", "w+b");
    if (file == NULL) {
        return;
    }
    for (int i = 0; i < 256; i++) {
        if (caracteres[i] != NULL) {
            fprintf(file, "%c %d %d\n", i, caracteres[i] -> h, caracteres[i] -> codificacao);
        }
    }
    fclose(file);
}



//---------------------VOID COMPLETA-----------------
// verifica se o bit i é igual a 1
void *bit_is_set(unsigned short c, int i) {
    void *comp = (void*)1;
    c = c >> i;
    return (void *)((intptr_t)comp & (intptr_t)c);
}



//---------------------VOID COMPLETA---------------------
void *add_byte(codificacao *tail){
    codificacao *novo = malloc(sizeof(codificacao));
    tail -> proximo = novo;
    novo -> proximo = NULL;
    return (codificacao*)novo;
}



//---------------------VOID INCOMPLETA-----------------
//c se movam uma posição para a esquerda
//e o segundo bit menos significativo se torna 0
unsigned char set_bit(unsigned char c){
	unsigned char comp = 1;
    c = c << 1;
    return c|comp;
}



//---------------------VOID inCOMPLETA---------------------
/* é responsável por percorrer o arquivo de entrada, criar uma lista encadeada de bytes
correspondentes ao arquivo comprimido e preencher a tabela de hash. A cada caractere lido,
a função verifica seu código na tabela de hash e adiciona os bits correspondentes à lista
encadeada de bytes.*/
long codificacao_b(char s[], codificacao *codificacao_cabeca){
    FILE *file = fopen(s,"rb");

    codificacao *tail = codificacao_cabeca;

    long int cont = 0, pos, h, i;

    unsigned char c;
    while(fscanf(file,"%c", &c) != EOF){
        pos = (long int)c;
        h = caracteres[pos]->h;
        for(i = h-1 ; i >= 0 ; i--){
			if(cont == 8){
				tail = add_byte(tail);
				cont = 0;
			}
			if(bit_is_set(caracteres[pos] -> codificacao,i)){
				tail -> byte = set_bit(tail -> byte);
			}
			else {
				tail -> byte = tail -> byte << 1;
			}
			cont ++;
		}
    }
	tail -> byte = tail -> byte << (8-cont);
    fclose(file);
    return 8-cont;
}



//---------------------VOID COMPLETA---------------------
void* trash(unsigned short lixo) {
    unsigned short result = lixo << 12;
    return (void*)(intptr_t)result;
}



//---------------------VOID inCOMPLETA---------------------
unsigned short tree(unsigned short inf, unsigned short tam){
	return inf|tam;
}


void *main(){
	unsigned char caractere;
	unsigned short tam_arv, lixo;
    char arquivoParaCodificar[100];
	no *cabeca = NULL;
	codificacao *codificacao_cabeca = malloc(sizeof(codificacao));
	codificacao_cabeca->proximo = NULL;

    FILE *file,*novo;
	printf("Nome do Arquivo que sera compactado:\n");
    scanf(" %[^\n]",arquivoParaCodificar);
    file = fopen(arquivoParaCodificar,"r+b");

	if (file == NULL){
		system("cls || clear");
	}
	else{
        while(fscanf(file,"%c",&caractere) != EOF){
            cabeca = add(cabeca,caractere);
        }
    }

    fclose(file);
	cabeca = ordenar(cabeca);

	while(cabeca->proximo != NULL){
        cabeca = add_arvore(cabeca);
    }
    cria_hash_table();

    tam_arv = codificacao_arvore(cabeca,0,0,0);
	lixo = codificacao_b(arquivoParaCodificar,codificacao_cabeca);
	char arquivoCodificado[100];
	printf("Digite o nome do arquivo para compactar :\n");
	scanf(" %s",arquivoCodificado);
	novo = fopen(arquivoCodificado,"wb");

	if(novo != NULL){
		unsigned long int info = (unsigned long int)(intptr_t)trash(lixo);
		info = tree(info,tam_arv);
		unsigned char primeiro_byte, segundo_byte;
		primeiro_byte = (unsigned char)(info >> 7);
		segundo_byte = (unsigned char)info;
		fprintf(file,"%c%c",primeiro_byte,segundo_byte);
		printar(cabeca, novo);
		while(codificacao_cabeca != NULL){
			fprintf(file,"%c",codificacao_cabeca->byte);
			codificacao_cabeca = codificacao_cabeca->proximo;
		}
	}
    fclose(novo);
	system("cls || clear");
	printf("\nArquivo compactado com sucesso.\n");
	return 0;
}
