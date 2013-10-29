#ifndef __RandomBox_h
#define __RandomBox_h

// gets an array of random boxes with data: x,y,z,lx,ly,lz
int* getRandomBoxes(int* minL, int* maxL, int* distance, int numBox);

// convert input x,y,z coordinates (0 indexed) to a linear index (0 indexed)
int getIndexFromXYZ(const int* dim, const int* boxVertice);

//calculates average integral based on target coordinates and random boxes
template< class dataType>
double* getRandomBoxIntegral(const int* targetCoord, const int numTargetCoord, const int* randomBoxes, const int numRandomBoxes, const dataType data, const int* dim, bool isMRI);

#include "RandomBox.hxx"

#endif
