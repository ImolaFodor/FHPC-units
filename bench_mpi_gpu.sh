#!/bin/bash


# Run for 1 mins:
#PBS -l walltime=00:10:00
#PBS -l nodes=2:ppn=2
#PBS -q dssc_gpu

# Where to write stderr:
#PBS -e output_header.err

# Where to write stdout:
#PBS -o outputs_openmpi_gpu.txt

# This command switched to the directory from which the "qsub" command was run:
cd $PBS_O_WORKDIR

#  Now run my programs

module load openmpi
mpirun -np 2 --report-bindings --map-by node ./IMB-MPI1 PingPong
echo 1st Done!

mpirun -np 2 --report-bindings --map-by socket ./IMB-MPI1 PingPong
echo 2nd Done!

mpirun -np 2 --report-bindings --map-by core ./IMB-MPI1 PingPong
echo 3rd Done!

mpirun -np 2 --report-bindings --mca pml ob1 --mca btl tcp,self ./IMB-MPI1 PingPong
echo 4th Done!

mpirun -np 2 --report-bindings --mca pml ob1 --mca btl vader,self ./IMB-MPI1 PingPong
echo 5th Done!

mpirun -np 2 --report-bindings --mca pml ob1 --mca btl tcp,vader,self ./IMB-MPI1 PingPong
echo 6th Done!

exit

