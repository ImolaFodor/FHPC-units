#!/bin/bash


# Run for 15 mins:
#PBS -l walltime=00:15:00
#PBS -l nodes=2:ppn=2
#PBS -q dssc_gpu

# Where to write stderr:
#PBS -e output_header_intel_gpu.err

# Where to write stdout:
#PBS -o outputs_intel_gpu.txt

# This command switched to the directory from which the "qsub" command was run:
cd $PBS_O_WORKDIR

#  Now run my programs
make clean
module load intel
make

#mpirun -np 2 --oversubscribe --map-by node./IMB-MPI1 PingPong -msglog 4
echo Map by node on ib done!

mpirun -np 2 --map-by socket ./IMB-MPI1 PingPong -msglog 4
echo Map by socket on ib done!

mpirun -np 2 --map-by core ./IMB-MPI1 PingPong -msglog 4
echo Map by core on ib one!

#mpirun -np 2 --oversubscribe -env I_MPI_FABRICS tcp --map-by node ./IMB-MPI1 PingPong -msglog 4
echo Map by node on tcp done!

mpirun -np 2 -env I_MPI_FABRICS tcp --map-by socket ./IMB-MPI1 PingPong -msglog 4
echo Map by socket on tcp done!

exit

