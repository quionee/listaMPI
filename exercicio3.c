#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mpi.h"

int main(int argc, char **argv){
    int i, j, proc, total, subContador = 0, size, n = 1000000;
    
    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int intervalo = n / size;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &proc);

    int inicio = (proc * intervalo) + 1, fim = (proc + 1) * intervalo;

    if (proc == (size - 1)) {
        if ((size % 2) != 0) {
            ++fim;
        }
    }
    else {
        if ((proc % 2) == 0) {
            fim += 2;
            if (proc == 0) {
                ++inicio;
            }
        }
        else {
            fim += 1;
        }
    }
    
    int primoAux = 0, primoAnterior;
    
    for (i = inicio; i <= fim; ++i) {
        if ((i % 2) != 0) {
            bool primo = true;
            for (j = i / 2; (j >= 2) && primo; --j) {
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
    
    MPI_Reduce(&subContador, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (proc == 0) {
        printf("total = %d, rank = %d\n", total, proc);
    }
    
    MPI_Finalize();
    
    return 0;
}
