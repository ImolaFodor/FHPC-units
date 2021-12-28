#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main (int argc, char * argv[])
{

int num_procs, rank;

// Times 
double start_time, end_time;

MPI_Init(&argc, &argv);

start_time = MPI_Wtime();

MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

int msg_forward, msg_backward, msg_prev_left, msg_prev_right;
msg_prev_left = 0;
msg_prev_right = 0;

// Making sure to remain in the boundaries of the ring
// Example Rank forward next for 0 is 1, rank backward next for 0 is 3. 
int rank_forward_next = (rank + 1) % num_procs;
int rank_forward_prev = rank == 0 ? num_procs - 1 : rank - 1;
int rank_backward_next = rank == 0 ? num_procs - 1 : rank - 1;
int rank_backward_prev = (rank + 1) % num_procs;

MPI_Request request;
MPI_Status status_forward;
MPI_Status status_backward;

int i_back, i_forw, i_recv_back, i_recv_forw;


for (int i = 0; i<num_procs; i++){

 // i_back (i_send_back_tag would be more meaningful) stands for the tag to send to the next rank in the backward modality 
 i_back = (i == 0 | i == (num_procs - 1)) ? rank * 10 : ((rank + i) % num_procs) * 10;
 i_forw = (i == 0 | i == (num_procs - 1)) ? rank * 10 : (abs((rank - i) % num_procs)) * 10;

 // i_recv_back_tag would be a more meaningful name
 i_recv_back = (i == 0 | i == (num_procs - 1)) ? rank_backward_prev * 10 : ((rank_backward_prev + i) % num_procs) * 10;
 i_recv_forw = (i == 0 | i == (num_procs - 1)) ? rank_forward_prev * 10 : (abs(rank_forward_prev - i) % num_procs) * 10;

 msg_forward = i == 0 ? -rank : msg_prev_right-rank;
 msg_backward = i == 0 ? rank :  msg_prev_left + rank;

 if (rank % 2 == 0) {
// printf("%d: sending to left %d msg %ld with tag %d \n",rank, rank_backward_next, msg_backward, i_back);
 MPI_Send((void *)&msg_backward, 1, MPI_INT, rank_backward_next, i_back, MPI_COMM_WORLD);

 MPI_Recv((void *)&msg_prev_right, 1, MPI_INT, rank_forward_prev, i_recv_forw, MPI_COMM_WORLD, &status_forward);
// printf("%d: receiving from left %d msg %d with tag %d\n",rank, rank_forward_prev, msg_prev_right, status_forward.MPI_TAG);

// printf("%d: sending to right %d msg %d with tag %d \n",rank, rank_forward_next,msg_forward, i_forw);
 MPI_Send((void *)&msg_forward, 1, MPI_INT, rank_forward_next, i_forw, MPI_COMM_WORLD);

 MPI_Recv((void *)&msg_prev_left, 1, MPI_INT, rank_backward_prev, i_recv_back, MPI_COMM_WORLD, &status_backward);
// printf("%d: receiving from right %d msg %ld with tag %d\n", rank, rank_backward_prev, msg_prev_left, status_backward.MPI_TAG);


} else{ 
 MPI_Recv((void *)&msg_prev_left, 1, MPI_INT, rank_backward_prev, i_recv_back, MPI_COMM_WORLD, &status_backward);
// printf("%d: receiving from right %d msg %d with tag %d\n", rank, rank_backward_prev, msg_prev_left, status_backward.MPI_TAG);
 
// printf("%d: sending to right %d msg %d with the tag %d\n",rank, rank_forward_next,msg_forward, i_forw);
 MPI_Send((void *)&msg_forward, 1, MPI_INT, rank_forward_next, i_forw, MPI_COMM_WORLD);

 MPI_Recv((void *)&msg_prev_right, 1, MPI_INT, rank_forward_prev, i_recv_forw, MPI_COMM_WORLD, &status_forward);
// printf("%d: receiving from left  %d msg %d with tag %d\n",rank,  rank_forward_prev, msg_prev_right, status_forward.MPI_TAG);

// printf("%d: sending to left %d msg %d with the tag %d\n",rank,rank_backward_next, msg_backward, i_back);
 MPI_Send((void *)&msg_backward, 1, MPI_INT,rank_backward_next, i_back, MPI_COMM_WORLD);
 
}
}

printf("I am process %d and I have received %d messages. My final message has the tag %d and value from backward loop %d and from forw loop %d \n", rank, num_procs, i_back, msg_backward, msg_forward);
end_time=MPI_Wtime();

 printf("Start Time: %f, End Time: %f, Elapsed Time: %10.8f \n",start_time, end_time, end_time-start_time);
MPI_Finalize();
}

