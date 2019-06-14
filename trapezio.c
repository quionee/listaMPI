
#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <math.h>
int quantidade_t = 14;
float a = 0;
float b = 5;

//funcao base, pode ser qualquer uma
float f(float x){ 
	float return_val; 
	return_val = x*x+2*x;  
	return return_val; 
} 

int main(int argc, char **argv){
	float h = (b-a)/quantidade_t; // calcula a altura
	int i,  proc, nprocs, **num;
	float subtotal, total;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &proc);
	// faz a razao de entre processadores e trapezios
	int d = floor(quantidade_t/nprocs);
	// o ultimo processo é um caso especial
	if(proc == nprocs-1){
		// verifica se todos é possivel que todos os processos calculem o mesmo numero de trapezios
		if(quantidade_t % nprocs != 0){
			// caso não seja possivel a distribuição igualitaria, o ultimo processo faz os sobressalentes
			// a variavel tam calcula a quantidade de tais trapezios
			int tam = quantidade_t - (d*nprocs);
			// faz o calculo dos trapezios do ultimo processo, ja com o aumento
			for(int aux = proc*d;aux < ((proc+1)*d)+tam;++aux){
				subtotal += f((h*(aux+1))+a);
			}
		}
		// caso seja possivel calcula normalmente
		else{
				// faz o calculo normal
				for(int aux = proc*d;aux < ((proc+1)*d);++aux){
					//printf("%i,%i",aux,proc);
					subtotal += f((h*(aux+1))+a);
					//printf("%f",h);
					//printf("\n");
				}
		}
	// todos os outros processadores calculam os trapezios normalmente
	} else{
		for(int aux = proc*d;aux < ((proc+1)*d);++aux){
		//printf("%i,%i",aux,proc);
		subtotal += f((h*(aux+1))+a);
		//printf("%f",h);
		//printf("\n");
		}
	}
	// soma todos os subtotais
MPI_Reduce(&subtotal,&total,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
  if (proc==0) {
	  // o processo zero faz as somas que não sâo paralelizadas
	  total += ((f(a)+f(b))/2);
	  total -= f(b);
	  total = total *h;
	  printf("com %i trapezoides, nossa estimativa da integral de %f a %f é = %f  ", quantidade_t,a,b,total );
	  printf("\n");	
  }
  MPI_Finalize();
  return 0;
}
