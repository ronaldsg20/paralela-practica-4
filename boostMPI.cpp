#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <iostream>
namespace mpi = boost::mpi;
using namespace mpi;
using namespace std;

int main(int argc, char* argv[]) 
{
  environment env(argc, argv);
  communicator world;
  std::cout << "I am process " << world.rank() << " of " << world.size()<< "." << std::endl;
  return 0;
}