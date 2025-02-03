// Pedro Gasparelo Leme - 14602421
// William Noboru Yoshihara - 14612461

#include "header.h"

// porcentagem de participacao
// Pedro Gasparelo Leme - 100
// William Noboru Yoshihara - 100

int main()
{
    // insercao do comando de qual operacao se deseja efetuar
    int comando;
    scanf("%d", &comando);

    switch (comando){
    case 1:
        le_csv_escreve_bin();
        break;

    case 2:
        recupera_dados_bin();
        break;

    case 3:
        recupera_dados_where_bin();
        break;

    case 4:
        remove_dados();
        break;

    case 5:
        insercao_novos_registros();
        break;

    case 6:
        compactacao_arquivo();
        break;

    default:
        printf("Comando inválido.\n");
        break;
    }

    return 0;
}
