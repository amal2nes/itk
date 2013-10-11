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
  for(int i = 0; i < 3; i++)
  {
    dimension[i] = 0;
    boxSizeMin[i] = 0;
    boxSizeMax[i] = 0;
  }
}

// Set the total dimension, box size ranges in x, y, z
bool RandomBox::setParameters(int dim[], int sizeMin[], int sizeMax[])
{
  for(int i = 0; i < 3; i++)
  {
    if(dim[i] == 0 || sizeMin[i] == 0 || sizeMax[i] == 0)
    {
      return false;
    }
    dimension[i] = dim[i];
    boxSizeMin[i] = sizeMin[i];
    boxSizeMax[i] = sizeMax[i];
  }
  return true;
}

// Generate box data given input location, distance from input location, number of boxes 
int ** RandomBox::getRandomBoxes(int targetCoord[], int distance, int numBox)
{
  // check input target coordinate if it exceeds dimension limits
  for(int j = 0; j < 3; j++)
  {
    if(targetCoord[j] >= dimension[j] || targetCoord[j] < 0 || targetCoord[j] + boxSizeMin[j] >= dimension[j])
    {
      return NULL;
    }
  }

  int** outBoxes = new int*[numBox]; // create boxes with index: [NumberOfBoxes][boxData(x,y,z,dx,dy,dz)]
  for(int i = 0; i < numBox; i++)
  {
    outBoxes[i] = new int[6];
  }
  
  //random number generator
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
     
  std::default_random_engine generator(seed);

  for(int i = 0; i < numBox; i++)    // For each random box
  {
    for(int j = 0; j < 3; j++)       
    {
      
      std::uniform_real_distribution<double> distribution2(boxSizeMin[j], boxSizeMax[j]); // box size distribution
      int size;

      size = (int)(distribution2(generator) + 0.5); // generate box size

      outBoxes[i][j+3] = size;	// save box size

      // limits of box positions based on image dimension and box size
      int limitLow = (targetCoord[j] - distance >= 0)? targetCoord[j] - distance : 0;
      int limitHigh = (targetCoord[j] + distance + (size-1) < dimension[j])? targetCoord[j] + distance : dimension[j] - size;
      
      std::uniform_real_distribution<double> distribution(limitLow, limitHigh); // box position distribution

      int position;
      
      position = (int)(distribution(generator) + 0.5); // generate box position
  
      outBoxes[i][j] = position; // save box position

    }
  }
  
  return outBoxes;
}

#endif
