#include "Header.h"

void recupera_dados_arvore(){
    // nome dos arquivos de dados e de indices que serao dadoss pelo operador
    char nome_arquivo_dados[50];
    char nome_arquivo_arvore[50];

    // como nao tem bbarra de espaco no nome do arquivo, pode ser utilizado scanf
    scanf("%s", nome_arquivo_dados);
    scanf("%s", nome_arquivo_arvore);

    // criacao dos arquivos com o seu nome e deeclaracao do modo de escrita, alem de verificacao caso haja algum erro de criacao de arquivo
    FILE* arquivo_dados = criarArquivo(nome_arquivo_dados, "rb");
    FILE* arquivo_arvore = criarArquivo(nome_arquivo_arvore, "rb");

    // leitura do cabecalho da arvore ja verificando se ele esta corrompido ou nao
    Cabecalho_Arvore titulo_arvore = leitura_cabecalho_arvore(arquivo_arvore);
    char le_nome[10]; // criada apenas para ler a entrada nome
    char dinossauro[50]; // le o nome do dinossauro para ser procurado em arvore binaria
    
    scanf("%s", le_nome);
    getchar(); // limpa buffer para usar fgets

    fgets(dinossauro, 50*sizeof(char), stdin); // le dinossauro para busca em arquivo binario
    removequotes(dinossauro);
    long long chave = converteNome(dinossauro); // converte o char nome para long long para insercao no arquivo arvore bin
    long long Posicao_byte;

    // caso nao encontre
    if( busca_chave_binario(arquivo_arvore, chave, &Posicao_byte, &titulo_arvore) == 0){
        printf("Registro inexistente.\n");
        return ;
    }

    // flag usado para caso haja erro na leitura
    int flag=1;
    fseek(arquivo_dados, Posicao_byte, SEEK_SET); // posiciona ponteiro para leitura no local correto
    Dados dinossauro_lido = leitura_dados_bin(arquivo_dados, &flag); // leitura do dinossauro correspondente a posicao buscada
    printa_dados(dinossauro_lido); // print das informacoes do dinossauro

    // fecha-se os arquivos abertos
    fclose(arquivo_arvore);
    fclose(arquivo_dados);

    return ;
};

// faz a leitura do cabecalho da arvore binaria
Cabecalho_Arvore leitura_cabecalho_arvore(FILE* arq_arvore_bin){
    Cabecalho_Arvore titulo_aux; // titulo auxiliar que recebera as informacoes e sera retornado
    fseek(arq_arvore_bin, 0, SEEK_SET); // posiciona vetor no inicio para leitura do cabecalho

    fread(&titulo_aux.status, sizeof(char), 1, arq_arvore_bin); // leitura do status do arquivo
    if(titulo_aux.status == '0'){ // caso arquivo esteja corrompido, nao realiza programa
        printf("Falha no processamento do arquivo.\n");
        exit(-1);
    }
    fread(&titulo_aux.noRaiz, sizeof(int), 1, arq_arvore_bin); // leitura de qual eh o no da raiz
    fread(&titulo_aux.RRNproxNo, sizeof(int), 1, arq_arvore_bin); // leitura de qual eh o proxno disponivel para criar pagina nova
    return titulo_aux; // retorno do titulo lido
}

// retorno eh se encontrou a busca
int busca_chave_binario(FILE* arq_arvore_bin, long long chave, long long* Posicao_byte, Cabecalho_Arvore* titulo_arvore) {
    Registro_Dados no_atual; // responsavel por varrer os nos
    fseek(arq_arvore_bin, tam_PagDisco*(titulo_arvore->noRaiz+1) ,SEEK_SET); // descolca vetor posicao do arquivo para a raiz da arvore
    // Loop para descer na arvore ate encontrar um no folha
    while (1) {
        no_atual = leitura_no_arvore(arq_arvore_bin); // le o no atual

        // procura chave no no atual
        for(int i=0; i<no_atual.nroChavesIndexadas ; i++){
            if( chave == no_atual.chaves[i].chave){ // encontrou valor
                *Posicao_byte = no_atual.chaves[i].RRN_chave;
                return 1;
            }
        }
        if(no_atual.folha=='0') { // descera nos ponteiros para achar chave
            // O no nao eh folha, entao precisamos descer mais um nivel
            if (chave < no_atual.chaves[0].chave) { // caso chave seja menor que a menor chave do no
                fseek(arq_arvore_bin, tam_PagDisco * (no_atual.Ponteiro_filho[0] + 1), SEEK_SET); // soma 1 pois esta pulando a primeira pagina de cabecalho
            }
            else if (chave > no_atual.chaves[no_atual.nroChavesIndexadas - 1].chave) { // caso chave seja maior que a maior chave do no
                fseek(arq_arvore_bin, tam_PagDisco * (no_atual.Ponteiro_filho[no_atual.nroChavesIndexadas] + 1), SEEK_SET);
            }
            else{ // caso em que se entra num ponteiro dentro do no
                // Encontra o ponteiro correto para descer
                for (int i = 0; i < no_atual.nroChavesIndexadas - 1; i++){
                    if (chave < no_atual.chaves[i + 1].chave) {
                        fseek(arq_arvore_bin, tam_PagDisco * (no_atual.Ponteiro_filho[i + 1] + 1), SEEK_SET);
                        break;
                    }
                }
            }
        }
        else{ // eh no folha e nao encontrou
            return 0;
        }
    }
}