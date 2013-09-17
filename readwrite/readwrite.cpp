#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

typedef short int PixelType;
const unsigned int Dimension = 3;
typedef itk::Image<PixelType, Dimension> ImageType;

typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;

int main(int argc, char** argv)
{
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  writer->SetInput(reader->GetOutput());

  try
  {
  writer->Update();
  }catch(itk::ExceptionObject & err)
  {
  std::cerr <<"ExceptionObject Caught !" <<std::endl;
  std::cerr << err << std::endl;
  return EXIT_FAILURE;
  }
  return 0;
}
