#pragma once

#include "inputs.h"
#include "wave.h"

typedef struct {
  input_t* frequency;
  input_t* amplitude;
  wave_t   wave;
} oscillator_t;

float oscillator_get(oscillator_t* osc, float frequency);
void oscillator_deinit(oscillator_t* osc);
