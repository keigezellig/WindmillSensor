#include "gradient.h"

Gradient::Gradient()
{

}
void Gradient::init(int min, int max, const RGB* colors, uint8_t numberOfColors) {
  m_min = min;
  m_max = max;
  int actualNumberOfColors = numberOfColors > MAX_NR_COLORS ? MAX_NR_COLORS : numberOfColors;
  setupLut(colors, actualNumberOfColors);
}


RGB Gradient::getColorFromValue(int value) {
  int index = calculateColorIndex(value);
  return RGB(m_gradient_lut[index]);
}

int Gradient::calculateColorIndex(int value) {

  int colorIndex = ( (float)((value - m_min)) / (float((m_max - m_min)))) * 256;

  //Clip values that are outside the subscript range
  if (colorIndex > 255)
  {
    colorIndex = 255;
  }

  if (colorIndex < 0)
  {
    colorIndex = 0;
  }

  return colorIndex;
}

void Gradient::setupLut(const RGB* colors, uint8_t numberOfColors) {
  int delta_r, delta_g, delta_b;
  int stepsize = 256 / (numberOfColors - 1);

  for (int sindex = 1; sindex < numberOfColors; sindex++) {
    int red = colors[sindex - 1].r; //+ delta_r;
    int green = colors[sindex - 1].g;// + delta_g;
    int blue = colors[sindex - 1].b; //+ delta_b;

    if (numberOfColors > 2) {
      delta_r = (colors[sindex].r - colors[sindex - 1].r) / stepsize;
      delta_g = (colors[sindex].g - colors[sindex - 1].g) / stepsize;
      delta_b = (colors[sindex].b - colors[sindex - 1].b) / stepsize;
    }
    else {
      delta_r = (colors[sindex].r - colors[sindex - 1].r) / 255;
      delta_g = (colors[sindex].g - colors[sindex - 1].g) / 255;
      delta_b = (colors[sindex].b - colors[sindex - 1].b) / 255;
    }


    for (int i=0; i< stepsize; i++) {
      int lutIndex = (sindex - 1) * stepsize + i;

      if (red > 255) {
        red = 255;
      }
      if (green > 255) {
        green = 255;
      }

      if (blue > 255) {
        blue = 255;
      }

      m_gradient_lut[lutIndex].r = red;
      m_gradient_lut[lutIndex].g = green;
      m_gradient_lut[lutIndex].b = blue;

      red += delta_r;
      green += delta_g;
      blue += delta_b;
    }
  }
}
