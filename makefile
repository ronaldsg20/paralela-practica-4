all:
	mpic++ -lboost_mpi -lboost_serialization boostMPI.cpp `pkg-config opencv --cflags --libs opencv` -lboost_mpi -lboost_serialization -o boost
