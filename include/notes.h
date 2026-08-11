#pragma once // TODO: get rid of this thing
#include <stdint.h>

#include "wave.h"

typedef enum {
  Note_A,   Note_Ais, Note_B,   Note_C,
  Note_Cis, Note_D,   Note_Dis, Note_E,
  Note_F,   Note_Fis, Note_G,   Note_Gis 
} note;

typedef struct {
  float frequency;
  wave_fn instrument;
} note_duration;

float frequency_by_note(note n, uint32_t octave);
