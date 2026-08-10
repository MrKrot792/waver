#include <math.h>

#include "wave.h"

float wave_square(float x, float w, void* duty) {
  w /= 2*M_PI;
  float phase = x * w;
  phase -= (int)phase;
  return phase < *(float*)duty ? 1.0f : -1.0f;
}

float wave_sine(float x, float w, void* d) {
  (void)d;
  return sin(x * w);
}
