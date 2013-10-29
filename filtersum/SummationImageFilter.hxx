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
#include <numeric>

int getIndex(int x, int y, int z, int dims[])
{
  return z*(dims[0]*dims[1]) + y*dims[0] + x;
}

template<class TInputPixel, class TOutputPixel>
void sumX(int dims[], TInputPixel * inRaw, TOutputPixel * outRaw)
{
  for(int index = 0; index < dims[1]*dims[2]; index++)
  {
    std::partial_sum(inRaw + index * dims[0], inRaw + index * dims[0] + dims[0], outRaw + index * dims[0]);
  }
}

template<class TOutputPixel>
void sumY(int dims[], TOutputPixel * outRaw)
{
  for(int z = 0; z < dims[2]; z++)
  {
    for(int x = 0; x < dims[0]; x++)
    {
      for(int y = 1; y < dims[1]; y++)
      {
        outRaw[z*(dims[0]*dims[1]) + y*dims[0] + x] += outRaw[z*(dims[0]*dims[1]) + (y-1)*dims[0] + x];  
      }
    }
  }
} 

template<class TOutputPixel>
void sumZ(int dims[], TOutputPixel * outRaw)
{
  for(int x = 0; x < dims[0]; x++)
  {
    for(int y = 0; y < dims[1]; y++)
    {
      for(int z = 1; z < dims[2]; z++)
      {   
        outRaw[z*(dims[0]*dims[1]) + y*dims[0] + x] += outRaw[(z-1)*(dims[0]*dims[1]) + y*dims[0] + x];  
      }
    }
  }
}

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

  typename TInputImage::SizeType inputSize = inputPtr->GetRequestedRegion().GetSize();

  int dims[3];
  dims[0] = inputSize[0];
  dims[1] = inputSize[1];
  dims[2] = inputSize[2];  

  outputPtr->SetRegions(inputPtr->GetRequestedRegion());
  outputPtr->Allocate();

  typedef typename TInputImage::PixelType TInputPixel;
  typedef typename TOutputImage::PixelType TOutputPixel;

  const TInputPixel* inRaw = inputPtr->GetBufferPointer(); 
  TOutputPixel* outRaw = outputPtr->GetBufferPointer(); 

  t1=clock();

  //sum up x
  sumX(dims, inRaw, outRaw);

  t2=clock();

  diff = ((float)t2-(float)t1);
  std::cout<<"Time 1: "<<diff/CLOCKS_PER_SEC<<"s"<<std::endl;

  t1=clock();

  //sum up y
  sumY(dims, outRaw);

  t2=clock();

  diff = ((float)t2-(float)t1);
  std::cout<<"Time 2: "<<diff/CLOCKS_PER_SEC<<"s"<<std::endl;

  t1=clock();

  sumZ(dims, outRaw);

  t2=clock();

  diff = ((float)t2-(float)t1);
  std::cout<<"Time 3: "<<diff/CLOCKS_PER_SEC<<"s"<<std::endl;
}

}

#endif
