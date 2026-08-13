#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "wave.h"

#define WAVE_USER_DATA_FLOAT(p)			\
  float* d = malloc(sizeof(float));		\
  *d = p;					\
  return (wave_user_data_t){ d, true, free };	\

float wave_square(float p, void* d) {
  float duty = d == NULL ? 0.5 : *(float*)d;
  return p < duty ? 1.0f : -1.0f;
}

wave_user_data_t wave_square_user_data(float duty_cycle) {
  WAVE_USER_DATA_FLOAT(duty_cycle)
}

float wave_sine(float p, void* d) {
  (void)d;
  return sin(2 * M_PI * p);
}

// TODO: Optimize
float wave_triangle(float p, void* d) {
  float data = d ? *(float*)d : 0.0f;
  float D = (data + 1.0f) * 0.5f;

  if (D <= 0.0f)
    return 1.0f - 2.0f * p;

  if (D >= 1.0f)
    return 2.0f * p - 1.0f;

  if (p < D)
    return -1.0f + 2.0f * p / D;

  return 1.0f - 2.0f * (p - D) / (1.0f - D);
}

wave_user_data_t wave_triangle_user_data(float shape) {
  WAVE_USER_DATA_FLOAT(shape)
}
