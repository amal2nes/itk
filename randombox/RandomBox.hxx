#ifndef __RandomBox_hxx
#define __RandomBox_hxx

#include "RandomBox.h"
#include <stdlib.h>

//#include <random>
//#include <chrono>
#include <iostream>

#include <time.h>       

// Generate box data given input location, distance from input location, number of boxes 
int * getRandomBoxes(int minL[], int maxL[], int distance[], int numBox)
{

  int* outBoxes = new int[numBox*6]; // create array of boxes with data x,y,z,lx,ly,lz
  
  // //random number generator
  // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
     
  // std::default_random_engine generator(seed);
  
  srand(time(NULL));

  for(int i = 0; i < numBox; i++)    // For each random box
  {
    for(int j = 0; j < 3; j++)       
    {
      // std::uniform_real_distribution<double> distributionDistance(-1*distance[j], distance[j]); // box distance distribution
      
      // int position = (int)(distributionDistance(generator) + 0.5); // generate box offset
  
      int position = (int)(rand()%(2*distance[j] + 1) -1*distance[j]);

      outBoxes[i*6 + j] = position; // save box offset

      // std::uniform_real_distribution<double> distributionLength(minL[j], maxL[j]); // box size distribution

      // int size = (int)(distributionLength(generator) + 0.5); // generate box size
      
      int size = (int)((rand()%(maxL[j] - minL[j] + 1)) + minL[j]);

      outBoxes[i*6 + j + 3] = size;	// save box size
    }
  }
  
  return outBoxes;
}


template< class dataType>
int* getRandomBoxIntegral(int* dim, const int* targetCoord, int numTargetCoord, const int* randomBoxes, int numRandomBoxes, dataType data)
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
        int box[8][3] = {}; //stores 8 vertice coordinates of the box
	box[0][0] = coord[0] + randomBoxData[0];
	box[0][1] = coord[1] + randomBoxData[1];
	box[0][2] = coord[2] + randomBoxData[2];

	box[1][0] = coord[0] + randomBoxData[0] + randomBoxData[3];
	box[1][1] = coord[1] + randomBoxData[1];
	box[1][2] = coord[2] + randomBoxData[2];

	box[2][0] = coord[0] + randomBoxData[0];
	box[2][1] = coord[1] + randomBoxData[1] + randomBoxData[4];
	box[2][2] = coord[2] + randomBoxData[2];

	box[3][0] = coord[0] + randomBoxData[0] + randomBoxData[3];
	box[3][1] = coord[1] + randomBoxData[1] + randomBoxData[4];
	box[3][2] = coord[2] + randomBoxData[2];

	box[4][0] = coord[0] + randomBoxData[0];
	box[4][1] = coord[1] + randomBoxData[1];
	box[4][2] = coord[2] + randomBoxData[2] + randomBoxData[5];

	box[5][0] = coord[0] + randomBoxData[0] + randomBoxData[3];
	box[5][1] = coord[1] + randomBoxData[1];
	box[5][2] = coord[2] + randomBoxData[2] + randomBoxData[5];

	box[6][0] = coord[0] + randomBoxData[0];
	box[6][1] = coord[1] + randomBoxData[1] + randomBoxData[4];
	box[6][2] = coord[2] + randomBoxData[2] + randomBoxData[5];

	box[7][0] = coord[0] + randomBoxData[0] + randomBoxData[3];
	box[7][1] = coord[1] + randomBoxData[1] + randomBoxData[4];
	box[7][2] = coord[2] + randomBoxData[2] + randomBoxData[5];

	int indexes[8];
	int integrals[8];
	
	int currentBox[3];
	
	for(int z = 0; z < 8; z++)
	{
	  for(int g = 0; g < 3; g++)
	  {
	    currentBox[g] = box[z][g];
	  }
  	  indexes[z] = getIndexFromXYZ(dim,currentBox);
	  integrals[z] = data[indexes[z]];
	}
	
        output[i*k + k] = integrals[0] - integrals[1] - integrals[2] + integrals[3] - integrals[4] + integrals[5] + integrals[6] - integrals[7];
      }
      else
      {
        output[i*k + k] = 999999;
      }
    }
  }

  return output;
}

int getIndexFromXYZ(const int* dim, const int* boxVertice)
{
  int index;
  
  int sizeSlice = dim[0]*dim[1];
  int sizeRow = dim[0];
  
  index = boxVertice[2]*sizeSlice;
  index += boxVertice[1]*sizeRow;
  index += boxVertice[0];

#ifdef DEBUG
  std::cout<<"x,y,z: "<<boxVertice[0]<<","<<boxVertice[1]<<","<<boxVertice[2]<<". index: "<<index<<std::endl;
#endif

  return index;
}

#endif
