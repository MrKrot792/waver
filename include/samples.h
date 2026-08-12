#pragma once // TODO: get rid of this thing
#include <stdint.h>

#include "wave.h"

typedef enum {
  NOTE_A,   NOTE_AIS, NOTE_B,   NOTE_C,
  NOTE_CIS, NOTE_D,   NOTE_DIS, NOTE_E,
  NOTE_F,   NOTE_FIS, NOTE_G,   NOTE_GIS 
} note;

float frequency_by_note(note n, uint32_t octave);

// Subject to change.
typedef struct {
  float attack_time;      // total attack time
  float attack_amplitude; // relative; 1.5 means total_amplitude * 1.5
  float decay;            // total decay time, etc
  float release;
} modulation;

static const modulation modulation_basic = (modulation){
  .attack_time = 0.1f,
  .attack_amplitude = 1.2f,
  .decay = 0.1f,
  .release = 0.25f,
};

float modulate(float time, float sample_length, modulation mod);

void set_sample_rate(uint32_t s);

// TODO: Allocation of sample + freeing and referencing counting.
// TODO: Potentially custom modulators (and rename them to envelopes).
typedef struct {
  float frequency;
  float amplitude; // TODO: Potentially move this to the modulator
  wave_user_data instrument_user_data;
  wave_fn instrument;
  modulation modulation;
  float length; // Total length in seconds.
} sample;

typedef struct {
  float phase; // From 0 to 1. Loops.
  uint32_t periods_passed; // Increments each time phase loops.
} sample_state;

// TODO: Global state sample rate
float sample_next(sample s, sample_state* state, bool total_length_includes_release);
//float sample_at_relative(float time, sample s);
