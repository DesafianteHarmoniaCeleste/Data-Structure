/*
    Training greedy algorithms
    N - Days 1< N < 10.000
    M - chores 1< M < 10.000
*/

#include <iostream>

using namespace std;

typedef struct t_robot_chore{
    int day;
    int robot;
}robot_chore;

int* make_correct_order(int M, int* dead_line){
    int* correct_order = new int[M];
    int* vector_copy = new int[M]; // created after i saw that dead_line was being changed after the function is being called

    for(int i=0; i<M; i++){
        vector_copy[i] = dead_line[i];
    }

    for(int i=0; i<M; i++){
        int aux=999000;
        int place= -1;

        for(int j=0; j<M; j++){
            if(vector_copy[j]<aux){
                aux = vector_copy[j];
                place = j;
            }
        }
        correct_order[place] = i;
        vector_copy[place] = 999999;
    }

    delete[] vector_copy;
    return correct_order;
}

void greedy_alg(int N, int M, int* dead_line){
    int min_robots=0;
    robot_chore* R_chores = new robot_chore[M];
    int* correct_order = make_correct_order(M, dead_line);
    int aux=0;    

    // um while pra verificar se todos os dias foram preenchidos de forma certa
    // caso de errado, aumentar em 1 o numero de robos

    // buscar o numero minimo na ordem correta e atribuir um robo a ele
};

int main(){
    int N, M;
    cin >> N >> M;
    int* dead_line = new int[M];
    int aux=0;
    while(aux!=M){
        cin>> dead_line[aux];
        aux++;
    }

    delete[] dead_line;
    return 0;
}
