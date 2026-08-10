#pragma once // TODO: get rid of this thing
#include <stdint.h>

typedef enum {
  Note_A,   Note_Ais, Note_B,   Note_C,
  Note_Cis, Note_D,   Note_Dis, Note_E,
  Note_F,   Note_Fis, Note_G,   Note_Gis 
} Note;

float frequency_by_note(Note n, uint32_t octave);
