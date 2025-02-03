#include "leitura_binario.h"

/*
    Arquivo responsabel pela aplicação do algoritmo de Kosaraju, o qual fornece se um ciclo eh fortemente conexo
    e quantos componentes fortemente conexos existem no ciclo
    
    tem-se as seguintes definicoes para a melhor leitura do codigo

    #define nao_visitado -1
    #define verdadeiro 1
    #define falso 0
*/

/*
    funcao responsavel por realizar a busca em profundidade do grafo transposto
    ela recebe como parametros o inteiro posicao_visitados, que diz respeito a posicao no grafo que o vertice esta
    o vetor booleano visitado, que indica se o vertice ja foi ou nao visitado, e o grafo a ser aplicado o metodo DFS

    a funcao eh iniciada com o vetor iniciado na posicao a ser visitada recebendo o valor verdadeiro
    e entao tem-se a declaracao da variavel inteira contador, o qual sera retornado indicando a quantidade de elementos presentes em um componente fortemente conexo

    apos isso, tem-se a declaracao da aresta atual, o qual ira auxiliar na varredura do grafo ao fornecer o nome do dinossauro para aplicar a funcao jaInserido
    a funcao jaInserido, entao retorna a posicao em que dinossauro lido foi inserido, para, entao, 
    realizar a chamada recursiva da funcao DFS com o novo dinossauro a ser visitado

    ao sair do loop, o vetor atual, ira receber a proxima presa para realizar a busca em profundiade, 
    o repetindo ate chegar ao final dos vetores proximos do predador de entrada

    caso o vertice nao tenha sido inserido como predador, eh ignorado a inspecao do vertice do mesmo, 
    caso que idealemente foi corrigido com a funcao corrige grafo
*/
int dfsTransposto(int posicao_visitado, bool visitado[], Grafo* grafo) {
    visitado[posicao_visitado] = true;
    if(DEBBUG) {printf("%d \n", posicao_visitado);} // printa o endereco da presa do grafo presente no componente fortemente conexo

    int contador = 1; // Conta o vértice atual

    Aresta* atual = grafo->adjacencia[posicao_visitado];
    while (atual != NULL) {
        int posicao_grafo = jaInserido(grafo, atual->dado.nome);
        if (posicao_grafo == -1) {
            if(DEBBUG) {printf("ENCONTREI VALOR INDISPONIVEL EM TRANSPOSTO\n");}
            atual = atual->prox;
            continue;
        }
        if (!visitado[posicao_grafo]) {
            contador += dfsTransposto(posicao_grafo, visitado, grafo); // Conta os vértices no componente
        }
        atual = atual->prox;
    }
    return contador;
}

/*
    funcao responsavel por criar um grafo transposto de um grafo ja dado
    recebe como parametro o grafo ja contruido e corrigido

    a funcao inicia alocando o espaco para o novo grafo que sera criado, recebendo o mesmo numero de vertices que o grafo anterior
    entao, tem-se a transcricao dos predadores no local original do grafo dado, facilitando a manipulacao dos vetores ja criados de pilha, 
    mudando, dessa forma, apenas a logica do predador virar quem eh predado

    para o ultimo loop tem-se a atribuicao às presas do predador como seu elemento proximo,
    indicando que, todos os elementos da lista de adjacencia de um vertice dado, corresponde aos predadores daquele vertice
*/
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
                if(DEBBUG) {printf("ENCONTREI VALOR INDISPONÍVEL EM TRANSPOR GRAFO\n");}
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


/*
    funcao recursiva que realiza a busca em profundidade
    recebe como parametros um inteiro posicao_v, que descreve a posicao no grafo a ser visitado e verificado a respectiva lista de adjacencia
    um vetor boleano que indica se o vertice ja foi visitado ou nao
    um vetor pilha que armazena a posicao do vertor que esta na pilha em ordem
    um inteiro topo que indica o topo do vetor pilha
    e o grafo ja modificado com os vertices corretos

    a funcao eh iniciada ja definindo que o vertice da posicao dadda foi visitado e inicia a chamada recursiva da funcao DFS para os seus vertices adjacentes (presas)
    a funcao eh executada ate que todas as presas de todos os predadores visitados sejam marcados como visitados,
    processo esse que implementa na pilha os valores inteiros dos enderecos acessados
*/
void dfs(int posicao_v, bool visitado[], int pilha[], int* topo, Grafo* grafo) {
    visitado[posicao_v] = true;
    //int posicao_grafo;
    Aresta* atual = grafo->adjacencia[posicao_v]->prox;
    while (atual != NULL) {

        if(DEBBUG) {printf("presa a ser procurada: %s \n", atual->dado.nome);}

        // obtem a posicao da presa no grafo para a chamada recursiva do DFS
        int posicao_grafo = jaInserido(grafo, atual->dado.nome );
        atual = atual->prox;
        
        if(posicao_grafo== -1){
            if(DEBBUG)  {printf("ENCONTREI VALOR INDISPONIVEL EM DFS\n");}
        }
        else if(!visitado[posicao_grafo]){ 
            dfs(posicao_grafo, visitado, pilha, topo, grafo);
        }
    }

    pilha[(*topo)++] = posicao_v; // Adiciona o vértice a pilha
}

/*
    A funcao `inserirPredador` tem como objetivo inserir novos predadores no grafo
    ela recebe como parametros o grafo a ser modificado e duas strings: o nome do predador e o nome da presa

    o processo comeca verificando se o nome do predador ja foi inserido no grafo, utilizando a funcao `jaInserido`
    caso o predador nao tenha sido encontrado (retorno -1), o grafo e expandido para incluir o novo vertice
    isso e feito aumentando o numero de vertices e realocando o vetor de adjacencias com `realloc`
    em seguida, uma nova aresta eh criada para representar o predador e adicionada ao final do grafo
    a ordem alfabetica nao e considerada, pois o algoritmo de Kosaraju utiliza apenas as conexoes entre os vertices

    depois disso, a funcao verifica se a presa ja foi inserida no grafo, novamente utilizando `jaInserido`
    caso a presa nao seja encontrada (retorno -1), a funcao exibe uma mensagem de erro (se o modo DEBUG estiver ativo)
    e retorna o grafo sem alteracoes

    se a presa for encontrada, a funcao verifica se a aresta que conecta o predador ja existe na lista de adjacencias da presa
    isso eh feito percorrendo a lista de adjacencias associada ao vertice da presa e comparando os nomes
    caso a aresta ja exista, nenhuma acao adicional eh feita

    se a aresta nao existir, uma nova e criada e adicionada ao inicio da lista de adjacencias da presa
*/
Grafo* inserirPredador(Grafo* grafo, char* nomePredador, char* nomePresa){

    // verifica se o nome "nomePredador" ja esta no grafo
    int posPredador = jaInserido(grafo, nomePredador);
    if (posPredador == -1) {
        // adiciona o novo vértice para o predador
        grafo->num_vertices++;
        grafo->adjacencia = (Aresta**)realloc(grafo->adjacencia, grafo->num_vertices * sizeof(Aresta*));

        // cria uma nova lista de adjacência para o novo vertice
        Aresta* novaAresta = (Aresta*)malloc(sizeof(Aresta));
        strcpy(novaAresta->dado.nome, nomePredador); // copia o nome do predador
        novaAresta->prox = NULL;
        grafo->adjacencia[grafo->num_vertices - 1] = novaAresta;

        posPredador = grafo->num_vertices - 1; // salva-se a nova posição do predador
    }

    // verifica se a aresta ja existe na lista de adjacencia da presa
    int posPresa = jaInserido(grafo, nomePresa);
    if (posPresa == -1) {
        if(DEBBUG) {printf("Erro: Presa '%s' não encontrada no grafo.\n", nomePresa);}
        return grafo;
    }

    Aresta* atual = grafo->adjacencia[posPresa]->prox;
    while (atual != NULL) {
        if (strcmp(atual->dado.nome, nomePredador) == 0) {
            // aresta ja existe, nao eh necessario adicionar novamente
            return grafo;
        }
        atual = atual->prox;
    }

    // adiciona a aresta do predador a lista de adjacencia da presa
    Aresta* novaAresta = (Aresta*)malloc(sizeof(Aresta));
    strcpy(novaAresta->dado.nome, nomePredador);
    novaAresta->prox = grafo->adjacencia[posPresa]->prox;
    grafo->adjacencia[posPresa]->prox = novaAresta;

    return grafo;
}

/*
    funcao que insere uma presa que nao predava nenhum outro animal como predador para o correto funcionamento do algoritmo
    recebe como parametro o grafo que deve ser atualizado

    a funcao faz a varredura de todas as presas que sao predadadas pelos predadores ja conhecidos, usando a funcao inserirpredador 
    para inserir a presa ao final do grafo
*/
void corrigirGrafo(Grafo* grafo) {
    for (int i = 0; i < grafo->num_vertices; i++) {
        Aresta* atual = grafo->adjacencia[i]->prox;
        while (atual != NULL) {
            grafo = inserirPredador(grafo, atual->dado.nome, grafo->adjacencia[i]->dado.nome);
            atual = atual->prox;
        }
    }
}

/*
    funcao de debbug que imprime um vetor booleano
    facilitou a analise do procedimento e funcionamento do algoritmo de kosajaru
*/
void imprimeVetorBool(bool vetor[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", vetor[i]); // 0 ou 1, representando false ou true
    }
    printf("\n");
}

/*
    A funcao kosaraju eh a funcao que eh chamada na main e inicia o processo de contagem de componentes fortemente conexos
    Ela recebe como parametros o grafo com as informacoes do arquivo binario

    a funcao se inicia corrigindo o grafo, para inserir presas que nao predam ninguem como vertices no final do grafo, ja alterando o seu numero de vertices

    apos isso tem-se a declaracao dos vetores que irao nos auxiliar na compreensao de caso o vertice ja foi visitado no caminho
    e qual a ordem de visitacao dos vertices dados pelo vetor pilha, o qual armazena o valoor do endereco no grafo, acessando-o assim, diretamente no futuro
    alem disso tem-se as variaveis inteiras topo, que indica qual é a posicao do topo no vetor pilha, e sscCount, que conta a quantidade de componentes fortemente conexos

    inicia-se todos os vertices como nao visitados ( ou no caso, visitado como falso ), e faz se um loop para visitar todos os vertices pela funcao recursiva
    dfs, ou seja, deep find search / busca em profundidade, a qual possui algumas aritmeticas diferentes comparadas ao mesmo procedimento declarado em ciclos_grafos
    sendo essa funcao chamada apenas se aquele ponto ainda nao foi visitado, construindo dessa forma, nosso vetor de inteiros na pilha

    Apos isso, tem-se a transposicao do grafo pela funcao criarGrafoTransposto e a redefinicao de que os nos ainda nao foram visitados para uma nova varredura
    cria-se entao a variavel inteira maior, a qual ira armazenar o maior valor de elementos em um mesmo componente fortemente conexo,
    onde, ao final da execucao do algoritmo de Kosajaru, indicara se o grafo eh fortemente se todos os elementos presentes no grafo se igualam ao maior valor de elementos

    entao entra-se no ultimo loop de verificacao, onde, caso o vertice ainda nao tenha sido visitado, chama-se a funcao dfsTransposto, que faz a varredura do grafo transposto
    o qual desempilha a pilha criada e calcula o numero de componentes fortemente conexos, armazenando a variavel de retorno dessa funçao no inteiro "maior"
    e incrementando o numero de ciclos caso ele exista

    printando no final, se o grafo eh fortemente conexo comparando com a variavel maior, e imprimindo o numero de componentes fortemente conexos
*/

void kosaraju(Grafo* grafo) {
    // correcao do grafo dado, onde serao inseridos como vertices animais que nao predam nenhuma presa, considerando principalmente o caso do sunlight
    corrigirGrafo(grafo);
    if(DEBBUG) {printf("NOVO TAMANHO GRAFO: %d", grafo->num_vertices);}

    bool visitado[grafo->num_vertices]; // vetor bool que indica se o vertice ja foi visitado durante a busca
    int pilha[grafo->num_vertices]; // pilha da posicao correspondente ao vertice no grafo

    int topo = 0; // variavel que indica qual eh a posicao do topo da fila
    int sccCount = 0; //Contador de componentes fortemente conexos
    
    // realizacao da busca em profundidade do grafo construido
    for (int i = 0; i < grafo->num_vertices; i++) {
        visitado[i] = false;
    }
    for (int i = 0; i < grafo->num_vertices; i++) {
        if (!visitado[i]) {
            dfs(i, visitado, pilha, &topo, grafo);
        }
    }

    if(DEBBUG){ ("SAI DA FUNCAO DFS COM SUCESSO\n"); }

    // Transposicao do grafo considerando a pilha construida no caso a nterior, o qual ira nos auxiliar para o calculo dos componentes fortemente conexos
    Grafo* transposto = criarGrafoTransposto(grafo);
    if(DEBBUG) {printf("SAI DA TRANSPOSICAO COM SUCESSO\n");}

    // define novamente que os vertices nao foram visitados para que possam ser revisitados
    for (int i = 0; i < grafo->num_vertices; i++) {
        visitado[i] = false;
    }

    int maior=0; // variavel que armazenara a maior quantidade de elementos em um componente fortemente conexo
    // enquanto houver elementos na pilha para serem desempilhados
    while (topo > 0) {
        int v = pilha[--topo]; // desempilha posicao do grafo armazenada
        if (!visitado[v]) { // caso o vertice nao tenha sido visitado
            if(DEBBUG) {printf("SCC #%d: ", sccCount + 1);}
            // realiza a busca em profundidade do grafo transposto e retorna o numero de elementos fortemente coonexos da conexao formada
            int tamanho_temporario = dfsTransposto(v, visitado, transposto); 
            if (tamanho_temporario > maior) {
                maior = tamanho_temporario; // atualiza o tamanho do maior componente
            }
            if(DEBBUG) {printf("\n");}
            sccCount++; // incrementa o contador de componentes fortemente conexos
        }
    }

    if(DEBBUG) { ("Número total de componentes fortemente conexos: %d\n", sccCount);}
    if(DEBBUG) {printf("MAIOR VALOR DE COMPONENTE: %d\n", maior); }
    if(DEBBUG) {imprimeVetorBool(visitado, transposto->num_vertices);}

    if(maior<transposto->num_vertices){ // grafo nao eh fortemente conexo
        printf("Não, o grafo não é fortemente conexo e possui %d componentes.", sccCount);
    }
    else{ // grafo eh fortemente conexo
        printf("Sim, o grafo é fortemente complexo e possui %d componente", sccCount);
    } 
}
