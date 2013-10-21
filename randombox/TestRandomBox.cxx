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

#define NUMBOX 2
#define NUMTARGETCOORD 2

int main(int argc, char **argv)
{
  if(argc < 2)
  {
    std::cout<<"need atleast 1 input file path"<<std::endl;
    return -1;
  }

  typedef itk::Image< double, 3 > ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
 
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(argv[1]);

  reader->Update();

  std::ofstream fileRandomBoxes, fileIntegral;

  fileRandomBoxes.open("randomBoxes.csv"/*, std::ofstream::app*/);
  fileIntegral.open("integralBoxes.csv");

  int boxSizeMin[3] = {1, 1, 1};

  int boxSizeMax[3] = {5, 5, 5};

  int distance[3] = {10, 10, 10};

  // int targetCoord[NUMTARGETCOORD*3] = {50,50,50,270,250,140,300,300,300};
  int targetCoord[NUMTARGETCOORD*3] = {1, 2, 9, 4, 24, 19};

  int dim[3];

  //get size of image
  typename ImageType::SizeType inputSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  
  for(int y = 0; y < 3; y++)
  {
    dim[y] = inputSize[y];
  }  

  // generate random boxes
  //int * out = getRandomBoxes(boxSizeMin, boxSizeMax, distance, NUMBOX);
  int testBoxes[NUMBOX*6] = { 0, 0, 0, 4, 4, 4, 0, 0, 0, 4, 8, 12};
  int * out = testBoxes;

  if(out == NULL)
  {
    std::cout<<"invalid parameters"<<std::endl;
    return -1;
  }

  // get integral of boxes using target coordinates
  // float * integral = getRandomBoxIntegral<const float*>(dim, targetCoord, NUMTARGETCOORD, out, NUMBOX, reader->GetOutput()->GetBufferPointer());

  double * integral = getRandomBoxIntegral<const double*>(dim, targetCoord, NUMTARGETCOORD, out, NUMBOX, reader->GetOutput()->GetBufferPointer());

  // std::cout<<"integral image first voxel intensity: "<<reader->GetOutput()->GetBufferPointer()[0]<<std::endl;

#ifdef DEBUG
  std::cout<<"point (2,3,10), length(4,4,4): "<<integral[0]<<std::endl;
  std::cout<<"point (5,25,20), length(4,8,12): "<<integral[3]<<std::endl;
#endif

  //write integral to file
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
