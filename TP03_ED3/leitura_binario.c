#include "leitura_binario.h"
/*
    Arquivo que contem todas as funcoes utilizadas em trabalhos anteriores,
    cuja funcao principal se resume na leitura correta do arquivo binario dado
*/

Grafo* recupera_dados_bin() {

    // operacoes relacionadas ao arquivo binario
    char nome_arquivo[20];
    scanf("%s", nome_arquivo);  // Nome do arquivo
    FILE* arq= criarArquivo(nome_arquivo, "rb");

    int flag=1; // indica se ja terminou a leitura do arquivo

    Cabecalho titulo = leitura_cabecalho_bin(arq);  // Lê o cabeçalho
    int leu_registro=0; // verifica se houve leitura do arquivo
    Dados dinossauro;

    // operacoes relacionadas ao grafo
    Grafo* grafo_universal = cria_grafo(); // inicializa o grafo com zero arestas, insercoes, adjacencias
    // Lê registros até encontrar o fim (indicado por removido = '%')
    do{
        dinossauro = leitura_dados_bin(arq, &flag);
        // Se o dinossauro não estiver marcado como removido e não for o final do arquivo e se existir o dinossauro
        if (dinossauro.removido == '0' && flag==1 && dinossauro.populacao!= -1) {
            //printa_dados(dinossauro);
            insere_grafo(grafo_universal, dinossauro);
            leu_registro++;
            //if(DEBBUG){printf("Numero de Registros lidos ate a fallha %d \n", leu_registro);}
        }
    }while(flag ==1);  // Para quando encontrar o fim
    if(!leu_registro){
        printf("Registro inexistente.\n");
        fclose(arq);
    }

    // calculo dos graus de entrada dos dinossauros
    ajusta_graus(grafo_universal);

    fclose(arq);
    return grafo_universal;
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
        fread(buffer, bytes_dados - 18, 1, arq);  // Lê o restante do registro no buffer
        dinossauro = atribuicao_dados(dinossauro, buffer);  // Preenche o struct com os dados
    } else {
        // Define flag =0  para indicar o fim do arquivo
        *flag = 0;
    }

    return dinossauro;
}

// faz a leitura dos dados para insecao de dinossauro no registro de acordo com as especificacoes
Dados leitura_dinossauro() {
    Dados dinossauro;
    char aux[300];

    // Inicializa os ponteiros de string como NULL
    dinossauro.nome = NULL;
    dinossauro.dieta = NULL;
    dinossauro.habitat = NULL;
    dinossauro.tipo = NULL;
    dinossauro.especie = NULL;
    dinossauro.alimento = NULL;

    // Lê e trata cada campo da estrutura

    // Nome
    scanf("%s", aux);
    removequotes(aux);
    if (!strcmp(aux, "NULO")) {
        dinossauro.nome = strdup("");  // Aloca uma string vazia
    } else {
        dinossauro.nome = strdup(aux);  // Aloca e copia a string
    }

    // Dieta
    scanf("%s", aux);
    removequotes(aux);
    if (!strcmp(aux, "NULO")) {
        dinossauro.dieta = strdup("");
    } else {
        dinossauro.dieta = strdup(aux);
    }

    // Habitat
    scanf("%s", aux);
    removequotes(aux);
    if (!strcmp(aux, "NULO")) {
        dinossauro.habitat = strdup("");
    } else {
        dinossauro.habitat = strdup(aux);
    }

    // População
    scanf("%s", aux);
    removequotes(aux);
    if (!strcmp(aux, "NULO")) {
        dinossauro.populacao = -1;
    } else {
        dinossauro.populacao = atoi(aux);  // Converte para int
    }

    // Tipo
    scanf("%s", aux);
    removequotes(aux);
    if (!strcmp(aux, "NULO")) {
        dinossauro.tipo = strdup("");
    } else {
        dinossauro.tipo = strdup(aux);
    }

    // Velocidade
    scanf("%s", aux);
    removequotes(aux);
    if (!strcmp(aux, "NULO")) {
        dinossauro.velocidade = -1;
    } else {
        dinossauro.velocidade = atoi(aux);  // Converte para int
    }

    // Unidade de Medida
    scanf("%s", aux);
    removequotes(aux);
    if (!strcmp(aux, "NULO")) {
        dinossauro.unidadeMedida = '$';  // Indica valor nulo com símbolo $
    } else {
        dinossauro.unidadeMedida = aux[0];  // Apenas o primeiro caractere
    }

    // Tamanho
    scanf("%s", aux);
    removequotes(aux);
    if (!strcmp(aux, "NULO")) {
        dinossauro.tamanho = -1.0;
    } else {
        dinossauro.tamanho = atof(aux);  // Converte para float
    }

    // Espécie
    scanf("%s", aux);
    removequotes(aux);
    if (!strcmp(aux, "NULO")) {
        dinossauro.especie = strdup("");
    } else {
        dinossauro.especie = strdup(aux);
    }

    // Alimento
    scanf("%s", aux);
    removequotes(aux);
    if (!strcmp(aux, "NULO")) {
        dinossauro.alimento = strdup("");
    } else {
        dinossauro.alimento = strdup(aux);
    }

    // Inicializa outros campos
    dinossauro.encadeamento = -1;
    dinossauro.removido = '0';

    return dinossauro;
}

// Função para ler a string até encontrar '#'
char* le_string(char* token, int inicio_da_palavra) {
    int i = 0;
    // Usar memória dinâmica para armazenar a string lida
    char* aux = (char*)malloc(100 * sizeof(char));
    if (!aux) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }

    while (token[inicio_da_palavra + i] != '#' && token[inicio_da_palavra + i] != '\0') {
        aux[i] = token[inicio_da_palavra + i];
        i++;
    }
    aux[i] = '\0'; // Finalizar a string com o terminador nulo
    return aux;
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
    return titulo;
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

FILE* criarArquivo(const char* nomeArquivo, const char* modo) {
    FILE* arquivo = fopen(nomeArquivo, modo);  // Abre o arquivo no modo passado

    if (arquivo == NULL) {  // Verifica se a abertura do arquivo falhou
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }

    return arquivo;  // Retorna o ponteiro para o arquivo
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