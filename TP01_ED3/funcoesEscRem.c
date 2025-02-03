#include "header.h"

// Função que cria e retorna o ponteiro para o arquivo
FILE* criarArquivo(const char* nomeArquivo, const char* modo) {
    FILE* arquivo = fopen(nomeArquivo, modo);  // Abre o arquivo no modo passado

    if (arquivo == NULL) {  // Verifica se a abertura do arquivo falhou
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }

    return arquivo;  // Retorna o ponteiro para o arquivo
}

// Função para escrever uma string no arquivo binário com separador '#'
void escreve_string_com_delimitador(FILE* fp, char* str) {
    if (str != NULL && strlen(str) > 0) {
        fwrite(str, sizeof(char), strlen(str), fp);  // Escreve a string
    }
    // Escreve o delimitador '#'
    char delimitador = '#';
    fwrite(&delimitador, sizeof(char), 1, fp);
}

// Função para escrever os dados de um Dino no binário
void escreve_dino_bin(FILE* fp, Dados animal) {
    int bytes_escritos = 0;

    // Escreve os campos de tamanho fixo (total: 18 bytes)
    fwrite(&animal.removido, sizeof(char), 1, fp);          // 1 byte
    fwrite(&animal.encadeamento, sizeof(int), 1, fp);       // 4 bytes
    fwrite(&animal.populacao, sizeof(int), 1, fp);          // 4 bytes
    fwrite(&animal.tamanho, sizeof(float), 1, fp);          // 4 bytes
    fwrite(&animal.unidadeMedida, sizeof(char), 1, fp);     // 1 byte
    fwrite(&animal.velocidade, sizeof(int), 1, fp);         // 4 bytes
    bytes_escritos += 18;  // Soma os bytes dos campos de tamanho fixo

    // Escreve os campos de tamanho variável com delimitador '#'
    escreve_string_com_delimitador(fp, animal.nome);        // Nome
    escreve_string_com_delimitador(fp, animal.especie);     // Espécie
    escreve_string_com_delimitador(fp, animal.habitat);     // Habitat
    escreve_string_com_delimitador(fp, animal.tipo);        // Tipo
    escreve_string_com_delimitador(fp, animal.dieta);       // Dieta
    escreve_string_com_delimitador(fp, animal.alimento);    // Alimento

    // Calcula o total de bytes escritos até agora (campos fixos + variáveis)
    bytes_escritos += strlen(animal.nome) + 1;    // Nome + '#'
    bytes_escritos += strlen(animal.dieta) + 1;   // Dieta + '#'
    bytes_escritos += strlen(animal.habitat) + 1; // Habitat + '#'
    bytes_escritos += strlen(animal.tipo) + 1;    // Tipo + '#'
    bytes_escritos += strlen(animal.especie) + 1; // Espécie + '#'
    bytes_escritos += strlen(animal.alimento) + 1;// Alimento + '#'

    // Preenche o restante do espaço do registro (160 bytes) com '$'
    preenche_com_lixo(fp, bytes_escritos);
}

// funcao para remover duplas aspas de char inserido, caso nao tenha, retorna string oriiginal
void removequotes(char *str){
    int size;

    if(strcspn(str, "\n")<strlen(str))
        str[strcspn(str, "\n")] = '\0';

    if(str[0] == 34)
    {
        size = strlen(str)-2;
        memmove(str, str+1, size+1);
        str[size] = '\0';
    }
    return;
}

// Função que marca um dinossauro como removido
void remove_dinossauro(FILE* arq, Dados dinossauro_atual, Cabecalho* titulo, int rrn_atual){

    long aux = ftell(arq);
    fseek(arq, aux -(160), SEEK_SET);  /// Volta para o início do registro atual

    dinossauro_atual.removido = '1';  // Marca como removido
    dinossauro_atual.encadeamento = titulo->topo;  // O dinossauro removido aponta para o topo atual

    // Atualiza o topo do cabeçalho para o RRN do dinossauro atual
    titulo->topo = rrn_atual;

    // Escreve o registro removido no arquivo
    fwrite(&dinossauro_atual.removido, sizeof(char), 1, arq);  // Atualiza o campo 'removido'
    fwrite(&dinossauro_atual.encadeamento, sizeof(int), 1, arq);

    // Preenche o restante com '$'
    for (int i = 0; i < 155; i++) {
        fwrite("$", sizeof(char), 1, arq);
    }

    fflush(arq); // garannte q faz insercao de dados

    return ;  // Retorna a posição atual (novo topo da lista de removidos)
}

// funcao que retorna o proximo rrn que deve ser inserido pelo encadeamento, ja deixando o vetor arq em local conveniente
int RRN_removido(FILE* arq){
    Dados dinossauro;
    fread(&dinossauro.removido, sizeof(char) , 1, arq);
    fread(&dinossauro.encadeamento , sizeof(int),1, arq);
    fread(&dinossauro.populacao , sizeof(int),1, arq);
    fread(&dinossauro.tamanho , sizeof(float),1, arq);
    fread(&dinossauro.unidadeMedida , sizeof(char),1, arq);
    fread(&dinossauro.velocidade , sizeof(int),1, arq);
    return dinossauro.encadeamento;

}

// faz a leitura ate o proximo # e insere a informacao lida no local adequado
Dados insere_valor_conhecido(Dados dinossauro, int contador, int inicio_da_palavra, char* token) {
    char* inserir = le_string(token, inicio_da_palavra); // Obtém a string até o próximo '#'

    // Dependendo do contador, insere o valor no campo correto da struct
    switch (contador) {
        case 0:
            dinossauro.nome = inserir;
            break;
        case 1:
            dinossauro.especie = inserir;
            break;
        case 2:
            dinossauro.habitat = inserir;
            break;
        case 3:
            dinossauro.tipo = inserir;
            break;
        case 4:
            dinossauro.dieta = inserir;
            break;
        case 5:
            dinossauro.alimento = inserir;
            break;
        default:
            free(inserir); // Liberar memória se o contador estiver fora do esperado
            break;
    }
    return dinossauro;
}

// Função para inserir valor nulo na struct
Dados insere_valor_nulo(Dados dinossauro, int contador) {
    // Dependendo do contador, insere uma string vazia no campo correto da struct
    switch (contador) {
        case 0:
            dinossauro.nome = strdup("");
            break;
        case 1:
            dinossauro.especie = strdup("");
            break;
        case 2:
            dinossauro.habitat = strdup("");
            break;
        case 3:
            dinossauro.tipo = strdup("");
            break;
        case 4:
            dinossauro.dieta = strdup("");
            break;
        case 5:
            dinossauro.alimento = strdup("");
            break;
        default:
            break;
    }
    return dinossauro;
}

// Função para preencher o restante do registro até 160 bytes com '$'
void preenche_com_lixo(FILE* fp, int bytes_ja_escritos) {
    int bytes_restantes = bytes_dados - bytes_ja_escritos;
    char lixo = '$';
    for (int i = 0; i < bytes_restantes; i++) {
        fwrite(&lixo, sizeof(char), 1, fp);
    }
}

// faz a leitura da parcela de dados de tamanho variavel sendo separados pelo # ate o seu final $
// inserindo as devidas informacoes nulas ou conhecidas conforme as especificacoes
Dados atribuicao_dados(Dados dinossauro, char* str){
    int posicao = 0;
    int aux;
    int contador = 0;
    int inicio_da_palavra = 0;

    while (str[posicao] != '$') {
        aux = 0;
        // Verifica se a próxima palavra é válida (não dupla ##)
        while (str[posicao] != '#' && str[posicao] != '$') {
            posicao++;
            aux = 1; // Indica que há conteúdo válido entre os '#'
        }

        // Se houver conteúdo entre os '#'
        if (aux){
            dinossauro = insere_valor_conhecido(dinossauro, contador, inicio_da_palavra, str);
        }
        else{
            dinossauro = insere_valor_nulo(dinossauro, contador);
        }

        inicio_da_palavra = posicao + 1; // Posição inicial da próxima palavra
        posicao++; // Avança para o próximo caractere após o '#'
        contador++;
    }

    return dinossauro;
}

// copia o conteudo do arquivo origem para o arquivo destino
int copiar_arquivo(FILE *arq_origem, FILE *arq_destino) {

    // Buffer para armazenar os dados durante a cópia
    char buffer[170];

    // Lê o arquivo de origem e escreve no arquivo de destino
    while (fread(buffer, 1, 160 , arq_origem)) {
        fwrite(buffer, 1, 160, arq_destino);
    }

    return 0;
}

// Função auxiliar para duplicar uma string dinamicamente
char* duplica_string(const char* str) {
    char* nova_str = (char*)malloc(strlen(str) + 1);
    if (nova_str) {
        strcpy(nova_str, str);
    }
    return nova_str;
}


