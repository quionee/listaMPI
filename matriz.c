// o exercicio cria uma matriz a partir de um arquivo txt no formato do "matriz.txt"

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>

int auxiliar;
int main(int argc, char **argv){
	// cria a matriz a partir do arquivo
	FILE *file = fopen ("matriz.txt", "r" );
	char* m1s;
	char* aux;
	char* m2s;
	char line [ 128 ]; 
    m1s = ( fgets ( line, sizeof line, file ));
    int lin1 = m1s[0] - 48;
    int col1 = m1s[2] - 48;    
    int m1[lin1][col1];
    for(int i = 0; i < lin1;++i){
		aux = ( fgets ( line, sizeof line, file ));
		for(int j = 0; j < col1; j++){
			m1[i][j] = aux[j*2]-48;
			}
	}
	m2s = ( fgets ( line, sizeof line, file ));
    int lin2 = m2s[0] - 48;
    int col2 = m2s[2] - 48;
    int m2[lin2][col2];   

	for(int i = 0; i < lin2;++i){
	aux = ( fgets ( line, sizeof line, file ));
		for(int j = 0; j < col2; j++){
			m2[i][j] = aux[j*2]-48;
			}
	}
	int proc, nprocs, **num;
	int** m3 = new int[lin1][col2];
	float subtotal, total;



	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &proc);
	// calcula a razao
	int r = ceil(lin1/nprocs);
	// o ultimo processo fica com o trabalho de realizar operações sobressalentes em caso da razao não ser um numero exato
	if(proc == nprocs-1){
		if(lin1 % nprocs != 0){
			int tam = lin1 - (r*nprocs);
			for(int i = proc*r; i <((proc+1)*r)+tam; ++i){
				printf("i =   %i",i);
				for(int j = 0; j < col2; ++j){
					int val = 0;
					for(int m = 0; m < col1; ++m){
						val += m1[i][m] * m2[m][j];
					}
					m3[i][j] = val;
				}
			}
				
		}
		else{
			for(int i = proc*r; i <(proc+1)*r; ++i){
				for(int j = 0; j < col2; ++j){
					int val = 0;
					for(int m = 0; m < col1; ++m){
						val += m1[i][m] * m2[m][j];
					}
					m3[i][j] = val;
				}
			}
		}
	}else{
		for(int i = proc*r; i <(proc+1)*r; ++i){
			for(int j = 0; j < col2; ++j){
				int val = 0;
				for(int m = 0; m < col1; ++m){
					val += m1[i][m] * m2[m][j];
				}
			m3[i][j] = val;
			}
		}
	} 
    fclose ( file );


  MPI_Finalize();
  return 0;
}
