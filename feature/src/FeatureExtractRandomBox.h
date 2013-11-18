#ifndef __FeatureExtractRandomBox_h
#define __FeatureExtractRandomBox_h

namespace FeatureExtractRandomBox{

// gets 1D array of random boxes with output tuples: ox,oy,oz,lx,ly,lz
int* getRandomBoxes(int* minL, int* maxL, int* distance, int numBox);

// convert input x,y,z coordinates (0 indexed) to a linear index (0 indexed)
int getIndexFromXYZ(const int* dim, const int* boxVertice);

//calculates average integral based on target coordinates and random
//boxes
// generate an array of integral corresponding to target coordinates
// and random boxes, returns continuous 1D array of rows of m x n input data
// for CT: m = number of target coordinates,n = number of random boxes
// for MRI: m = number of target coordinates,n = (number of random boxes)/2
// targetCoord = array of input coordinates (x,y,z)'s
// numTargetCoord = number of coordinate sets
// randomBoxes = array of random boxes (ox,oy,oz,lx,ly,lz)'s
// numRandomBoxes = number of random boxes
// data = pointer to image data having template dataType
// dim = size of image (SizeX,SizeY,SizeZ)
// isMRI = true if MRI, false if CT
// output = output array
// return true if successful
template< class dataType>
bool getRandomBoxIntegral(const int* targetCoord, const int numTargetCoord, const int* randomBoxes, const int numRandomBoxes, const dataType * data, const int* dim, bool isMRI, double *& output);


//returns 1D array of dx,dy,dz tuples where diff = pixel coordinate -
//fixed point input
bool getTrainingData(const int* targetCoord, const int numTargetCoord, int* trainInput, int* & trainOutput);

//convert from actual length to pixel length using image spacing factor
int* convertToPixelLength(const double * actual, const double * factor);

//convert from pixel length to actual length using image spacing factor
double* convertToActualLength(const int * pixelLength, const double * factor);

}

#include "FeatureExtractRandomBox.cpp"

#endif
