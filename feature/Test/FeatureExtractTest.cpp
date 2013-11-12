/**
 *
 * Test bench for the created filter(s)
 * 
 *=========================================================================*/

//number of random boxes to test with
#define BOXNUMMAX 10000

//number of target pixels to test with
#define TARGETCOORDNUMMAX 10000

//number of tests
#define TESTNUM 20

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
#include "FeatureExtractRandomBox.h"

using namespace std;
using namespace FeatureExtractRandomBox;

int main(int argc, char **argv)
{
  if(argc < 3)
    return EXIT_FAILURE;

  typedef itk::Image< int, 3 > ImageTypeIn;
  typedef itk::Image< float, 3 > ImageTypeOut;
  typedef itk::ImageFileReader<ImageTypeIn> ReaderType;
  typedef itk::ImageFileWriter<ImageTypeOut> WriterType;
  //image integral itk filter
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
  cout<<"Image Integral CPU Time: "<<diff/CLOCKS_PER_SEC<<"s"<<endl;

  // try
  //   {
  //   writer->Update();
  //   }catch(itk::ExceptionObject & err)
  //   {
  //   std::cerr <<"ExceptionObject Caught !" <<std::endl;
  //   std::cerr << err << std::endl;
  //   return EXIT_FAILURE;
  //   }

  //grab raw data
  typedef typename ImageTypeOut::PixelType TOutputPixel;
  TOutputPixel * outRaw = filter->GetOutput()->GetBufferPointer();  

  //get size of image
  typename ImageTypeOut::SizeType imageSize = filter->GetOutput()->GetLargestPossibleRegion().GetSize();
  int dim[3];
  for(int y = 0; y < 3; y++)
  {
    dim[y] = imageSize[y];
  }     
  cout << "image dimension: "<<dim[0]<<","<<dim[1]<<","<<dim[2]<<endl;

 //get value at some points of the output
  cout<<"test sum at (0,0,0): "<<outRaw[0]<<endl;
  cout<<"test sum at ("<< (dim[0]-1)/2 <<","<<(dim[1]-1)/2<<","<<(dim[2]-1)/2<<"): "<<outRaw[((dim[2]-1)/2)*dim[0]*dim[1] + ((dim[1]-1)/2)*dim[0] + (dim[0]-1)/2]<<endl;
  cout<<"test sum at ("<< dim[0]-1 <<","<<dim[1]-1<<","<<dim[2]-1<<"): "<<outRaw[(dim[2]-1)*dim[0]*dim[1] + (dim[1]-1)*dim[0] + dim[0]-1]<<endl;

  cout << "Finished Feature Extract Test" << endl;



  return 0;
}
