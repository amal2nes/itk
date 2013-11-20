
#include <iostream>

#include <fstream>
#include <string>
#include <map>
#include <utility>

#include "fiducial.h"

using namespace std;

int main(int argc, char **argv)
{

  if(argc < 2)
  {
    cout<<"need: <file path>"<<endl;
    return -1;
  }

  //set input file path
  ifstream infile(argv[1]);
  
  map<int, Fiducial> markers;

  int line = 0;
  string dummyLine;
	
  while(infile.good())
  {
     
     if(line == 0)
     {
       getline(infile, dummyLine); 	   
     }
 
     Fiducial newMarker;
     infile >> newMarker.id >> newMarker.name >> newMarker.value[0] >> newMarker.value[1] >> newMarker.value[2];
     
     markers[newMarker.id] = newMarker;

  }//end of ifstream


  map<int,Fiducial>::iterator it = markers.begin();
  cout << "map contains:\n";
  for (it=markers.begin(); it!=markers.end(); ++it)
    cout << it->first << " => " << (it->second).name <<", " << (it->second).value[0] <<", "<< (it->second).value[1] <<", "<< (it->second).value[2] << endl;

  return 0;
}
