#include <math.h>

#include "samples.h"

float frequency_get(frequency_t f) {
  if (f.kind == FREQUENCY_KIND_RAW) return f.data.raw;
  return 27.5f *
    powf(2.0f, (float)f.data.note.octave +
	       (float)f.data.note.note / 12.0f);
}
