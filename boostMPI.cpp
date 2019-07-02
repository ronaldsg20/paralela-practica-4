#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <iostream>
namespace mpi = boost::mpi;
using namespace mpi;
using namespace std;

class Person{
    public:
    String name;
    void printName(int rank){
        cout<<"my name is "<<name<<" on process "<<rank<<endl;
    }
};

int main(int argc, char* argv[]) 
{
  environment env(argc, argv);
  communicator world;
  Person myPerson;
  //cout << "I am process " << world.rank() << " of " << world.size()<< "." << endl;
  vector<Person> persons;
  if(world.rank() == 0){
    Person p1;
    Person p2;
    Person p3;
    Person p4;
    Person p5;
    p1.name = "Ronald";
    p2.name = "Alexander";
    p3.name = "Sarmiento";
    p4.name = "Galviz";
    p5.name = "Fin";
    persons.push_back(p1);
    persons.push_back(p2);
    persons.push_back(p3);
    persons.push_back(p4);
    persons.push_back(p5);

    scatter(world,persons,myPerson,0);

  }else{
    scatter(world,persons,myPerson,0);
    myPerson.printName(world.rank());
  }
  
  return 0;
}