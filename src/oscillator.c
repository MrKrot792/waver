#include "oscillator.h"
#include "inputs/inputs.h"
#include "wave.h"

float oscillator_get(const oscillator_t* osc) {
  // float f = input_get(osc->frequency);
  float a = input_get(osc->amplitude);
  return a * wave_get(&osc->wave);
}

void oscillator_tick(oscillator_t* osc) {
  wave_tick(&osc->wave, input_get(osc->frequency));
  input_tick(osc->frequency);
  input_tick(osc->amplitude);
}

void oscillator_deinit(oscillator_t* osc) {
  input_deinit(osc->frequency);
  input_deinit(osc->amplitude);
  if (osc->wave.user_data.deinit_fn != NULL)
    osc->wave.user_data.deinit_fn(osc->wave.user_data.data);
}
