#include "oscillator.h"
#include "inputs/inputs.h"
#include "wave.h"

float oscillator_get(const oscillator_t* osc) {
  float a = input_get(&osc->amplitude);
  return a * wave_get(&osc->wave);
}

void oscillator_tick(oscillator_t* osc) {
  wave_tick(&osc->wave, input_get(&osc->frequency));
  input_tick(&osc->frequency);
  input_tick(&osc->amplitude);
}

void oscillator_reset(oscillator_t* osc) {
  wave_reset(&osc->wave);
  input_reset(&osc->frequency, NULL);
  input_reset(&osc->amplitude, NULL);
}

void oscillator_deinit(oscillator_t* osc) {
  wave_deinit(&osc->wave);
  input_deinit(&osc->frequency);
  input_deinit(&osc->amplitude);
}
