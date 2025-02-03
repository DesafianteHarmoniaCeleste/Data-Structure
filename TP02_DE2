#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// definicao de quais variaveis o aluno vai ter para realizar a aritmetica de bolsas
typedef struct elemento{
    char nome[15];
    float media;
    int rep;
    int verdadeiro; //declaracao de variavel boleana, caso receba a bolsa, recebe verdadeiro ( 1 )
}Elem;

typedef Elem Alunos; // definicao de Alunos, sera o array que armazenara os Elementos ( alunos )

// funcao responsavel por trocar a posicao de 2 alunos no array
void troca(Elem *aluno1, Elem *aluno2){
    Elem temp = *aluno1;
    *aluno1 = *aluno2;
    *aluno2 = temp;
}

//ordena em selection sort os alunos pelas reprovacoes, onde o de menor reprovacao tem mais prioridade
Alunos* ordena_rep_selection(Alunos aluno[], int inicio, int fim){
    int i, j, antes;
    // recebe o inicio de onde deve comecar a ordenacao
    i=inicio;
    // executa selection sort ate o ultimo aluno de mesma media
    while(i<fim){
        antes = i;
        j= i+1;
        // varre o array de alunos desde a posicao nao ordenada inicial ate o final procurando o aluno de menor reprovacoes
        while(j<fim){
            if (aluno[j].rep < aluno[antes].rep){
                antes = j;
            }
            j++;
        }
        // caso tenha um aluno de ordem de reprovacao mais prioritaria, ocorre a troca
        if (antes != i){
           troca(&aluno[antes], &aluno[i]);
        }
        i++;
    }
    return aluno;
}

//ordena em selection sort os alunos pela ordem alfabetica, onde o de menor reprovacao tem mais prioridade
Alunos* ordena_nome_selection(Alunos aluno[],int inicio, int fim){
    int i, j, antes;
    // recebe o inicio de onde deve comecar a ordenacao
    i=inicio;
    // executa selection sort ate o ultimo aluno de mesma media e reprovacao
    while(i<fim){
        antes = i;
        j= i + 1;
        // varre o array de alunos desde a posicao nao ordenada inicial ate o final procurando o aluno de ordem alfabetica prioritaria
        while(j<fim){
            if (strcmp(aluno[j].nome, aluno[antes].nome)<0){
                antes = j;
            }
            j++;
        }
        // caso tenha um aluno de ordem alfabetica mais prioritaria, ocorre a troca
        if (antes != i){
           troca(&aluno[antes], &aluno[i]);
        }
        i++;
    }
    return aluno;
}

// selection sort visando ordenacao de reprovacoes e ordem alfabetica ja conhecendo a posicao que deve ser ordenada
Alunos* alunos_verdadeiros_selection(Alunos alunos[],int N, int bolsas){
    //contador que passa quantos alunos devem ser ordenados ao terem mesma media e em seguida mesma reprovacao e media
    int cont=0;
    //reprovacao
    for(int i=0; i<N;i++){
        while(alunos[i].media==alunos[i+1].media){
            cont++;
            i++;
        }
        if(cont!=0){
            alunos= ordena_rep_selection(alunos, i-cont, i+1);
        }
        cont=0;
    }
    //Alfabetica
    for(int i=0; i<N;i++){
        while(alunos[i].rep==alunos[i+1].rep && alunos[i].media==alunos[i+1].media){
            cont++;
            i++;
        }
        if(cont!=0){
            alunos= ordena_nome_selection(alunos, i-cont, i+1 );
        }
        cont=0;
    }
    return alunos;
}

// ordenacao da media por insertion sort
Alunos* selection_sort(Alunos alunos[],  int N, int bolsas){
    // variavel auxilar que sera responsavel por guardar a posicao de troca de aluno para inicio do array
    int aux=0;

    // variavel responsavel por verificacao de aluno selecionado ter a maior media do array
    float media=0;

    // variavel responsavel por varrer o array N vezes em busca do aluno de maior media
    int temp = 0;

    // variavel responsavel por guardar a posicao do aluno de maior media
    int posicao=0;

    // variavel responsavel por nao trocar dois alunos de mesma media
    char Nome[10];
    strcpy(Nome, alunos[0].nome);

    //inicio de selection sort
    while(aux!=N){
        while(temp != N){
            if(media<= alunos[temp].media){
                if(strcmp(Nome, alunos[temp].nome)< 0 && media== alunos[temp].media){

                }
                else{
                    media= alunos[temp].media;
                    strcpy(Nome, alunos[temp].nome);
                    posicao=temp;
                }
            }
            temp++;
        }
        //troca a posicao de inicio do array com o aluno de maior media
        troca(&alunos[posicao], &alunos[aux]);
        media=0;
        aux++;
        temp=aux;
    }
    // selection sort visando ordenacao de reprovacoes e ordem alfabetica
    alunos = alunos_verdadeiros_selection(alunos, N, bolsas);
    return alunos;
}

// funcao responsavel pela aglutinacao do array repartido
void merge_total(Alunos alunos[], int inicio, int divisao, int fim, int bolsas){
    int aux, n1, n2;
    // variavel que representa a insercao do aluno ja ordenado, caminha para o proximo pos insercao
    aux= inicio;

    // tamanho dos 2 arrays que foram repartidos
    n1= divisao-inicio+1;
    n2=  fim-divisao;

    // alocacao de memoria dos 2 arrays repartidos que serao ordenados
    Alunos* L = calloc(n1, sizeof(Alunos));
    Alunos* R = calloc(n2, sizeof(Alunos));

    // insercao dos alunos nos arrays que serao ordenados
    for (int i = 0; i < n1; i++){
        L[i] = alunos[inicio + i];
    }
    for (int j = 0; j < n2; j++){
        R[j] = alunos[divisao + 1 + j];
    }

    // inicio da ordenacao, que ocorrera em ordem prioridade: maior media, caso tenha mesma media: menos reprovacoes,
    //                                                        caso trenha mesmo numero de reprovacoes :alfabetica
    int i=0;
    int j=0;
    while (i < n1 && j < n2) {
        // verificacao das medias
        if (L[i].media > R[j].media) {
            alunos[aux] = L[i];
            i++;
        }
        else if(L[i].media < R[j].media){
            alunos[aux] = R[j];
            j++;
        }
        // caso media seja igual, ordenar reprovacoes
        else{
            if (L[i].rep < R[j].rep) {
                alunos[aux] = L[i];
                i++;
            }
            else if(L[i].rep > R[j].rep){
                alunos[aux] = R[j];
                j++;
            }
            // caso media e numero de reprovacoes seja igual, ordenar alfabetico
            else{
                if ((strcmp(L[i].nome,R[j].nome)<0)){
                    alunos[aux] = L[i];
                    i++;
                }
                else{
                    alunos[aux] = R[j];
                    j++;
                }
            }
        }
        aux++;
    }
    // insercao dos alunos que ainda nao foram inseridos pos termino do while, restando apenas um dos 2 arrays para serem inseridos
    while (i < n1) {
        alunos[aux] = L[i];
        i++;
        aux++;
    }
    while (j < n2) {
        alunos[aux] = R[j];
        j++;
        aux++;
    }
}

// funcao mestre do mergesort, responsavel por repartir o array Alunos e ordenar visando a aglutinacao das partes
void merge_Sort(Alunos alunos[], int inicio, int fim, int bolsas){
    int divisao;

    // verificacao caso chegue no array de tamanho 1
    if(inicio >= fim){

    }
    else{
        // escolha de divisao do array para merge
        divisao = (inicio+fim) / 2;
        // divisao do array
        merge_Sort(alunos, inicio, divisao, bolsas);
        merge_Sort(alunos, divisao + 1, fim, bolsas);

        // aglutinacao dos arrays divididos
        merge_total(alunos, inicio, divisao, fim, bolsas);
    }
}

// atribuicao e printa alunos que receberam a bolsa
void printa_alunos(Alunos alunos[], int N, int M){
    int i=0;
    int cont=0;

    // atribuicao dos alunos que receberao a bolsa e contador de quantas bolsas foram dadas
    while(i!=M && i<N){
            if(alunos[i].rep<=10){
                if(i== (M-1) && alunos[i+1].media==alunos[i].media && alunos[i+1].rep==alunos[i].rep){
                    alunos[i].verdadeiro=1;
                    M=M+1;
                    cont++;
                }
                else{
                    alunos[i].verdadeiro=1;
                    cont++;
                }
                i++;
           }
           else{
            i++;
            M=M+1;
           }

    }
    printf("%d", cont);

    // printa os alunos que receberam a bolsa
    i=0;
    while(i<N){
        if(alunos[i].verdadeiro==1){
            printf("\n%s",alunos[i].nome);
       }
        i++;
    }
}

int main()
{
    int T; // escolha de ordenacao
    int N, M;   // numero de alunos e bolsas respectivamente

    // variaveis de escaneio de entrada
    char Nome[15];
    int recup;
    float media;

    //escaneio de escolha de metodo, numero de alunos e bolsas
    scanf("%d", &T);
    scanf("%d %d", &N, &M);

    // alocacao de memoria para o numero de alunos que serao inseridos
    Alunos *alunos = calloc(N, sizeof(Alunos));

    // leitura dos dados
        for(int i=0; i<N; i++){
            scanf("%f %d %s", &media, &recup, Nome);
            fflush(stdin);
            alunos[i].media= (media)*1000;
            alunos[i].rep=  recup;
            strcpy(alunos[i].nome, Nome);
            alunos[i].verdadeiro=0;
        }

    // selecao do selection sort
    if(T==1){
       selection_sort(alunos, N, M);
       printa_alunos(alunos, N, M);
    }

    //selecao do mergesort
    if(T==2){
        merge_Sort(alunos, 0 ,N-1, M); // e passado N-1 uma vez que ele fara parte das divisoes e
                                       // as comparacoes ocorrerao ate a coordenada em que existe um aluno
        printa_alunos(alunos, N, M);
    }

    //libera memoria alocada pos operacao do programa
    free(alunos);
    return 0;
}
