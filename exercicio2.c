#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **argv){
    int proc, total, subtotal;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc);
    
    subtotal = proc + 1;
    
    MPI_Reduce(&subtotal, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (proc == 0) {
        printf("total = %d, rank = %d\n", total, proc);
    }
    
    MPI_Finalize();
    
    return 0;
}

