#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h> // usado para fazer calculo do numero de andares da arvore

#define tam_PagDisco 93

#define bytes_dados 160
#define MAX_LINHA 1024
#define bytes_dados 160
#define PAG_DISCO 1600
#define Ordem_m 5

/*--------------- Definicao das structs Cabecalho e Dado*/
typedef struct t_cabecalho{
    char status;
    int topo; // armazena o RRN de um registro logicamente removido
    int proxRRN; //  armazena o valor do pr�ximo RRN dispon�vel, esse ta removido
    int nroRegRem; // armazena o n�mero de registros logicamente marcados como removidos
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
    int encadeamento;  //armazena o RRN do pr�ximo registro logicamente removido

    // campos de tamanho variavel // 160-18 = 142 bytes
    char* nome;
    char* especie;
    char* habitat;
    char* tipo;
    char* dieta;
    char* alimento;
} Dados; // Deve ter 160 bytes

/*--------------- Definicao das structs Cabecalho e Dado para arvore*/
typedef struct t_cabecalho_arvore{
     char status; //indica a consistência do arquivo de índice, assume valor 0 ou 1
     int noRaiz; //armazena o RRN do nó (página) raiz do índice árvore-B.
     int RRNproxNo; //armazena o RRN do próximo nó (página) do índice árvore-B a ser inserido

} Cabecalho_Arvore;

typedef struct t_chave{
    long long chave;
    long long RRN_chave;
} Chave;

typedef struct t_registro_dados{
    char folha; //indica se o nó é um nó folha ou não. assume valores 0 ou 1
    int nroChavesIndexadas; //armazena o número de chaves de busca indexadas no nó
    int RRNdoNo; //armazena o número do RRN referente ao nó
    Chave chaves[Ordem_m -1];
    int Ponteiro_filho[Ordem_m];
} Registro_Dados;

// funcao global
FILE* criarArquivo(const char* nomeArquivo, const char* modo);
Dados leitura_dados_bin(FILE* arq, int* flag);
void cria_arvoreB();
char* le_string(char* token, int inicio_da_palavra);
Cabecalho leitura_cabecalho_bin(FILE* arq);
Dados atribuicao_dados(Dados dinossauro, char* str);
Dados insere_valor_nulo(Dados dinossauro, int contador);
Dados insere_valor_conhecido(Dados dinossauro, int contador, int inicio_da_palavra, char* token);
void cabecalho_arvore_inicia(Cabecalho_Arvore* cab_aux);
void preenche_com_lixo(FILE* fp, int bytes_ja_escritos);
void removequotes(char *str);
void remove_quotes(char *str);
void insere_arvore_binaria(FILE* arq_arvore_bin, char* nome, long long Posicao_byte,int flag, Cabecalho_Arvore* titulo_arvore);
void printa_dados(Dados dinossauro);
char confere_no_folha(Registro_Dados nodo);
void printa_vetor_int(int* vetor, int tamanho);
void printa_vetor_long(long long* vetor, int tamanho);
char* ler_campo_composto();


// funcoes fornecidas
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);
long long converteNome(char* str);

// busca_binario
int calcula_altura_arvore(int num_nos, int ordem_m);
Registro_Dados leitura_no_arvore(FILE* arq_arvore_bin);
void split_e_promocao(FILE* arq_arvore_bin, Registro_Dados *no_atual, long long chave, long long Posicao_byte, Cabecalho_Arvore* titulo_arvore, int* RRN_acessados, int andares_escalados, int no_filho);
void insere_em_no_folha( FILE* arq_arvore_bin, Registro_Dados* no_atual, long long chave, long long Posicao_byte);
void escreve_cabecalho_arvore(FILE* arquivo_arvore, Cabecalho_Arvore titulo_arvore);

// funcoes recupera_dados_arvore
void recupera_dados_arvore();
Cabecalho_Arvore leitura_cabecalho_arvore(FILE* arq_arvore_bin);
int busca_chave_binario(FILE* arq_arvore_bin, long long chave, long long* Posicao_byte, Cabecalho_Arvore* titulo_arvore);

// insere arquivo de indices
int RRN_removido(FILE* arq);
void escreve_dino_bin(FILE* fp, Dados animal);
int escreve_cabecalho_bin(FILE* arq, Cabecalho cab);
void insercao_novos_registros();