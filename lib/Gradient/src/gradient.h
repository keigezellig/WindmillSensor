#ifndef GRADIENT_H
#define GRADIENT_H

#include <stdint.h>

const int MAX_NR_COLORS = 3;

struct RGB {
   int    r;
   int    g;
   int    b;
   RGB(int red, int green, int blue) : r(red), g(green), b(blue) { }
   RGB() { }
};

class Gradient {
private:
  int m_min;
  int m_max;

  RGB m_gradient_lut[256];
  int calculateColorIndex(int value);
  void setupLut(const RGB* colors, uint8_t numberOfColors);
public:
  void init(int min, int max, const RGB* colors, uint8_t numberOfColors);
  RGB getColorFromValue(int value);
  Gradient();

};

#endif
