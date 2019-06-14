#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv) {
    int tamanho, id, rc;
    
    // inicializa o MPI
    MPI_Init(&argc, &argv);
    // define um identificador para cada processo
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    
    printf("hello, world, from process %d\n", id);
    
    // finaliza o MPI
    MPI_Finalize();
    
    return 0;
}
