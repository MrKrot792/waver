#include <math.h>

#include "samples.h"

static inline float lerp(float a, float b, float t) {
  return a + t * (b - a);
}

float frequency_by_note(note n, uint32_t octave) {
  return 27.5f * powf(2.0f, (float)octave + (float)n / 12.0f);
}

float modulate(float time, float sample_length, modulation mod) {
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
float sample_at(float time, sample s, bool total_length_includes_release) {
  float r = s.instrument(time, s.frequency, s.instrument_user_data.data);
  r *= s.amplitude;
  if (time >= s.start) {
    float release_time =
      total_length_includes_release ? 0 : s.modulation.release;
    r *= modulate(time - s.start, s.end - s.start + release_time, s.modulation);
  }
  else r = 0;
  return r;
}

//float sample_at_relative(float time, sample s);
