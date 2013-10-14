/**
 *
 * Test bench for the created filter(s)
 * 
 *=========================================================================*/

#include "RandomBox.h"
#include <iostream>
#include <fstream>

#include "itkImage.h"
#include "itkImageFileReader.h"

#define NUMBOX 10000
#define NUMTARGETCOORD 3

int main(int argc, char **argv)
{
  if(argc < 2)
  {
    std::cout<<"need atleast 1 input file path"<<std::endl;
    return -1;
  }

  typedef itk::Image< float, 3 > ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
 
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(argv[1]);

  reader->Update();

  std::ofstream fileRandomBoxes, fileIntegral;

  fileRandomBoxes.open("randomBoxes.csv"/*, std::ofstream::app*/);
  fileIntegral.open("integralBoxes.csv");

  int boxSizeMin[3] = {10, 10, 10};

  int boxSizeMax[3] = {100, 100, 100};

  int distance[3] = {100, 100, 100};

  int targetCoord[NUMTARGETCOORD*3] = {50,50,50,100,100,100,300,300,300};

  int dim[3];

  //get size of image
  typename ImageType::SizeType inputSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  
  for(int y = 0; y < 3; y++)
  {
    dim[y] = inputSize[y];
  }  

  // generate random boxes
  int * out = getRandomBoxes(boxSizeMin, boxSizeMax, distance, NUMBOX);
  
  if(out == NULL)
  {
    std::cout<<"invalid parameters"<<std::endl;
    return -1;
  }

  // get integral of boxes using target coordinates
  int * integral = getRandomBoxIntegral<const float*>(dim, targetCoord, NUMTARGETCOORD, out, NUMBOX, reader->GetOutput()->GetBufferPointer());

  for(int k = 0; k < NUMTARGETCOORD; k++)
  {
    for(int o = 0; o < NUMBOX; o++)
    {
      fileIntegral << integral[k*o + o];
      
      if(o != NUMBOX -1)
	fileIntegral << ",";
      else
	fileIntegral << "\n";
    }
  }

  fileIntegral.close();

  //write random box offset and length data to file
  for(int i = 0; i < NUMBOX; i++)
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
  
  fileRandomBoxes.close();

  return 0;
}
