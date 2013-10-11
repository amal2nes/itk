#ifndef __RandomBox_h
#define __RandomBox_h

class RandomBox{
private:
  int dimension[3];		// max dimension
  int boxSizeMin[3];		// minimum box dimension per axis
  int boxSizeMax[3];		// maximum box dimension per axis
public:
  RandomBox();
  // gets positional data of random boxes: [BoxNum][BoxData(x,y,z,dx,dy,dz)]
  int** getRandomBoxes(int targetCoord[], int distance, int numBox);
  // Sets module properties: max dimensions, box size ranges
  bool setParameters(int dim[], int sizeMin[], int sizeMax[]);
};

#include "RandomBox.hxx"

#endif
