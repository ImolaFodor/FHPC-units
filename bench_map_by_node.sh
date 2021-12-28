# module load openmpi-4.1.1+gnu-9.3.0
module load intel
#make clean
#make all
# mpirun -np 2 --map-by node --report-bindings ./IMB-MPI1 PingPong >> outputs_map_by_node.txt
mpirun -np 2 --map-by node ./IMB-MPI1 PingPong -msglog 4 >> outputs_map_by_node.txt
