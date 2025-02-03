#include "leitura_binario.h"


// Realiza DFS no grafo transposto
void dfsTransposto(int v, bool visitado[], Grafo* grafo) {
    visitado[v] = true;
    printf("%d \n", v);

    Aresta* atual = grafo->adjacencia[v];
    while (atual != NULL) {
        int posicao_grafo = jaInserido(grafo, atual->dado.nome );
        if(posicao_grafo== -1){
            printf("ENCONTREI VALOR INDISPONIVEL EM TRANSPOSTO\n");
            continue;
        }
        if (!visitado[posicao_grafo]) {
            dfsTransposto(posicao_grafo, visitado, grafo);
        }
        atual = atual->prox;
    }
}


Grafo* criarGrafoTransposto(Grafo* grafo){
    Grafo* transposto = (Grafo*)malloc(sizeof(Grafo));
    transposto->num_vertices = grafo->num_vertices;
    transposto->adjacencia = (Aresta**)malloc(grafo->num_vertices * sizeof(Aresta*));

    // Inicializa o grafo transposto com a presa no início de cada lista
    for (int i = 0; i < grafo->num_vertices; i++) {
        Aresta* cabeça = (Aresta*)malloc(sizeof(Aresta));
        cabeça->dado = grafo->adjacencia[i]->dado; // Copia os dados da presa
        cabeça->prox = NULL; // Lista de predadores inicialmente vazia
        transposto->adjacencia[i] = cabeça;
    }

    // Adiciona os predadores às listas de adjacência
    for (int i = 0; i < grafo->num_vertices; i++){

        Aresta* atual = grafo->adjacencia[i]->prox; // Começa na primeira conexão
        while (atual != NULL) {

            int pos_presa = jaInserido(grafo, atual->dado.nome); // Encontra o índice do destino no grafo
            if (pos_presa == -1) {
                printf("ENCONTREI VALOR INDISPONÍVEL EM TRANSPOR GRAFO\n");
            }
            else {
                // Cria uma nova aresta representando o predador
                Aresta* nova_aresta = (Aresta*)malloc(sizeof(Aresta));
                nova_aresta->dado = grafo->adjacencia[i]->dado; // Predador é o vértice atual
                nova_aresta->prox = transposto->adjacencia[pos_presa]->prox;
                transposto->adjacencia[pos_presa]->prox = nova_aresta;
            }
            atual = atual->prox; // Próxima conexão
        }
    }

    return transposto;
}


// Realiza DFS para preencher a pilha com base no tempo de finalização
void dfs(int v, bool visitado[], int pilha[], int* topo, Grafo* grafo) {
    visitado[v] = true;
    //int posicao_grafo;
    Aresta* atual = grafo->adjacencia[v]->prox;
    while (atual != NULL) {

        printf("presa a ser procurada: %s \n", atual->dado.nome);

        int posicao_grafo = jaInserido(grafo, atual->dado.nome );
        atual = atual->prox;
        
        if(posicao_grafo== -1){
            printf("ENCONTREI VALOR INDISPONIVEL EM DFS\n");
        }
        else if(!visitado[posicao_grafo]){ 
            dfs(posicao_grafo, visitado, pilha, topo, grafo);
        }
    }

    pilha[(*topo)++] = v; // Adiciona o vértice à pilha
}

void kosaraju(Grafo* grafo) {
    
    bool visitado[grafo->num_vertices];
    int pilha[grafo->num_vertices];
    int topo = 0;
    int sccCount = 0; // Contador de SCCs

    // Passo 1: DFS no grafo original para preencher a pilha
    for (int i = 0; i < grafo->num_vertices; i++) {
        visitado[i] = false;
    }
    for (int i = 0; i < grafo->num_vertices; i++) {
        if (!visitado[i]) {
            dfs(i, visitado, pilha, &topo, grafo);
        }
    }

    if(DEBBUG){ ("SAI DA FUNCAO DFS COM SUCESSO\n"); }

    // Passo 2: Transpor o grafo
    Grafo* transposto = criarGrafoTransposto(grafo);
    if(DEBBUG) {printf("SAI DA TRANSPOSICAO COM SUCESSO\n");}

    // Passo 3: Processar vértices na ordem definida pela pilha
    for (int i = 0; i < grafo->num_vertices; i++) {
        visitado[i] = false;
    }
    while (topo > 0) {
        int v = pilha[--topo];
        if (!visitado[v]) {
            printf("SCC #%d: ", sccCount + 1);
            dfsTransposto(v, visitado, transposto);
            printf("\n");
            sccCount++; // Incrementa o contador
        }
    }

    printf("Número total de componentes fortemente conexos: %d\n", sccCount);

    // Libera o grafo transposto
    for (int i = 0; i < transposto->num_vertices; i++) {
        Aresta* atual = transposto->adjacencia[i];
        while (atual != NULL) {
            Aresta* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(transposto->adjacencia);
    free(transposto);
}