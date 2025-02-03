#include "header.h"

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

// faz a leitura do arquivo csv dinossauro por dinossauro
Dados ler_csv(FILE* fp, int* flag) {
    char linha[MAX_LINHA];
    char temp_nome[100] = "", temp_dieta[100] = "", temp_habitat[100] = "", temp_tipo[100] = "", temp_especie[100] = "", temp_alimento[100] = "";
    int temp_populacao = 0, temp_velocidade = 0;
    float temp_tamanho = 0.0;
    char temp_unidadeMedida = ' ';
    Dados animais;

    // Leitura linha por linha
    if (fgets(linha, sizeof(linha), fp)) {
        linha[strcspn(linha, "\n")] = 0;  // Remove o newline no final da linha
        linha[strcspn(linha, "\r")] = 0;

        // Contador para os campos
        int campo = 0;

        // Ponteiro para percorrer a linha
        char* inicio = linha;
        char* fim = NULL;

        // Itera sobre a linha até processar todos os campos (separados por ',')
        while ((fim = strchr(inicio, ',')) != NULL || *inicio != '\0') {
            // Último campo (não seguido de vírgula)
            if (fim == NULL) {
                fim = inicio + strlen(inicio);
            }

            // Tamanho do campo atual
            int tamanho = fim - inicio;

            // Processa o campo atual com base no número de campo
            switch (campo) {
                case 0: // Nome
                    if (tamanho > 0) strncpy(temp_nome, inicio, tamanho);
                    break;
                case 1: // Dieta
                    if (tamanho > 0) strncpy(temp_dieta, inicio, tamanho);
                    break;
                case 2: // Habitat
                    if (tamanho > 0) strncpy(temp_habitat, inicio, tamanho);
                    break;
                case 3: // População
                    if (tamanho > 0){
                        temp_populacao = atoi(inicio);
                    }
                    else{
                        temp_populacao= -1;
                    }
                    break;
                case 4: // Tipo
                    if (tamanho > 0) strncpy(temp_tipo, inicio, tamanho);
                    break;
                case 5: // Velocidade
                    if (tamanho > 0){
                        temp_velocidade = atoi(inicio);
                    }
                    else{
                        temp_velocidade = -1;
                    }
                    break;
                case 6: // Unidade de Medida
                    if (tamanho > 0){
                        temp_unidadeMedida = inicio[0];
                    }
                    else{
                        temp_unidadeMedida = '$';
                    }
                    break;
                case 7: // Tamanho
                    if (tamanho > 0){
                        temp_tamanho = atof(inicio);
                    }
                    else{
                        temp_tamanho = -1;
                    }
                    break;
                case 8: // Espécie
                    if (tamanho > 0) strncpy(temp_especie, inicio, tamanho);
                    break;
                case 9: // Alimento
                    if (tamanho > 0) strncpy(temp_alimento, inicio, tamanho);
                    break;
            }

            // Passa para o próximo campo
            campo++;

            // Move o ponteiro para o próximo campo
            if (*fim == ',') {
                inicio = fim + 1;  // Após a vírgula
            } else {
                break;  // Chegou ao fim da linha
            }
        }

        // Aloca dinamicamente e copia os valores para cada campo da struct
        animais.nome = duplica_string(temp_nome);
        animais.dieta = duplica_string(temp_dieta);
        animais.habitat = duplica_string(temp_habitat);
        animais.populacao = temp_populacao;  // Inteiro
        animais.tipo = duplica_string(temp_tipo);
        animais.velocidade = temp_velocidade;  // Inteiro
        animais.unidadeMedida = temp_unidadeMedida;  // Caractere
        animais.tamanho = temp_tamanho;  // Float
        animais.especie = duplica_string(temp_especie);
        animais.alimento = duplica_string(temp_alimento);
        animais.removido = '0';
        animais.encadeamento = -1;

        *flag = 1;
    } else {
        *flag = 0; // caso nao leita o arquivo, flag=0 para indicar seu fim
    }

    return animais;
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
