#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **argv) {
    int processo, total, tamanho, subtotal;
    
    // inicializa o MPI
    MPI_Init(&argc, &argv);
    // size recebe a quantidade de processos pedidos
    MPI_Comm_size(MPI_COMM_WORLD, &tamanho);
    // define um identificador para cada processo
    MPI_Comm_rank(MPI_COMM_WORLD, &processo);
    
    subtotal = processo + 1;
    
    // redução da soma dos subtotais de cada processo para o processo 0
    MPI_Reduce(&subtotal, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (processo == 0) {
        printf("Total calculado paralelamente = %d, processo = %d\n", total, processo);
        printf("Total pela formula do somatorio = %d\n", (tamanho * (tamanho + 1)) / 2);
    }
    
    MPI_Finalize();
    
    return 0;
}

