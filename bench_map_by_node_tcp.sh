module load openmpi-4.1.1+gnu-9.3.0
#module load intel
make clean
make all

mpirun -np 2 --mca pml ob1 --report-bindings --oversubscribe --map-by node --mca btl tcp,self ./IMB-MPI1 PingPong >> outputs_map_by_node_tcp.txt
#mpirun -np 2 -env I_MPI_FABRICS tcp --map-by node ./IMB-MPI1 PingPong -msglog 4 >> outputs_map_by_node_tcp.txt
echo The above run is on gpu, openmpi, tcp by node >> outputs_map_node_tcp.txt

