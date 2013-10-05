/**
 *
 * Test bench for the created filter(s)
 * 
 *=========================================================================*/

#include "RandomBox.h"
#include <iostream>

int main()
{

  RandomBox boxes;
  boxes.setParameters(500,250,100, 30, 3, 40);
  int ** out = boxes.getRandomBoxes(200,200,85,20);
  
  for(int i = 0; i < 30; i++)
  {

    std::cout<<i<<": ";
    
    for(int j = 0; j < 6; j++)
    {

      std::cout<<j;
      if(j != 5)
	std::cout<<",";
    }
  }
  
  return 0;
}
