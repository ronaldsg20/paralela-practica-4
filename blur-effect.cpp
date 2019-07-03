#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <string>
#include <cstdlib> 
#include <vector>
#include <opencv2/opencv.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>
#include <omp.h>

namespace mpi = boost::mpi;
using namespace std;

 
/* FOR SERIALIZATION  OF MAT OPENCV  */
BOOST_SERIALIZATION_SPLIT_FREE(::cv::Mat)
namespace boost {
  namespace serialization {
 
    /** Serialization support for cv::Mat */
    template<class Archive>
    void save(Archive & ar, const ::cv::Mat& m, const unsigned int version)
    {
      size_t elem_size = m.elemSize();
      size_t elem_type = m.type();
 
      ar & m.cols;
      ar & m.rows;
      ar & elem_size;
      ar & elem_type;
 
      const size_t data_size = m.cols * m.rows * elem_size;
      ar & boost::serialization::make_array(m.ptr(), data_size);
    }
 
    /** Serialization support for cv::Mat */
    template<class Archive>
    void load(Archive & ar, ::cv::Mat& m, const unsigned int version)
    {
      int cols, rows;
      size_t elem_size, elem_type;
 
      ar & cols;
      ar & rows;
      ar & elem_size;
      ar & elem_type;
 
      m.create(rows, cols, elem_type);
 
      size_t data_size = m.cols * m.rows * elem_size;
      ar & boost::serialization::make_array(m.ptr(), data_size);
    }
 
  }
}

//Global variables on each process
int KERNEL,THREADS;
int width,height;
cv::Mat myImage,myImageOut;

// aplies the blur effect on the pixel (x,y) and stores the result on the output matrix
void aplyBlur(int x, int y){
    // collect the average data of neighbours 
    int blue,green,red;
    blue=green=red=0;
    int n=0;
    cv::Vec3b pixel;

    for(int i = x - (KERNEL/2); i < x+(KERNEL/2); i++)
    {    
        for (int j = y-(KERNEL/2); j < y+(KERNEL/2); j++)
        {
            //check if the point is in the image limits
            if(0<=i && i<width-1 && 0<=j && j<height-1){
                pixel = myImage.at<cv::Vec3b>(cv::Point(i,j));
                blue += pixel.val[0];
                green += pixel.val[1];
                red += pixel.val[2];
                n++;
            }
        }
    }
    
    if(n!=0){
         //write the average on the output image
        cv::Vec3b pixelBlur = cv::Vec3b(blue/n, green/n, red/n);
        myImageOut.at<cv::Vec3b>(cv::Point(x,y))= pixelBlur;
    }
   
}


int main(int argc, char* argv[]) {
    //Read the arguments
    KERNEL=atoi(argv[3]);
    THREADS = atoi(argv[4]);
    string oFile = argv[2];
    string iFile = argv[1];

    //set the mpi environment
    mpi::environment env(argc, argv);
    mpi::communicator world;

    //local variables
    cv::Mat input;
    cv::Mat output;
    vector<cv::Mat> mats;
    vector<cv::Mat> matsReceived;


    if(world.rank() == 0){
        //slice image and stores in the vector
        input = cv::imread(iFile);
        if ( !input.data )
        {
            printf("No image data \n");
            return -1;
        }
        int w = input.cols;
        int h = input.rows/world.size();
        cv::Mat sliced; 
        for(int i =0;i<world.size();i++){
            sliced = input(cv::Rect(0,i*h,w,h));
            mats.push_back(sliced);
        }
    }
    //scatter on the processes
    scatter(world,mats,myImage,0);
    
    // Do the blur on the slice
    width = myImage.cols;
    height = myImage.rows;
    myImageOut=myImage.clone();
    if ( !myImageOut.data ){
        printf("No image data on node %d \n",world.rank());
        return -1;
    }
    #pragma omp parallel num_threads(THREADS)
    {
        int tn = omp_get_thread_num();
        int ini = (int)(width/THREADS)*(tn);
        int fin = (int)(width/THREADS)+ini;
        //printf("[%d]: thread : %d , Inicio: %d , Fin: %d \n",world.rank(),tn,ini,fin);
        for (int i = ini; i < fin; i++)
        {
            for (int j = 0; j < height; j++)
            {
                aplyBlur(i,j);
            }
        }
        //printf("I'm the thread %d on process %d \n",tn,world.rank());
    }

    //gather the result
    gather(world,myImageOut,matsReceived,0);
    if(world.rank()==0){
        cv::vconcat(matsReceived,output);
        imwrite( oFile, output );
    }
    return 0;
}