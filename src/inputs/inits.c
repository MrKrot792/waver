#include <stdlib.h>

#include "inputs/inits.h"
#include "inputs/inputs.h"

void input_init_static(input_t* input, float value) {
  *input = (input_t){
    .kind = INPUT_KIND_STATIC,
    .references = 1,
    .static_number = value,
    .data = NULL,
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

void input_init_mixer(input_t* input, input_mixer_t mixer) {
  input_mixer_t* p = malloc(sizeof(input_mixer_t));
  *p = mixer;
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
