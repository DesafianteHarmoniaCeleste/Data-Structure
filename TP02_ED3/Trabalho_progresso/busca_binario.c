#include "Header.h"

// escreve arvore binaria do zero a partir de arquivo binario
void cria_arvoreB(){
    // nome dos arquivos a serem abertos, dados pelo operador  
    char nome_arquivo_dados[50];
    char nome_arquivo_arvore[50];

    // como nao tem bbarra de espaco no nome do arquivo, pode ser utilizado scanf
    scanf("%s", nome_arquivo_dados);
    scanf("%s", nome_arquivo_arvore);

    // cria os arquivos com os nomes dados e com o seu respectivo modo de abertura, caso haja erros, da saida -1 e printa saida de erro
    FILE* arquivo_dados = criarArquivo(nome_arquivo_dados, "rb");
    FILE* arquivo_arvore = criarArquivo(nome_arquivo_arvore, "w+b");

    int flag=1; // indica se ja terminou a leitura do arquivo / primeira insercao

    Cabecalho titulo = leitura_cabecalho_bin(arquivo_dados);  // Le o cabecalho ja verificando se ha arquivo corrompido

    Cabecalho_Arvore titulo_arvore;
    cabecalho_arvore_inicia(&titulo_arvore); //inicia titulo do arquivo de indicies que sera criado

    Dados dinossauro; // dinossauro responsavel por receber dados lidos do arquivo binario de dados
    long long Posicao_byte; // posicao no arquivo de dados do dinossauro a ser inserido

    // Le registros ate encontrar o fim 
    do{
        Posicao_byte= ftell(arquivo_dados); // obtem posicao byte do arquivo de dados onde o no se citua
        dinossauro = leitura_dados_bin(arquivo_dados, &flag); // faz leitura do dinossauro

        // Se o dinossauro nao estiver marcado como removido e nao for o final do arquivo
        if (dinossauro.removido == '0' && flag!=0) {
            insere_arvore_binaria(arquivo_arvore, dinossauro.nome, Posicao_byte, flag, &titulo_arvore);
            flag++; // indica que primeira insercao na arvore binaria ja foi feita
        }
    }while(flag !=0);  // Para quando encontrar o fim

    //atualiza cabec da arv no arq
    titulo_arvore.status= '1'; // finaliza transcricao com sucesso

    escreve_cabecalho_arvore(arquivo_arvore, titulo_arvore);
    
    fclose(arquivo_arvore);
    fclose(arquivo_dados);

    binarioNaTela(nome_arquivo_arvore);

    return;
}

// escreve as informacoes do cabecalho no arquivo da arvore de indices
void escreve_cabecalho_arvore(FILE* arquivo_arvore, Cabecalho_Arvore titulo_arvore){
    fseek(arquivo_arvore,0,SEEK_SET); // posiciona ponteiro no inicio do arquivo para escrita de informacoes do cabecalho da arvore
    fwrite(&titulo_arvore.status , sizeof(char), 1, arquivo_arvore );
    fwrite(&titulo_arvore.noRaiz , sizeof(int), 1, arquivo_arvore );
    fwrite(&titulo_arvore.RRNproxNo , sizeof(int), 1, arquivo_arvore );

    return ;
}

// cria o cabecalho com lixo
void Cria_cabecalho(FILE* arq_arvore_bin){
    //posiciona ponteiro do arquivo em seu inicio
    fseek(arq_arvore_bin, 0, SEEK_SET);
    preenche_com_lixo(arq_arvore_bin, tam_PagDisco);
    return ;
}


// cria pagina no arquivo de arvore binaria, preenchendoo com lixo
void Cria_Pagina_arvore(FILE* arq_arvore_bin, Cabecalho_Arvore* titulo_arvore, char folha, int chaves_indexadas) {
    // Posiciona o ponteiro do arquivo com base no RRN proximo no
    fseek(arq_arvore_bin, tam_PagDisco * (titulo_arvore->RRNproxNo + 1 ), SEEK_SET); // soma 1 pois RRNproxNo comeca em 0
    
    fwrite( &folha  ,sizeof(char), 1, arq_arvore_bin); // folha
    fwrite( &chaves_indexadas ,sizeof(int), 1, arq_arvore_bin); // nro chave indexada
    fwrite( &titulo_arvore->RRNproxNo ,sizeof(int), 1, arq_arvore_bin); // RRNdoNO

    // Atualiza o valor de RRNproxNo diretamente na estrutura original
    titulo_arvore->RRNproxNo = titulo_arvore->RRNproxNo + 1;

    // retorna vetor para o iinicio da pagina para futura insercao
    int aux= Ordem_m-1;
    int valor_invalido = -1;
    long long chave_invalida = -1;

    fwrite(&valor_invalido, sizeof(int), 1, arq_arvore_bin); // Ponteiro filho
    while (aux) {

        fwrite(&chave_invalida, sizeof(long long), 1, arq_arvore_bin); // Chave
        fwrite(&chave_invalida, sizeof(long long), 1, arq_arvore_bin); // RRN chave
        fwrite(&valor_invalido, sizeof(int), 1, arq_arvore_bin); // Ponteiro filho

        aux--;
    }
    fseek(arq_arvore_bin, -(tam_PagDisco), SEEK_CUR); // posiciona vetor no inicio do no para insercao no noh recem criado
    return ;
}

// escreve registro na arvore binaria e preenche resto do no com lixo
void escreve_registro_arvore(FILE* arq_arvore_bin, Registro_Dados* no_inserido) {
    // escreve informacoes principais acerca do no
    fwrite(&no_inserido->folha, sizeof(char), 1, arq_arvore_bin);
    fwrite(&no_inserido->nroChavesIndexadas, sizeof(int), 1, arq_arvore_bin);
    fwrite(&no_inserido->RRNdoNo, sizeof(int), 1, arq_arvore_bin);

    int i = 0;
    // Escreve as chaves e ponteiros do nó
    while (i < no_inserido->nroChavesIndexadas) {
        fwrite(&no_inserido->Ponteiro_filho[i], sizeof(int), 1, arq_arvore_bin);
        fwrite(&no_inserido->chaves[i].chave, sizeof(long long), 1, arq_arvore_bin);
        fwrite(&no_inserido->chaves[i].RRN_chave, sizeof(long long), 1, arq_arvore_bin);
        i++;
    }
    // Escreve o último ponteiro filho, preenchendo com -1 se for inválido
    fwrite(&no_inserido->Ponteiro_filho[i], sizeof(int), 1, arq_arvore_bin);
    // Preenche os campos restantes com -1 até o limite de 4 chaves
    int campos_a_preencher = Ordem_m -1 - no_inserido->nroChavesIndexadas;
    while (campos_a_preencher > 0) {
        int valor_invalido = -1;
        long long chave_invalida = -1;

        fwrite(&chave_invalida, sizeof(long long), 1, arq_arvore_bin); // Chave
        fwrite(&chave_invalida, sizeof(long long), 1, arq_arvore_bin); // RRN chave
        fwrite(&valor_invalido, sizeof(int), 1, arq_arvore_bin); // Ponteiro filho

        campos_a_preencher--;
    }
    return ;
}

// faz leitura do no da arvore, o retornando
Registro_Dados leitura_no_arvore(FILE* arq_arvore_bin) {
    Registro_Dados no_atual;

    // Le os dados basicos do no
    fread(&no_atual.folha , sizeof(char), 1, arq_arvore_bin);
    fread(&no_atual.nroChavesIndexadas, sizeof(int), 1, arq_arvore_bin);
    fread(&no_atual.RRNdoNo, sizeof(int), 1, arq_arvore_bin);

    // Le as chaves, ponteiro filho e rrn da chave
    int i = 0;
    while (i < no_atual.nroChavesIndexadas) {
        fread(&no_atual.Ponteiro_filho[i], sizeof(int), 1, arq_arvore_bin);   // Le o ponteiro do filho
        fread(&no_atual.chaves[i].chave, sizeof(long long), 1, arq_arvore_bin);  // Le a chave
        fread(&no_atual.chaves[i].RRN_chave, sizeof(long long), 1, arq_arvore_bin);  // Le o RRN da chave
        i++;
    }

    // Le ultimo ponteuro filho
    fread(&no_atual.Ponteiro_filho[i], sizeof(int), 1, arq_arvore_bin);

    return no_atual;
}

// Funcao para calcular o numero de andares (altura) da arvore B
int calcula_altura_arvore(int num_nos, int ordem_m) {
    if (num_nos == 0) {
        return 0; // se n tem insercoa, retorna zero
    }

    // Calcula o valor da metade arredondada para cima (minimo de filhos por no)
    int filhos_min = ceil(ordem_m / 2.0);

    // Calcula a altura da arvore usando a formula log base (m/2)
    double altura = log2((num_nos + 1) / 2.0) / log2(filhos_min);

    // Arredonda para o inteiro mais proximo
    return ceil(altura);
}

// funcao responsavel por ordenar chaves no noh, retornando a chave do meio caso tenham 5 chaves para ser ordenadas
// no_filho correspondente ao meio da ordenacao eh retornado por meio de referencia
Chave ordena_no(Registro_Dados* no_atual, long long chave, long long Posicao_byte, int* no_filho) {
    Chave vetor_auxiliar[Ordem_m]; // Vetor auxiliar para reescrever na ordem correta as chaves
    int nos_filhos[Ordem_m + 1];   // Vetor auxiliar para reescrever na ordem correta os nos filhos
    int aux = 0; // auxiliar utilizada na verificacao dos loops
    int inserida = 0; // Verifica se a chave já foi inserida

    // Se o nó está vazio (nroChavesIndexadas == 0)
    if (no_atual->nroChavesIndexadas == 0) {
        vetor_auxiliar[0].chave = chave;
        vetor_auxiliar[0].RRN_chave = Posicao_byte;
        nos_filhos[0] = -1; // O primeiro filho se torna o filho do nó
        nos_filhos[1] = -1;
        no_atual->nroChavesIndexadas = 1; // Atualiza o número de chaves
    } else {
        nos_filhos[0] = no_atual->Ponteiro_filho[0]; // Primeiro ponteiro filho
        
        while (aux < no_atual->nroChavesIndexadas) {
            // Se a chave a ser inserida for menor que a chave atual no nó e ainda não foi inserida
            if (chave < no_atual->chaves[aux].chave && !inserida) {
                nos_filhos[aux + 1] = *no_filho;
                vetor_auxiliar[aux].chave = chave;
                vetor_auxiliar[aux].RRN_chave = Posicao_byte;
                inserida = 1; // Marca que a chave foi inserida
            }

            // Copia as chaves existentes do noh atual
            nos_filhos[aux + inserida + 1] = no_atual->Ponteiro_filho[aux + 1];
            vetor_auxiliar[aux + inserida].chave = no_atual->chaves[aux].chave;
            vetor_auxiliar[aux + inserida].RRN_chave = no_atual->chaves[aux].RRN_chave;
            aux++;
        }

        // Caso a chave seja maior que todas as chaves no noh, insere ao final
        if (!inserida) {
            nos_filhos[aux + 1] = *no_filho;
            vetor_auxiliar[aux].chave = chave;
            vetor_auxiliar[aux].RRN_chave = Posicao_byte;
            inserida++;
        }

        no_atual->nroChavesIndexadas = aux + inserida; // Atualiza o número de chaves
    }

    // Transcreve vetor para struct no a ser inserida
    int meio = (Ordem_m - 1) / 2; // Calcula a posição do elemento do meio

    // Transcreve as chaves para o noh atual, ignorando o elemento do meio, o qual será retornado, caso tenha 5 chaves
    if (no_atual->nroChavesIndexadas == 5) {
        int indice_no_atual = 0;

        for (aux = 0; aux < no_atual->nroChavesIndexadas; aux++) {
            if (aux == meio) {
                continue; // Ignora o elemento do meio
            }
            // Insere a chave e o ponteiro no nó atual, ajustando o índice corretamente
            no_atual->chaves[indice_no_atual].chave = vetor_auxiliar[aux].chave;
            no_atual->chaves[indice_no_atual].RRN_chave = vetor_auxiliar[aux].RRN_chave;
            no_atual->Ponteiro_filho[indice_no_atual] = nos_filhos[aux];
            indice_no_atual++; // Incrementa apenas quando insere uma chave
        }

        // Atualiza o último ponteiro filho
        no_atual->Ponteiro_filho[indice_no_atual] = nos_filhos[aux];
    } else { // Caso de menos de 5 chaves, ordena normalmente
        for (aux = 0; aux < no_atual->nroChavesIndexadas; aux++) {
            no_atual->chaves[aux].chave = vetor_auxiliar[aux].chave;
            no_atual->chaves[aux].RRN_chave = vetor_auxiliar[aux].RRN_chave;
            no_atual->Ponteiro_filho[aux] = nos_filhos[aux];
        }
        no_atual->Ponteiro_filho[aux] = nos_filhos[aux]; // Atualiza o último ponteiro filho
        return vetor_auxiliar[0]; // Retorno de lixo que não será usado
    }

    // Correção no uso da função printa_vetor_long
    long long chaves_vetor_aux[Ordem_m];
    long long chaves_no_atual[Ordem_m];
    
    // Prepara os vetores para impressão
    for (int i = 0; i < Ordem_m; i++) {
        chaves_vetor_aux[i] = vetor_auxiliar[i].chave;
        if (i < no_atual->nroChavesIndexadas) {
            chaves_no_atual[i] = no_atual->chaves[i].chave;
        } else {
            chaves_no_atual[i] = -1; // Preenche com -1 quando não houver mais chaves validas
        }
    }

    *no_filho= nos_filhos[meio]; // retorno do noh do meio por referencia
    
    return vetor_auxiliar[meio];
}

// funcao eh chamada para se inserir em no folha
void insere_em_no_folha( FILE* arq_arvore_bin, Registro_Dados* no_atual, long long chave, long long Posicao_byte){

            int aux= -1; // no filho correspondente do noh eh -1
            ordena_no(no_atual, chave, Posicao_byte, &aux); // ordena no corretamente

            // reescreve no de forma correta // nao precisa mudar a raiz da arvore
            fseek(arq_arvore_bin, (tam_PagDisco)*(no_atual->RRNdoNo + 1), SEEK_SET); // posiciona ponteiro no inicio do no para sobrescrita
            escreve_registro_arvore(arq_arvore_bin, no_atual);
            return ;
}


// Função que inverte um vetor de inteiros
void inverter_vetor(int* vetor, int tamanho_valido) {
    int temp;
    for (int i = 0; i < tamanho_valido / 2; i++) {
        // Troca os elementos do início com os do fim
        temp = vetor[i];
        vetor[i] = vetor[tamanho_valido - 1 - i];
        vetor[tamanho_valido - 1 - i] = temp;
    }
}

// verifica se eh no folha para atribuicao correta no arquivo
char confere_no_folha(Registro_Dados nodo){
    int aux = nodo.nroChavesIndexadas;
    while(aux!= -1){
        if(nodo.Ponteiro_filho[aux]!= -1){ // caso possua no filho, retorna 0, nao eh folha
            return '0';
        }
        aux--;
    }

    return '1';
}

//funcao responsavel por realizar o split e promocao do no recursivamente, onde o no filho eh salvo por referencia
// tem se uma lista de inteiros que corresponde aos rrn acessados caso seja necessario subir a arvore para realizar a insercao
// o inteiro andares escalados diz respeito a quantos andares desceram-se para realizar a insercao
void split_e_promocao(FILE* arq_arvore_bin, Registro_Dados *no_atual, long long chave, long long Posicao_byte, Cabecalho_Arvore* titulo_arvore, int* RRN_acessados, int andares_escalados, int no_filho) {
    
    //calcula no medial
    int no_medial = (no_atual->nroChavesIndexadas+1)/2;

    // ordena no
    Chave chave_promovida = ordena_no(no_atual, chave, Posicao_byte, &no_filho); 

    // cria um novo nó para a metade superior das chaves e escreve ele nessa pagina
    Registro_Dados nodo2; 
    nodo2.nroChavesIndexadas =0;
    nodo2.RRNdoNo = titulo_arvore->RRNproxNo;
    
    //copia chaves e filho nó da metade superior para o nodo2
    for(int i = no_medial, j=0;i< (no_atual->nroChavesIndexadas-1); i++, j++ )
    {
        nodo2.chaves[j]=no_atual->chaves[i];
        nodo2.Ponteiro_filho[j]=no_atual->Ponteiro_filho[i];
        nodo2.nroChavesIndexadas++;
    }
    nodo2.Ponteiro_filho[nodo2.nroChavesIndexadas]= no_atual->Ponteiro_filho[no_atual->nroChavesIndexadas-1];

    //ajuste no atual para metade inferior
    no_atual->nroChavesIndexadas=no_medial;
    no_atual->Ponteiro_filho[no_medial] = no_filho;

    // sobrescreve no atual no arq binario com o lixo dos ultimos registros
    
    fseek(arq_arvore_bin, (tam_PagDisco)*(no_atual->RRNdoNo +1), SEEK_SET); 
    escreve_registro_arvore(arq_arvore_bin, no_atual);

    // criar nodo2 e colocar infos
    // ja preenche com lixo
    nodo2.folha = confere_no_folha( nodo2);
    Cria_Pagina_arvore(arq_arvore_bin, titulo_arvore, nodo2.folha, nodo2.nroChavesIndexadas); // altera titulo_arvoore_proxno++ / ja passa valor correto de folha e chaves_index
    escreve_registro_arvore(arq_arvore_bin, &nodo2);

    // Inserir a chave mediana no nó pai (ou criar um novo nó raiz se o nó pai for a raiz)
    if(titulo_arvore->noRaiz == no_atual->RRNdoNo){
        //nó atual era a raiz, criar nova raiz
        Registro_Dados nova_raiz;
        nova_raiz.folha = '0'; // eh certeiro que no promovido nao sera folha
        nova_raiz.nroChavesIndexadas = 1;
        nova_raiz.chaves[0] = chave_promovida;
        nova_raiz.Ponteiro_filho[0] = no_atual->RRNdoNo;
        nova_raiz.Ponteiro_filho[1] = nodo2.RRNdoNo;
        nova_raiz.RRNdoNo = titulo_arvore->RRNproxNo;
        //escrever raiz arqarv
        Cria_Pagina_arvore(arq_arvore_bin, titulo_arvore, nova_raiz.folha, nova_raiz.nroChavesIndexadas);
        escreve_registro_arvore(arq_arvore_bin, &nova_raiz);

        //atualiza cabec
        titulo_arvore->noRaiz = nova_raiz.RRNdoNo;
        return ;
    }
    else{ //inserir no nó pai
        
        Registro_Dados no_pai;
        fseek(arq_arvore_bin, tam_PagDisco*(RRN_acessados[andares_escalados] + 1), SEEK_SET); // +1 pula cabecalho do arquivo
        andares_escalados++;
        no_pai = leitura_no_arvore(arq_arvore_bin);
        // no pai tem espaco
        if(no_pai.nroChavesIndexadas< (Ordem_m-1)){
            ordena_no(&no_pai, chave_promovida.chave, chave_promovida.RRN_chave, &nodo2.RRNdoNo);
            fseek(arq_arvore_bin, (tam_PagDisco)*(no_pai.RRNdoNo + 1), SEEK_SET); // posiciona ponteiro no inicio do no para sobrescrever
            escreve_registro_arvore(arq_arvore_bin, &no_pai);
            return ;
        }
        else{ // no pai precisa de split
            split_e_promocao(arq_arvore_bin, &no_pai, chave_promovida.chave, chave_promovida.RRN_chave, titulo_arvore, RRN_acessados, andares_escalados, nodo2.RRNdoNo); 
        }
    }
    return ;
}

// busca chave livre para fazer insercao, caso necessario realiza split
void busca_chave_livre(FILE* arq_arvore_bin, long long chave, long long Posicao_byte, Cabecalho_Arvore* titulo_arvore) {
    Registro_Dados no_atual; // ira ler o no atual
    // calcula tamanho da altura da arvore com sobra para armzenar RRN_ acessados
    int aux_altura_arvore = calcula_altura_arvore( titulo_arvore->RRNproxNo , Ordem_m);
    int *RRN_acessados = (int*)malloc( sizeof(int*) * (aux_altura_arvore+3));
    
    int altura_atual=0; // somador que indica em qual altura esta sendo lido o noh
    fseek(arq_arvore_bin, tam_PagDisco*(titulo_arvore->noRaiz+1) ,SEEK_SET); // descolca vetor posicao do arquivo para a raiz da arvore
    // Loop para descer na arvore ate encontrar um no folha
    while (1){
        no_atual = leitura_no_arvore(arq_arvore_bin); // le o no atual
        RRN_acessados[altura_atual] = no_atual.RRNdoNo; // ver se isso ta no local correto ( as vezes +1 altura total)
        altura_atual++;
        // Se o no atual eh uma folha
        if (no_atual.folha == '1'){
            // Se o no esta cheio, precisa fazer o split
            if (no_atual.nroChavesIndexadas == Ordem_m - 1) {
                inverter_vetor(RRN_acessados, altura_atual); // inverte ate o valor valido
                int andares_escalados =1;
                split_e_promocao(arq_arvore_bin, &no_atual, chave, Posicao_byte, titulo_arvore, RRN_acessados, andares_escalados, -1);
            } else {
                // Se o no nao esta cheio, insere a chave na folha
                insere_em_no_folha(arq_arvore_bin, &no_atual, chave, Posicao_byte);
            }
            break; // Apos a insercao ou split, termina a busca
        }
        else{
            // O no nao eh folha, entao precisamos descer mais um nivel
            if (chave < no_atual.chaves[0].chave) {
                fseek(arq_arvore_bin, tam_PagDisco * (no_atual.Ponteiro_filho[0] + 1), SEEK_SET); // soma 1 pois esta pulando a primeira pagina de cabecalho
            }
            else if (chave > no_atual.chaves[no_atual.nroChavesIndexadas - 1].chave) {
                fseek(arq_arvore_bin, tam_PagDisco * (no_atual.Ponteiro_filho[no_atual.nroChavesIndexadas] + 1), SEEK_SET);
            }
            else{
                // Encontra o ponteiro correto para descer
                for (int i = 0; i < no_atual.nroChavesIndexadas - 1; i++){
                    if (chave < no_atual.chaves[i + 1].chave) {
                        fseek(arq_arvore_bin, tam_PagDisco * (no_atual.Ponteiro_filho[i + 1] + 1), SEEK_SET);
                        break;
                    }
                }
            }
        }
    }
    free(RRN_acessados); // libera memoria alocada apos ser utilizado
    return ;
}

// funcao mestre para realizar insecao, cria a primeira pagina caso necessario
void insere_arvore_binaria(FILE* arq_arvore_bin, char* nome, long long Posicao_byte,int flag, Cabecalho_Arvore* titulo_arvore){

    long long chave = converteNome(nome);
    // verificar se eh primeira insercao, criar cabecalho e criar primeira pagina

    if(flag==1){
        Cria_cabecalho(arq_arvore_bin);
        Cria_Pagina_arvore(arq_arvore_bin, titulo_arvore, '1', 0); /// verificar se eu nao estou passando referencia errado
        titulo_arvore->noRaiz = 0; // primeiro no criado sera a raiz primeiramente
        titulo_arvore->status = 0; // enquanto arvore esta aberta para escrita, muda-se o status
    }
    // caso contrario segue comparando ate chegar no no filho ( com insercao livre )
    busca_chave_livre(arq_arvore_bin, chave, Posicao_byte, titulo_arvore); // busca e insere
    
    /// atualiza cabecalho ( so vou escrever cabecalho terminando leitura ), pois ela esta sendo passada como referencia

    return ;
}