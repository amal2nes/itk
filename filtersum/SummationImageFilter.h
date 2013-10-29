#ifndef __SummationImageFilter_h
#define __SummationImageFilter_h

#include "itkImageToImageFilter.h"

template<class TInputPixel, class TOutputPixel>
void sumX(int dims[], TInputPixel * inRaw, TOutputPixel * outRaw);

template<class TOutputPixel>
void sumY(int dims[], TOutputPixel * outRaw);

template<class TOutputPixel>
void sumZ(int dims[], TOutputPixel * outRaw);

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

  typedef typename TInputImage::PixelType TInputPixel;
  typedef typename TOutputImage::PixelType TOutputPixel;

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
 
protected:
  SummationImageFilter();
  virtual ~SummationImageFilter() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

  // implemented method for integral image
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
