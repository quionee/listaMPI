#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mpi.h"

int main(int argc, char **argv) {
    int processo, total, subContador = 0, tamanho, n = 1000000;
    
    // inicializa o MPI
    MPI_Init(&argc, &argv);
    // tamanho recebe a quantidade de processos pedidos
    MPI_Comm_size(MPI_COMM_WORLD, &tamanho);

    // intervalo é o tamanho do intervalo que cada processo vai executar
    int intervalo = n / tamanho;
    
    // define um identificador para cada processo
    MPI_Comm_rank(MPI_COMM_WORLD, &processo);

    // define onde o processo começa e termina sua execução
    int inicio = (processo * intervalo) + 1, fim = (processo + 1) * intervalo;

    // caso a quantidade de processos seja ímpar, o último processo
    // é incrementado para que todos os números sejam atendidos
    if (processo == (tamanho - 1)) {
        if ((tamanho % 2) != 0) {
            ++fim;
        }
    }
    else {
        // caso o rank do processo seja par, o último número analisado
        // pelo processo será ímpar, por esse motivo, fim recebe fim + 2
        if ((processo % 2) == 0) {
            fim += 2;
            
            // para o primeiro processo começar em 2, não em 1
            if (processo == 0) {
                ++inicio;
            }
        }
        else {
            // caso o rank do processo seja ímpar, o último número analisado
            // pelo processo será par, por esse motivo, fim recebe fim + 1
            fim += 1;
        }
    }
    
    // primoAux incrementa quando encontra um primo, se ele for 2 e o
    // primoAnterior for consecutivo a ele, subContador é incrementado
    int primoAux = 0, primoAnterior;
    
    for (int i = inicio; i <= fim; ++i) {
        if ((i % 2) != 0) {
            bool primo = true;
            for (int j = i / 2; (j >= 2) && primo; --j) {
                if ((i % j) == 0) {
                    primo = false;
                }
            }
            if (primo) {
                ++primoAux;
                if (primoAux == 1) {
                    primoAnterior = i;
                }
                else if (primoAux == 2) {
                    if (i == (primoAnterior + 2)) {
                        subContador += 1;
                    }
                    primoAux = 1;
                    primoAnterior = i;
                }
            }
        }
    }
    
    // os sub-contadores são somados ao total e colocados no processo 0
    MPI_Reduce(&subContador, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (processo == 0) {
        printf("Total = %d, processo = %d\n", total, processo);
    }
    
    MPI_Finalize();
    
    return 0;
}
