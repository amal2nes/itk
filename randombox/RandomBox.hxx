#ifndef __RandomBox_hxx
#define __RandomBox_hxx

#include "RandomBox.h"
#include <stdlib.h>

#include <iostream>

#include <time.h>       

// Generate box data from min and max box dimensions (lx,ly,lz), offset (ox,oy,oz), number of boxes 
int * getRandomBoxes(int minL[], int maxL[], int distance[], int numBox)
{

  int* outBoxes = new int[numBox*6]; // create array of boxes with data ox,oy,oz,lx,ly,lz
  
  //seed random generator
  srand(time(NULL));

  for(int i = 0; i < numBox; i++) 
  {
    for(int j = 0; j < 3; j++)       
    {  
      // offset generation
      int position = (int)(rand()%(2*distance[j] + 1) -1*distance[j]);

      outBoxes[i*6 + j] = position; // save box offset
 
      // size generation
      int size = (int)((rand()%(maxL[j] - minL[j] + 1)) + minL[j]);

      outBoxes[i*6 + j + 3] = size;	// save box size
    }
  }
  
  return outBoxes;
}

// generate an array of row-based m x n matrix, m = number of target coordinates,n = number of random boxes
// dim = size of image (SizeX,SizeY,SizeZ)
// targetCoord = array of input coordinates (x,y,z)'s
// numTargetCoord = number of coordinate sets
// randomBoxes = array of random boxes (ox,oy,oz,lx,ly,lz)'s
// numRandomBoxes = number of random boxes
// data = pointer to image data
template< class dataType>
double* getRandomBoxIntegral(const int* dim, const int* targetCoord, const int numTargetCoord, const int* randomBoxes, const int numRandomBoxes, const dataType data)
{
  //temporary containers
  int coord[3];
  int randomBoxData[6];

  double* output = new double[numTargetCoord * numRandomBoxes]; // output initialization

  // for each input coordinate
  for(int i = 0; i < numTargetCoord; i++) 
  {
    // get current target coordinate (x,y,z)
    for(int j = 0; j < 3; j++)
    {
      coord[j] = targetCoord[i*3 + j]; 
    }

    // for each random box
    for(int k = 0; k < numRandomBoxes; k++) 
    {
      // get current random box data (ox,oy,oz,lx,ly,lz)
      for(int m = 0; m < 6; m++)
      {
        randomBoxData[m] = randomBoxes[k*6 + m]; 
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
      
      // compute integral if not out of bounds
      if(!outOfBounds)
      {
        int box[8][3]; //stores 8 vertice coordinates of the box
	
	// set coordinates of 8 vertices of the current box
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
	double integrals[8];
	int currentBox[3];
	
	for(int z = 0; z < 8; z++)
	{
	  for(int g = 0; g < 3; g++)
	  {
	    currentBox[g] = box[z][g];
	  }
	  // convert image index from x,y,z 
  	  indexes[z] = getIndexFromXYZ(dim,currentBox);
	  integrals[z] = (double)data[indexes[z]];
	}
	//compute average integral of the current box
        output[i*numRandomBoxes + k] = (-integrals[0] + integrals[1] + integrals[2] - integrals[3] + integrals[4] - integrals[5] - integrals[6] + integrals[7]) / (randomBoxData[3]*randomBoxData[4]*randomBoxData[5]);
      }// end of box integral 

      // if out of bounds
      else		       
      {
        // set some value
        output[i*numRandomBoxes + k] = 1e16;
      }
    }
  }

  return output;
}

// convert input x,y,z coordinates (0 indexed) to a linear index (0 indexed)
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
