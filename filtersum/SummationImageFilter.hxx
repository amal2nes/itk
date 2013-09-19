#ifndef __SummationImageFilter_hxx
#define __SummationImageFilter_hxx

#include "SummationImageFilter.h"

#include "itkNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkConstantBoundaryCondition.h"
#include "itkRescaleIntensityImageFilter.h"

#include <iostream>
#include <limits>

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

  InputImageConstPointer inputPtr  = this->GetInput();
  OutputImagePointer     outputPtr = this->GetOutput();

  ImageTypeDoublePointer tempPtr = ImageTypeDouble::New();
  tempPtr->SetRegions(inputPtr->GetRequestedRegion());

  tempPtr->Allocate();
  
  itk::ImageRegionConstIterator<TInputImage> inputIterator(inputPtr, inputPtr->GetRequestedRegion());

  itk::ImageRegionIterator<ImageTypeDouble> outputIterator(tempPtr, tempPtr->GetRequestedRegion());

  while(!inputIterator.IsAtEnd())
  {
    outputIterator.Set((double)(inputIterator.Get() + 1024) / (double)1000.0);
    ++inputIterator;
    ++outputIterator;
  }

  //perform 3D summation with neighborhood iterators

  //pad boundary to zero
  RadiusType radius;
  radius.Fill(1);
  NeighborhoodIteratorType it( radius, tempPtr, tempPtr->GetLargestPossibleRegion());

  IteratorType out(tempPtr, tempPtr->GetLargestPossibleRegion());

  OffsetType offset0 = {{0,0,0}};
  OffsetType offset1 = {{-1,0,0}};
  OffsetType offset2 = {{0,-1,0}};
  OffsetType offset3 = {{-1,-1,0}};
  OffsetType offset4 = {{0,0,-1}};
  OffsetType offset5 = {{-1,0,-1}};
  OffsetType offset6 = {{0,-1,-1}};
  OffsetType offset7 = {{-1,-1,-1}};

  double maxVal = 0;

  for (it.GoToBegin(), out.GoToBegin(); !it.IsAtEnd(); ++it, ++out)
  {
    double sum = 0;
    double temp[8];
    temp[0] = (double)it.GetPixel(offset0); 
    temp[1] = (double)it.GetPixel(offset1); 
    temp[2] = (double)it.GetPixel(offset2); 
    temp[3] = -1.0 * (double)it.GetPixel(offset3);
    temp[4] = (double)it.GetPixel(offset4);
    temp[5] = -1.0 * (double)it.GetPixel(offset5);
    temp[6] = -1.0 * (double)it.GetPixel(offset6);
    temp[7] = (double)it.GetPixel(offset7);
    
    for( int i = 0; i < 8; i++)
    { 
      sum += temp[i]; 
    }

    out.Set(sum);
    if(maxVal < sum)
      maxVal = sum;
    } 

  std::cout<<"maxval:"<<maxVal<<std::endl;

  typedef itk::RescaleIntensityImageFilter<ImageTypeDouble, TOutputImage > RescaleFilterType;
  typename RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum( 3000 );

  rescaler->SetInput(tempPtr);
  rescaler->GraftOutput( this->GetOutput() );
  rescaler->Update();
  this->GraftOutput( rescaler->GetOutput() );

}

}

#endif
