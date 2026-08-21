#pragma once

#include "inputs/inputs.h"
#include "oscillator.h"

void input_init_static(input_t* input, float value);
void input_init_lfo(input_t* input, oscillator_t osc);

typedef struct {
  input_t* i1;
  input_t* i1_volume;
  input_t* i2;
  input_t* i2_volume;
} input_mixer_t;

void input_init_mixer(input_t* input, input_mixer_t mixer);
void input_init_pointer(input_t* input, float* pointer);
