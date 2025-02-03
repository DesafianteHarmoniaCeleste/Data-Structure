#include "leitura_binario.h"

/*
    Arquivo com descricao completa das operacoes para criacao e manipulacao dos Grafos
*/

// funcao que aloca a memoria do grafo principal, nao declara nenhum vertice ou aresta
Grafo *cria_grafo(){
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    if (grafo == NULL) {
        printf(FALHA_ARQUIVO);
        return NULL;
    }
    grafo->num_vertices = 0;
    grafo->adjacencia = NULL; // Nenhum array de adjacência inicialmente


    return grafo;
}

/*
    funcao reesponsavel por criar uma aresta nova para ser inserida no grafo
    a funcao recebe como parametros a struct Dinossauro, o qual possui os dados que serao inseridos

    inicia-se alocando espaco na memoria da aresta nova
    depois os dados do dinossauro sao salvos na aresta nova
    por fim, declara-se, inicialmente, que a proxima aresta é nula, e eh retornado a aresta nova
*/
Aresta* criarAresta(Dinossauro destino){
    Aresta* novaAresta = (Aresta*) malloc(sizeof(Aresta)); // aloca o espaco
    novaAresta->dado = destino; // salva os dados do dinossauro
    novaAresta->prox = NULL;
    return novaAresta; // retorno da aresta alocada
}

/*
    funcao responsavel por expandir o grafo com uma nova insercao de vertice em ordem alfabetica
    recebe como parametros o grafo o qual sera inserido a nova aresta,
    e a struct Dinossauro, que contem os dados do predador e da presa a ser inseridos

    inicia-se alocando memoria utilizando realoc, o qual guarda as insercoes ja feita e aumenta o espaço na memoria para uma nova insercao
    apos isso, o grafo recebe a alocacao efetuada, tem-se a atribuicao das  caracteristicas do grafo na nova insercao do dinossauro
    e por fim a insercao em ordem alfabetica da aresta a ser inserida, baseando no nome do dinossauro
    com a insercao ja ocorrida, retorna-se a posicao onde o vertice foi inserido para que a presa seja inserida diretamente nessa posicao
*/
int expandirGrafo(Grafo* grafo , Dinossauro dino_novo ){
    // Realloc para aumentar o espaço na lista de adjacência
    Aresta** nova_insercao = (Aresta**)realloc(grafo->adjacencia, (grafo->num_vertices+1) * sizeof(Aresta*));
    
    // caso de erro de alocacao
    if (nova_insercao == NULL){
        if (DEBBUG) { printf("Erro ao expandir o grafo.\n"); }
        printf(FALHA_ARQUIVO);
        return -1; // Indica erro na expansão
    }

    // lista de adjacencia recebe a alocacao nova
    grafo->adjacencia = nova_insercao;

    // Aloca memória para o novo vértice
    Aresta* novaAresta = (Aresta*)malloc(sizeof(Aresta));

    // caso de erro de alocacao
    if (novaAresta == NULL){
        if (DEBBUG) { printf("Erro de alocação para nova aresta.\n"); }
        return -1; // Indica erro na alocação
    }

    // Inicializa os campos da nova aresta
    dino_novo.grau = 0;
    dino_novo.grau_de_entrada = 0;
    dino_novo.grau_de_saida = 0;

    novaAresta->dado = dino_novo;
    novaAresta->prox = NULL;
    novaAresta->valor_ponderado = dino_novo.populacao;

    // Insere o novo vértice em ordem alfabética
    int pos = grafo->num_vertices;
    while (pos > 0 && strcmp(grafo->adjacencia[pos - 1]->dado.nome, dino_novo.nome) > 0) {
        grafo->adjacencia[pos] = grafo->adjacencia[pos - 1]; // alocacao dos vertices ate encontrar posicao de insercao correta
        pos--;
    }
    grafo->adjacencia[pos] = novaAresta; // encontrou a posicao correta de insercao

    // Atualiza o número de vértices do grafo
    grafo->num_vertices = grafo->num_vertices+1 ;

    if (DEBBUG) {
        printf("Dinossauro '%s' inserido na posição %d.\n", dino_novo.nome, pos);
    }

    return pos; // Retorna a posição onde o vértice foi inserido para futura insercao de presa
}

/*
    funcao jaInserido eh responsavel por verificar se um dinossauro ja foi inserido, retornando sua posicao no grafo
    a funcao recebe como parametros o grafo com as informacoes e o nome do dinossauro para fazer  a afericao de insercao

    a funcao se inicia fazendo a verificacao de, caso ela nao tenha elementos, de retornar -1,
    indicando que a funcao expande grafo deve ser chamada para que ocorra a primeira insercao de predador

    apos isso, tem-se um loop que ira varrer o grafo ate se encontrar o nome da nova insercao, 
    casa seja encontrado o dinossauro predador ja foi inserido, retornando entao a posicao que deve ser inserida a presa

    caso tenha percorrido o grafo todo e nao tenha encontrado, retorna -1 indicando que o dinossauro predador nao foi inserido ainda
    e que espaco deve ser alocado para sua insercao no grafo
*/

int jaInserido(Grafo* grafo, const char* nome){
    if (DEBBUG) { printf("NOME A SER INSERIDO %s\t ", nome); }

    if (grafo->adjacencia == NULL) {
        return -1;  // Grafo sem adjacências, portanto o elemento ainda não foi inserido
    }

    // aresta auxiliar para varrer as arestas no grafo principal 
    Aresta* atual;

    // Percorre todos os vértices do grafo
    for (int i = 0; i < grafo->num_vertices; i++) {
        atual = grafo->adjacencia[i];
        if (strcmp(atual->dado.nome, nome) == 0){
            if(DEBBUG){printf("ESTOU SENDO INSERIDO EM %d\n", i);}

            return i; // Retorna o índice do vértice onde o nome foi encontrado
        }
    }

    return -1; // Nome não encontrado
}

/*
    funcao responsavel por adicionar aresta nova no grafo ja considerando a ordem alfabetica
    a funcao recebe como parametros o grafo principal, os dinossauros origem e destino, correspondente respectivamente a presa e predador,
    e a posicao de insercao no grafo, dado anteriormente pela funcao jaInserido

    inicia-se a funcao criando o espaco na memoria e insere as informacoes da presa na nova aresta que sera inserida,
    ja definindo seu valor ponderado como a populacao do predador

    apos isso eh declarado duas arestas que irao auxiliar na leitura do grafo para a correta insercao da aresta nova,
    considerando que a ordem alfabetica deve ser mantida

    inicia-se com o ponteiroo atual recebendo a primeira presa para que possa ocorrer a varredura do grafo

    primeiramente, caso o ponteiro atual seja nulo, nao houve insercoes de presa ainda, entao eh inserido a aresta nova na posicao conhecida e funcao ja da retorno

    caso contrario tem-se o loop que faz a varredura das presas pelos ponteuros de posicao atual e anterior, o qual, ao caminhar na lista de presas
    faz com que o anterior receba o atual e o atual receba a proxima presa

    ao sair do loop, encontrando o local certo de insersao ou chegando em seu final tem-se duas possibilidades

    1- insercao no primeiro espaco disponivel, em que a aresta nova tem que ser recebida pelo proximo elemento do gradfo

    2- insercao no meio/fim da lista, em que o proximo do valor anterior recebe a aresta a ser inserida e o proximo da aresta a ser inserido recebe o ponteiro atual,
    funcionando, dessa forma, para o caso em que a aresta atual le nulo, ou seja, o final da lista

    por fim o grau de saida do vertice do predador eh incrementado em 1, indicando que ele tem mais uma presa para se alimentar
*/
void adicionarAresta(Grafo* grafo, Dinossauro origem, Dinossauro destino, int pos_ins){

    // aresta a ser inserida com o dinossauro novo
    Aresta* novaAresta   = criarAresta(destino);
     // atribuicao do peso da coneccao
    novaAresta->valor_ponderado= origem.populacao;
    
    if(DEBBUG){printf( "NOME DA PRESA: %s \n", destino.nome);}
    // ponteiros que avancam na lista encadeada, para inserir dinossauro na ordem alfabetica na posicao correta
    Aresta* pos_atual    = NULL;
    Aresta* pos_anterior = NULL;

    // recebe a lista de adjacencia, iniciando pelo predador
    pos_atual = grafo->adjacencia[pos_ins]->prox;

    if(pos_atual == NULL){ // insere no primeiro espaco disponivel
        grafo->adjacencia[pos_ins]->prox= novaAresta;
        grafo->adjacencia[pos_ins]->dado.grau_de_saida++;
        if(DEBBUG){("ENTREI NO IF NOVO\n");}
        return;
    }

    while(pos_atual!= NULL && strcmp(pos_atual->dado.nome, destino.nome)<0){
        pos_anterior= pos_atual;
        pos_atual = pos_atual->prox;
    }

    if(pos_anterior == NULL){ // insere no primeiro espaco disponivel
        if(DEBBUG){("ENTREI NO IF NOVO aq tb\n");}
        novaAresta->prox= grafo->adjacencia[pos_ins]->prox;
        grafo->adjacencia[pos_ins]->prox= novaAresta;
    }
    else{ // insere no meio da lista
        pos_anterior->prox = novaAresta;
        novaAresta->prox = pos_atual;
    }

    // atualiza o numero de presas do predador
    grafo->adjacencia[pos_ins]->dado.grau_de_saida++;

    return ;
}

// inicializa com valores nulos, evita problemas com print nulo e alocacao de memoria
Dinossauro inicializa_dino(){
    Dinossauro aux;
    strcpy(aux.nome ,"" );
    strcpy(aux.especie ,"");
    strcpy(aux.habitat ,"");
    strcpy(aux.dieta ,"");
    strcpy(aux.tipo ,"");
    strcpy(aux.alimento ,"");
    aux.grau_de_entrada= 0;
    aux.grau_de_saida= 0;
    aux.grau = 0;
    aux.populacao = 0;
    return aux;
}

// traduz 2 structs semelhantes, sendo elas a do arquivo binario, e a struct do grafo
// copiando as informacoes necessarias para o funcionamento do codigo
Dinossauro copia_dino(Dados dino_lido){
    Dinossauro aux;
    strcpy(aux.nome, dino_lido.nome);
    strcpy(aux.especie, dino_lido.especie);
    strcpy(aux.habitat, dino_lido.habitat);
    strcpy(aux.dieta, dino_lido.dieta);
    strcpy(aux.tipo, dino_lido.tipo);
    strcpy(aux.alimento, dino_lido.alimento);
    aux.populacao= dino_lido.populacao;
    
    return aux;
}

/*
    funcao principal da insercao nos grafos, pega as informacoes do dinossauro, encontra posicao correta e insere
    recebe como parametro o grafo principal e os dados do dinossauro lido do arquivo binario

    inicia-se a funcao traduzindo as informacoes lidas do arquivo binario para a struct definida no grafo
    apos isso, varifica em qual posicao do grafo esta o predador pela funcao jaInserida

    caso a funcao retorne -1, indicando que o nome ainda nao foi inserido, faz-se a chamada da funcao expandirGrafo,
    a qual aloca a memoria e insere em ordem alfabetica o predador novo, retornando a posicao de insercao

    apos isso, tem-se a declaracao da presa a ser inserida como aresta no grafo na posicao dada pelas funcoes descritas acima

    como a funcao abaixo esta sendo usada conforme os dados dos dinossauros sao lidos do arquivo binario, ela esta presente na funcao global recupera_dados_bin
    tendo entao, um loop que fornece a struct Dados do dinossauro lido ate atingir seu fim
*/
void insere_grafo(Grafo* grafo, Dados dino_lido){
    // verifica se o dinossauro ja foi inserido na lista de adjacencia
    Dinossauro dino_lido_grafo = copia_dino(dino_lido);

    // salva qual a posicao que o dinossaufo predador foi salvo    
    int posicao_inserida = jaInserido(grafo, dino_lido_grafo.nome);

    if( posicao_inserida == -1 ){ // caso nao tenha sido salvo na lista, aloca espaco no grafo
        if(DEBBUG){printf("INSERI NOME NOVO\n");}
        posicao_inserida= expandirGrafo(grafo, dino_lido_grafo);
    }

    // insercao da presa 
    Dinossauro presa = inicializa_dino();
    strcpy(presa.nome, dino_lido_grafo.alimento);
    if(DEBBUG){ printf("POSICAO INSERIDA: %d", posicao_inserida);}
    adicionarAresta(grafo, dino_lido_grafo, presa, posicao_inserida); /// WWW vou tentar ja inserir em ordem alfabetica pelo bem do nosso senhor
    

    return ;
}

/*
    funcao responsavel por printar as informacoes contidas no grafo, sendo elas, em ordem:
    nome, especie, habitat, dieta, tipo do alimento, grau entrada, grau saida, grau, alimento (presa), populacao   

    recebe como parametro o grafo principal, onde, o predador sera lido copia_predador ate o fim do grafo
    e a Aresta* auxiliar que ira ler as presas do respectivo predador, ate atingir um valor nulo, indicando que nao ha mais presas
    dessa forma printando em sequencia todas as informacoes do grafo

*/
void printa_grafo(Grafo* grafo){
    Aresta* auxiliar=NULL;
    Dinossauro copia_predador;
    // variavel que ira ajudar na verificacao da quantidade de operacoes efetuadas pelo loop, o qual ira ser efetuaco ate atingir o fim do grafo, dado pelo num de vertice
    int contador=0; 
    while(contador != grafo->num_vertices){ // execucao ate chegar ao fim dos vertices do grafo

        auxiliar=grafo->adjacencia[contador]; // leitura do predador que da acesso as suas presas
        if (auxiliar == NULL) {
            if(DEBBUG){printf("PARTICULA NAO EXISTENTE LIDA EM PRINTA GRAFO\n");}
            continue; 
        }
        copia_predador = auxiliar->dado; // copia dos dados do predador para print
        
        while(auxiliar->prox!=NULL){
            // armazenamento da proxima presa a ser printada
            auxiliar = auxiliar->prox; 
            // print das informacoes
            printf("%s %s %s %s %s %d %d %d %s %d \n", copia_predador.nome, copia_predador.especie, copia_predador.habitat, copia_predador.dieta, copia_predador.tipo,
                                                             copia_predador.grau_de_entrada,
                                                             copia_predador.grau_de_saida,
                                                             copia_predador.grau,
                                                             auxiliar->dado.nome,
                                                             auxiliar->valor_ponderado);
            }
        // limpa informacao lida
        auxiliar= NULL;
        // aumenta contador para leitura do proximo predador
        contador++;
    }
    return ;
}

/*
    funcao responsavel por ajustar o valor do grau, grau de entrada e grau de saida
    recebe como parametros o grafo principal

    a funcao inicia percorrendo a lista de adjacencia lendo as presas de todos os predadores,
    com a presa lida tem-se sua procura no grafo principal, quando eh achado sua posicao, o valor de entrada do respectivo vertice eh incrementado em 1
    indicando que aquele vertice tem mais um predador. esse processo se repete ate serem lidos todas as presas de todos os predadores 

    por fim, ajusta-se o valor de grau, que eh a soma do grau de entrada com o de saida
    lembrando que o valor do grau de saida ja foi calculado conforme as presas iam sendo inseridas no grafo pela funcao adicionarAresta
*/
void ajusta_graus(Grafo* grafo) {
    
    // Percorre todas as listas de adjacência 
    for (int i = 0; i < grafo->num_vertices; i++) {
        Aresta* atual = grafo->adjacencia[i]->prox;
        while (atual != NULL) {
            // Encontra o vértice correspondente à presa e incrementa seu grau de entrada
            for (int j = 0; j < grafo->num_vertices; j++) {
                if (strcmp(grafo->adjacencia[j]->dado.nome, atual->dado.nome) == 0) {
                    grafo->adjacencia[j]->dado.grau_de_entrada++;
                    break;
                }
            }
            atual = atual->prox;
        }
    }

    // ajusta o grau total
    for (int j = 0; j < grafo->num_vertices; j++) {
        grafo->adjacencia[j]->dado.grau = grafo->adjacencia[j]->dado.grau_de_entrada + grafo->adjacencia[j]->dado.grau_de_saida;
    }
}