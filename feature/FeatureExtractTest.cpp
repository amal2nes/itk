/**
 *
 * Test bench for the created filter(s)
 * 
 *=========================================================================*/

#include "itkImage.h"
#include <iostream>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include <itkPasteImageFilter.h>
#include <itkImageDuplicator.h>
#include <itkCastImageFilter.h>

#include <time.h>

//image integral filter
#include "FeatureExtractSum.h"

int main(int argc, char **argv)
{
  if(argc < 3)
    return EXIT_FAILURE;

  typedef itk::Image< int, 3 > ImageTypeIn;
  typedef itk::Image< float, 3 > ImageTypeOut;
  typedef itk::ImageFileReader<ImageTypeIn> ReaderType;
  typedef itk::ImageFileWriter<ImageTypeOut> WriterType;
  typedef itk::FeatureExtractFilter< ImageTypeIn, ImageTypeOut > FilterType;
 
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  reader->Update();

  //image integral timing
  clock_t t1,t2;
  float diff;
  t1=clock();

  filter->Update();

  t2 = clock();
  diff = ((float)t2-(float)t1);
  std::cout<<"Image Integral CPU Time: "<<diff/CLOCKS_PER_SEC<<"s"<<std::endl;

  try
    {
    writer->Update();
    }catch(itk::ExceptionObject & err)
    {
    std::cerr <<"ExceptionObject Caught !" <<std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }
  
  std::cout << "Finished Feature Extract Test" << std::endl;
  
  return EXIT_SUCCESS;
}
