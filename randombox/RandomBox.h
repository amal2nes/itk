#ifndef __RandomBox_h
#define __RandomBox_h

class RandomBox{
private:

public:
  RandomBox();
  // gets random boxes: x,y,z,lx,ly,lz
  static int* getRandomBoxes(int minL[], int maxL[], int distance[], int numBox);
  static int* getRandomBoxIntegral(int dim[], const int* targetCoord, int numTargetCoord, const int* randomBoxes, int numRandomBoxes);
};

#include "RandomBox.hxx"

#endif
