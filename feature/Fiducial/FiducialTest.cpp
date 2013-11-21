
#include <iostream>

#include <fstream>
#include <string>
#include <map>
#include <utility>

#include "fiducial.h"
#include "FeatureExtractRandomBox.h"

using namespace std;
using namespace FeatureExtractRandomBox;

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

     infile >> newMarker.id;
     
     if(infile.good())
     {
       infile >> newMarker.name >> newMarker.value[0] >> newMarker.value[1] >> newMarker.value[2];
       markers[newMarker.id] = newMarker;
     }
  }//end of ifstream

  map<int,Fiducial>::iterator it = markers.begin();
  cout << "map contains:\n";
  for (it=markers.begin(); it!=markers.end(); ++it)
    cout << it->first << " => " << (it->second).name <<", " << (it->second).value[0] <<", "<< (it->second).value[1] <<", "<< (it->second).value[2] << endl;


  int * target = new int[3*5];
  for(int i = 0; i < 5; i++)
  {
    for(int j = 0; j < 3; j++)
    {
      target[i*3 + j] = j + 10;
    }
  }
  int * trainInput = new int[3*3];
  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < 3; j++)
    {
      trainInput[i*3 + j] = i;
    }
  }
  int * response;
  int numTrain = 3;
  int numTarget = 5;
  getTrainingData(target, numTarget, trainInput, numTrain, response);
  cout<<"response:"<<endl;
  for(int i = 0; i < numTrain * numTarget; i++)
  {
    cout<<i<<": ";
    for(int j = 0; j < 3; j++)
    {
      cout<<response[i*3 + j]<<" ";
    }
    cout<<endl;
  }

  return 0;
}
