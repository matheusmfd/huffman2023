
void insertNonFull(BTreeNode *node, int key) {
    int i = node->numKeys - 1;
    /*i e a inicializa com node->numKeys - 1. node->numKeys representa o número de chaves atualmente armazenadas no 
    nó node, e i será usada para iterar pelas chaves existentes no nó.*/

    if (node->leaf) {
        /*Este bloco if verifica se o nó node é uma folha (ou seja, não tem filhos). Se for uma folha, isso significa que podemos inserir diretamente a chave no nó atual.*/
        
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];/*Este é um loop while que começa a partir da posição i (a partir do final das chaves do nó) e avança em direção ao início do nó. 
            Ele move as chaves maiores que key uma posição à frente para abrir espaço para a nova chave key que estamos inserindo.*/
        }
        node->keys[i + 1] = key;
        node->numKeys++;
        //Incrementa o contador de chaves numKeys do nó node para refletir a adição bem-sucedida da nova chave.
    }
    else {
        while (i >= 0 && key < node->keys[i]) {
            i--; //Este é um loop semelhante ao anterior, mas apenas para encontrar a posição correta para inserir a chave key no nó interno. Ele ajusta i para apontar para a posição correta.
        }
        i++;
        if (node->children[i]->numKeys == ORDER - 1) {
            splitChild(node, i, node->children[i]);
            /*Depois de dividir o filho, verifica se a chave key é maior que a chave do meio que foi 
            promovida durante a divisão. Se for maior, incrementa i para que possamos continuar a inserção no filho direio*/
            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}