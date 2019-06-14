#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv) {
    int size, rank, rc;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    printf ("hello, world, from process %d\n",rank);
    
    MPI_Finalize();
    
    return 0;
}
