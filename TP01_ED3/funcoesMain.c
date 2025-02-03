#include "header.h"


void le_csv_escreve_bin(){
    // Lê os dados do CSV e armazena na struct
    char nome_arquivo_csv[20];
    scanf("%s", nome_arquivo_csv); // Lê o nome do arquivo

    FILE* arq_csv = criarArquivo(nome_arquivo_csv, "r");

    char nome_arquivo_bin[20];
    scanf("%s", nome_arquivo_bin); // Lê o nome do arquivo
    getchar();
    FILE* arq_bin = criarArquivo(nome_arquivo_bin, "wb");
    fseek(arq_bin, 1600, SEEK_SET); // deixa devido espaco para cabecalho
    // Inicializa a struct Cabecalho
    Cabecalho cabecalho;
    cabecalho_inicia(&cabecalho);

    char linha[MAX_LINHA];
    Dados animal;
    // Ignora a primeira linha (cabeçalho)
    fgets(linha, sizeof(linha), arq_csv);
    int flag=1;
    while(1){ // enquanto nao chegar ao fim da leitura do csv
        animal = ler_csv(arq_csv, &flag);
        if(flag==0){ // chegou ao fim do arquivo
            break;
        }
        escreve_dino_bin(arq_bin, animal); // escreve no arquivo binario caso contrario
        cabecalho.proxRRN= cabecalho.proxRRN+1;
    }

    cabecalho.nroPagDisco= (cabecalho.proxRRN)/10 + 2; // cada pagina tem 10 dinossauros, e tem mais 1 pagina de cabecalho e a outra eh a ultima
    cabecalho.status = '1';

    fseek(arq_bin, 0, SEEK_SET); // vetor de arquivo arq no inicio para escrita de cabecalho no local devido
    escreve_cabecalho_bin(arq_bin, cabecalho);
    fclose(arq_bin);
    fclose(arq_csv);
    // Exibe o conteúdo binário na tela
    binarioNaTela(nome_arquivo_bin);

}

void recupera_dados_bin() {
    char nome_arquivo[20];
    scanf("%s", nome_arquivo);  // Nome do arquivo
    FILE* arq= criarArquivo(nome_arquivo, "rb");

    int flag=1; // indica se ja terminou a leitura do arquivo

    Cabecalho titulo = leitura_cabecalho_bin(arq);  // Lê o cabeçalho
    int leu_registro=0; // verifica se houve leitura do arquivo
    Dados dinossauro;

    // Lê registros até encontrar o fim (indicado por removido = '%')
    do{
        dinossauro = leitura_dados_bin(arq, &flag);

        // Se o dinossauro não estiver marcado como removido e não for o final do arquivo
        if (dinossauro.removido == '0' && flag==1) {
            printa_dados(dinossauro);
            leu_registro++;
        }
    }while(flag ==1);  // Para quando encontrar o fim
    if(!leu_registro){
        printf("Registro inexistente.\n");
    }
    
    printf("Numero de paginas de disco: %d\n\n", titulo.nroPagDisco);

    fclose(arq);
}

void recupera_dados_where_bin(){ // funcao que printa registro e nome de registro especifico

    char nome_arquivo[20];
    scanf("%s", nome_arquivo); // como nome de arquivo nao possui espaco, pode utilizar scanf

    FILE* arq= criarArquivo(nome_arquivo, "rb");

    // declaracao de variaveiis utilizadas
    Cabecalho titulo;

    char* NomeCampo;
    char* Valorcampo;
    char buffer[400]; // buffer para ler a entrada do usuario para entao poder ser separado corretamente para operacao

    int aux=0;
    int verdadeiro=1;

    // quantidade de buscas a serem realizadas
    int quantidade_insercoes;
    scanf("%d", &quantidade_insercoes);

    int numero_buscas = 1;
    int verifica_achado = 0;
    getchar(); // pega o \n dado no scanf, permite bom funcionamento do fgets

    fgets(buffer, 400, stdin);
    char *token; // separa a entrada buffer em espacos " "
    token = strtok(buffer, " ");
    int flag=1; // flag para indicar fim de leitura de arquivo

    while(quantidade_insercoes){ // enquanto houver buscas

        NomeCampo = token;
        token = strtok(NULL, " ");
        if(aux){
            fseek(arq, 1600, SEEK_SET); // reposiciona no in�cio dos dados
        }
        Dados dinossauro;
        NomeCampo[strcspn(NomeCampo, "\n")] = 0; // Remove o '\n' se presente
        Valorcampo = token;
        token = strtok(NULL, " ");

        removequotes(Valorcampo);

        printf("Busca %d\n", numero_buscas);
        do{
            if(!aux){
                titulo = leitura_cabecalho_bin(arq); // le cabecalho uma vez apenas
                aux++;
            }
            else{
                    dinossauro  = leitura_dados_bin(arq, &flag);
                    verdadeiro= verifica_dino(dinossauro, NomeCampo, Valorcampo);
                    if(verdadeiro==1){
                        printa_dados(dinossauro);
                        verifica_achado++;
                    }
                }
        }
        while(flag ==1); // At� n�o serem lidos mais registros
        quantidade_insercoes--;
        numero_buscas++;
        if(!verifica_achado){
            printf("Registro inexistente.\n\n");
        }
        printf("Numero de paginas de disco: %d\n\n", titulo.nroPagDisco);
        verifica_achado = 0;
        flag=1;
        fflush(stdin);
        }

    fclose(arq);
    return;
}

void insercao_novos_registros(){
    char nome_arquivo[20];
    scanf("%s", nome_arquivo); // Lê o nome do arquivo

    FILE* arq = criarArquivo(nome_arquivo, "rb+");

    Cabecalho titulo = leitura_cabecalho_bin(arq);  // Lê o cabeçalho uma vez
    // titulo recebe status 0 pois ira comecar a escrita no arquivo
    titulo.status = '0';
    int numero_insercoes;
    scanf("%d", &numero_insercoes);
    getchar();

    Dados dinossauro_inserido;

    while(numero_insercoes){
        // salva valores inseridos no dinossauro a ser inserido
        dinossauro_inserido = leitura_dinossauro();
        if(titulo.topo!= -1){   // insere em espaco removido
            fseek(arq, 160*(titulo.topo), SEEK_CUR); // RRN comeca em 0
            titulo.topo = RRN_removido(arq);
            fseek(arq, -(18), SEEK_CUR); // volta no inicio do dinossauro removido para sobrescrever
            escreve_dino_bin(arq, dinossauro_inserido);
            titulo.nroRegRem = titulo.nroRegRem - 1; /// nao ta funcionando
        }
        else{ // insere no proxrrn do titulo
            fseek(arq, 160*(titulo.proxRRN), SEEK_CUR); // RRN comeca em 0
            titulo.proxRRN = titulo.proxRRN + 1;
            escreve_dino_bin(arq, dinossauro_inserido);
        }
        fseek(arq, 1600, SEEK_SET); // volta no fim na pagina do cabecalho
        numero_insercoes--;
    }

    titulo.nroPagDisco = (titulo.proxRRN-1)/10 +2; /// numero de dinossauros / quantos dinossauros por pagina
    // terminou de inserir registros, status recebe 1 ( nao ha inconsistencias )
    titulo.status = '1';
    fseek(arq, 0, SEEK_SET);
    escreve_cabecalho_bin(arq, titulo);
    fclose(arq);

    binarioNaTela(nome_arquivo);

    return ;
}

void remove_dados() {
    char nome_arquivo[20];
    scanf("%s", nome_arquivo); // Lê o nome do arquivo

    // cria arquivo para leitura e escrita
    FILE* arq = criarArquivo(nome_arquivo, "rb+");

    Cabecalho titulo = leitura_cabecalho_bin(arq);  // Lê o cabeçalho uma vez
    // titulo recebe status 0 pois ira comecar a escrita no arquivo
    titulo.status='0';
    char* NomeCampo;
    char* Valorcampo;
    Dados dinossauro_removido;
    int quantidade_remocoes;

    // Lê a quantidade de remoções a serem feitas
    scanf("%d", &quantidade_remocoes);
    char buffer[400];
    getchar();

    // leitura em buffer da entrada do operador, separa os campos pelas barras de espacos
    fgets(buffer, 400, stdin);
    char *token;
    token = strtok(buffer, " ");
    int flag = 1;

    for (int i = 0; i < quantidade_remocoes; i++) {

        NomeCampo = token;
        token = strtok(NULL, " ");
        NomeCampo[strcspn(NomeCampo, "\n")] = 0;  // Remove o '\n' se presente

        Valorcampo = token;
        token = strtok(NULL, " ");

        // Lê o valor do campo de acordo com o tipo do campo
        removequotes(Valorcampo);

        int rrn_atual = -1;

        // Começa a buscar e remover registros
        while (1) {
            // Lê o próximo dinossauro e retorna o RRN
            dinossauro_removido = leitura_dados_bin(arq, &flag);

            if (flag == 0) {
                break;  // Fim do arquivo
            }

            rrn_atual++;  // Calcula o RRN atual

            // Verifica se o registro já foi removido
            if (dinossauro_removido.removido == '1') {
                continue;  // Pula registros que já foram removidos
            }

            // Verifica se o dinossauro corresponde ao critério de busca
            if (verifica_dino(dinossauro_removido, NomeCampo, Valorcampo) == 1) {

                // Remove o dinossauro e atualiza o topo
                remove_dinossauro(arq, dinossauro_removido, &titulo, rrn_atual);
                titulo.nroRegRem++;  // Incrementa o número de registros removidos
            }
        }
        fseek(arq, 1600, SEEK_SET);  // Reposiciona no início dos dados
        flag=1;
    }
    // atualiza cabecalho para verdadeiro pois as remocoes ocorreram sem corrupcoes
    titulo.status='1';
    // Atualiza o cabeçalho no arquivo após as remoções
    fseek(arq, 0, SEEK_SET);
    escreve_cabecalho_bin(arq, titulo);

    fclose(arq); // Fecha o arquivo

    binarioNaTela(nome_arquivo);
}

void compactacao_arquivo() {  // Função de compactação (função 6)
    char nome_arquivo_principal[20];
    scanf("%s", nome_arquivo_principal);  // Lê o nome do arquivo

    // Abre o arquivo principal em modo leitura/escrita binária
    FILE* arq = criarArquivo(nome_arquivo_principal, "rb+");

    // Abre o arquivo auxiliar em modo escrita binária (vai sobrescrever ou criar um novo)
    FILE* arq_aux = criarArquivo("arquivo_aux", "wb+");

    int flag = 1;  // Indica o fim da leitura do arquivo
    int contador = 0;  // Indica quantos dinossauros não são removidos

    Dados dinossauro;
    Cabecalho cabec = leitura_cabecalho_bin(arq);  // Lê o cabeçalho uma vez
    cabec.status = '0';  // Marca o arquivo como inconsistente durante a compactação

    // Processa cada dinossauro do arquivo principal
    while (flag) {
        dinossauro = leitura_dados_bin(arq, &flag);  // Lê um registro do arquivo principal
        if (flag == 0) {
            break;  // Sai do loop quando não houver mais registros
        }
        // Se o dinossauro não foi removido, escreve no arquivo auxiliar
        if (dinossauro.removido == '0') {
            escreve_dino_bin(arq_aux, dinossauro);  // Escreve o dinossauro no arquivo auxiliar
            contador++;
        } else {
            // Atualiza o cabeçalho para refletir a quantidade de registros removidos
            cabec.nroRegRem = cabec.nroRegRem - 1;
        }
    }

    // Atualiza as informações do cabeçalho após a compactação
    cabec.topo = -1;  // Não tem mais registros removidos
    cabec.proxRRN = contador;
    cabec.qttCompacta = cabec.qttCompacta + 1;
    cabec.nroPagDisco = (cabec.proxRRN) / 10 + 2;
    cabec.status = '1';  // Marca o arquivo como consistente

    fclose(arq);

    FILE* arq_escrita = criarArquivo(nome_arquivo_principal, "wb");
    
    // Copia os dados do arquivo auxiliar de volta para o arquivo principal
    rewind(arq_escrita);  // Volta o ponteiro para o início do arquivo principal
    rewind(arq_aux);  // Volta o ponteiro para o início do arquivo auxiliar
    escreve_cabecalho_bin(arq_escrita, cabec);
    copiar_arquivo(arq_aux, arq_escrita);  // Copia todo o conteúdo do arquivo auxiliar para o principal

    // Fecha os arquivos
    fclose(arq_escrita);
    fclose(arq_aux);

    // Exibe o binário final na tela
    binarioNaTela(nome_arquivo_principal);
    return;
}

