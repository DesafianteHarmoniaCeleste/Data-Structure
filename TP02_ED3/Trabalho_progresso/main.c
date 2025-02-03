///////////////////////////////////
// Trabalho pratico ED 3 2024
// Nome :Pedro Gasparelo Leme    - 14602421 - taxa de participacao 100%
// Nome William Noboru Yoshihara - 14612461 - taxa de participacao 100%
///////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "Header.h"

// funcao principal para escolha do comando a ser executado
int main()
{
    int escolha;
    scanf("%d", &escolha);

    switch (escolha){

    case 7:
        cria_arvoreB();
        break;
    case 8:
        recupera_dados_arvore();
        break;
    case 9:
        insercao_novos_registros();
        break;
    default:
        printf("COMANDO INVALIDO\n");
        break;
    }

    return 0;
}