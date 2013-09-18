#ifndef __SummationImageFilter_hxx
#define __SummationImageFilter_hxx

#include "SummationImageFilter.h"

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

  // Get the input and output pointers
  InputImageConstPointer inputPtr  = this->GetInput();
  OutputImagePointer     outputPtr = this->GetOutput();

  outputPtr->SetRegions(inputPtr->GetLargestPossibleRegion());
  outputPtr->Allocate();
  
  itk::ImageRegionConstIterator<TInputImage> inputIterator(inputPtr, inputPtr->GetLargestPossibleRegion());
  itk::ImageRegionIterator<TOutputImage> outputIterator(outputPtr, outputPtr->GetLargestPossibleRegion());
 
  while(!inputIterator.IsAtEnd())
  {
    outputIterator.Set(inputIterator.Get());
    ++inputIterator;
    ++outputIterator;
  }

}

}

#endif
