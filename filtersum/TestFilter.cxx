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

//the filter to test
#include "SummationImageFilter.h"

int main(int argc, char **argv)
{
  if(argc < 3)
    return EXIT_FAILURE;

  typedef itk::Image< short int, 3 > ImageTypeIn;
  typedef itk::Image< float, 3 > ImageTypeOut;
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

/*
  reader->Update();
  ImageTypeIn::RegionType largest = reader->GetOutput()->GetLargestPossibleRegion();
  itk::ImageRegion<3> selectRegion;
  itk::Index<3> sourceIndex;
  itk::Size<3> sourceSize;
  sourceIndex[0] = (unsigned long) (0.25 * largest.GetSize(0));
  sourceIndex[1] = (unsigned long) (0.25 * largest.GetSize(1));
  sourceIndex[2] = (unsigned long) (0.25 * largest.GetSize(2));
  sourceSize[0] = (unsigned long) (0.25 * largest.GetSize(0));
  sourceSize[1] = (unsigned long) (0.25 * largest.GetSize(1));
  sourceSize[2] = (unsigned long) (0.25 * largest.GetSize(2));

  selectRegion.SetIndex(sourceIndex);
  selectRegion.SetSize(sourceSize);
  filter->GetOutput()->SetRequestedRegion(selectRegion);
  filter->Update();  
  writer->SetInput(filter->GetOutput());
*/

  reader->Update();

  clock_t t1,t2;
  float diff;

  t1=clock();
  filter->Update();


  t2=clock();
  diff = ((float)t2-(float)t1);
  std::cout<<"Summation Time: "<<diff/CLOCKS_PER_SEC<<"s"<<std::endl;


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
