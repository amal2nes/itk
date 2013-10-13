#ifndef __RandomBox_hxx
#define __RandomBox_hxx

#include "RandomBox.h"
#include <stdlib.h>

#include <random>
#include <chrono>
#include <iostream>

// Initialize this module
RandomBox::RandomBox()	      
{

}

// Generate box data given input location, distance from input location, number of boxes 
int * RandomBox::getRandomBoxes(int minL[], int maxL[], int distance[], int numBox)
{

  int* outBoxes = new int[numBox*6]; // create array of boxes with data x,y,z,lx,ly,lz
  
  //random number generator
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
     
  std::default_random_engine generator(seed);

  for(int i = 0; i < numBox; i++)    // For each random box
  {
    for(int j = 0; j < 3; j++)       
    {
      std::uniform_real_distribution<double> distributionDistance(-1*distance[j], distance[j]); // box distance distribution
      
      int position = (int)(distributionDistance(generator) + 0.5); // generate box offset
  
      outBoxes[i*6 + j] = position; // save box offset

      std::uniform_real_distribution<double> distributionLength(minL[j], maxL[j]); // box size distribution

      int size = (int)(distributionLength(generator) + 0.5); // generate box size

      outBoxes[i*6 + j + 3] = size;	// save box size
    }
  }
  
  return outBoxes;
}

int* RandomBox::getRandomBoxIntegral(int dim[], const int* targetCoord, int numTargetCoord, const int* randomBoxes, int numRandomBoxes)
{
  //temporary containers
  int coord[3];
  int randomBoxData[6];

  int* output = new int[numTargetCoord * numRandomBoxes]; // output initialization

  for(int i = 0; i < numTargetCoord; i++) // for each input coordinate
  {
    for(int j = 0; j < 3; j++)
    {
      coord[j] = targetCoord[i*3 + j]; // get the coordinate
    }

    for(int k = 0; k < numRandomBoxes; k++) // for each random box
    {
      for(int m = 0; m < 6; m++)
      {
        randomBoxData[m] = randomBoxes[k*6 + m]; // get random box data
      }
      
      //check if position is out of bounds
      bool outOfBounds = false;
      for(int x = 0; x < 3; x++)
      {
        int pos = coord[x] + randomBoxData[x];
	int posPlusLength = pos + randomBoxData[x+3];
        if(pos >= dim[x] || pos < 0 || posPlusLength >= dim[x])
	{
  	  outOfBounds = true;
	  break;
	}
      }

      // compute integral
      if(!outOfBounds)
      {
        output[i*k + k] = 1;
      }
      else
      {
        output[i*k + k] = 99999;
      }
      
      // std::cout<<"input coord: "<<coord[0]<<","<<coord[1]<<","<<coord[2]<<" box data: ";
      
      // for(int n = 0; n < 5; n++)
      // {
      // std::cout<<randomBoxData[n]<<",";
      // }
      // std::cout<<randomBoxData[5]<<std::endl;

    }
  }
  return output;
}

#endif
