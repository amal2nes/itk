/**
 *
 * Test bench for the created filter(s)
 * 
 *=========================================================================*/

#include "RandomBox.h"
#include <iostream>
#include <fstream>

int main()
{

  std::ofstream myfile;
  myfile.open ("result_8.csv");

  RandomBox boxes;

  int imageDim[3] = {500, 500, 500};

  int boxSizeMin[3] = {10, 10, 10};

  int boxSizeMax[3] = {100, 100, 100};

  boxes.setParameters(imageDim,boxSizeMin,boxSizeMax);

  int targetCoord[3] = {200, 200, 200};
  int distance = 100;
  int numBox = 10000;

  for(int d = 0; d < 1; d++)
  {
    
    int ** out = boxes.getRandomBoxes(targetCoord, distance, numBox);
  
    if(out == NULL)
    {
      std::cout<<"invalid parameters"<<std::endl;
      return -1;
    }

    for(int i = 0; i < numBox; i++)
    {

      std::cout<<i<<": ";
    
      for(int j = 0; j < 6; j++)
      {

        std::cout<<out[i][j];
	if(j != 5)
	  std::cout<<",";
	else
	  std::cout<<std::endl;

	//write to file
	myfile << out[i][j];
      
	if(j != 5)
	  myfile << ",";
	else
	  myfile <<"\n";
      }
    }
  }  

  myfile.close();

  return 0;
}
