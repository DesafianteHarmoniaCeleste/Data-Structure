#include "Header.h"


// Função para ler um campo composto ou "NULO"
char* ler_campo_composto() {
    char buffer[300]; 
    char *result = NULL;

    // Lê a próxima string incluindo a possibilidade de ser uma string composta entre aspas
    if (scanf(" \"%299[^\"]\"", buffer) == 1) {
        // String composta entre aspas
        result = strdup(buffer);
    } else {
        // Se não for composta, tenta-se ler como um campo simples
        scanf(" %299s", buffer);  // Le-se a string sem aspas

        // Se for "NULO", retorna uma string com lixo conhecido
        if (strcmp(buffer, "NULO") == 0) {
            result = strdup("$");
        } else {
            result = strdup(buffer);  // Caso contrário, retorna o valor normal
        }
    }

    return result;  // Retorna o resultado final
}

// faz a leitura do dinossauro dado pelo operador
Dados leitura_dinossauro() {
    Dados dinossauro;
    
    // inicializacao do dinossauro
    dinossauro.nome = NULL;
    dinossauro.dieta = NULL;
    dinossauro.habitat = NULL;
    dinossauro.tipo = NULL;
    dinossauro.especie = NULL;
    dinossauro.alimento = NULL;
    dinossauro.populacao = -1;
    dinossauro.velocidade = -1;
    dinossauro.unidadeMedida = '$';  
    dinossauro.tamanho = -1.0;
    dinossauro.encadeamento = -1;
    dinossauro.removido = '0';

    //leitura dos campos em sua devida ordem
    //nome
    dinossauro.nome = ler_campo_composto();

    // dieta     
    dinossauro.dieta = ler_campo_composto();

    char buffer[50];  

    scanf(" %49s", buffer);
    if (strcmp(buffer, "NULO") == 0) {
        dinossauro.populacao = -1;
    } else {
        dinossauro.populacao = atoi(buffer);
    }

    // Tipo
    dinossauro.tipo = ler_campo_composto();

    // Velocidade
    scanf(" %49s", buffer);
    if (strcmp(buffer, "NULO") == 0) {
        dinossauro.velocidade = -1;
    } else {
        dinossauro.velocidade = atoi(buffer);
    }

    // Unidade de Medida
    scanf(" %49s", buffer);
    if (strcmp(buffer, "NULO") == 0) {
        dinossauro.unidadeMedida = '$';  // Indica valor nulo
    } else {
        dinossauro.unidadeMedida = buffer[0];  // Apenas o primeiro caractere
    }

    // Habitat
    dinossauro.habitat = ler_campo_composto();

    // Tamanho
    scanf(" %49s", buffer);
    if (strcmp(buffer, "NULO") == 0) {
        dinossauro.tamanho = -1.0;
    } else {
        dinossauro.tamanho = atof(buffer);
    }

    // Espécie
    dinossauro.especie = ler_campo_composto();

    // Alimento
    dinossauro.alimento = ler_campo_composto();

    return dinossauro;
}




// Função para preencher o restante do registro com '$'
void preenche_com_lixo_dinossauro(FILE* fp, int bytes_ja_escritos) {
    int bytes_restantes = bytes_dados - bytes_ja_escritos;
    char lixo = '$';
    for (int i = 0; i < bytes_restantes; i++) {
        fwrite(&lixo, sizeof(char), 1, fp);
    }
}

// Função para escrever o cabeçalho no arquivo binário
int escreve_cabecalho_bin(FILE* arq, Cabecalho cab) {

    int tam_pagina = PAG_DISCO;  // 1600 bytes (uma página de disco)
    char lixo = '$';  // Caractere de lixo
    int cabecalho_size = 21;  // Tamanho do cabeçalho (21 bytes)

    // Posiciona o ponteiro no início do arquivo
    fseek(arq, 0, SEEK_SET);

    // Escreve os campos do cabeçalho
    fwrite(&cab.status, sizeof(char), 1, arq);
    fwrite(&cab.topo, sizeof(int), 1, arq);
    fwrite(&cab.proxRRN, sizeof(int), 1, arq);
    fwrite(&cab.nroRegRem, sizeof(int), 1, arq);
    fwrite(&cab.nroPagDisco, sizeof(int), 1, arq);
    fwrite(&cab.qttCompacta, sizeof(int), 1, arq);

    // Preenche o restante da página com lixo ($)
    int bytes_restantes = tam_pagina - cabecalho_size;
    for (int i = 0; i < bytes_restantes; i++) {
        fwrite(&lixo, sizeof(char), 1, arq);
    }

    return 0;
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
    preenche_com_lixo_dinossauro(fp, bytes_escritos);
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

// funcao que realiza a insercao de novos registros
void insercao_novos_registros(){
    // nome dos arquivos que serao dados pelo operador
    char nome_arquivo_principal[50];
    char nome_arquivo_indices[50];
    scanf("%s", nome_arquivo_principal); // Lê o nome do arquivo
    scanf("%s", nome_arquivo_indices); // Lê o nome do arquivo

    // criacao do arquivo com os nomes dados, e seu devido modo de escrita, ja declara erro caso haja erros na criacao do mesmo
    FILE* arq_principal = criarArquivo(nome_arquivo_principal, "rb+");
    FILE* arq_indices = criarArquivo(nome_arquivo_indices, "rb+");

    // leitura dos respectivos cabecalhos
    Cabecalho titulo_principal = leitura_cabecalho_bin(arq_principal);  // Lê o cabeçalho uma vez
    Cabecalho_Arvore titulo_arvore = leitura_cabecalho_arvore(arq_indices);

    // titulo recebe status 0 pois ira comecar a escrita no arquivo
    titulo_principal.status = '0';
    titulo_arvore.status = '0';

    // declaracao do numero de insercoes a serem realizadas  
    int numero_insercoes;
    scanf("%d", &numero_insercoes);
    getchar();  // limpa buffer do scanf

    long long Byte_arquivo_bin; // leitura da posicao do byte no arquivo principal de dados

    Dados dinossauro_inserido; // utilizado para ler as informacoes do dinossauro atual

    while(numero_insercoes){
        // salva valores inseridos no dinossauro a ser inserido
        
        dinossauro_inserido = leitura_dinossauro();
        
        if(titulo_principal.topo!= -1){   // insere em espaco removido
            fseek(arq_principal, 160*(titulo_principal.topo), SEEK_CUR); // RRN comeca em 0
            titulo_principal.topo = RRN_removido(arq_principal);
            fseek(arq_principal, -(18), SEEK_CUR); // volta no inicio do dinossauro removido para sobrescrever
            Byte_arquivo_bin = ftell(arq_principal);
            escreve_dino_bin(arq_principal, dinossauro_inserido);
            insere_arvore_binaria(arq_indices, dinossauro_inserido.nome, Byte_arquivo_bin, titulo_arvore.RRNproxNo+1, &titulo_arvore); // soma +1 pois rrnproxno comeca em 0 e ele cria o cabecalho caso necessario
            titulo_principal.nroRegRem = titulo_principal.nroRegRem - 1; 
        }
        else{ // insere no proxrrn do titulo
            fseek(arq_principal, 160*(titulo_principal.proxRRN), SEEK_CUR); // RRN comeca em 0
            titulo_principal.proxRRN = titulo_principal.proxRRN + 1;
            Byte_arquivo_bin = ftell(arq_principal);
            escreve_dino_bin(arq_principal, dinossauro_inserido);
            insere_arvore_binaria(arq_indices, dinossauro_inserido.nome, Byte_arquivo_bin, titulo_arvore.RRNproxNo+1, &titulo_arvore); // soma +1 pois rrnproxno comeca em 0 e ele cria o cabecalho caso necessario
        }
        fseek(arq_principal, 1600, SEEK_SET); // volta no fim na pagina do cabecalho
        numero_insercoes--;
    }

    // finaliza transcricao com sucesso e o escreve 
    titulo_arvore.status= '1'; 
    escreve_cabecalho_arvore(arq_indices, titulo_arvore);

    titulo_principal.nroPagDisco = (titulo_principal.proxRRN-1)/10 +2; // numero de dinossauros / quantos dinossauros por pagina
    // terminou de inserir registros, status recebe 1 ( nao ha inconsistencias )
    titulo_principal.status = '1';
    fseek(arq_principal, 0, SEEK_SET);
    escreve_cabecalho_bin(arq_principal, titulo_principal);

    // fecha os arquivos
    fclose(arq_principal);
    fclose(arq_indices);

    // imprime o valor dos binarios presentes no arquivo para afericao 
    binarioNaTela(nome_arquivo_indices);

    return ;
}