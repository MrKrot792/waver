#include <stdlib.h>

#include "inputs/inits.h"
#include "inputs/inputs.h"

// TODO: Maybe remove/generalize helper functions
void input_init_static(input_info_static_t* input, float value) {
  *input = (input_info_static_t){
    INPUT_KIND_STATIC, 1, value,
  };
}

void input_init_lfo(input_info_lfo_t* input,
		    input_info_t* freq,
		    input_info_t* ampl,
		    wave_info_t*  wave) {
  *input = (input_info_lfo_t){
    INPUT_KIND_LFO, 1, freq, ampl, wave,
  };
}

void input_init_mixer(input_info_mixer_t* input,
		      input_mixer_mode_t mode,
		      input_info_t* i1,
		      input_info_t* i2) {
  *input = (input_info_mixer_t){
    INPUT_KIND_MIXER, 1, mode, i1, i2
  };
}

void input_init_pointer(input_info_pointer_t* input, float* pointer) {
  *input = (input_info_pointer_t){
    INPUT_KIND_POINTER, 1, pointer,
  };
}
