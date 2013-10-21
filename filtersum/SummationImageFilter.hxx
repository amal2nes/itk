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

  // start timing
  t1=clock();

  itkDebugMacro(<< "SummationImageFilter::GenerateData() called");
  
  InputImageConstPointer inputPtr  = this->GetInput();
  OutputImagePointer     outputPtr = this->GetOutput();

  //get size of image
  typename TInputImage::SizeType inputSize = inputPtr->GetLargestPossibleRegion().GetSize();

  outputPtr->SetRegions(inputPtr->GetLargestPossibleRegion());
  outputPtr->Allocate();

  // get image total, slice, and row sizes
  int sizeTotal = inputSize[0]*inputSize[1]*inputSize[2]; 
  int sizeSlice = inputSize[0]*inputSize[1];
  int sizeRow =  inputSize[0];

  // pointer to input image data
  const typename TInputImage::PixelType * in = inputPtr->GetBufferPointer(); 

  // temporary var to store voxel intensity
  typename TOutputImage::PixelType sum;

  // storing current position
  unsigned int position; 

  int divSlice;
  int remainderSlice;
  int divRow;
  int remainderRow;

  // pointers to output image data
  typename TOutputImage::PixelType * out[8]; 

  // convention - array index : postion (x,y,x)
  // 0 : 0,0,0			
  // 1 : -1,0,0
  // 2 : 0,-1,0
  // 3 : -1,-1,0
  // 4 : 0,0,-1
  // 5 : -1,0,-1
  // 6 : 0,-1,-1
  // 7 : -1,-1,-1

  // addition/subtraction sign
  int sign[8]; 
  sign[0] = 1;
  sign[1] = 1;
  sign[2] = 1;
  sign[3] = -1;
  sign[4] = 1;
  sign[5] = -1;
  sign[6] = -1;
  sign[7] = 1;

  // initialize output pointers
  for(int i = 0; i<8; i++)
  {
    out[i] = outputPtr->GetBufferPointer(); 
  }

  //main algorithm starts
  for(int i = 0; i < sizeTotal; i++)
  {
    position = 0;

    // get position info
    divSlice = i / sizeSlice;   	     // slice
    remainderSlice = i % sizeSlice; 
    divRow = remainderSlice / sizeRow;       // row
    remainderRow = remainderSlice % sizeRow; // column

    if(divSlice >= 1)
      position |= 1 << 2;       // set 3rd bit to 1 if it's not top slice
    if(divRow >= 1)      
      position |= 1 << 1;	// set 2nd bit to 1 if it's not 1st row
    if(remainderRow >= 1)
      position |= 1; 	        // set 1st bit to 1 if it's not 1st column

    sum = *in;		// get current input voxel value

    switch(position)
    {
    case 7:			// 111 : sum all
      for(int j = 1; j < 8; j++)
      {   
        sum += sign[j] * (*out[j]);  
      }
      break;
    case 6:			// 110: sum all except 1st column 
      for(int j = 1; j < 8; j++)
      {   
        if(j % 2 != 0)
	  continue;
        sum += sign[j] * (*out[j]);  
      }
      break;
    case 5:			// 101: sum all except 1st row
      for(int j = 1; j < 8; j++)
      {   
        if(j == 2 || j == 3 || j == 6 || j == 7)
	  continue;
        sum += sign[j] * (*out[j]);  
      }
      break;
    case 4:			// 100: sum all except 1st row and 1st column
      sum += sign[4] * (*out[4]);  
      break;
    case 3:			// 011: sum all except top slice
      for(int j = 1; j < 4; j++)
      {   
        sum += sign[j] * (*out[j]);  
      }
      break;
    case 2:	  		// 010: sum all except top slice and
				// 1st column
      sum += sign[2] * (*out[2]);
      break; 
    case 1:			// 001: sum all except top slice and
				// 1st row
      sum += sign[1] * (*out[1]);
      break;
    default:			// 000: sum nothing
      break;
    }
    
    *out[0] = sum;		// set sum to current output voxel

#ifdef DEBUG
    if(i == 50*50*9 + 50*9 + 9 )
       std::cout<<"voxel intensity at 10,10,10: "<<*out[0]<<std::endl;

    if(i == 50*50*29 + 50*19 + 9)
       std::cout<<"voxel intensity at 10,20,30: "<<*out[0]<<std::endl;
#endif

    // increment pointers depending if it's out of bounds or not
    if(i >= 1)			
    {
      out[1]++;
      if(i >= sizeRow)
      {
        out[2]++;
        if( i >= sizeRow + 1)
    	{
    	  out[3]++;
          if( i >= sizeSlice)
    	  {
    	    out[4]++;
    	    if( i >= sizeSlice + 1)
    	    {
    	      out[5]++;
    	      if( i >= sizeSlice + sizeRow)
    	      {
    	        out[6]++;
    		if( i >= sizeSlice + sizeRow + 1)
    	        {
    	          out[7]++;
    		}
    	      }
    	    }
    	  }
    	}
      }
    }
    
    ++out[0];
    ++in;
  } // end main algorithm

  //show timing
  t2=clock();
  diff = ((float)t2-(float)t1);
  std::cout<<"Summation Time: "<<diff/CLOCKS_PER_SEC<<"s"<<std::endl;
}

}

#endif
