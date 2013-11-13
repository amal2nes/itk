/**
 *
 * Test bench for the created filter(s)
 * Logs time for computing avg integral of boxes with random input data
 *=========================================================================*/

#include "FeatureExtractRandomBox.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>

#include "itkImage.h"
#include "itkImageFileReader.h"

using namespace FeatureExtractRandomBox;

//number of random boxes to test with
#define BOXNUMMAX 10000

//number of target pixels to test with
#define TARGETCOORDNUMMAX 10000

//#define DEBUG

//number of tests
#define TESTNUM 20

int main(int argc, char **argv)
{
  srand(time(NULL));
     
  if(argc < 3)
  {
    std::cout<<"need input file path and output folder path"<<std::endl;
    return -1;
  }

  typedef itk::Image< char, 3 > ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
 
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(argv[1]);
  reader->Update();

  //get size of image
  typename ImageType::SizeType inputSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();

  //dimension of input image
  int dim[3];
  
  for(int y = 0; y < 3; y++)
  {
    dim[y] = inputSize[y];
  }  

  //debug file to save data
  std::ofstream fileRandomBoxes, fileIntegral[2], fileTiming;
  fileRandomBoxes.open((std::string(argv[2]) + std::string("/randomBoxes.csv")).c_str(), std::ofstream::app);
  fileIntegral[0].open((std::string(argv[2]) + std::string("/integralBoxes_MRI.csv")).c_str(), std::ofstream::app);
  fileIntegral[1].open((std::string(argv[2]) + std::string("/integralBoxes_NoMRI.csv")).c_str(), std::ofstream::app);
  fileTiming.open((std::string(argv[2]) + std::string("/timing.csv")).c_str(), std::ofstream::app);

  fileTiming<<"dim x, dim y, dim z, size min x, size min y, size min z, size max x, size max y, size max z, dist x, dist y, dist z, num target coord, num box, isMRI?, time(ms)"<<std::endl;

  for(int testNum = 0; testNum <  TESTNUM; testNum++)
  {
    int boxSizeMin[3];
    int boxSizeMax[3];
    int distance[3];
    int numTargCoord;
    int numBox;
    int* targetCoord;

    for(int i = 0; i<3; i++)
    {
      boxSizeMin[i] = rand()%(dim[i]-1) + 1;
      boxSizeMax[i] = rand()%(dim[i]-boxSizeMin[i]) + boxSizeMin[i];
      distance[i] = rand()%(dim[i]) + 1;      
    }

    numBox = rand()%(BOXNUMMAX) + 1;
    numTargCoord = rand()%(TARGETCOORDNUMMAX) + 1;
    targetCoord = new int[numTargCoord*3];

    //generate random target pixel coordinates
    for(int i = 0; i < numTargCoord; i++)
    {
      targetCoord[i*3] = rand()%(dim[0]);
      targetCoord[i*3+1] = rand()%(dim[1]);
      targetCoord[i*3+2] = rand()%(dim[2]);
    }

    // generate random boxes    
    int * out = getRandomBoxes(boxSizeMin, boxSizeMax, distance, numBox);
    
    if(out == NULL)
    {
      std::cout<<"invalid parameters"<<std::endl;
      return -1;
    }

    //generate data for MRI and no MRI
    bool isMRI[2];

    isMRI[0] = true;
    isMRI[1] = false;

    for(int i = 0; i < 2; i++)
    {

      clock_t t1,t2;
      double diff;
      
      // start timing
      t1=clock();
     
      //compute integral of boxes
      double * integral;
      
      getRandomBoxIntegral<const ImageType::PixelType>(targetCoord, numTargCoord, out, numBox, reader->GetOutput()->GetBufferPointer(), dim, isMRI[i], integral);
  
      //end timing
      t2=clock();
      diff = ((double)t2-(double)t1);

      for(int j = 0; j < 3; j++)
      {
        fileTiming<<dim[j]<<",";
      }
      for(int j = 0; j < 3; j++)
      {
        fileTiming<<boxSizeMin[j]<<",";
      }
      for(int j = 0; j < 3; j++)
      {
        fileTiming<<boxSizeMax[j]<<",";
      }
      for(int j = 0; j < 3; j++)
      {
        fileTiming<<distance[j]<<",";
      }
      fileTiming<<numTargCoord<<","<<numBox<<",";
      fileTiming<<isMRI[i]<<",";
      fileTiming<<diff/CLOCKS_PER_SEC*1000<<std::endl;
          
      int finalNumBox = isMRI[i] ? numBox/2: numBox;
#ifdef DEBUG
      //write integral to file
      for(int k = 0; k < numTargCoord; k++)
      {
        for(int o = 0; o < finalNumBox; o++)
        {
          fileIntegral[i] << integral[k*o + o];
      
	  if(o != finalNumBox -1)
	    fileIntegral[i] << ",";
	  else
	    fileIntegral[i] << "\n";
	}
      }
#endif
      delete [] integral;
    }
#ifdef DEBUG
    //write random box offset and length data to file
    for(int i = 0; i < numBox; i++)
    {
      for(int j = 0; j < 6; j++)
      {    
        fileRandomBoxes << out[i*6 + j];
      
	if(j != 5)
	  fileRandomBoxes << ",";
	else
	  fileRandomBoxes <<"\n";
      }
    }  
#endif
    delete [] out;
    delete [] targetCoord;

    std::cout<<"test# "<<testNum+1<<"/"<<TESTNUM<<std::endl;
      
  }// of testNum loop 
 
  fileIntegral[0].close();
  fileIntegral[1].close();
  fileRandomBoxes.close();
  fileTiming.close(); 
  
  return 0;
}
