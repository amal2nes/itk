/**
 *
 * Test bench for the created filter(s)
 * 
 *=========================================================================*/

#include "itkImage.h"
#include <iostream>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

//the filter to test
#include "SummationImageFilter.h"

int main(int argc, char **argv)
{
  if(argc < 3)
    return EXIT_FAILURE;

  typedef itk::Image< short int, 3 > ImageTypeIn;
  typedef itk::Image< int, 3 > ImageTypeOut;
  typedef itk::ImageFileReader<ImageTypeIn> ReaderType;
  typedef itk::ImageFileWriter<ImageTypeOut> WriterType;
  typedef itk::SummationImageFilter< ImageTypeIn, ImageTypeOut > FilterType;
 
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  //filter->Update();  

  try
    {
    writer->Update();
    }catch(itk::ExceptionObject & err)
    {
    std::cerr <<"ExceptionObject Caught !" <<std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }
  
  std::cout << "Executed Test Bench" << std::endl;
  
  return EXIT_SUCCESS;
}
