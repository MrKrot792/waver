#include <stdlib.h>

#include "inputs/inits.h"
#include "inputs/inputs.h"

void input_init_static(input_t* input, float value) {
  float* p = malloc(sizeof(float));
  *p = value;
  *input = (input_t){
    .kind = INPUT_KIND_STATIC,
    .references = 1,
    .data = p,
  };
}

void input_init_lfo(input_t* input, oscillator_t osc) {
  oscillator_t* p = malloc(sizeof(oscillator_t));
  *p = osc;
  *input = (input_t){
    .kind = INPUT_KIND_LFO,
    .references = 1,
    .data = p,
  };
}
// TODO: This is declared in two files. Fix it.
typedef struct {
  input_t* i1;
  input_t* i1_volume;
  input_t* i2;
  input_t* i2_volume;
} input_mixer_t;

void input_init_mixer(input_t* input,
		      input_t* i1, input_t* i1_volume,
		      input_t* i2, input_t* i2_volume) {
  input_mixer_t* p = malloc(sizeof(input_mixer_t));
  *p = (input_mixer_t){i1, i1_volume, i2, i2_volume};
  *input = (input_t){
    .kind = INPUT_KIND_MIXER,
    .references = 1,
    .data = p,
  };
}

void input_init_pointer(input_t* input, float* pointer) {
  float** p = malloc(sizeof(void*));
  *p = pointer;
  *input = (input_t){
    .kind = INPUT_KIND_POINTER,
    .references = 1,
    .data = p,
  };
}
