#ifndef __FeatureExtractRandomBox_hxx
#define __FeatureExtractRandomBox_hxx

#include "FeatureExtractRandomBox.h"
#include <stdlib.h>

#include <iostream>

#include <time.h>    
#include <math.h>   

namespace FeatureExtractRandomBox{

// Generate box data from min and max box dimensions (lx,ly,lz),
// offset distance limit (ox,oy,oz), number of boxes 
int * getRandomBoxes(int minL[], int maxL[], int distance[], int numBox)
{

  int* outBoxes = new int[numBox*6]; // create array of boxes with data ox,oy,oz,lx,ly,lz
  
  //seed random generator
  //srand(time(NULL));

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

//convert from actual length to pixel length using image spacing factor
int* convertToPixelLength(const double * actual, const double * factor)
{
  int * pixelLength = new int[3];

  for(int i = 0; i < 3; i++)
  {
    pixelLength[i] = ceil(actual[i]/factor[i]);
  }

  return pixelLength;
}

//convert from pixel length to actual length using image spacing factor
double* convertToActualLength(const int * pixelLength, const double * factor)
{
  double * actualLength = new double[3];

  for(int i = 0; i < 3; i++)
  {
    actualLength[i] = pixelLength[i] * factor[i];
  }

  return actualLength;
}

//subtract origin offset from input actual length
double* subtractOrigin(const double * input, const double * origin)
{
  double * output = new double[3];

  for(int i = 0; i < 3; i++)
  {
    output[i] = input[i] - origin[i];
  }

  return output;
}

//add origin offset to input actual length
double* addOrigin(const double * input, const double * origin)
{
  double * output = new double[3];

  for(int i = 0; i < 3; i++)
  {
    output[i] = input[i] + origin[i];
  }

  return output;
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
// output = output array
// return true if successful
template< class dataType>
bool getRandomBoxIntegral(const int* targetCoord, const int numTargetCoord, const int* randomBoxes, const int numRandomBoxes, const dataType * data, const int* dim, bool isMRI, double *& output)
{

  //check targetCoord with image size
  // for each input coordinate
  for(int i = 0; i < numTargetCoord; i++) 
  {
    for(int j = 0; j < 3; j++)
    {
      if(targetCoord[i*3 + j] < 0 || targetCoord[i*3 + j] >= dim[j])
	return false;
    }
  }
  
  //temporary containers
  int coord[3];
  int randomBoxData[6];

  // number of random boxes for MRI halves
  int finalNumRandomBoxes = (isMRI? numRandomBoxes/2: numRandomBoxes);

  output = new double[numTargetCoord * finalNumRandomBoxes]; // output initialization

  double* avgIntegral = new double[numRandomBoxes]; // intermediate data

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
      for(int x = 0; x < 3; x++)
      {
        //absolute position of random box
        int pos = coord[x] + randomBoxData[x];

	// if box is out of bounds, make it nearest to image boundary
        if(pos >= dim[x])
	{
  	  randomBoxData[x] += dim[x] - pos;
	  pos = dim[x] - 1;
	}
	else if(pos < 0)
	{
  	  randomBoxData[x] += 0 - pos;
	  pos = 0;
	}
       
	int posPlusLength = pos + randomBoxData[x+3];

	if(posPlusLength >= dim[x])
	{
  	  randomBoxData[x+3] += dim[x] - posPlusLength;
	}
      }
      
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
	indexes[z] = FeatureExtractRandomBox::getIndexFromXYZ(dim,currentBox);
	
	//get value from integral image
	integrals[z] = (double)data[indexes[z]];
      }

      //calculate current avg integral
      avgIntegral[k] = (-integrals[0] + integrals[1] + integrals[2] - integrals[3] + integrals[4] - integrals[5] - integrals[6] + integrals[7]) / (randomBoxData[3]*randomBoxData[4]*randomBoxData[5]);
      
    }//end of random box loop

    //make half of results to be subtracted for MRI
    double* subtract = avgIntegral + finalNumRandomBoxes;
    
    for(int s = 0; s < finalNumRandomBoxes; s++)
    {
      //perform subtraction on 2 boxes for MRI
      if(isMRI)
      {
        output[i*finalNumRandomBoxes + s] = avgIntegral[s] - subtract[s];
      }
      else
      {
        output[i*finalNumRandomBoxes + s] = avgIntegral[s];
      }
    }
  }// end of input coordinate loop
  
  return true;
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

//returns 1D array of dx,dy,dz tuples where diff = fixed training input -
//target coordinate, result ordered by target coordinate, training points
bool getTrainingData(const int* targetCoord, const int numTargetCoord, const int* trainInput, const int numTrain, int* & trainOutput)
{
  trainOutput = new int[numTargetCoord * numTrain * 3];
  
  for(int i = 0; i < numTargetCoord; i++)
  {
    for(int j = 0; j < numTrain; j++)
    {
      for(int k = 0; k < 3; k++)
      {
        trainOutput[i*numTrain*3 + j*3 + k] = trainInput[j*3 + k] - targetCoord[i*3 + k];
      }
    }
  }
  return true;
}

}//end namespace

#endif
