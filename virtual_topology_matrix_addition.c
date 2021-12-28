//#define NX 800
//#define NY 300
//#define NZ 100

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
   int NX = atoi(argv[1]);
   int NY = atoi(argv[2]);
   int NZ = atoi(argv[3]);  

    // MPI Variables
    int rank, size;

   
    double start_time, end_time;
    // Create the main matrices with the predefined size
   
     float *A, *B, *C;
    
    // Start the MPI parallel sequence
    MPI_Init(&argc, &argv);

    start_time=MPI_Wtime();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int subarray_size = (NX*NY*NZ)/size;

    //Allocate memory of arrays to contain all values of the cubes
    A = malloc(NX*NY*NZ * sizeof(double));
    B = malloc(NX*NY*NZ * sizeof(double));
    C =  malloc(NX*NY*NZ * sizeof(double));


    if (rank == 0){
        for(int i = 0; i < NX*NY*NZ-1; ++i) {
                A[i] = ((double)rand()) + 1;
                B[i] = ((double)rand()) + 1;
        }
	printf("Initialized arrays... value A[2] is %f, value of B[2] is %f \n",A[2] , B[2]);
    }

    // Ask MPI to decompose our processes in a 3D cartesian grid
    int dims[3] = {0, 0, 0};
    MPI_Dims_create(size, 3, dims);
    // Make both dimensions non-periodic
    int periods[3] = {false, false, false};
 
    // Let MPI assign arbitrary ranks if it deems it necessary
    int reorder = true;

    MPI_Comm new_communicator;
    MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, reorder,
        &new_communicator);
    int my_rank;
    MPI_Comm_rank(new_communicator, &my_rank);
    
    // Get my coordinates in the new communicator
    int max_num_coords = 3;
    int my_coords[max_num_coords];
    MPI_Cart_coords(new_communicator, my_rank,max_num_coords , my_coords);
    // Print my location in the 3D torus.
    printf("[MPI process %d] I am located at (%d, %d, %d).\n", my_rank, my_coords[0],my_coords[1], my_coords[2]);

    // ALlocate memory for the sub-arrays
    double *Asub_array = malloc(subarray_size * sizeof(double));
    double *Bsub_array = malloc(subarray_size * sizeof(double));
    double *Csub_array = malloc(subarray_size * sizeof(double));

    // Collective operation, send equal chunks of the full arrays to the processors
    MPI_Scatter(A, subarray_size, MPI_DOUBLE,Asub_array ,subarray_size, MPI_DOUBLE, 0,new_communicator);
    MPI_Scatter(B, subarray_size, MPI_DOUBLE,Bsub_array , subarray_size, MPI_DOUBLE, 0,new_communicator);
    
//    printf("Rank %d, value Asub_array[1] is %f, value of Bsub_array[1] is %f, the first sum will be %f \n", my_rank,Asub_array[1] , Bsub_array[1], Asub_array[1] + Bsub_array[1]);

    // Performing the sum on sub-array on each processor
    for (int i = 0; i< subarray_size; i++){
      Csub_array[i] = Asub_array[i] + Bsub_array[i];
    }

    // The sums saved in Csub_arrays, get gathered on the root rank
    MPI_Gather(Csub_array, subarray_size, MPI_DOUBLE, &C[my_rank*subarray_size], subarray_size, MPI_DOUBLE, 0, new_communicator);
    
    if (my_rank == 0){
     printf("Values collected on process %d: %f, %f, %f, %f.\n", my_rank, C[0], C[1], C[2],C[3]);
     }

    end_time = MPI_Wtime();
    MPI_Finalize();

 printf("Start Time: %f, End Time: %f, Elapsed Time: %10.8f \n",start_time, end_time, end_time-start_time);
}

