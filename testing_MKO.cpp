// testing the make_correct_order function for the main problem
#include <iostream>

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


void printArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int deadlines[] = {2, 3, 3, 1, 2};
    int M = sizeof(deadlines) / sizeof(deadlines[0]);

    std::cout << "Vetor original: ";
    printArray(deadlines, M);

    int* order = make_correct_order(M, deadlines);

    std::cout << "Prioridades: ";
    printArray(order, M);

    delete[] order;
    return 0;
}
