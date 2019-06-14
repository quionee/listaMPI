#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mpi.h"

int main(int argc, char **argv) {
    int i, j, proc, maior, subMaior = 0, size, n = 1000000;
    
    MPI_Init(&argc, &argv);
    
    // size recebe a quantidade de processos pedidos
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // intervalo é o tamanho do intervalo que cada processo vai executar
    int intervalo = n / size;
    
    // define um identificador para cada processo
    MPI_Comm_rank(MPI_COMM_WORLD, &proc);

    // define onde o processo começa e termina sua execução
    int inicio = (proc * intervalo) + 1, fim = (proc + 1) * intervalo;

    // caso a quantidade de processos seja ímpar, o último processo
    // é incrementado para que todos os números sejam atendidos
    if (proc == (size - 1)) {
        if ((size % 2) != 0) {
            ++fim;
        }
    }
    else {
        // caso o rank do processo seja par, o último número analisado
        // pelo processo será ímpar, por esse motivo, fim recebe o valor
        // do proximo primo encontrado
        if ((proc % 2) == 0) {
            bool achouPrimo = false;
            i = fim;
            while (!achouPrimo) {
                i += 2;
                bool primo = true;
                for (j = i / 2; (j >= 2) && primo; --j) {
                    if ((i % j) == 0) {
                        primo = false;
                    }
                }
                if (primo) {
                    achouPrimo = true;
                }
            }
            fim = i;
            
            // para o primeiro processo começar em 2, não em 1
            if (proc == 0) {
                ++inicio;
            }
        }
        else {
            // caso o rank do processo seja ímpar, o último número analisado
            // pelo processo será par, por esse motivo, fim recebe o valor 
            // do proximo primo encontrado
            bool achouPrimo = false;
            i = fim - 1;
            while (!achouPrimo) {
                i += 2;
                bool primo = true;
                for (j = i / 2; (j >= 2) && primo; --j) {
                    if ((i % j) == 0) {
                        primo = false;
                    }
                }
                if (primo) {
                    achouPrimo = true;
                }
            }
            fim = i;
        }
    }
    
    // primoAux incrementa quando encontra um primo, se ele for 2 e o
    // valor da diferença entre o primo atual e o primo anterior for
    // maior que o valor de "subMaior", então atualizamos o valor deste
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
                    if ((i - primoAnterior) > subMaior) {
                        subMaior = (i - primoAnterior);
                    }
                    primoAux = 1;
                    primoAnterior = i;
                }
            }
        }
    }
    
    // os sub-contadores são somados ao maior e colocados no processo 0
    MPI_Reduce(&subMaior, &maior, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (proc == 0) {
        printf("maior = %d, rank = %d\n", maior, proc);
    }
    
    MPI_Finalize();
    
    return 0;
}
