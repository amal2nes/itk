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
//  srand(time(NULL));

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

//calculates average integral based on target coordinates and random
//boxes
// generate an array of integral corresponding to target coordinates
// and random boxes, returns continuous 1D array of rows of m x n input data
// for CT: m = number of target coordinates,n = number of random boxes
// for MRI: m = number of target coordinates,n = (number of random boxes)/2
// targetCoord = array of input coordinates (x,y,z)'s
// numTargetCoord = number of coordinate sets
// randomBoxes = array of random boxes (ox,oy,oz,lx,ly,lz)'s
// numRandomBoxes = number of random boxes
// data = pointer to image data having template dataType
// dim = size of image (SizeX,SizeY,SizeZ)
// isMRI = true if MRI, false if CT
template< class dataType>
double* getRandomBoxIntegral(const int* targetCoord, const int numTargetCoord, const int* randomBoxes, const int numRandomBoxes, const dataType * data, const int* dim, bool isMRI)
{
  //temporary containers
  int coord[3];
  int randomBoxData[6];

  // number of random boxes for MRI halves
  int finalNumRandomBoxes = (isMRI? numRandomBoxes/2: numRandomBoxes);

  double* output = new double[numTargetCoord * finalNumRandomBoxes]; // output initialization

  double* avgIntegral = new double[numRandomBoxes]; // intermediate data
  
  bool* outOfBounds = new bool[numRandomBoxes]; // intermediate data

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
     
      // reset current bounds
      outOfBounds[k] = false;      
      
      //check if position is out of bounds
      for(int x = 0; x < 3; x++)
      {
        int pos = coord[x] + randomBoxData[x];
	int posPlusLength = pos + randomBoxData[x+3];
        if(pos >= dim[x] || pos < 0 || posPlusLength >= dim[x])
	{
  	  outOfBounds[k] = true;
	  break;
	} 
      }
      
      // start of avg integral calculation
      if(outOfBounds[k] == false)
      {  
        //stores linear index of 8 vertices of the box
	int indexes[8];
	
	//stores integral of the box
	double integrals[8];
	
	//stores (x,y,z) coordinates of a vertice of a box
	int currentBox[3];

	// for each vertice
	for(int z = 0; z < 8; z++)
	{
	  //get origin of box from target coordinate + box offset
	  for(int g = 0; g < 3; g++)
	  {
  	    currentBox[g] =  coord[g] + randomBoxData[g];
	  }
	  
	  //add length offset(s) to 8 vertices of the box
	  switch(z)
	  {
	  case 1:
	    currentBox[0] += randomBoxData[3];
	    break;
	  case 2:
	    currentBox[1] += randomBoxData[4];
	    break;
	  case 3:
	    currentBox[0] += randomBoxData[3];
	    currentBox[1] += randomBoxData[4];
	    break;
	  case 4:
	    currentBox[2] += randomBoxData[5];
	    break;
	  case 5:
	    currentBox[0] += randomBoxData[3];
	    currentBox[2] += randomBoxData[5];
	    break;
	  case 6:
	    currentBox[1] += randomBoxData[4];
	    currentBox[2] += randomBoxData[5];
	    break;
	  case 7:
	    for(int y = 0; y < 3; y++)
	      currentBox[y] += randomBoxData[y+3];
	    break;
	  default:
	    break;
	  }

	  // convert x,y,z index to linear index 
  	  indexes[z] = getIndexFromXYZ(dim,currentBox);
	  
	  //get value from integral image
	  integrals[z] = (double)data[indexes[z]];
	}

	//calculate current avg integral
	avgIntegral[k] = (-integrals[0] + integrals[1] + integrals[2] - integrals[3] + integrals[4] - integrals[5] - integrals[6] + integrals[7]) / (randomBoxData[3]*randomBoxData[4]*randomBoxData[5]);

      }// end of box integral 
    }//end of random box loop

    //make half of results to be subtracted for MRI
    double* subtract = avgIntegral + finalNumRandomBoxes;

    for(int s = 0; s < finalNumRandomBoxes; s++)
    {
      //perform subtraction on 2 boxes for MRI
      if(isMRI)
      {
        if(!outOfBounds[s] && !outOfBounds[s + finalNumRandomBoxes])
	{
	  output[i*finalNumRandomBoxes + s] = avgIntegral[s] - subtract[s];
	}
	else
	{
	  // out of bounds
	  output[i*finalNumRandomBoxes + s] = 1e20;
	}
      }
      else
      {
        if(!outOfBounds[s])
	{ 
	  output[i*finalNumRandomBoxes + s] = avgIntegral[s];
	}
	else
	{
          // out of bounds
	  output[i*finalNumRandomBoxes + s] = 1e20;
	}
      }
    }
  }// end of input coordinate loop
  
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
