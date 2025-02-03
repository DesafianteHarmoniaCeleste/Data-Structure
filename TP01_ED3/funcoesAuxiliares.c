#include "header.h"

// faz as comparacoes se o campo em que se procura o dinossauro coincide com o nome e valor do campo dado,
// retornando 1 caso verdadeiro, se eles se equivalem e -1 caso seja falso
int verifica_dino(Dados dinossauro, char* Nomecampo, char* Valorcampo){
    int verdadeiro;
    if(!strcmp(Nomecampo, "nome")){
        if(!strcmp(dinossauro.nome, Valorcampo)){
            verdadeiro=1;
        }
        else{
            verdadeiro= -1;
        }
    }
    else if(!strcmp(Nomecampo, "habitat")){
        if(!strcmp(dinossauro.habitat, Valorcampo)){
            verdadeiro=1;
        }
        else{
            verdadeiro= -1;
        }
    }
    else if(!strcmp(Nomecampo, "velocidade")){
        float velocidade_float = atof(Valorcampo);
        if(dinossauro.velocidade == velocidade_float){
            verdadeiro=1;
        }
        else{
            verdadeiro= -1;
        }
    }
    else if(!strcmp(Nomecampo, "populacao")){
        int populacao_int = atoi(Valorcampo);
        if(dinossauro.populacao == populacao_int){
            verdadeiro=1;
        }
        else{
            verdadeiro= -1;
        }
    }
    else if(!strcmp(Nomecampo, "especie")){
        if(!strcmp(dinossauro.especie, Valorcampo)){
            verdadeiro=1;
        }
        else{
            verdadeiro= -1;
        }
    }
    else if(!strcmp(Nomecampo, "tipo")){
        if(!strcmp(dinossauro.tipo, Valorcampo)){
            verdadeiro=1;
        }
        else{
            verdadeiro= -1;
        }
    }
    else if(!strcmp(Nomecampo, "dieta")){
        if(!strcmp(dinossauro.dieta, Valorcampo)){
            verdadeiro=1;
        }
        else{
            verdadeiro= -1;
        }
    }
    else if(!strcmp(Nomecampo, "tamanho")){
        float tamanho_float = atof(Valorcampo);
        if(dinossauro.tamanho == tamanho_float){
            verdadeiro=1;
        }
        else{
            verdadeiro= -1;
        }
    }
    else if(!strcmp(Nomecampo, "alimento")){
        if(!strcmp(dinossauro.alimento, Valorcampo)){
            verdadeiro=1;
        }
        else{
            verdadeiro= -1;
        }
    }
    return verdadeiro;
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

// funcao dada para verificar os bytes inseridos nos arquivos, se coincidem com o valor esperado
void binarioNaTela(char* nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */
    unsigned long i, cs;
    unsigned char* mb;
    size_t fl;
    FILE* fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
        fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char*)malloc(fl);
    fread(mb, 1, fl, fs);

    cs = 0;
    for (i = 0; i < fl; i++) {
        cs += (unsigned long)mb[i];
    }
    printf("%lf\n", (cs / (double)100));
    free(mb);
    fclose(fs);
}
