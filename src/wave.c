#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "wave.h"

static uint32_t sample_rate = 0;

void wave_set_sample_rate(float s) {
  sample_rate = s;
}

float wave_get(const wave_t* wave) {
  return wave->info->function(wave->phase, wave->info->user_data);
}

void wave_tick(wave_t* wave, float frequency) {
  wave->phase += frequency / sample_rate;
  if (wave->phase >= 1.0f) {
    float int_part = floorf(wave->phase);
    wave->phase -= int_part;
    wave->periods_passed += int_part;
  }
}

void wave_reset(wave_t* wave) {
  wave->phase = 0;
  wave->periods_passed = 0;
}

void wave_deinit(wave_t* wave) {
  if (wave->info->references == 0) return;
  wave->info->references--;
  if (wave->info->references == 0) {
    if (wave->info->user_data.deinit_fn != NULL) {
      wave->info->user_data.deinit_fn(wave->info->user_data.data):
    }
    
    wave->info->user_data.data = NULL;
  }
}

wave_info_t* wave_info_clone(wave_info_t* info) {
  info->references++;
  return info;
}

#define WAVE_USER_DATA_FLOAT(p)			\
  float* d = malloc(sizeof(float));		\
  *d = p;					\
  return (wave_user_data_t){ free, d };		\

float wave_square(float p, const void* d) {
  float duty = d == NULL ? 0.5 : *(float*)d;
  return p < duty ? 1.0f : -1.0f;
}

wave_user_data_t wave_square_user_data(float duty_cycle) {
  WAVE_USER_DATA_FLOAT(duty_cycle)
}

float wave_sine(float p, const void* d) {
  (void)d;
  return sin(2 * M_PI * p);
}

// TODO: Optimize
float wave_triangle(float p, const void* d) {
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
