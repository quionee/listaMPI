#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mpi.h"

int main(int argc, char **argv) {
    int processo, maior, subMaior = 0, tamanho, n = 1000000;
    
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
        // pelo processo será ímpar, por esse motivo, fim recebe o valor
        // do proximo primo encontrado
        if ((processo % 2) == 0) {
            bool achouPrimo = false;
            int i = fim;
            while (!achouPrimo) {
                i += 2;
                bool primo = true;
                for (int j = i / 2; (j >= 2) && primo; --j) {
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
            if (processo == 0) {
                ++inicio;
            }
        }
        else {
            // caso o rank do processo seja ímpar, o último número analisado
            // pelo processo será par, por esse motivo, fim recebe o valor 
            // do proximo primo encontrado
            bool achouPrimo = false;
            int i = fim - 1;
            while (!achouPrimo) {
                i += 2;
                bool primo = true;
                for (int j = i / 2; (j >= 2) && primo; --j) {
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
                    if ((i - primoAnterior) > subMaior) {
                        subMaior = (i - primoAnterior);
                    }
                    primoAux = 1;
                    primoAnterior = i;
                }
            }
        }
    }
    
    // coloca no processo 0 o maior dos submaiores de cada processo
    MPI_Reduce(&subMaior, &maior, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (processo == 0) {
        printf("maior = %d, processo = %d\n", maior, processo);
    }
    
    MPI_Finalize();
    
    return 0;
}
