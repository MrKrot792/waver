#include <math.h>

#include "notes.h"

float frequency_by_note(Note n, uint32_t octave) {
  return 27.5f * powf(2.0f, (float)octave + (float)n / 12.0f);
}
