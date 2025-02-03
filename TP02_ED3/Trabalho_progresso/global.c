#include "Header.h"

// Funcao que cria e retorna o ponteiro para o arquivo
FILE* criarArquivo(const char* nomeArquivo, const char* modo) {
    FILE* arquivo = fopen(nomeArquivo, modo);  // Abre o arquivo no modo passado

    if (arquivo == NULL) {  // Verifica se a abertura do arquivo falhou
        printf("Falha no processamento do arquivo.\n");
        exit(-1);
    }

    return arquivo;  // Retorna o ponteiro para o arquivo
}

// Funcao para ler a string ate encontrar '#'
char* le_string(char* token, int inicio_da_palavra) {
    int i = 0;
    // Usar memoria dinamica para armazenar a string lida
    char* aux = (char*)malloc(100 * sizeof(char));
    if (!aux) {
        printf("Erro de alocacaco de memoria.\n");
        exit(1);
    }

    while (token[inicio_da_palavra + i] != '#' && token[inicio_da_palavra + i] != '\0') {
        aux[i] = token[inicio_da_palavra + i];
        i++;
    }
    aux[i] = '\0'; // Finalizar a string com o terminador nulo
    return aux;
}

// leitura dos dados do dinossauro do arquivo binario
Dados leitura_dados_bin(FILE* arq, int* flag) {
    Dados dinossauro;
    char buffer[160];

    // Tenta ler o campo 'removido', se falhar, significa que chegou no final do arquivo
    if (fread(&dinossauro.removido, sizeof(char), 1, arq)) {
        fread(&dinossauro.encadeamento, sizeof(int), 1, arq);
        fread(&dinossauro.populacao, sizeof(int), 1, arq);
        fread(&dinossauro.tamanho, sizeof(float), 1, arq);
        fread(&dinossauro.unidadeMedida, sizeof(char), 1, arq);
        fread(&dinossauro.velocidade, sizeof(int), 1, arq);
        fread(buffer, bytes_dados - 18, 1, arq);  // L� o restante do registro no buffer
        dinossauro = atribuicao_dados(dinossauro, buffer);  // Preenche o struct com os dados
    } else {
        // Define flag =0  para indicar o fim do arquivo
        *flag = 0;
    }

    return dinossauro;
}

// faz a leitura dos dados do cabecalho, as retornando como parametro
Cabecalho leitura_cabecalho_bin(FILE* arq){
    Cabecalho titulo;
    if(fread(&titulo.status, sizeof(char), 1, arq)){
        fread(&titulo.topo, sizeof(int), 1, arq);
        fread(&titulo.proxRRN, sizeof(int), 1, arq);
        fread(&titulo.nroRegRem, sizeof(int), 1, arq);
        fread(&titulo.nroPagDisco, sizeof(int),1,arq);
        fread(&titulo.qttCompacta, sizeof(int),1, arq);
        fseek(arq, 1600, SEEK_SET); // posiciona vetor arq no final na pagina
    }
    if(titulo.status== '0'){
        printf("Falha no processamento do arquivo.\n");
        exit(-1);
    }
    return titulo;
}

// faz a leitura ate o proximo # e insere a informacao lida no local adequado
Dados insere_valor_conhecido(Dados dinossauro, int contador, int inicio_da_palavra, char* token) {
    char* inserir = le_string(token, inicio_da_palavra); // Obt�m a string at� o pr�ximo '#'

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
            free(inserir); // Liberar mem�ria se o contador estiver fora do esperado
            break;
    }
    return dinossauro;
}

// Funcao para inserir valor nulo na struct
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

// faz a leitura da parcela de dados de tamanho variavel sendo separados pelo # ate o seu final $
// inserindo as devidas informacoes nulas ou conhecidas conforme as especificacoes
Dados atribuicao_dados(Dados dinossauro, char* str){
    int posicao = 0;
    int aux;
    int contador = 0;
    int inicio_da_palavra = 0;

    while (str[posicao] != '$') {
        aux = 0;
        // Verifica se a pr�xima palavra � v�lida (n�o dupla ##)
        while (str[posicao] != '#' && str[posicao] != '$') {
            posicao++;
            aux = 1; // Indica que h� conte�do v�lido entre os '#'
        }

        // Se houver conte�do entre os '#'
        if (aux){
            dinossauro = insere_valor_conhecido(dinossauro, contador, inicio_da_palavra, str);
        }
        else{
            dinossauro = insere_valor_nulo(dinossauro, contador);
        }

        inicio_da_palavra = posicao + 1; // Posi��o inicial da pr�xima palavra
        posicao++; // Avan�a para o pr�ximo caractere ap�s o '#'
        contador++;
    }

    return dinossauro;
}

// Inicializa a struct de cabe�alho conforme as especificacoes
void cabecalho_arvore_inicia(Cabecalho_Arvore* cab_aux) {
    cab_aux->status= 0;
    cab_aux->noRaiz=-1;
    cab_aux->RRNproxNo= 0;
}

// Fun��o para preencher o restante do registro com n byte dados com '$'
void preenche_com_lixo(FILE* fp, int numero_bytes) {
    char lixo = '$';
    for (int i = 0; i < numero_bytes; i++) {
        fwrite(&lixo, sizeof(char), 1, fp);
    }
}

// printa os dados relacionados com o dinossauro
void printa_dados(Dados dinossauro) {
    if (dinossauro.nome != NULL && dinossauro.nome[0] != '\0') {
        printf("Nome: %s\n", dinossauro.nome);
    }
    if (dinossauro.especie != NULL && dinossauro.especie[0] != '\0') {
        printf("Especie: %s\n", dinossauro.especie);
    }
    if (dinossauro.tipo != NULL && dinossauro.tipo[0] != '\0') {
        printf("Tipo: %s\n", dinossauro.tipo);
    }
    if (dinossauro.dieta != NULL && dinossauro.dieta[0] != '\0') {
        printf("Dieta: %s\n", dinossauro.dieta);
    }
    if (dinossauro.habitat != NULL && dinossauro.habitat[0] != '\0') {
        printf("Lugar que habitava: %s\n", dinossauro.habitat);
    }
    if (dinossauro.tamanho > 0) {
        printf("Tamanho: %.1f m\n", dinossauro.tamanho);
    }
    if (dinossauro.velocidade > 0) {
        printf("Velocidade: %d %cm/h\n", dinossauro.velocidade, dinossauro.unidadeMedida);
    }
    printf("\n");
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