#!/bin/bash


# Run for 1 mins:
#PBS -l walltime=00:10:00
#PBS -l nodes=2:ppn=2
#PBS -q dssc

# Where to write stderr:
#PBS -e output_header_intel.err

# Where to write stdout:
#PBS -o outputs_intel_cpu.txt

# This command switched to the directory from which the "qsub" command was run:
cd $PBS_O_WORKDIR

#  Now run my programs
make clean
module load intel
make

mpirun -np 2 --map-by node ./IMB-MPI1 PingPong -msglog 4
echo 1st Done!

mpirun -np 2 --map-by socket ./IMB-MPI1 PingPong -msglog 4
echo 2nd Done!

mpirun -np 2 --map-by core ./IMB-MPI1 PingPong -msglog 4
echo 3rd Done!

mpirun -np 2 --mca pml ob1 --mca btl tcp,self ./IMB-MPI1 PingPong -msglog 4
echo 4th Done!

mpirun -np 2 --mca pml ob1 --mca btl vader,self ./IMB-MPI1 PingPong -msglog 4
echo 5th Done!

mpirun -np 2 --mca pml ob1 --mca btl tcp,vader,self ./IMB-MPI1 PingPong -msglog 4
echo 6th Done!

exit

