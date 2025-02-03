// NOME: Pedro Gasparelo Leme 14602421
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struct para representar as caracteristicas de um item
typedef struct {
    int numero; // numero que representa o item com as caracteristicas abaixo
    int peso;
    int valor;
    float preferencia; // variavel criada para a utilizacao do algoritmo guloso, o qual pega os itens de maior prioridade valor/peso
} Items;

//Struct para armazenar os resultados das funções
typedef struct {
    int valor; // valor total
    int quantidade; // quantidade de itens selecionados
    int* items_selecionados;
} Resultados;

// fucao que ordena e retorna um array em ordem decrescente com relacao a sua prioridade ( valor/peso )
Items* ordena_array_guloso( int Q, Items* items_ordenados ){
    int i, j;
    Items aux; // variavel auxiliar para armazenamento durante troca de algoritmo de ordenacao
    for (i = 0; i < Q; ++i){
        for (j = i + 1; j < Q; ++j){
            if (items_ordenados[i].preferencia < items_ordenados[j].preferencia){
                aux = items_ordenados[i];
                items_ordenados[i] = items_ordenados[j];
                items_ordenados[j] = aux;
         }
      }
   }
   return items_ordenados;
}

// funcao correspondente ao algoritmo guloso
Resultados funcao_alg_guloso(int peso_mochila, int Q, Items* items) {
    // mochila inicialmente vazia
    int valor_total = 0;
    int peso = 0;
    int quantidade = 0;

    // ordenacao do array dos itens em ordem decrescente de prioridade ( valor/peso)
    items = ordena_array_guloso(Q, items);

    // efetua algoritmo guloso, pegando os itens mais promissores inicialmente ( maior valor de prioridade )
    for (quantidade = 0; quantidade < Q; quantidade++) {
        if (peso + items[quantidade].peso > peso_mochila) { // caso item mais promissor a ser selecionado é mais pesado que a mochila, sai do loop
            break;
        }
        else{    // Caso contrario, item é pego
            peso = peso + items[quantidade].peso;
            valor_total = valor_total + items[quantidade].valor;
        }
    }

    //secao para armazenamento de informacoes para retorno na main
    Resultados guloso;
    guloso.valor = valor_total;
    guloso.quantidade = quantidade;
    guloso.items_selecionados = (int*)calloc(quantidade, sizeof(int));

    // atribuicao dos itens selecionados no parametro que sera retornado
    for (int i = 0; i < guloso.quantidade; i++) {
        guloso.items_selecionados[i] = items[i].numero;
    }

    return guloso;
}

// ordena o array com os itens ja selecionados para print na main em ordem crescente
Resultados ordena_array_dinamico(Resultados prog_dinamica, int contador){
    int i, j, aux;
    for (i = 0; i < contador; ++i){
        for (j = i + 1; j < contador; ++j){
            if (prog_dinamica.items_selecionados[i] > prog_dinamica.items_selecionados[j]){
                aux = prog_dinamica.items_selecionados[i];
                prog_dinamica.items_selecionados[i] = prog_dinamica.items_selecionados[j];
                prog_dinamica.items_selecionados[j] = aux;
         }
      }
   }
   return prog_dinamica;
}

// funcao que realiza programacao dinamica de forma iterativa
Resultados funcao_programacao_dinamica(int P, int Q, Items* items){
    int tabela[Q+1][P+1];  //criação da tabela para programação dinâmica

    //inicialização da tabela com zeros
    for (int i = 0; i <= Q; i++) {
        for (int j = 0; j <= P; j++) {
            tabela[i][j] = 0;
        }
    }

    //execucao da iteracao para programacao dinamica
    for (int i = 1; i <= Q; i++) { //varre os itens que podem ser selecionados
        for (int j = 1; j <= P; j++) { // capacidade da mochila que vai de 0 a P
            if (items[i - 1].peso <= j) { // caso peso do item nao seja maior que a da mochila
                // verificacao de item de maior valor,
                // considera o valor do item a ser inserido somados com o valor calculado para mochila de tamanho j-peso
                if( items[i - 1].valor + tabela[i - 1][j - items[i - 1].peso] > tabela[i - 1][j] ){
                    tabela[i][j] = items[i - 1].valor + tabela[i - 1][j - items[i - 1].peso];
                }
                else{ // caso contrario nao insere item
                    tabela[i][j]= tabela[i-1][j];
                }
            } else { // caso peso do item seja maior, item nao é adicionado
                tabela[i][j] = tabela[i - 1][j];
            }
        }
    }

    int itens_selecionados[Q];

    // inicio do backtracking
    int j = P;
    int contador = 0;
    // percorre a tabela para selecionar a melhor solucao comparando os elementos presentes na mesma linha
    for (int i = Q; i > 0 && j > 0; i--) {
        if (tabela[i][j] != tabela[i - 1][j]) {
            itens_selecionados[contador++] = items[i - 1].numero;
            j -= items[i - 1].peso;
        }
    }

    // valor recebera o maior valor possivel dada a capacidade P
    int valor = tabela[Q][P];

    // armazenamento dos valores obtidos para retorno em main
    Resultados prog_dinamica;
    prog_dinamica.valor = valor;
    prog_dinamica.quantidade = contador;
    prog_dinamica.items_selecionados = (int*)calloc(contador, sizeof(int));

    // insercao dos itens pegos na mochila ate a quantidade de itens pegos
    for (int i = 0; i < prog_dinamica.quantidade; i++) {
        prog_dinamica.items_selecionados[i] = itens_selecionados[i];
    }

    // ordena os valores em ordem crescente para print na main
    prog_dinamica = ordena_array_dinamico(prog_dinamica, contador);

    return prog_dinamica;
}

int main()
{
    // insercao dos valores da capacidade da mochila e numeros de itens informados
    int P, Q;
    scanf("%d %d", &P, &Q);

    Items items[Q];

    // leitura dos pesos e vlores dos itens
    for (int i = 0; i < Q; i++) {
        items[i].numero = i;
        scanf("%d %d", &items[i].peso, &items[i].valor);
        items[i].preferencia= (float)(items[i].valor) / (items[i].peso);
    }

    // resolucao do problema pelo algoritmo guloso e usando programacao dinamica de forma iterativa
    Resultados res_guloso = funcao_alg_guloso(P, Q, items);

    Resultados res_dinamica = funcao_programacao_dinamica(P, Q, items);

    // print dos itens pegos
    for(int i = 0; i < res_dinamica.quantidade; i++) {
        printf("%d ", res_dinamica.items_selecionados[i]);
    }

    printf("\n");
    for(int i = 0; i < res_guloso.quantidade; i++) {
        printf("%d ", res_guloso.items_selecionados[i]);
    }

    printf("\n");

    // print do valor total pego
    printf("%d %d\n", res_dinamica.valor, res_guloso.valor);

    // print da divisao dos dois valores
    printf("%.2f\n", ((float)res_guloso.valor/res_dinamica.valor)*100);

    // libera espaço alocado para operacoes
    free(res_guloso.items_selecionados);
    free(res_dinamica.items_selecionados);

    return 0;
}
