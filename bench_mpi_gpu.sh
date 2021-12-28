#!/bin/bash


# Run for 15 mins:
#PBS -l walltime=00:15:00
#PBS -l nodes=2:ppn=2
#PBS -q dssc_gpu

# Where to write stderr:
#PBS -e output_header.err

# Where to write stdout:
#PBS -o outputs_openmpi_gpu.txt

# This command switched to the directory from which the "qsub" command was run:
cd $PBS_O_WORKDIR

#  Now run my programs
make clean
module load openmpi
make

#mpirun -np 2 --report-bindings --oversubscribe --map-by node ./IMB-MPI1 PingPong
echo Map by node on ip done!

mpirun -np 2 --report-bindings --map-by socket ./IMB-MPI1 PingPong
echo echo Map by socket on ip done!


mpirun -np 2 --report-bindings --map-by core ./IMB-MPI1 PingPong
echo echo Map by core on ip done!

#mpirun -np 2 --mca pml ob1 --report-bindings --oversubscribe --map-by node --mca btl tcp,self ./IMB-MPI1 PingPong
echo Map by node on tcp done!

mpirun -np 2 --mca pml ob1 --report-bindings --map-by socket --mca btl tcp,self ./IMB-MPI1 PingPong
echo Map by socket on tcp done!

mpirun -np 2 --report-bindings --mca pml ob1 --mca btl tcp,vader,self ./IMB-MPI1 PingPong
echoMap by core(vader) on tcp done!

exit

