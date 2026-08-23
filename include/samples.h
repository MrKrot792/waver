#pragma once // TODO: get rid of this thing
#include <stdint.h>

#include "inputs/inputs.h"
#include "wave.h"

typedef enum {
  NOTE_A,   NOTE_AIS, NOTE_B,   NOTE_C,
  NOTE_CIS, NOTE_D,   NOTE_DIS, NOTE_E,
  NOTE_F,   NOTE_FIS, NOTE_G,   NOTE_GIS 
} frequency_note_t;

typedef enum {
  FREQUENCY_KIND_NOTE,
  FREQUENCY_KIND_RAW,
} frequency_kind_t;

typedef struct {
  frequency_kind_t kind;
  union {
    struct {
      frequency_note_t note;
      uint32_t octave;
    } note;
    float raw;
  } data;
} frequency_t;

float frequency_get(frequency_t f);

typedef struct {
  float start;
  float duration;
} span_t;

// TODO: Reference counting
typedef struct {
  input_t*     function;
  frequency_t* notes;
  span_t*      durations;
  uint32_t     count;
} clip_t;
