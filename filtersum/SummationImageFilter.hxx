#ifndef __SummationImageFilter_hxx
#define __SummationImageFilter_hxx

#include "SummationImageFilter.h"

#include "itkNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkConstantBoundaryCondition.h"
#include "itkRescaleIntensityImageFilter.h"

#include <iostream>
#include <limits>
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

  ImageTypeDoublePointer tempPtr = ImageTypeDouble::New();
  tempPtr->SetRegions(inputPtr->GetRequestedRegion());

  tempPtr->Allocate();
  
  itk::ImageRegionConstIterator<TInputImage> inputIterator(inputPtr, inputPtr->GetRequestedRegion());

  itk::ImageRegionIterator<ImageTypeDouble> outputIterator(tempPtr, tempPtr->GetRequestedRegion());

  t1=clock();

  while(!inputIterator.IsAtEnd())
  {
    outputIterator.Set((double)inputIterator.Get() + 1024);
    ++inputIterator;
    ++outputIterator;
  }

  t2=clock();
  diff = ((float)t2-(float)t1);
  std::cout<<"copy image: "<<diff/CLOCKS_PER_SEC<<"s"<<std::endl;

  //perform 3D summation with neighborhood iterators

  //pad boundary to zero
  RadiusType radius;
  radius.Fill(1);
  NeighborhoodIteratorType it( radius, tempPtr, tempPtr->GetLargestPossibleRegion());

  OffsetType offset0 = {{0,0,0}};
  OffsetType offset1 = {{-1,0,0}};
  OffsetType offset2 = {{0,-1,0}};
  OffsetType offset3 = {{-1,-1,0}};
  OffsetType offset4 = {{0,0,-1}};
  OffsetType offset5 = {{-1,0,-1}};
  OffsetType offset6 = {{0,-1,-1}};
  OffsetType offset7 = {{-1,-1,-1}};

//  itk::Neighborhood< double, 3, TAllocator >

  double sum;
  double temp[8];
  
  t1=clock();

  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
  {   
    sum = 0;
    temp[0] = it.GetPixel(offset0); 
    temp[1] = it.GetPixel(offset1); 
    temp[2] = it.GetPixel(offset2); 
    temp[3] = -1.0 * it.GetPixel(offset3);
    temp[4] = it.GetPixel(offset4);
    temp[5] = -1.0 * it.GetPixel(offset5);
    temp[6] = -1.0 * it.GetPixel(offset6);
    temp[7] = it.GetPixel(offset7);

    //get neighborhood region from one operation

    //  GetNeighborhood()

//    SetNeighborhood(NeighborhoodType &a)

    for( int i = 0; i < 8; i++)
    { 
      sum += temp[i]; 
    }

    //out.Set(sum);
    it.SetCenterPixel(sum);
  } 

  t2=clock();
  diff = ((float)t2-(float)t1);
  std::cout<<"sum image: "<<diff/CLOCKS_PER_SEC<<"s"<<std::endl;


  t1=clock();

  typedef itk::RescaleIntensityImageFilter<ImageTypeDouble, TOutputImage > RescaleFilterType;
  typename RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum( 3000 );
  rescaler->SetInput(tempPtr);
  rescaler->GraftOutput(outputPtr);
  rescaler->Update();
  this->GraftOutput( rescaler->GetOutput() );

  t2=clock();
  diff = ((float)t2-(float)t1);
  std::cout<<"rescale image: "<<diff/CLOCKS_PER_SEC<<"s"<<std::endl;

/*
  typedef itk::RescaleIntensityImageFilter<TInputImage, TOutputImage > RescaleFilterType2;
  typename RescaleFilterType2::Pointer rescaler2 = RescaleFilterType2::New();

  rescaler2->SetOutputMinimum(0);
  rescaler2->SetOutputMaximum( 3000 );
  rescaler2->SetInput(inputPtr);
  rescaler2->GraftOutput(outputPtr);
  rescaler2->Update();
  this->GraftOutput( rescaler2->GetOutput() );
*/

}

}

#endif
