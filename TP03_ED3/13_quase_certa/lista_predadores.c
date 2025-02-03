#include "leitura_binario.h"

/*
    funcao responsavel por listar os predadores a partir do grafo e 
    do nome da presa correspondente

    inicia-se percorrendo todos os vertices do grafo e comparando os
    nomes com o nome da presa procurada

    caso a comparação seja verdadeira, printa-se o nome, a não ser que
    a variavel encontrou indique que é o primeiro nome lido.

    em seguida a varivel encontrou é trocada para 1 e o loop é encerrado
    e passa-se pro proximo nó.

   um último if é adicionado para a mensagem de erro em caso de não 
    correspondencia.



*/
void listarPredadores(Grafo* grafo, const char* nomePresa) {
    if(DEBBUG) {printf("=== Predadores de '%s' ===\n", nomePresa);}

    int encontrou = 0;

    //percorre todos os vértices do grafo
    for (int i = 0; i < grafo->num_vertices; i++) {
        Aresta* atual = grafo->adjacencia[i]->prox;

        //lista de adjacência do vértice atual
        while (atual != NULL) {
            if (strcmp(atual->dado.nome, nomePresa) == 0) {
                //se encontrou, printa nome
                if(DEBBUG) printf("Predador: %s\n", grafo->adjacencia[i]->dado.nome);
                if(encontrou==0){
                    printf("%s: ", nomePresa);
                }
                else if(encontrou!=0){ // primeiro nome lido, nao printa virgula
                    printf(", ");
                }

                printf("%s", grafo->adjacencia[i]->dado.nome);
                encontrou = 1;
                break; //se encontrou, sai loop
            }
            atual = atual->prox;
        }
    }

    if (!encontrou) {
        if(DEBBUG) printf("Nenhum predador encontrado para %s.\n", nomePresa);
        printf("Registro Inexistente.");
    }
    printf("\n \n");
}

/*
    funcao responsavel por executar a funcao anterior (listarPredadores)
    n vezes a partir deu um grafo.

    inicia-se recebendo o numero n de entradas e atribuindo uma string de
    tamanho 100*n (maior tamanho possivel para n).

    em seguida, declara-se um token e recebe-se o nome da presa e atribui
    um valor ao token.

    em seguida, é executado um loop n vezes, atualizando o token e chamando
    a funcao de removequotes para remover as aspas do nome lido e listamos os
    predadores utilizado a funcao anterior.

*/
void executarListagem(Grafo* grafo)  // executa n vezes a listagem
{
    int n;

    // leitura do numero de entradas
    scanf("%d", &n);
    char nomePresa[n*100]; // considerando maior nome possivel como 100 char
    char* token;
    fgets(nomePresa, n*100, stdin);
    if(DEBBUG) {printf("STRING TODA LIDA: %s", nomePresa);}
    
    token = strtok(nomePresa, "\""); // Primeiro token baseado nas aspas
    for (int i = 0; i < n; i++) {
        token = strtok(NULL, "\"");
        if(DEBBUG) printf("Digite o nome da presa (tentativa %d/%d): ", i + 1, n);
        
        removequotes(nomePresa);
        listarPredadores(grafo, token); 
        token = strtok(NULL, "\"");
    }
}