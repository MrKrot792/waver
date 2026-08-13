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
  float sustain = sample_length - (mod.attack_time + mod.decay + mod.release);
  if (time <= mod.attack_time)
    return fmax(lerp(0, mod.attack_amplitude, time / mod.attack_time), 0);
  else if (time <= mod.attack_time + mod.decay)
    return lerp(mod.attack_amplitude, 1, (time - mod.attack_time) / mod.decay);
  else if (time <= mod.attack_time + mod.decay + sustain)
    return 1;
  else
    return fmax(lerp(1, 0, (time - (mod.attack_time + mod.decay + sustain)) / mod.release), 0);
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

//float sample_at_relative(float time, sample s);
