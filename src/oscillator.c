#include "oscillator.h"
#include "wave.h"

float oscillator_get(oscillator_t* osc, float frequency) {
  float f = frequency * input_get(osc->frequency);
  float a = input_get(osc->amplitude);
  return a * wave_get(&osc->wave, f);
}

void oscillator_deinit(oscillator_t* osc) {
  input_deinit(osc->frequency);
  input_deinit(osc->amplitude);
  osc->wave.user_data.deinit_fn(osc->wave.user_data.data);
}
