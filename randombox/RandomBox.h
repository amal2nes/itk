#ifndef __RandomBox_h
#define __RandomBox_h

class RandomBox{
private:
  int dimension[3];
  int numBox;
  int boxSizeMin;
  int boxSizeMax;
public:
  RandomBox();
  int** getRandomBoxes(int x, int y, int z, int distance);
  void setParameters(int w, int h, int d, int numBox, int sizeMin, int sizeMax);
};

#include "RandomBox.hxx"

#endif
