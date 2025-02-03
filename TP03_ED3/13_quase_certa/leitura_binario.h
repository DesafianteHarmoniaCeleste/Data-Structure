//Arquivo ListaDinEncad.h
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h> // utilizada no arquivo fortemente_conexo.c
#include <stdbool.h>

/*--------------- Definicao das structs Cabecalho e Dado*/
typedef struct t_cabecalho{
    char status;
    int topo; // armazena o RRN de um registro logicamente removido
    int proxRRN; //  armazena o valor do próximo RRN disponível, esse ta removido
    int nroRegRem; // armazena o número de registros logicamente marcados como removidos
    int nroPagDisco;
    int qttCompacta; // indica a quantidade de vezes que o arquivo de dados foi compactado.

} Cabecalho; // sizeof  = 21 bytes

typedef struct t_dados{
    // campos de tamanho fixo tamanho= 18 bytes
    int populacao;
    float tamanho;
    char unidadeMedida;
    int velocidade;
    char removido;
    int encadeamento;  //armazena o RRN do próximo registro logicamente removido

    // campos de tamanho variavel // 160-18 = 142 bytes
    char* nome;
    char* especie;
    char* habitat;
    char* tipo;
    char* dieta;
    char* alimento;
} Dados; // Deve ter 160 bytes

#define bytes_dados 160
#define MAX_LINHA 1024
#define bytes_dados 160
#define PAG_DISCO 1600

#define FALHA_ARQUIVO "Falha no processamento do Arquivo.\n"
#define DEBBUG 0

typedef struct t_tipo_dado{
    char nome[50];
    char especie[50];
    char habitat[50];
    char dieta[50];
    char tipo[50];
    int grau_de_entrada; // numero de predadores que predam o vertice
    int grau_de_saida;   // numero de presas que o animal preda
    int grau;            // grau = saida + entrada
    char alimento[50];
    int populacao;
}Dinossauro;

//Definição do tipo lista
typedef  struct _elemento{
    Dinossauro dado;
    int valor_ponderado;
    struct _elemento *prox;
} Aresta;

typedef struct _graph {
    int num_vertices;
    Aresta** adjacencia;
} Grafo;

// leitura_binario
Grafo* recupera_dados_bin();
Dados insere_valor_conhecido(Dados dinossauro, int contador, int inicio_da_palavra, char* token);
Dados insere_valor_nulo(Dados dinossauro, int contador);
Dados atribuicao_dados(Dados dinossauro, char* str);
Dados leitura_dados_bin(FILE* arq, int* flag);
Dados leitura_dinossauro();
char* le_string(char* token, int inicio_da_palavra);
Cabecalho leitura_cabecalho_bin(FILE* arq);
void printa_dados(Dados dinossauro);
FILE* criarArquivo(const char* nomeArquivo, const char* modo);
void removequotes(char *str);

//ListaDinEnc
Grafo* cria_grafo();
Aresta* criarAresta(Dinossauro destino);
int jaInserido(Grafo* inicio, const char* nome);
int expandirGrafo(Grafo* grafo , Dinossauro dino_novo );
void adicionarAresta(Grafo* grafo, Dinossauro origem, Dinossauro destino, int pos_ins);
void insere_grafo(Grafo* grafo, Dados dino_lido);
Dinossauro inicializa_dino();
void printa_grafo(Grafo* grafo);
void ajusta_graus(Grafo* grafo);

// funcao lista_predadores
void executarListagem(Grafo* grafo);
void listarPredadores(Grafo* grafo, const char* nomePresa);

// Ciclos grafo

#define BRANCO 0
#define CINZA  1
#define PRETO  2

void ciclos_grafo(Grafo* grafo);
int busca_em_profundidade(Grafo* grafo, int posicao_vertice_atual, int* ja_verificado);

// fortemente conexo

#define nao_visitado -1

#define verdadeiro 1
#define falso 0

/*
int menor(int a, int b);
int encontrarTopo(Grafo* grafo, int stack[]);
int push(Grafo* grafo, int stack[], int valor, int* no_stack);
int pop(Grafo* grafo, int stack[], int* no_stack);

void executa_Tarjan(Grafo* grafo, int posicao_atual, int* no_stack, int* stack_principal, int* aresta_visitada, int* menor_valor, int* conta_SCC, int* Posicao_ID);
void encontra_SCCS(Grafo* grafo);
*/

void kosaraju(Grafo* grafo);
void dfs(int v, bool visitado[], int pilha[], int* topo, Grafo* grafo);


void dfsTransposto(int v, bool visitado[], Grafo* grafo);

