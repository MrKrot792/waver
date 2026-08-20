#include <math.h>

#include "samples.h"

float frequency_get(frequency_t f) {
  if (f.kind == FREQUENCY_KIND_RAW) return f.data.raw;
  return 27.5f *
    powf(2.0f, (float)f.data.note.octave +
	       (float)f.data.note.note / 12.0f);
}

static inline float lerp(float a, float b, float t) {
  return a + t * (b - a);
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
