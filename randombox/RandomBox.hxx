#ifndef __RandomBox_hxx
#define __RandomBox_hxx

#include "RandomBox.h"
#include <stdlib.h>
#include <time.h>

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

*int RandomBox::getRandomBoxes(int x, int y, int z, int distance)
{
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
      if(j == 0)
      {
        temp = reference[i] - distance;

        if(temp >= 0)
	  limits[i][j] = temp;
	else
	  limits[i][j] = 0;
	
	temp = reference[i] + distance + boxSizeMax;

	if(temp < dimension[i])
	  limits[i][j] = temp;
	else
	  limits[i][j] = dimension[i] - 1;
      }
    }
  }

  for(int i = 0; i < numBox; i++) // make boxes
  {
    for(int j = 0; j < 3; j++)	// for each dimension
    {
      //seed random generator
      srand (time(NULL));

      //generate number within limits
      int position = (rand() % (limits[j][1] - limits[j][0]))/2 + reference[j];
      outBoxes[i][j] = position;

      //seed random generator
      srand (time(NULL));

      int dim = rand() % (sizeMax - sizeMin) + sizeMin;
      outBoxes[i][j+3] = dim;
    }
  }
  
  return outBoxes;

}

#endif
