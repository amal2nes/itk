// A module for generating a volume summation of the input image.
// This follows ITK examples and GenerateData function is implemented
// for this SummationImageFilter. Input and output image typedefs are declared.

#ifndef __SummationImageFilter_h
#define __SummationImageFilter_h

#include "itkImageToImageFilter.h"

#include "itkNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkConstantBoundaryCondition.h"

//#define DEBUG //debugging message

namespace itk{

template <class TInputImage, class TOutputImage>
class SummationImageFilter:public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
/** Standard class typedefs. */
  typedef SummationImageFilter Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

/** Method for creation through the object factory. */
  itkNewMacro(Self);
  
/** Run-time type information (and related methods). */
  itkTypeMacro(SummationImageFilter, ImageToImageFilter);

/** Typedef for images */
  typedef TInputImage                           InputImageType;
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;

protected:
  SummationImageFilter();
  virtual ~SummationImageFilter() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

  /** Method for evaluating the implicit function over the image. */
  void GenerateData();  

private:
  SummationImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);          //purposely not implemented
  
};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "SummationImageFilter.hxx"
#endif

#endif
