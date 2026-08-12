#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "wave.h"

#define WAVE_USER_DATA_FLOAT(p)			\
  float* d = malloc(sizeof(float));		\
  *d = p;					\
  return (wave_user_data){ d, true, free };	\

float wave_square(float x, float w, void* d) {
  float phase = x * w;
  phase -= (int)phase;
  float duty = d == NULL ? 0.5 : *(float*)d;
  return phase < duty ? 1.0f : -1.0f;
}

wave_user_data wave_square_user_data(float duty_cycle) {
  WAVE_USER_DATA_FLOAT(duty_cycle)
}

float wave_sine(float x, float w, void* d) {
  (void)d;
  return sin(2 * M_PI * x * w);
}

// TODO: Optimize
float wave_triangle(float x, float w, void* d) {
  float T = 1.0 / w;
  float D = d == NULL ? 0.5 : (*(float*)d + 1.0) / 2.0;
  float tau = fmod(x, T);

  if (tau < 0.0)
    tau += T;

  float y;

  if (tau < D * T)
    y = tau / (D * T);
  else
    y = (T - tau) / ((1.0 - D) * T);

  return y * 2.0 - 1.0;
}

wave_user_data wave_triangle_user_data(float shape) {
  WAVE_USER_DATA_FLOAT(shape)
}
