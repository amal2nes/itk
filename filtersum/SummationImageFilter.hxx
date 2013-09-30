#ifndef __SummationImageFilter_hxx
#define __SummationImageFilter_hxx

#include "SummationImageFilter.h"

#include "itkNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkConstantBoundaryCondition.h"
#include "itkRescaleIntensityImageFilter.h"
#include <itkImageLinearConstIteratorWithIndex.h>
#include <itkImageLinearIteratorWithIndex.h>

#include <iostream>
#include <time.h>

namespace itk{

template <class TInputImage, class TOutputImage>
SummationImageFilter<TInputImage, TOutputImage>::SummationImageFilter()
{
    itkDebugMacro(<< "SummationImageFilter called");
}

template< class TInputImage, class TOutputImage>
void
SummationImageFilter< TInputImage, TOutputImage>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template< class TInputImage, class TOutputImage>
void SummationImageFilter< TInputImage, TOutputImage >::GenerateData()
{
  clock_t t1,t2;
  float diff;

  itkDebugMacro(<< "SummationImageFilter::GenerateData() called");

  InputImageConstPointer inputPtr  = this->GetInput();
  OutputImagePointer     outputPtr = this->GetOutput();

  //get size of image
  typename TInputImage::SizeType inputSize = inputPtr->GetLargestPossibleRegion().GetSize();

  std::cout<<"input size: "<<inputSize[0]<<", "<<inputSize[1]<<", "<<inputSize[2]<<std::endl;

  outputPtr->SetRegions(inputPtr->GetRequestedRegion());
  outputPtr->Allocate();

  int sizeTotal = inputSize[0]*inputSize[1]*inputSize[2]; // size of image
  int sizeSlice = inputSize[0]*inputSize[1];
  int sizeRow =  inputSize[0];

  float sum = 0;

  float * temp = new float[sizeTotal];
  float * temp_start = temp;

  int offset[7];
  offset[0] = -1; // -1,0,0
  offset[1] = -1; // 0,-1,0
  offset[2] = -1; // -1,-1,0
  offset[3] = -1; // 0,0,-1
  offset[4] = -1; // -1,0,-1
  offset[5] = -1; // 0,-1,-1
  offset[6] = -1; // -1,-1,-1
  
  int sign[7];
  sign[0] = 1;
  sign[1] = 1;
  sign[2] = -1;
  sign[3] = 1;
  sign[4] = -1;
  sign[5] = -1;
  sign[6] = 1;

  const short int * in = inputPtr->GetBufferPointer();  
  float * out;
  float * out_center = outputPtr->GetBufferPointer();

  t1=clock();

  for(int i = 0; i < sizeTotal; i++)
  {
    if (i >= sizeSlice) // if it's not at top slice
    {
      if(i % sizeSlice > 0) // if it's not the first voxel in the row
      { 
        for(int j = 0; j < 7; j++) // sum neighbour voxels
	{
          out = temp + offset[j];
          sum+= sign[j] * (*out);  
	}        
        sum+=*in; // sum current voxel
	*temp = sum; // set output
      }      
    }
    else
    {

    }
    temp++;
    in++;
  }

  for(int i=0; i < sizeTotal; i++, out_center++, temp_start++)
  {
    *out_center = *temp_start;
  }

  t2=clock();
  diff = ((float)t2-(float)t1);
  std::cout<<"Summation Time: "<<diff/CLOCKS_PER_SEC<<"s"<<std::endl;
}

}

#endif
