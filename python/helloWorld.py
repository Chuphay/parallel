#http://kermit.epska.org/2010/ubuntu-cluster-setup-for-parallel-programming-using-mpi-in-python/

from mpi4py import MPI

comm = MPI.COMM_WORLD

print("Hello! I'm rank {0} of {1}".format(comm.rank, comm.size) )
