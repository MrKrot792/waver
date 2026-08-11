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

// Potentially make a simple structure that contains
typedef struct {
  float frequency;
  float amplitude;
  wave_user_data instrument_user_data;
  wave_fn instrument;
  modulation modulation;
  float start;
  float end;
} sample;

float sample_at(float time, sample s);
//float sample_at_relative(float time, sample s);
