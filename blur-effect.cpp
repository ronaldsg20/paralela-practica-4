# include <cstdlib>
# include <iomanip>
# include <iostream>
# include <mpi.h>
# include <omp.h>
# include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main ( int argc, char *argv[] ){
    int count;
    int ierr;
    int num_procs;
    int iam;
    MPI_Status status;
    int tag;
    if ( argc != 5 )
        {
            printf("usage: ./blur-effect <Image_Path> <Image_out_Path> <THREADS> <KERNEL>\n");
            return -1;
        }
    //store the arguments
    string imgSrc = argv[1];
    string imgDest = argv[2];
    int threads = atoi(argv[3]);
    int kernel = atoi(argv[4]);


//
//  Initialize MPI.
//
    ierr = MPI_Init ( &argc, &argv );
    if ( ierr != 0 )
    {
        cout << "\n";
        cout << "Blur - Fatal error!\n";
        cout << "  MPI_Init returned ierr = " << ierr << "\n";
        exit ( 1 );
    }
    //
    //  Determine this process's rank.
    //
    ierr = MPI_Comm_rank ( MPI_COMM_WORLD, &iam );
    //
    //  Determine the number of available processes.
    //
    ierr = MPI_Comm_size ( MPI_COMM_WORLD, &num_procs );

// Read the image with openCV

// transform the matrix into an array 

// find the limits of process's computation

// Create an array with the propper size

// split the array and launch the number of threads specified

    #pragma omp parallel num_threads(threads)
        {
            int tn = omp_get_thread_num();
            //int ini = (int)(width/THREADS)*(tn-1);
            //int fin = (int)(width/THREADS)+ini;
            //for (int i = ini; i < fin; i++)
           /*  {
                for (int j = 0; j < height; j++)
                {
                    aplyBlur(i,j);
                }
            } */
        } 

// send to process 0 all the data

//  Terminate MPI.
    MPI_Finalize ( );
//
//  Terminate.

  return 0;
}
