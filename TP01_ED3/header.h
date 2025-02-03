#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define bytes_dados 160
#define MAX_LINHA 1024
#define bytes_dados 160
#define PAG_DISCO 1600

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


/*========================================Funções feitas=====================================================*/

/*---------------- Funções Cabeçalho*/
Cabecalho leitura_cabecalho_bin(FILE* arq);
void cabecalho_inicia(Cabecalho* cab_aux);
int escreve_cabecalho_bin(FILE* arq, Cabecalho cab);



/*---------------- Funções Leitura arquivos e dados*/
Dados leitura_dados_bin(FILE* arq, int* flag);
Dados leitura_dinossauro();
Dados ler_csv(FILE* fp, int* flag);
char* le_string(char* token, int inicio_da_palavra);


/*---------------- Funções Escrita/Remoção arquivos e dados*/
FILE* criarArquivo(const char* nomeArquivo, const char* modo);
void escreve_dino_bin(FILE* fp, Dados animal); //
void remove_dinossauro(FILE* arq, Dados dinossauro_atual, Cabecalho* titulo, int rrn_atual); //
int RRN_removido(FILE* arq); //
Dados leitura_remocao_dados_bin(FILE* arq, char* Nomecampo, char* Valorcampo, Cabecalho* titulo, int* rrn_atual);
void escreve_string_com_delimitador(FILE* fp, char* str); //
void preenche_com_lixo(FILE* fp, int bytes_ja_escritos);//
Dados insere_valor_conhecido(Dados dinossauro, int contador, int inicio_da_palavra, char* token);//
Dados insere_valor_nulo(Dados dinossauro, int contador);//
Dados atribuicao_dados(Dados dinossauro, char* str);//
char* duplica_string(const char* str); //
void removequotes(char* str);//
int copiar_arquivo(FILE *arq_origem, FILE *arq_destino);

/*------------------------------------------------*/
/*------Funções auxiliares------------------------*/
/*------------------------------------------------*/

/*----------------Funções para comparação de dados*/
int verifica_dino(Dados dinossauro, char* Nomecampo, char* Valorcampo);

/*--------------- Função de print de dados*/
void printa_dados(Dados dinossauro);

/*--------------- Funções Dadas*/
void binarioNaTela(char* nomeArquivoBinario);

/*========================================Funções compostas para funcionalidade=====================================================*/

/*1: */
void le_csv_escreve_bin();

/*2: */
void recupera_dados_bin();

/*3: */
void recupera_dados_where_bin();

/*4: */
void remove_dados();

/*5: */
void insercao_novos_registros();

/*6: */
void compactacao_arquivo();

/*===========================================================================================================*/
