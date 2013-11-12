//this module performs integral 3D sum using ITK class implementation
//to perform integral 3D, use Update() from an instance of this class or invoke it from another ITK
//element connected to this instance down the pipeline

#ifndef __FeatureExtractSum_h
#define __FeatureExtractSum_h

#include "itkImageToImageFilter.h"

namespace FeatureExtractSum{
  //sums in x direction
  //call this first before sumY and sumZ to save output to another
  //buffer
  template<class TInputPixel, class TOutputPixel>
  void sumX(int dims[], TInputPixel * inRaw, TOutputPixel * outRaw);

  //sums in y direction
  //this sums in-place
  template<class TOutputPixel>
  void sumY(int dims[], TOutputPixel * outRaw);

  //sums in z direction
  //this sums in-place
  template<class TOutputPixel>
  void sumZ(int dims[], TOutputPixel * outRaw);
}

namespace itk{

template <class TInputImage, class TOutputImage>
class FeatureExtractFilter:public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
/** Standard class typedefs. */
  typedef FeatureExtractFilter Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  typedef typename TInputImage::PixelType TInputPixel;
  typedef typename TOutputImage::PixelType TOutputPixel;

/** Method for creation through the object factory. */
  itkNewMacro(Self);
  
/** Run-time type information (and related methods). */
  itkTypeMacro(FeatureExtractFilter, ImageToImageFilter);

/** Typedef for images */
  typedef TInputImage                           InputImageType;
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
 
protected:
  FeatureExtractFilter();
  virtual ~FeatureExtractFilter() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

  // implemented method for integral image
  void GenerateData();  

private:
  FeatureExtractFilter(const Self &); //purposely not implemented
  void operator=(const Self &);          //purposely not implemented 
  
};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "FeatureExtractSum.cpp"
#endif

#endif
