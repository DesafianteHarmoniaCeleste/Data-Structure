////////////////////////////////////
//  ALUNO: Pedro Gasparelo Leme - 14602421
//  ALUNO: William Noboru Yoshihara - 14612461
//  ALUNO: Luana Fialho de Neto Filho - 14755061
/////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Elemento{    // Definicao do no da lista
    int n;  // inteiro armazenadon
    struct Elemento *prox; // armazena a regiao de memoria onde se encontra o proximo no da lista
}Elem;

typedef Elem* Lista; // definicao da lista como macro dos elementos, aponta para o primeiro elemento


Lista* cria_lista(){
    Lista* li = (Lista*) malloc(sizeof(Lista)); // alocaçao de espaço na memoria
    if (li != NULL) // veriificacao se a lista foi criada com lixo de memoria
        *li = NULL;
    return li;
}

void libera_lista(Lista* li)
{
    if (li != NULL) // verificacao se a lista inteira ja nao teve o espaco liberado
	{
        Elem* no;   // elemento no tempoario que ira varrer a lista e liberar no por no
        while ((*li) != NULL)
		{
            no = *li;
            *li = (*li)->prox;
            free(no);
        }
        free(li);
    }
}

int insere_lista(Lista* li, int dt) //insercao no final da lista
{
    Elem *no;

    if (li == NULL) return -1;  // afericao se espaco de memoria foi alocado com sucesso pela lista
    no = (Elem*) malloc(sizeof(Elem)); // alocacao de memoria para o no a ser inserido
    if (no == NULL)  return -1; //afericao se espaco de memoria foi alocada com sucesso pelo elemento

    no->n = dt; // atribuicao do numero ao no
    no->prox = NULL;    // proxima regiao de memoria é nula por conta da iinserçao no final da lista

	if ((*li) == NULL) // inserçao do primeiro elemento
	{
        *li = no;
    }else{
        Elem *aux; // elemento auxiliar para percorrer a lista ate chegar ao fim dela para insercao
        aux = *li;
        while (aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = no;
    }
    return 1;
}

void imprime_lista(Lista* li)
{
    Elem* no = *li; // no ira percorrer a lista

    if (li == NULL) // verificacao se memoria alocada da lista existe
        return;
    while (no != NULL)  //percorrer a lista imprimindo-a ate o ultimo elemento
    {
        printf("%d ",no->n);
        no = no->prox;
    }
}

void insertion_sort(Elem* NovoNo, Lista* ordenado)
{
    /* caso especial para o fim da lista*/
    if ( *ordenado == NULL || (*ordenado)->n >= NovoNo->n) {
     NovoNo->prox = (*ordenado);
        *ordenado = NovoNo;
    }
    else {
        Elem* atual = *ordenado;
        /* Localiza o no antes do ponto de insercao */
        while (atual->prox != NULL && atual->prox->n < NovoNo->n) {
            atual = atual->prox;
        }
     NovoNo->prox = atual->prox;
        atual->prox = NovoNo;
    }
}

//insertion sort para uma lista, utiliza funcao anterior de insertion/
Lista* insertion_sort_lista(Lista* li)
{
    Lista* ordenado = cria_lista();
    Elem* atual = *li;

    //insere cada no para a lista ordenada
    while (atual != NULL) {

        // recebe o proximo elemento da lista
        Elem* prox = atual->prox;

        // insert atual na lista ordenada
        insertion_sort(atual, ordenado);

        // atualiza no atual
        atual = prox;
    }
    // atualiza o ponteiro lista para a nova lista ordenada
    li = ordenado;
    return li;
}

int main()
{
    Lista *li = cria_lista();

    int num, i;
    int temp;
    scanf("%d", &num);

    clock_t Inicio, Fim; // declaracao dos tempos iniciais e finais
    float tempo_total;

    for(i=0; i<num; i++){
        //insere inputs na lista
        scanf("%d", &temp);
        insere_lista(li, temp);
    }

    Inicio = clock(); // tempo inicial

    li = insertion_sort_lista(li);

    Fim = clock();  // tempo final pos insertion_sort

    imprime_lista(li);

    libera_lista(li);

    tempo_total = ((float) (Fim - Inicio)) / CLOCKS_PER_SEC; // calculo do tempo total do insertion_sort
    // printf(" \n %f", tempo_total); // funcao comentada decorrente dos casos de teste do run codes

    return 0;
}
