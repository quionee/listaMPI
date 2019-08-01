#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <math.h>

int qtdTrapezios = 14;
float a = 0;
float b = 5;

// função base, pode ser utilizada qualquer função para o cálculo
float f(float x){ 
	float valorFuncao; 
	valorFuncao = x*x+2*x;  // exemplo dado no problema
	
	return valorFuncao; 
} 

int main(int argc, char **argv){
	int i,  processo, nProcessos, **num;
	// calcula o tamanho da base
	float h = (b-a)/qtdTrapezios; 
	// a variável "subtotal" é usada para armazenar o valor função f(x)
	// em cada processo, e a variável "integral" armazena o valor final da função
	float subtotal, integral;

	// inicializa o MPI
	MPI_Init(&argc, &argv);
	// tamanho recebe a quantidade de processos pedidos
	MPI_Comm_size(MPI_COMM_WORLD, &nProcessos);
	// define um identificador para cada processo
	MPI_Comm_rank(MPI_COMM_WORLD, &processo);

	// intervalo é o tamanho do intervalo que cada processo vai executar
	int intervalo = floor(qtdTrapezios/nProcessos);
	

	if(processo == nProcessos-1){
		// caso o número de processos não consiga atender igualmente o número 
		// de trapézios, o ultimo processo trata uma maior quantidade
		int tamanhoAux = 0;
		if(qtdTrapezios % nProcessos != 0){
			tamanhoAux = qtdTrapezios - (intervalo * nProcessos);
			
		}
		for(int x = processo * intervalo; x < ((processo + 1) * intervalo) + tamanhoAux; ++x){
			subtotal += f((h * (x + 1)) + a);
		}
	}
	else {
		for(int x = processo * intervalo; x < ((processo + 1) * intervalo); ++x){
				subtotal += f((h * (x + 1)) + a);
		}
	}
	
	// os sub-totais são somados a variável "integral" e colocados no processo 0
	MPI_Reduce(&subtotal,&integral,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
	
	if (processo==0) {
		// o processo zero faz as somas que não foram paralelizadas
		integral += ((f(a)+f(b))/2);
		// é necessário retirar o último, pois calcula uma vez mais
		integral -= f(b);
		integral = integral * h;
		printf("com %i trapezios, nossa estimativa da integral de %f a %f é = %f  ", qtdTrapezios,a,b,integral );
		printf("\n");	
	}
	MPI_Finalize();
	return 0;
}
