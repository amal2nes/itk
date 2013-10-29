#ifndef __SummationImageFilter_h
#define __SummationImageFilter_h

#include "itkImageToImageFilter.h"

#include "itkNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkConstantBoundaryCondition.h"

inline int getIndex(int x, int y, int z, int dims[]);

template <class TOutputPixel>
inline void sum(int dims[], TOutputPixel * outRaw);

template <class TInputPixel, class TOutputPixel>
void sumX(int dims[], TInputPixel * inRaw, TOutputPixel * outRaw);

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
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef itk::Image< double, 3 > ImageTypeDouble;
  typedef typename ImageTypeDouble::Pointer ImageTypeDoublePointer;

  typedef itk::ConstantBoundaryCondition<ImageTypeDouble> BoundaryConditionType;

  typedef itk::NeighborhoodIterator<ImageTypeDouble,BoundaryConditionType> NeighborhoodIteratorType;
  typedef itk::ImageRegionIterator< ImageTypeDouble> IteratorType;
  typedef typename NeighborhoodIteratorType::RadiusType RadiusType;
  typedef typename NeighborhoodIteratorType::OffsetType OffsetType;

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
