#pragma once

#include "inputs/inputs.h"
#include "oscillator.h"

void input_init_static(input_t* input, float value);
void input_init_lfo(input_t* input, oscillator_t osc);
void input_init_mixer(input_t* input,
		      input_t* i1, input_t* i1_volume,
		      input_t* i2, input_t* i2_volume);
void input_init_pointer(input_t* input, float* pointer);
