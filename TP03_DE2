// Pedro Gasparelo Leme 14602421
// importa as bibliotecas necessarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// definicao do elemento da hash
typedef struct t_elemento {
    char key[32]; // palavra a ser armazenada
    int ocupado; // indica se o elemento esta ocupado por alguma palavra
    int contador; // conta quantas iguais tem palavras
} Elem;

// definicao da hash, que sera o dicionario de elementos palavra
typedef struct t_thash {
    Elem* elems; // ponteiro que sera alocado para as palavras
    int len_hash; // tamanho da hash, dado pelo operador
    int C1;       // constantes de operacao quadratica para rehash
    int C2;
    int qnt;        // quantas palavras diferentes tem na hash
} Hash;

// operacao de hash para insercao de elementos novos
int operacao_hash(const char* key, int len_hash) {
    int hash = 0;
    while (*key) {
        hash = (hash * 4 + *key) % len_hash; // percorre letra por letra, pegando seu calor ascii
        key++;
    }
    return hash; // retorna a posicao de insercao inicial na tabela hash
}

// operacao quadratica de rehash para achar posicao nao ocupada
int operacao_rehash(int hash, int i, Hash* dicionario) {
    return (hash + dicionario->C1 * i + dicionario->C2 * i * i) % dicionario->len_hash;
}

// funcao que retorna o elemento que sera inserido na tabela hash
Elem return_elem(char* key) {
    Elem dado;
    strcpy(dado.key, key); // key corresponde a palavra que sera inserida
    dado.contador = 1;
    dado.ocupado = 1;
    return dado;
}

// funcao que insere o elemento na tabela hahs em sua devida posicao
void insere_palavras(Hash* dicionario) {
    char palavra[120001];
    fgets(palavra, 120001, stdin); // leitura da frase a ser inserida
    palavra[strcspn(palavra, "\n")] = '\0'; // procura a posicao de ocorrencia do primeiro \n (pular linha) e faz essa posicao receber \0 para indicar fim da string

    char* chave = strtok(palavra, " "); // separacao das palavras inseridas na frase pela barra de espaco
    while (chave != NULL) { // enquanto todas as palavras nao tiiverem sido lidas
        char palavra_aux[32];
        strcpy(palavra_aux, chave);
        chave = strtok(NULL, " "); // movimenta a chave para a proxima palavra da frase

        int hash = operacao_hash(palavra_aux, dicionario->len_hash); // recebe a posicao de insercao
        int i = 0;
        int aux = hash;
        int salva_primeira_remocao = -1; // variavel criada para salvar a posicao onde houve a primeira remocao,
                                        //para caso a palavra nao seja encontrada ao varrer a hash, ela sera inserida na posicao removida
        int verificacao = 0;// variavel criada para efetuar insercao de palavras iguais e dar procedencia para primeira isercao de palavra nova

        while (dicionario->elems[aux].ocupado == 1 || strcmp(dicionario->elems[aux].key, "####") == 0) { // varre ate encontrar uma posicao nao ocupada que nao tenha sido previamente removida
            if (strcmp(dicionario->elems[aux].key, "####") == 0 && salva_primeira_remocao == -1) { // caso encontre posicao previamente removida, salva posicao para possivel insercao
                salva_primeira_remocao = aux;
            }
            if (strcmp(dicionario->elems[aux].key, palavra_aux) == 0) { // caso palavra ja tenha sido inserida previamente
                dicionario->elems[aux].contador++;
                verificacao = 1;
                break;
            }
            i++;
            aux = operacao_rehash(hash, i, dicionario); // efetua operacao de rehash para encontrar posicao valida
            if (i >= dicionario->len_hash) { // caso o hash esteja cheio, da um retorno forcado
                return;
            }
        }

        if (verificacao) {
            continue;
        }
        else if (salva_primeira_remocao != -1) {// operacao para caso seja inserida em posicao ja removida
            dicionario->elems[salva_primeira_remocao] = return_elem(palavra_aux);
            dicionario->qnt++;
        } else if (!dicionario->elems[aux].ocupado) { // operacao para isercao em lugar nao ocupado e que nao foi removido
            dicionario->elems[aux] = return_elem(palavra_aux);
            dicionario->qnt++;
        }
    }
}

// funcao que retorna a posicao da palavra com maior freequencia de aparicoes
int palavra_frequente(Hash* dicionario) {
    int pos = -1;
    int freq = 0;
    for (int aux = 0; aux < dicionario->len_hash; aux++) { // varre a hash inteira
        if (dicionario->elems[aux].ocupado) { // caso tenha uma palavra naquela posicao
            if (dicionario->elems[aux].contador > freq ||
                (dicionario->elems[aux].contador == freq && strcmp(dicionario->elems[aux].key, dicionario->elems[pos].key) < 0)) {
                freq = dicionario->elems[aux].contador; // salva a frequencia para futuras comparacoes
                pos = aux;  // salva a posicao em que se tem a maior frequencia dapalavra
            }
        }
    }
    return pos;
}

// funcao que cria, aloca o espaco e retorna a hash
Hash* create_hash(int size) {
    Hash* thash = calloc(1, sizeof(Hash));
    if (thash == NULL) { // caso haja falha de alocacao de memoria
        return NULL;
    }
    thash->elems = calloc(size, sizeof(Elem)); // alocacao de quantos elementos tera a hash
    thash->len_hash = size;
    return thash;
}

// funcao que printa as palavras inseridas e qual sua posicao na hash
void exibe_hash(Hash* dicionario) {
    printf("imprimindo tabela hash\n");
    for (int aux = 0; aux < dicionario->len_hash; aux++) {
        if (dicionario->elems[aux].ocupado) {
            printf("%s %d\n", dicionario->elems[aux].key, aux);
        }
    }
    printf("fim da tabela hash\n");
}

// funcao que busca palavras
void busca_palavras(Hash* dicionario) {
    int buscas;
    scanf("%d", &buscas); // numero de buscas a ser realizado
    char palavras_encontrar[buscas][32];
    for (int i = 0; i < buscas; i++) {
        scanf("%s", palavras_encontrar[i]); // leitura das palavras a serem buscadas
    }

    for (int inter = 0; inter < buscas; inter++) { // loop que efetua as seguintes acoes para cada palavra a ser encontrada
        int hash = operacao_hash(palavras_encontrar[inter], dicionario->len_hash); // efetua operacao hash para a palavra

        int i = 0;
        int aux = hash;
        int found = 0;
        while (i < dicionario->len_hash && dicionario->elems[aux].ocupado) { // loop ate encontrar posicao  nao ocupada ou ate varrer o hash todo
            if (strcmp(dicionario->elems[aux].key, palavras_encontrar[inter]) == 0) { // caso ache a palavra
                printf("%s encontrada %d\n", palavras_encontrar[inter], dicionario->elems[aux].contador);
                found = 1; // palavra foi encontrada
                break; // sai do loop de rehash, vai para a proxima palavra
            }
            i++;
            aux = operacao_rehash(hash, i, dicionario); // efetua operacao de rehash ate sair do loop ou satisfazer as condicoes
        }
        if (!found) { // caso nao encontre
            printf("%s nao encontrada\n", palavras_encontrar[inter]);
        }
    }
}

// funcao que remove a palavra buscada
void remove_palavras(Hash* dicionario) {
    int remocoes;
    scanf("%d", &remocoes);// numero de remocoes a ser realizado
    char palavras_remover[remocoes][32];
    for (int i = 0; i < remocoes; i++) {
        scanf("%s", palavras_remover[i]); // leitura das palavras
    }

    for (int inter = 0; inter < remocoes; inter++) { // loop que sera efetuado para cada palavra
        int hash = operacao_hash(palavras_remover[inter], dicionario->len_hash); // operacao hash

        int i = 0;
        int aux = hash;
        int found = 0;
        while (i < dicionario->len_hash && ( dicionario->elems[aux].ocupado==1 || strcmp(dicionario->elems[aux].key, "####" )==0)) { // loop ate encontrar posicao nao ocupada e nao removida,
                                                                                                                                     // ou ate chegar ao fim do hash
            if (strcmp(dicionario->elems[aux].key, palavras_remover[inter]) == 0) { // caso encontre a palavra
                printf("%s removida\n", palavras_remover[inter]);
                dicionario->elems[aux].contador = 0;
                dicionario->elems[aux].ocupado = 0;
                strcpy(dicionario->elems[aux].key, "####"); // apaga a palavra antes presente
                dicionario->qnt--;
                found = 1; // palavra foi encontrada
                break;
            }
            i++;
            aux = operacao_rehash(hash, i, dicionario); //efetua rehash ate sair do loop ou satisfazer as condicoes
        }
        if (!found) { // palavra nao encontrada
            printf("%s nao encontrada\n", palavras_remover[inter]);
        }
    }
}

int main() {
    int S, C1, C2;
    scanf("%d", &S);    // tamanho da hash
    scanf("%d %d", &C1, &C2);   // constantes para operacao quadratica de rehash

    Hash* dicionario = create_hash(S); // cria o hash e define suas caracteristicas
    dicionario->C1 = C1;
    dicionario->C2 = C2;

    int comando; // comando de manipulacao da tabela hash
    while (1) {
        scanf("%d", &comando);
        if (comando == 1) {
            insere_palavras(dicionario); // funcao de insercao de palavras
        } else if (comando == 2) {
            int pos = palavra_frequente(dicionario); // salva a posicao da palavra com maior frequencia
            if (pos != -1) {
                printf("foram encontradas %d palavras diferentes\n", dicionario->qnt);
                printf("palavra mais frequente = %s, encontrada %d vezes\n", dicionario->elems[pos].key, dicionario->elems[pos].contador);
            } else {
                printf("Nenhuma palavra encontrada.\n");
            }
        } else if (comando == 3) {
            busca_palavras(dicionario); // busca as palavras inseridas na hahs
        } else if (comando == 4) {
            remove_palavras(dicionario); // remove palavras, caso presentes na hash
        } else if (comando == 5) {
            exibe_hash(dicionario); // exibe a tabela hash
        } else if (comando == 0) {
            break;          // finaliza o programa
        }
    }
    // libera a memoria alocada para a execucao do codigo
    free(dicionario->elems);
    free(dicionario);
    return 0;
}
