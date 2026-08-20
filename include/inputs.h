// Here, by "input" i mean anything that can be an LFO or something
// LFO-like. So like a knob.
#pragma once

#include "oscillator.h"

typedef struct input_t input_t;

void input_init_static(input_t* input, float value);
void input_init_lfo(input_t* input, oscillator_t osc);
void input_init_mixer(input_t* input,
		      input_t* i1, input_t* i1_volume,
		      input_t* i2, input_t* i2_volume);

float input_get(input_t* input);
// gotta `free` the pointer afterwards if you allocated it on heap
void input_deinit(input_t* input);
input_t* input_clone(input_t* input);
