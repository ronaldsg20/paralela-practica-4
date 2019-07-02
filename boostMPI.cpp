#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <string>
#include <cstdlib> 
#include <vector>
namespace mpi = boost::mpi;
//using namespace mpi;
using namespace std;
//using namespace cv;
#include <opencv2/opencv.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>
 
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
int main(int argc, char* argv[]) 
{
  mpi::environment env(argc, argv);
  mpi::communicator world;
  cv::Mat input;
  cv::Mat myMat;
  //cout << "I am process " << world.rank() << " of " << world.size()<< "." << endl;
  vector<cv::Mat> mats;
  
  if(world.rank() == 0){
    input = cv::imread("720-image.jpg");
    //scatter(world,persons,myPerson,0);
    int w = input.cols;
    int h = input.rows/world.size();
    cv::Mat sliced; 
    cout<<"I am  process "<<world.rank()<<" and input at (0,0) = "<<input.at<cv::Vec3b>(cv::Point(0,0))<<endl;
    for(int i =0;i<world.size();i++){
        sliced = input(cv::Rect(0,i*h,w,h));
        mats.push_back(sliced);
        cout<<"I am  process "<<world.rank()<<" and sliced at (0,0) = "<<sliced.at<cv::Vec3b>(cv::Point(0,0))<<endl;
        cout<<"pushed : "<<i<<endl;
       // mats.push_back(input.clone());
    }
  }
  scatter(world,mats,myMat,0);
  cout<<"I am  process "<<world.rank()<<" and my mat at (0,0) = "<<myMat.at<cv::Vec3b>(cv::Point(0,0))<<endl;

  return 0;
}