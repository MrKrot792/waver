#include <math.h>
#include <stdlib.h>

#include "wave.h"

float wave_square(float x, float w, void* d) {
  w /= 2*M_PI;
  float phase = x * w;
  phase -= (int)phase;
  float duty = d == NULL ? 0.5 : *(float*)d;
  return phase < duty ? 1.0f : -1.0f;
}

float wave_sine(float x, float w, void* d) {
  (void)d;
  return sin(x * w);
}
