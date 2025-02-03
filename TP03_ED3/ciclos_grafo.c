#include "leitura_binario.h"

/*
    Arquivo responsavel por calcular o numero de ciclos presentes em um grafo

    utiliza como definicoes para melhor compreensao do codigo
    #define BRANCO 0
    #define CINZA  1
    #define PRETO  2
*/

/*
    funcao que realiza operacao de busca em profundidade (DFS) utilizando recursao e pilha de dados
    recebe como parametros o grafo principal, o inteiro que diz a posicao do vertice que sera olhada as arestas e a lista que corresponde se os vertices ja foram inspecionados

    a funcao inicia incializando o contador de ciclos, o qual sera retornado ao final da execucao 
    e inicializa o vertice atual como Cinza, ou seja, ja foram verificadas algumas arestas mas nao todas

    apos isso eh declarado uma aresta auxiliar ( denominada atual ) que percorrera as presas do vertice atual
    no loop principal do programa tem-se, enquanto a aresta atual lida nao for nula ( indicando que todas as presas daquele vertice foram lidas )
    tem-se a leitura da posicao da presa lida no grafo principal pela funcao ja_inserida e a chamada recursiva utilizando agora a posicao lida
    caso ela nao seja um ciclo em si ou SUNLIGHT, o qual eh um alimento que nao foi inserido no grafo

    Dessa forma, sera desempilhada a pilha, tendo o valor do numero de ciclos encontrados na variavel inteira "ciclos"
    onde, ao fim do codigo se tem o seu retorno e a mudanca dos vertices atuais percorridos durante a formacao da pilha para PRETO
    indicando que todos os vertices da aresta visitada ja foram inspecionados
*/
int busca_em_profundidade(Grafo* grafo, int posicao_vertice_atual, int* ja_verificado){

    // declaracao do numero de ciclos, os quais estarao presentes em um mesmo ramo da arvore dentro do grafo principal
    int ciclos =0;
    // vertice atual ja eh alterado como cinza ( valor 1 ) 
    ja_verificado[posicao_vertice_atual] = CINZA;

    Aresta* atual = grafo->adjacencia[posicao_vertice_atual]->prox;

    // recebe a posicao da aresta lida no grafo principal
    int posicao_aresta;

    // enquanto houver presas ( arestas adjacentes )
    while (atual != NULL){

        // transcreve a posicao da aresta para chamada recursiva da funcao
        posicao_aresta = jaInserido(grafo, atual->dado.nome);
        if(DEBBUG) { printf("POSICAO LIDA ja_inserido: %d NOME LIDO NA POSICAO: %s \n", posicao_aresta, atual->dado.nome);}

        // deslocamento do ponteiro aresta para a proxima presa da aresta da pilha    
        atual = atual->prox;

        // caso em que temos a leitura de sunlight, o qual nao eh um animal e nao possui insercao no grafo de dinossauros
        // como ele nao esta inserido no grafo, apenas nao faz nenhuma operacao
        if(posicao_aresta<0){
            continue;
        }
        // se o no verificado ja for cinza na busca, entao tem-se um ciclo fechado
        else if( ja_verificado[posicao_aresta] == CINZA){
            if(DEBBUG) { printf("CICLO ENCONTRADO \n");}
            ciclos++; // Encontrou ciclo
        }
        // caso a posicao nao tenha sido varrida, realiza-se uma pilha com chamada recursiva para acesso da nova posicao a ser verificada
        else if( ja_verificado[posicao_aresta] == BRANCO){
            if(DEBBUG) { printf("DFS CHAMADO NA BUSCA NUMERO: %d DENTRO DE DPS COM VERTICE %d \n", posicao_aresta, posicao_vertice_atual); }
            ciclos += busca_em_profundidade(grafo, posicao_aresta, ja_verificado);
        }
    }
    // apos verificacao de todas as saidas possiveis do vertice selecionada, ela eh classificada como PRETO 
    ja_verificado[posicao_vertice_atual] = PRETO;

    // retorno do numero de ciclos contabilizados no caminho percorrido, o qual, pela pilha, ira se somando ao desempilhar
    return ciclos;
}

/*
    Funcao principal relacionada a contagem dos ciclos dos grafos
    recebe como argumentos o grafo ja construido pela leitura do arquivo binario.

    a funcao cria um vetor de inteiros que recebe valores 0, 1 e 2 correspondentes as cores BRANCA, CINZA e PRETO respectivamente
    informando se os vertices ja foi visitado e se todas as suas arestas ja foram visitadas

    apos isso, enquanto houver vertices no grafo que nao foram visitados ( == BRANCO ) tem-se a chamada da funcao recursiva busca_em_profundidade
    a funcao retorna o numero de ciclos encontrados na busca, o qual sera printado ao final da funcao
*/
void ciclos_grafo(Grafo* grafo){

    // declaracao e alocacao da lista do estado dos vertices do grafo
    int ja_verificado[grafo->num_vertices];

    if(DEBBUG) { printf(" NUMERO DE VERTICES: %d \n", grafo->num_vertices); }

    // inicializa todas as arestas como branco ( 0 )
    for(int i = 0; i < grafo->num_vertices; i++){
        ja_verificado[i] = BRANCO;
    }

    // inteiro responsavel por contar o numero de ciclos no grafo
    int contador_ciclos = 0;

    // inteiro responsavel por varrer as arestas e realizar a busca em profundiade ( DFS ) uma por uma
    int verificacao_vertice = 0;

    // realiza acao de DFS para todos os vertices do grafo 
    while(verificacao_vertice < grafo->num_vertices){
        if(ja_verificado[verificacao_vertice] == BRANCO){ // realiza DFS caso vértice não tenha sido visitado
            if(DEBBUG) { printf("DFS CHAMADO NA BUSCA NUMERO: %d \n", verificacao_vertice); }
            contador_ciclos += busca_em_profundidade(grafo, verificacao_vertice, ja_verificado);
        }
        verificacao_vertice++;
    }

    // print do resultado
    printf("Quantidade de ciclos: %d", contador_ciclos);

    return ;
}