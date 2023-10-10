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
	//começa declarando um ponteiro auxiliar e o inicializa com o ponteiro para a cabeça da lista. Isso é feito para percorrer a lista enquanto procuramos pelo caractere desejado.
	while(auxiliar != NULL && auxiliar -> caractere != caractere){
		/*Em seguida, a função entra em um loop while que percorre a 
		lista encadeada enquanto o nó atual não é nulo e o caractere do nó atual não corresponde ao caractere que estamos procurando .*/
		auxiliar = auxiliar -> proximo;
	}
	if(auxiliar == NULL){
		novo = malloc(sizeof(no));//esse caso, a função aloca memória para um novo nó (novo)
		novo -> caractere = caractere; //Isso significa que estamos armazenando o caractere fornecido como argumento dentro do novo nó que acabou de ser criado.
		novo -> frequencia = 1;
		/*é usado para inicializar a frequência do caractere associado ao novo nó como 1 para indicar que é a primeira ocorrência desse caractere na lista ou na estrutura de dados.*/
		novo -> proximo = cabeca;
		/*garantir que o novo nó seja inserido corretamente no início da lista encadeada, permitindo a expansão da lista à medida que novos elementos são adicionados.*/
		novo -> esquerda = NULL;
		novo -> direita = NULL;
		//Se a estrutura no tiver campos esquerda e direita, esses campos são definidos como NULL
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
	//O cabeca->proximo->proximo pode ser usado para indicar o terceiro nó na lista original, que será usado como parte desse processo de fusão.
	novo = malloc(sizeof(no)); //cria um novo nó chamado "novo" usando a função malloc para alocar memória para ele.
	novo -> frequencia = cabeca -> frequencia + cabeca -> proximo -> frequencia; //Calcula a frequência do novo nó como a soma das frequências dos dois primeiros nós da lista
	
	novo -> esquerda = cabeca;
	novo -> direita = cabeca->proximo;
	// Configurar os ponteiros esquerda e direita do novo nó para os dois primeiros nós
	novo -> esquerda -> proximo = NULL;
	novo -> direita -> proximo = NULL;
	/*à medida que combina os dois nós com as menores frequências para criar um novo nó interno, você deseja remover 
	esses dois nós da lista, pois eles não serão mais usados como nós individuais na árvore.*/
	novo -> caractere = '*'; /*Define o caractere do novo nó como '*' que é usado para representar nós internos em árvores de Huffman).*/
	novo -> proximo = NULL;

	
    // Verifica se a lista de nós (novo_cabeca) está vazia
	if(novo_cabeca == NULL) {
		// Se estiver vazia, retorna o novo nó como a nova cabeça da lista
        return (no*)novo;// Se estiver vazia, retorna o novo nó como a nova cabeça da lista
	}
	else{
		no *auxiliar = novo_cabeca, *anterior = NULL;
		/*auxiliar é usado para percorrer a lista e anterior é usado para manter o nó anterior a auxiliar durante a travessia. */
		while(auxiliar != NULL && auxiliar->frequencia < novo->frequencia){
			anterior = auxiliar;
			auxiliar = auxiliar->proximo;
			/*Isso inicia um loop while que percorre a lista ordenada de nós enquanto auxiliar não for NULL e a frequência do nó apontado por auxiliar for 
			menor do que a frequência do novo nó novo. Isso significa que o loop continuará até encontrar o ponto onde a frequência do novo nó deve ser inserida
			*/
		}
		if(anterior == NULL){
			novo->proximo = novo_cabeca;
			return (no*)novo;
			//Isso significa que o novo nó deve ser inserido no início da lista, pois ele tem a menor frequência até agora.
		}
		else {
			anterior->proximo = novo;
			novo->proximo = auxiliar;
			//Isso significa que o novo nó deve ser inserido entre anterior e auxiliar na lista.
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
	if(cabeca != NULL){ //Verifica se o nó é diferente de nulo
		if(cabeca->direita == NULL && cabeca->esquerda == NULL && (cabeca->caractere == '*' || cabeca->caractere == '\\')){
			//Essa condição verifica se o nó atual não tem filhos à esquerda ou à direita
			fprintf(novo, "\\");
			/*Se essas condições forem atendidas, ela imprime uma 
			barra invertida ('\') no arquivo novo usando fprintf.*/
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
			//verifica se não tem filho a esquerda e a direita se está com o caracter especial
            cont  ++; 
			/*'CONT' é para rastrear o número de folhas que atendem a esses critérios específicos durante a travesssia da árvore*/
		}
		if(cabeca -> direita == NULL && cabeca -> esquerda == NULL){
			int i = (int)cabeca->caractere;
			/*converte o caractere armazenado em um nó da lista encadeada em um valor inteiro e armazena esse valor em i.*/
			/*Por conta que as tabelas de hash geralmente usam valores inteiros como índices para acessar elementos, é necessário converter o caractere 
			(que é um tipo de dado unsigned char) em um valor inteiro que possa ser usado como índice na tabela de hash.*/
			
			caracteres[i] = malloc(sizeof(hash_table));/*Aloca dinamicamente memória para um objeto do tipo hash_table e armazena o ponteiro resultante no array caracteres na posição i.*/
			caracteres[i]->h = alturaAtual;/*Define a altura atual da árvore no objeto apontado pelo ponteiro em caracteres[i].*/
			caracteres[i]->codificacao = direction;/*Define a direção no objeto apontado pelo ponteiro em caracteres[i].*/
		}
		cont = 1 + codificacao_arvore(cabeca -> esquerda, alturaAtual +1, direction << 1, cont);
		//cabeca->esquerda: O ponteiro para o filho esquerdo do nó atual.
		//alturaAtual + 1: A altura é incrementada em 1 para representar que estamos descendo um nível na árvore.
		//direction << 1: A direção é deslocada para a esquerda, o que está sendo usado para rastrear o caminho na árvore.
		
		/*A função retorna um valor inteiro que representa a quantidade de nós folha encontrados ao processar o subárvore do filho esquerdo. O 1 + é adicionado ao resultado porque estamos contando 
		o nó atual também como uma folha (caso seja uma folha). O resultado é então atribuído de volta à variável cont.*/

		cont = cont + codificacao_arvore(cabeca -> direita, alturaAtual +1, (direction << 1) +1, cont);
		/*cont: O contador cont é passado como argumento. A função também retorna um valor inteiro que representa a quantidade de nós folha 
		encontrados ao processar o subárvore do filho direito. Esse valor é então somado ao valor atual de cont.*/
	}
    return cont;
}



//---------------------VOID COMPLETA---------------------
// cria um arquivo de saída contendo a tabela de hash(caractere, altura e codigo)
void create_file_hash_table() {
    FILE *file = fopen("hash.txt", "w+b");//Esta linha de código está realizando a abertura do arquivo chamado "hash.txt" no modo de escrita e binário. 
    if (file == NULL) { /* Esta verificação condicional garante que o arquivo tenha sido aberto com sucesso.*/
        return;
    }
    for (int i = 0; i < 256; i++) { /*Essa faixa de valores é usada para representar todos os caracteres possíveis 
	em uma tabela ASCII estendida, que inclui letras, números, símbolos e caracteres especiais.*/
        if (caracteres[i] != NULL) { //Vai verificar se o caracter não é nulo naquele indice i
            fprintf(file, "%c %d %d\n", i, caracteres[i] -> h, caracteres[i] -> codificacao); 
			//fprintf é uma função de saída formatada em C que permite escrever dados formatados em um arquivo 
			//escrever informações no arquivo file.
			//É o valor inteiro h associado ao caractere no índice i do array caracteres.
			//É o valor inteiro codificacao associado ao caractere no índice i do array caracteres

        }
    }
    fclose(file); //fecha o fluxo do arquivo 
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
