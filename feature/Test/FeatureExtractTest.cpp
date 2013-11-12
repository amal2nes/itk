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
#include <fstream>
#include <string>

//image integral filter
#include "FeatureExtractSum.h"
#include "FeatureExtractRandomBox.h"

using namespace std;
using namespace FeatureExtractRandomBox;

int main(int argc, char **argv)
{

  if(argc < 3)
  {
    cout<<"need test case file path"<<endl;
    return EXIT_FAILURE;
  }

  typedef itk::Image< int, 3 > ImageTypeIn;
  typedef itk::Image< float, 3 > ImageTypeOut;
  typedef itk::ImageFileReader<ImageTypeIn> ReaderType;
  typedef itk::ImageFileWriter<ImageTypeOut> WriterType;
  //image integral itk filter
  typedef itk::FeatureExtractFilter< ImageTypeIn, ImageTypeOut > FilterType;
 
  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();
  
  ifstream infile(argv[2]);
  reader->SetFileName(argv[1]);
  
  filter->SetInput(reader->GetOutput());
  
  reader->Update();
  
  //image integral timing
  clock_t t1,t2;
  float diff;
  t1=clock();
  
  filter->Update();
  
  t2 = clock();
  diff = ((float)t2-(float)t1);
  cout<<"image integral CPU time: "<<diff/CLOCKS_PER_SEC<<"s"<<endl;
  
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
  
  //image integral test
  cout<<"image integral test .................... started"<<endl;

  int numIntegralPoints;
  infile >> numIntegralPoints;
  int pos[3];
  float expected;
  int error = 0;  

  for(int i = 0; i < numIntegralPoints; i++)
  {
    infile >> pos[0] >> pos[1] >> pos[2] >> expected;
    //get integral value
    float integral = outRaw[pos[2]*dim[0]*dim[1] + pos[1]*dim[0] + pos[0]];
    
    if(expected != integral)
    {
      cout<<"Integral at ("<< pos[0] <<","<<pos[1]<<","<<pos[2]<<"): "<<integral<<", expected: "<<expected<<endl;
      error++;
    }
  }
  
  if(error == 0)
  {
    cout<<"image integral test .................... passed"<<endl;
  }
  else
  {
    cout<<"image integral test .................... failed: "<<error<<" errors"<<endl;
  }

  //test for random box offset and length
  cout<<"random box generation test .................... started"<<endl;
  int numRandomBoxCases;
  int sizeMin[3];
  int sizeMax[3];
  int distance[3];
  int numBox;
  error = 0;

  infile >> numRandomBoxCases;
  for(int i = 0; i < numRandomBoxCases; i++)
  {
    infile >> sizeMin[0] >> sizeMin[1] >> sizeMin[2] >> sizeMax[0] >> sizeMax[1] >> sizeMax[2] >> distance[0] >> distance[1] >> distance[2] >> numBox;
    int * out = getRandomBoxes(sizeMin, sizeMax, distance, numBox);
    
    //check if output offsets and lengths are within range
    for(int j = 0; j < numBox; j++)
    {
      int offset;
      int length;
      for(int k = 0; k < 3; k++)
      {
	offset = out[j*6 + k];
	length = out[j*6 + 3 + k];
	if(offset < -distance[k] || offset > distance[k])
	{
	  cout<<"random box offset out of bound, returned:"<<offset<<",expected range:["<<-distance[k]<<","<<distance[k]<<"]"<<endl;
	  error++;
	}
	if(length < sizeMin[k] || length > sizeMax[k])
	{
  	  cout<<"random box length out of bound, returned:"<<length<<",expected range:["<<sizeMin[k]<<","<<sizeMax[k]<<"]"<<endl;	
	  error++;
	}
      }      
    }
    delete [] out;
  }
  
  if(error == 0)
  {
    cout<<"random box generation test .................... passed"<<endl;
  }
  else
  {
    cout<<"random box generation test .................... failed: "<<error<<" errors"<<endl;
  }

  //test if pixel is within range of image
  cout<<"pixel within image range test .................... started"<<endl;
  int numPixels;
  int pixelPos[3];
  error = 0;
  
  infile >> numPixels;
  for(int i = 0; i < numPixels; i++)
  {
    infile >> pixelPos[0] >> pixelPos[1] >> pixelPos[2];

    double * output;
    int * randomBoxes = getRandomBoxes(sizeMin, sizeMax, distance, numBox);
    bool returnVal = getRandomBoxIntegral<TOutputPixel>(pixelPos, 1, randomBoxes, numBox, outRaw, dim, false, output);  

    // benchmark value
    bool success = true;
    for(int j = 0; j < 3; j++)
    {
      //out of bounds
      if(pixelPos[j] < 0 || pixelPos[j] >= dim[j])
      {
        success = false;
      }
    }

    if(success != returnVal)
      error++;
  }
 
  if(error == 0)
    cout<<"pixel within image range test .................... passed"<<endl;
  else
    cout<<"pixel within image range test .................... failed"<<endl;

  //test if box integral is correct
  cout<<"box integral test .................... started"<<endl;
  error = 0;
  int numBoxIntegral;
  int offsetBox[3];
  int lengthBox[3];
  int posPixel[3];
  int expectedVal;

  infile >> numBoxIntegral;
  for(int i = 0; i < numBoxIntegral; i++)
  {
    infile >> offsetBox[0] >> offsetBox[1] >> offsetBox[2] >> lengthBox[0] >> lengthBox[1] >> lengthBox[2] >> posPixel[0] >> posPixel[1] >> posPixel[2] >> expectedVal;

    int * randomBox = new int[6];
    for(int j = 0; j < 3; j++)
    {
      randomBox[j] = offsetBox[j];
      randomBox[j+3] = lengthBox[j];
    }

    double * boxIntegralOut;
    int * posPixelPtr = &posPixel[0];

    bool returnVal = getRandomBoxIntegral<TOutputPixel>(posPixelPtr, 1, randomBox, 1, outRaw, dim, false, boxIntegralOut);  
    
    if(boxIntegralOut[0] != expectedVal)
    {
      cout<<"box integral returned: "<<boxIntegralOut[0]<<", expected: "<<expectedVal<<endl;
      error++;
    }
  }
 
  if(error == 0)
    cout<<"box integral test .................... passed"<<endl;
  else
    cout<<"box integral test .................... failed, "<<error<<" errors"<<endl;

  cout << "Finished Feature Extraction Test" << endl;

  return 0;
}
