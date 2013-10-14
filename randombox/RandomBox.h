#ifndef __RandomBox_h
#define __RandomBox_h

// gets random boxes: x,y,z,lx,ly,lz
int* getRandomBoxes(int* minL, int* maxL, int* distance, int numBox);

int getIndexFromXYZ(const int* dim, const int* boxVertice); // convert from
						  // x,y,z to image
						  // data index

//calculates integral based on target coordinate and random boxes
template< class dataType>
int* getRandomBoxIntegral(int* dim, const int* targetCoord, int numTargetCoord, const int* randomBoxes, int numRandomBoxes, dataType data);

#include "RandomBox.hxx"

#endif
