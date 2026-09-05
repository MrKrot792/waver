#pragma once

#include "inputs/inputs.h"
#include "oscillator.h"

typedef struct {
  input_kind_t kind;
  uint32_t references;
  float number;
} input_info_static_t;
void input_init_static(input_info_static_t* input, float value);

typedef struct {
  input_kind_t kind;
  uint32_t references;
  input_info_t* freq;
  input_info_t* ampl;
  wave_info_t*  wave;
} input_info_lfo_t;
void input_init_lfo(input_info_lfo_t* input,
		    input_info_t* freq,
		    input_info_t* ampl,
		    wave_info_t*  wave);

typedef enum {
  INPUT_MIXER_MODE_ADD,
  INPUT_MIXER_MODE_MULT,
} input_mixer_mode_t;

typedef struct {
  input_kind_t kind;
  uint32_t references;
  input_mixer_mode_t mode;
  input_info_t* i1;
  input_info_t* i2;
} input_info_mixer_t;
void input_init_mixer(input_info_mixer_t* input,
		      input_mixer_mode_t mode,
		      input_info_t* i1,
		      input_info_t* i2);

// TODO: Change the name 'pointer' to 'ptr'
typedef struct {
  input_kind_t kind;
  uint32_t references;
  float* ptr;
} input_info_pointer_t;
void input_init_pointer(input_info_pointer_t* input, float* pointer);
