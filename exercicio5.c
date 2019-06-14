#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mpi.h"

// função auxilixar para verificar a restrição de números consecutivos
bool naoConsecutivo(int vetor[]) {
    int i = 0;
    while (i < 5) {
        if (vetor[i] == vetor[i + 1]) {
            return false;
        }
        ++i;
    }
    return true;
}

// função auxiliar para verificar se a soma dos dígitos não são 7, 11 ou 13
bool somaOk(int vetor[]) {
    int i = 0, soma = 0;
    while (i < 6) {
        soma += vetor[i];
        ++i;
    }
    if ((soma != 7) && (soma != 11) && (soma != 13)) {
        return true;
    }
    return false;
}

int main(int argc, char **argv) {
    int numero, processo, contadorFinal, subContador = 0, tamanho, n = 1s000000;
    
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

	// decrementa o fim, pois iniciamos com um a mais para facilitar cálculos
    if (processo == (tamanho - 1)) {
        --fim;
    }
    
    // se for o primeiro processo, ele pode partir de 100000, uma vez que
    // o número não pode se iniciar com 0 de acordo com uma das restrições
    if (processo == 0) {
        inicio = 100000;
    }
    
    // o número é colocado em um vetor para representar os dígitos
    // separadamente e então realizar a verificação das restrições
    // pelas funções auxiliares
    for (numero = inicio; numero <= fim; ++numero) {
        int vetor[6];
        
        int numeroAux = numero;
        
        int i = 5;
        while (i >= 0) {
            vetor[i] = numeroAux % 10;
            numeroAux /= 10;
            --i;
        }
        
        if (naoConsecutivo(vetor) && somaOk(vetor)) {
            ++subContador;
        }
        
    }
    
    // os sub-contadores são somados ao contadorFinal e colocados no processo 0
    MPI_Reduce(&subContador, &contadorFinal, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (processo == 0) {
        printf("contadorFinal = %d, processo = %d\n", contadorFinal, processo);
    }
    
    MPI_Finalize();
    
    return 0;
}
