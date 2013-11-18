#ifndef __FeatureExtractSum_hxx
#define __FeatureExtractSum_hxx

#include "FeatureExtractSum.h"

#include <iostream>
#include <numeric>

namespace FeatureExtractSum{

template<class TInputPixel, class TOutputPixel>
void sumX(int dims[], TInputPixel * inRaw, TOutputPixel * outRaw)
{ 
  for(int index = 0; index < dims[1]*dims[2]; index++)
  {
    // sums up for every element in every row
    std::partial_sum(inRaw + index * dims[0], inRaw + index * dims[0] + dims[0], outRaw + index * dims[0]);
  }
}

template<class TOutputPixel>
void sumY(int dims[], TOutputPixel * outRaw)
{
  int index = 0;

  for(int z = 0; z < dims[2]; z++)
  {
    // skip every 1st row
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
  
  //offset current ptr to 2nd slice
  TOutputPixel * outRawOffset = outRaw;
  outRawOffset += dims[0]*dims[1];
  
  for(int i = 0; i < dims[0]*dims[1]*dims[2] - dims[0]*dims[1]; i++)
  {
    outRawOffset[i] += prev[i];
  }
}

}//end of FeatureExtractSum namespace


namespace itk{

template <class TInputImage, class TOutputImage>
FeatureExtractFilter<TInputImage, TOutputImage>::FeatureExtractFilter()
{
    itkDebugMacro(<< "FeatureExtractFilter called");
}

template< class TInputImage, class TOutputImage>
void
FeatureExtractFilter< TInputImage, TOutputImage>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template< class TInputImage, class TOutputImage>
void FeatureExtractFilter< TInputImage, TOutputImage >::GenerateData()
{
  itkDebugMacro(<< "FeatureExtractFilter::GenerateData() called");

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

  FeatureExtractSum::sumX(dims, inRaw, outRaw);

  FeatureExtractSum::sumY(dims, outRaw);

  FeatureExtractSum::sumZ(dims, outRaw);

  // std::cout<<"test sum at (10,10,10): "<<outRaw[9*dims[0]*dims[1] + 9*dims[0] + 9]<<std::endl;
  // std::cout<<"test sum at (10,20,30): "<<outRaw[29*dims[0]*dims[1] + 19*dims[0] + 9]<<std::endl;
}

}//end of itk namespace


#endif
