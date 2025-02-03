#include "leitura_binario.h"
#define INFINITO INT_MAX

// Função para encontrar o vértice com a menor distância ainda não visitado
/*
    funcao responsavel por encontrar o vertice com a menor distancia ainda nao
    visitado. 

    inicia-se inicializando a menor distancia como infinita e o indice como -1
    em seguida, procura-se o vertic nao visitado com a menor distancia e por fim,
    retorna o indice do vertice com menor distancia.
    
*/
int encontrarMenorDistancia(int* dist, bool* visitado, int tamanho) {
    int menor = INFINITO, indice = -1; // Inicializa a menor distância como infinita e o índice como inválido
    for (int i = 0; i < tamanho; i++) {
        // Procura o vértice não visitado com a menor distância
        if (!visitado[i] && dist[i] < menor) {
            menor = dist[i];
            indice = i;
        }
    }
    return indice; // Retorna o índice do vértice com a menor distância
}

// Algoritmo de Dijkstra para encontrar o menor caminho por população
/*
    funcao responsavel por utilizar o algoritmo de djikstra para encontrar o menor caminho por populacao.
    Recebe como parametros um grafo, o nome da presa e do predador.

    inicia-se alocando as variaveis, inicializando os vetores.
    em seguida, verifica-se se os vertices ja existem no grafo, e apos isso, atribui-se a distancia inicial para
    o vertice inicial a 0.

    depois, aplica-se o algoritmo de djikstra para encontrar o vertice com a menor distancia. Verifica-se caso foi encontrado
    o caminho e recontrui-se para o print. a memoria é liberdada ao fim.

*/
void menorCaminhoPopulacao(Grafo* grafo, const char* nomePresa, const char* nomePredador) {
    int numVertices = grafo->num_vertices; 
    int* dist = (int*)malloc(numVertices * sizeof(int)); 
    bool* visitado = (bool*)malloc(numVertices * sizeof(bool)); 
    int* anterior = (int*)malloc(numVertices * sizeof(int)); 

    // Inicializa os vetores
    for (int i = 0; i < numVertices; i++) {
        dist[i] = INFINITO; 
        visitado[i] = false; 
        anterior[i] = -1; 
    }

    int inicio = jaInserido(grafo, nomePredador); 
    int fim = jaInserido(grafo, nomePresa); 

    // Verifica se os vértices existem no grafo
    if (inicio == -1 || fim == -1) {
        if(DEBBUG) {printf("Um ou ambos os nomes não existem no grafo.\n");}
        printf(" CAMINHO INEXISTENTE \n");
        free(dist);
        free(visitado);
        free(anterior);
        return;
    }

    // A distância inicial para o vértice de partida é 0
    dist[inicio] = 0;

    // Dijkstra
    for (int i = 0; i < numVertices; i++) {
        // Encontra o vértice não visitado com a menor distância
        int u = encontrarMenorDistancia(dist, visitado, numVertices);
        if (u == -1) break; // Se não há mais vértices acessíveis, encerra o loop
        visitado[u] = true; // Marca o vértice como visitado

        // Explora as arestas adjacentes
        Aresta* adj = grafo->adjacencia[u]->prox;
        while (adj != NULL) {
            int v = jaInserido(grafo, adj->dado.nome); // Obtém o índice do vértice adjacente
            // Atualiza a distância caso um caminho menor seja encontrado
            if (!visitado[v] && dist[u] + adj->valor_ponderado < dist[v]) {
                dist[v] = dist[u] + adj->valor_ponderado;
                anterior[v] = u; // Armazena o vértice anterior no caminho
            }
            adj = adj->prox; // Avança para a próxima aresta
        }
    }

    // Verifica se um caminho foi encontrado
    if (dist[fim] == INFINITO) {
        if(DEBBUG) {printf("Não há caminho entre '%s' e '%s'.\n", nomePredador, nomePresa);}
        printf(" CAMINHO INEXISTENTE\n");
    } else {
        if(DEBBUG) { printf("Caminho de menor população entre '%s' e '%s' (peso: %d):\n", nomePredador, nomePresa, dist[fim]);}
        printf(" %d", dist[fim]);
        // Reconstrução do caminho
        int atual = fim;
        while (atual != -1) {
            if(DEBBUG) {printf("%s ", grafo->adjacencia[atual]->dado.nome);}
            atual = anterior[atual];
            if(DEBBUG){if (atual != -1) printf("<- ");}
        }
        printf("\n");
    }

    // Libera memória alocada
    free(dist);
    free(visitado);
    free(anterior);
}

/*
    funcao responsavel por executar n vezes o algoritmo como pedido.

    inicia-se corrigindo o grafo inicial utilizando a funcao corrigirGrafo,
    fazendo com que animais presas que nao predavam nenhum outro animal, como por exemplo o sunlight,
    tivessem um vertice, de modo a realizar a aritmetica do algoritmo de djikstra corretamente,
    e depois disso aloca memoria para as variaveis de armazenamento.

    em seguida, a entrada é lida e separada em tokens, extraindo cada par
    predador e presa, removendo aspas e ignorando separadores desnecessarios na leitura e insercao no grafo.
    Para cada par, é chamada a funcao menorCaminhoPopulacao, que calcula e exibe
    o menor caminho entre os vertices.

    Mensagens de erro podem ser printadas com debbug igual a 1 e com possiveis erros ocorrendo 
    e por fim, a memoria alocda para a leitura da entrada é liberada.
*/

void executarAnaliseCaminho(Grafo* grafo, int numero_buscas){

    // corrige o grafo inserindo presas antes nao inseridas
    corrigirGrafo(grafo);

    // define que cada nome vai ter no maximo 50 caracteres como pior caso e o multiplica pelo numero de entradas que serao feitas  
    int tamanho_maximo = 50 * numero_buscas;
    char* entrada = (char*)malloc(tamanho_maximo * sizeof(char));
    if (entrada == NULL) {
        if(DEBBUG) {printf("Erro: Falha ao alocar memória.\n");}
        return;
    }

    if(DEBBUG) {printf("Digite as entradas (predador e presa entre aspas, separados por espaços):\n");}

    // realiza a leitura de todas as entradas que devem ser verificadas como presa e predador
    if (fgets(entrada, tamanho_maximo, stdin) == NULL) {
        if(DEBBUG) {printf("Erro: Não foi possível ler a entrada.\n");}
        free(entrada);
        return;
    }

    if(DEBBUG) {printf("ENTRADA LIDA: %s\n", entrada);}

    // separacao dos espacos e possivel separacao da aspas duplas
    char* token = strtok(entrada, "\"");
    for (int i = 0; i < numero_buscas; i++) {
        // faz letura do predador corretamente
        token = strtok(NULL, "\"");
        if (token == NULL) {
            if(DEBBUG) {printf("Erro: Falta o predador correspondente na busca %d.\n", i + 1);}
            free(entrada);
            return;
        }
        removequotes(token); // remove as aspas duplas caso tenha sobrado
        char* predador = token; // leitura do nome do predador

        // ignora outro separador entre as palavras
        token = strtok(NULL, "\"");

        // faz leitura corretamente da presa
        token = strtok(NULL, "\"");
        if (token == NULL) {
            if(DEBBUG) {printf("Erro: Falta a presa correspondente na busca %d.\n", i + 1);}
            free(entrada);
            return;
        }
        removequotes(token); // remove as aspas duplas caso tenha sobrado
        char* presa = token; // leitura do nome da presa

        if(DEBBUG) {printf("Consulta %d/%d\n", i + 1, numero_buscas);}
        printf("%s %s:", predador, presa);

        // Chamar a função de análise
        menorCaminhoPopulacao(grafo, presa, predador);

        // Ignora o separador antes do próximo predador
        token = strtok(NULL, "\"");
    }

    free(entrada);
}