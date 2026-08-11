#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "wave.h"

float wave_square(float x, float w, void* d) {
  float phase = x * w;
  phase -= (int)phase;
  float duty = d == NULL ? 0.5 : *(float*)d;
  return phase < duty ? 1.0f : -1.0f;
}

wave_user_data wave_square_user_data(float duty_cycle) {
  float* d = malloc(sizeof(float));
  *d = duty_cycle;
  return (wave_user_data){ d, true, free };
}

float wave_sine(float x, float w, void* d) {
  (void)d;
  return sin(2 * M_PI * x * w);
}
