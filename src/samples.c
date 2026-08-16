#include <math.h>

#include "samples.h"

static inline float lerp(float a, float b, float t) {
  return a + t * (b - a);
}

static uint32_t sample_rate = 0;

void set_sample_rate(uint32_t s) {
  sample_rate = s;
}

float frequency_by_note(note_t n, uint32_t octave) {
  return 27.5f * powf(2.0f, (float)octave + (float)n / 12.0f);
}

float modulate(float time, float sample_length, modulation_t mod) {
  float sustain_len = sample_length - (mod.attack + mod.decay + mod.release);
  float r;
  if (time <= mod.attack)
    r = lerp(0, 1, time / mod.attack);
  else if (time <= mod.attack + mod.decay)
    r = lerp(1, mod.sustain, (time - mod.attack) / mod.decay);
  else if (time <= mod.attack + mod.decay + sustain_len)
    r = mod.sustain;
  else
    r = lerp(mod.sustain, 0, (time - (mod.attack + mod.decay + sustain_len)) / mod.release);
  
  return fmin(1, fmax(0, r));
}

// TODO: Potentially move total_length_includes_release to either
// modulation, or sample.
float sample_next(sample_t s, sample_state_t* state, bool total_length_includes_release) {
  float r = s.instrument(state->phase, s.instrument_user_data.data);
  r *= s.amplitude;
  float release_time =
    total_length_includes_release ? 0 : s.modulation.release;
  r *= modulate(((float)state->periods_passed + state->phase) / s.frequency,
		s.duration + release_time, s.modulation);

  state->phase += s.frequency / sample_rate;
  if (state->phase >= 1.0f) {
    float int_part = floorf(state->phase);
    state->phase -= int_part;
    state->periods_passed += int_part;
  }
  
  return r;
}

void sample_free(sample_t s) {
  if (s.instrument_user_data.deinit_fn != NULL)
    s.instrument_user_data.deinit_fn(s.instrument_user_data.data);
}
