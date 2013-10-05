#ifndef __RandomBox_hxx
#define __RandomBox_hxx

#include "RandomBox.h"
#include <stdlib.h>

#include <random>
#include <chrono>

RandomBox::RandomBox()
{
  for(int i = 0; i < 3; i++)
    dimension[i] = 0;

  numBox = 1000;
}

void RandomBox::setParameters(int w, int h, int d, int numBox, int sizeMin, int sizeMax)
{
  dimension[0] = w;
  dimension[1] = h;
  dimension[2] = d;

  numBox = numBox;
  boxSizeMin = sizeMin;
  boxSizeMax = sizeMax;
}

int ** RandomBox::getRandomBoxes(int x, int y, int z, int distance)
{
  if(x >= dimension[0] || y >= dimension[1] || z >= dimension[2])
    return NULL;

  int** outBoxes = new int*[numBox]; // [y][x]
  for(int i = 0; i < numBox; i++)
  {
    outBoxes[i] = new int[6];
  }

  int limits[3][2];		// [x,y,z][lower,upper]

  int reference[3];		// target location
  reference[0] = x;
  reference[1] = y;
  reference[2] = z;

  int temp;

  for(int i = 0; i < 3; i++)	// dimensions
  {
    for(int j = 0; j < 2; j++)	// lower, upper
    {
      temp = reference[i] - distance;

      if(temp >= 0)
        limits[i][0] = temp;
      else
        limits[i][0] = 0;
	
      temp = reference[i] + distance + boxSizeMax;

      if(temp < dimension[i])
        limits[i][1] = temp;
      else
        limits[i][1] = dimension[i] - 1;
     
    }
  }

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
     
  std::default_random_engine generator(seed);

  for(int i = 0; i < numBox; i++) // make boxes
  {
    for(int j = 0; j < 3; j++)	// for each dimension
    {

      std::uniform_real_distribution<double> distribution(limits[j][0], limits[j][1]);

      distribution.reset();

      //generate number within limits
      int position = (int)(distribution(generator) + 0.5);
      outBoxes[i][j] = position;

      std::uniform_real_distribution<double> distribution2(boxSizeMin, boxSizeMax);

      distribution2.reset();

      int dim = (int)(distribution2(generator) + 0.5);
      outBoxes[i][j+3] = dim;
    }
  }
  
  return outBoxes;

}

#endif
