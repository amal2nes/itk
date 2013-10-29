#ifndef __SummationImageFilter_hxx
#define __SummationImageFilter_hxx

#include "SummationImageFilter.h"

#include <iostream>
#include <time.h>
#include <numeric>

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
  int index = 0;

  for(int z = 0; z < dims[2]; z++)
  {
    index += dims[0];
    for(int y = 1; y < dims[1]; y++) 
    {
      for(int x = 0; x < dims[0]; x++)
      {
        outRaw[index] += outRaw[index - dims[0]];  
	++index;
      }      
    }
  }
} 

template<class TOutputPixel>
void sumZ(int dims[], TOutputPixel * outRaw)
{
  TOutputPixel * prev = outRaw;
  
  outRaw += dims[0]*dims[1];

  for(int i = 0; i < dims[0]*dims[1]*dims[2] - dims[0]*dims[1]; i++)
  {
    outRaw[i] += prev[i];
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
  itkDebugMacro(<< "SummationImageFilter::GenerateData() called");

  clock_t t1,t2;
  float diff;

  t1=clock();

  InputImageConstPointer inputPtr  = this->GetInput();
  OutputImagePointer     outputPtr = this->GetOutput();

  typename TInputImage::SizeType inputSize = inputPtr->GetRequestedRegion().GetSize();

  int dims[3];

  for(int i = 0; i < 3; i++)
  {
    dims[i] = inputSize[i];  
  }

  outputPtr->SetRegions(inputPtr->GetRequestedRegion());
  outputPtr->Allocate();

  const TInputPixel* inRaw = inputPtr->GetBufferPointer(); 
  TOutputPixel* outRaw = outputPtr->GetBufferPointer(); 

  sumX(dims, inRaw, outRaw);

  sumY(dims, outRaw);

  sumZ(dims, outRaw);
  
  t2 = clock();

  diff = ((float)t2-(float)t1);
  std::cout<<"CPU Time: "<<diff/CLOCKS_PER_SEC<<"s"<<std::endl;
}

}

#endif
