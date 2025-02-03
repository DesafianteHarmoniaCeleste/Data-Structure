/*                                      taxa de participação
    Pedro Gasparelo Leme     - 14602421 -     100%
    William Noboru Yoshihara - 14612461 -     100%
*/
#include <stdio.h>
#include <stdlib.h>

#include "leitura_binario.h"

/*
    Arquivo que contem a main.
    tem-se a leitura do comando a ser realizado e a leitura do arquivo binario,
    o qual eh comum para todas as escolhas feitas pelo operador

    as escolhas vao de 10 a 14 e elas sao, respectivamente:
    printa_grafo           - responsavel por printar os vertices e as listas de adjacencias
    executar_Listagem      - lista todos os predadores de uma presa dada pelo operador
    ciclos_grafo           - diz quantos ciclos tem para um grafo dado
    kosaraju               - encontra a quantidade de ciclos fortemente conexos
    executarAnaliseCaminho - encontra o menor caminho entre dois pontos 

    possui como definicoes globais para a melhor compreensao do codigo

    #define bytes_dados 160
    #define MAX_LINHA 1024
    #define bytes_dados 160
    #define PAG_DISCO 1600

    #define FALHA_ARQUIVO "Falha no processamento do Arquivo.\n"
    #define DEBBUG 0

    onde o debbbug pode ser acionado ao trocar-se 0 pelo valor 1
*/

int main(){

    // variavel inteira para ler acao a ser realizada
    int escolha_operacao;
    scanf("%d", &escolha_operacao);

     // declaracao do grafo que sera recebido como retorno pela funcao recupera_dados_bin
    Grafo* grafo_universal;
    grafo_universal = recupera_dados_bin();

    // execucao da operacao dada
    switch (escolha_operacao)
    {
    case 10:
        // printa o grado dado
        printa_grafo(grafo_universal);
        break;
    case 11:
        // lista os predadores para uma presa dada pelo operador  
        executarListagem(grafo_universal);
        break;
    case 12:
        // realiza contagem dos ciclos em um grafo
        ciclos_grafo(grafo_universal);
        break;
    case 13:
        // encontra a quantidade de ciclos fortemente conexos
        kosaraju(grafo_universal);
        break;
    case 14:
        // realiza operacao de djikstra de menor caminho
        int numero_buscas;
        scanf("%d", &numero_buscas);
        executarAnaliseCaminho(grafo_universal, numero_buscas);  
    default:
        break;
    }
    return 0;
}