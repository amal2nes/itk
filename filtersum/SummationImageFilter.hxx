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
  
  int total = inputSize[0]*inputSize[1]*inputSize[2];

  const short int * in = inputPtr->GetBufferPointer();  
  float * out = outputPtr->GetBufferPointer(); 


  t1=clock();

  for(int i = 0; i < total; i++)
  {
    *out = *in;
    in++;
    out++;
  }

  t2=clock();
  diff = ((float)t2-(float)t1);
  std::cout<<"Summation Time: "<<diff/CLOCKS_PER_SEC<<"s"<<std::endl;
}

}

#endif
