#define NX 2400
#define NY 100
#define NZ 100

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char **argv)
{
    // MPI Variables
    int my_rank, size;

    // Create the main matrices with the predefined size
    double *A, *B, *C;
    // Start the MPI parallel sequence
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

        A = malloc(NX*NY*NZ * sizeof(double));
        B = malloc(NX*NY*NZ * sizeof(double));
        C =  malloc(NX*NY*NZ * sizeof(double));


    if (my_rank == 0){
        for(int i = 0; i < NX*NY*NZ-1; ++i) {
                A[i] = ((double)rand()) + 1; /* this will generate a random number between 1 and 10 */
                B[i] = ((double)rand()) + 1; /* this will generate a random number between 1 and 10 */
        }
	printf("Initialized arrays... value A[2] is %f, value of B[2] is %f \n",A[2] , B[2]);
    }

    double *Asub_array = malloc((NX*NY*NZ)/size * sizeof(double));
    double *Bsub_array = malloc((NX*NY*NZ)/size * sizeof(double));
    double *Csub_array = malloc((NX*NY*NZ)/size * sizeof(double));

        printf("About to scatter... value A[1] is %f \n",A[1]);
        MPI_Scatter(A, (NX*NY*NZ)/size, MPI_DOUBLE,Asub_array ,(NX*NY*NZ)/size, MPI_DOUBLE, 0,MPI_COMM_WORLD);
        MPI_Scatter(B, (NX*NY*NZ)/size, MPI_DOUBLE,Bsub_array , (NX*NY*NZ)/size, MPI_DOUBLE, 0,MPI_COMM_WORLD);
    
    printf("Rank %d, value Asub_array[1] is %f, value of Bsub_array[1] is %f, the first sum will be %f \n", my_rank,Asub_array[1] , Bsub_array[1], Asub_array[1] + Bsub_array[1]);

    for (int i = 0; i< (NX*NY*NZ)/size; i++){
      Csub_array[i] = Asub_array[i] + Bsub_array[i];
//	printf("Local sum running... Rank %d, value Asub_array[i] is %.6f, value of Bsub_array[i] is %.6f, the sum is %.6f \n", my_rank,Asub_array[i] , Bsub_array[i], Csub_array[i]);
}
      printf("About to gather on rank 0...");
      MPI_Gather(Csub_array, (NX*NY*NZ)/size, MPI_DOUBLE, &C[my_rank*(NX*NY*NZ)/size], (NX*NY*NZ)/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
if (my_rank == 0){      
   printf("Values collected on process %d: %f, %f, %f, %f \n", my_rank, C[0], C[1], C[2],C[3]);
}

MPI_Finalize();

    // if it's on the master node
    //if (rank == 0)
    //    printResults(matrixA, matrixB, matrixC, calcTime);
    //    printf("First 4  values collected in C: %f, %f, %f, %f \n", C[0], C[1], C[2], C[3]);
    //    return 0;
}

