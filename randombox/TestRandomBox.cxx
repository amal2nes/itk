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
  myfile.open ("result.csv");

  RandomBox boxes;
  boxes.setParameters(500,500,500, 50, 50, 150);
for(int d = 0; d < 1000; d++)
{
  int ** out = boxes.getRandomBoxes(250,250,250,50);
  
  if(out == NULL)
  {
    std::cout<<"invalid parameters"<<std::endl;
    return -1;
  }

  for(int i = 0; i < 30; i++)
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
