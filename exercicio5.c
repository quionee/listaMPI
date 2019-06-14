#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mpi.h"

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
    int numero, proc, contadorFinal, subContador = 0, size, n = 1s000000;
    
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
		--fim;
    }
    
    
    if (proc == 0) {
		inicio = 100000;
	}
    
    // primoAux incrementa quando encontra um primo, se ele for 2 e o
    // valor da diferença entre o primo atual e o primo anterior for
    // contadorFinal que o valor de "subContador", então atualizamos o valor deste
    int primoAux = 0, primoAnterior;
    
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
    
    if (proc == 0) {
        printf("contadorFinal = %d, rank = %d\n", contadorFinal, proc);
    }
    
    MPI_Finalize();
    
    return 0;
}
