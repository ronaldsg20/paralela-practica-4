all:
	mpic++ -lboost_mpi -lboost_serialization blur-effect.cpp `pkg-config opencv --cflags --libs opencv` -lboost_mpi -lboost_serialization -fopenmp -o blur-effect
