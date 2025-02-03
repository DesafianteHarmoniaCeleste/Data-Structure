#include "header.h"


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

// Inicializa a struct de cabeçalho conforme as especificacoes
void cabecalho_inicia(Cabecalho* cab_aux) {
    cab_aux->status = '0';
    cab_aux->topo = -1;
    cab_aux->proxRRN = 0;
    cab_aux->nroRegRem = 0;
    cab_aux->nroPagDisco = 0;
    cab_aux->qttCompacta = 0;
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

