#pragma once // TODO: get rid of this thing
#include <stdint.h>

#include "wave.h"

typedef enum {
  NOTE_A,   NOTE_AIS, NOTE_B,   NOTE_C,
  NOTE_CIS, NOTE_D,   NOTE_DIS, NOTE_E,
  NOTE_F,   NOTE_FIS, NOTE_G,   NOTE_GIS 
} note_t;

float frequency_by_note(note_t n, uint32_t octave);

// Standard ASDR. Nothing to explain.
typedef struct {
  float attack;
  float sustain;
  float decay;
  float release;
} modulation_t;

static const modulation_t modulation_basic = (modulation_t){
  .attack  = 0.02f,
  .decay   = 0.1f,
  .sustain = 0.7f,
  .release = 0.10f,
};

float modulate(float time, float sample_length, modulation_t mod);

void set_sample_rate(uint32_t s);

// TODO: Allocation of sample + freeing and referencing counting.
// TODO: Potentially custom modulators (and rename them to envelopes).
// TODO: Pluggable functions for frequencies, and maybe something
// else. And let amplitude be managed by the modulator.
typedef struct {
  float frequency;
  float amplitude; // TODO: Potentially move this to the modulator
  wave_user_data_t instrument_user_data;
  wave_fn instrument;
  modulation_t modulation;
  float duration; // Total length in seconds.
} sample_t;

typedef struct {
  float phase;             // From 0 to 1. Loops.
  uint32_t periods_passed; // Increments each time phase loops.
} sample_state_t;

// TODO: Integrate this into the sample_next function.
// Or think of another solution, as this looks out of place for now.
typedef struct {
  sample_t s;
  sample_state_t st;
  float start;
  float end;
} complete_sample_t;

float sample_next(sample_t s, sample_state_t* state, bool total_length_includes_release);
void sample_free(sample_t s);
