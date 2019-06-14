
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char **argv){
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
	float subtotal, total;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &proc);
	
	int m3[lin1][col2];
	int r = lin1/nprocs;
	for(int i = proc*r; i <(proc+1)*r; ++i){
		for(int j = 0; j < col1; ++j){
			for(int m = 0;m < col2; ++m){
				int val = 0;
				for(int n = 0; n < lin2; ++n){
					val += m1[i][j] * m2[n][m];
				}
				m3[i][m] = val;
			}
		}
	}
	for(int i = 0; i < lin1; ++i){
		for(int j = 0; j < col2; ++j){
			printf("%i   ",m3[i][j]);
		}
		printf("\n");
	} 
    fclose ( file );


  MPI_Finalize();
  return 0;
}
