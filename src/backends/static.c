#include <stdlib.h>

#include "backends/static.h"
#include "samples.h"
#include "wave.h"
#include "backends/backends.h"

static complete_sample_t* notes = NULL;
static uint32_t* max_note = 0;

static sample_t make_note(float duration, note_t n, uint32_t octave) {
  (*max_note)++;
  return (sample_t){
    frequency_by_note(n, octave),
    1.0,
    {0},
    wave_triangle,
    (modulation_t){0.01, 0.25, 0.7, 0.15},
    duration,
  };
}

static void note_append(sample_t s) {
  static uint32_t last = 0;
  static float last_time = 0;
  notes[last] = (complete_sample_t){s, {0}, last_time, last_time + s.duration};
  last_time += s.duration;
  last++;
}

void backend_static_generate(
   complete_sample_t** samples,
   uint32_t* count,
   backend_user_data_t user_data
) {
  (void)user_data;
  *samples = malloc(128 * sizeof(complete_sample_t));
  notes = *samples;
  max_note = count;

  float pbm = 104;
  float half_tone_time = 60.0 / pbm;
  
  note_append(make_note(half_tone_time, NOTE_C, 5));
  note_append(make_note(half_tone_time, NOTE_A, 4));
  note_append(make_note(half_tone_time, NOTE_E, 5));
  note_append(make_note(half_tone_time, NOTE_A, 4));
  
  note_append(make_note(half_tone_time, NOTE_C, 5));
  note_append(make_note(half_tone_time, NOTE_A, 4));
  note_append(make_note(half_tone_time, NOTE_C, 5));
  note_append(make_note(half_tone_time/2.0,NOTE_D, 4));
  note_append(make_note(half_tone_time/2.0,NOTE_E, 5));

  note_append(make_note(half_tone_time, NOTE_B, 4));
  note_append(make_note(half_tone_time, NOTE_G, 4));
  note_append(make_note(half_tone_time, NOTE_E, 5));
  note_append(make_note(half_tone_time, NOTE_G, 4));

  note_append(make_note(half_tone_time, NOTE_B, 4));
  note_append(make_note(half_tone_time, NOTE_G, 4));
  note_append(make_note(half_tone_time, NOTE_B, 4));
  note_append(make_note(half_tone_time/2.0,NOTE_C, 5));
  note_append(make_note(half_tone_time/2.0,NOTE_D, 5));

  note_append(make_note(half_tone_time, NOTE_A, 4));
  note_append(make_note(half_tone_time, NOTE_F, 4));
  note_append(make_note(half_tone_time, NOTE_C, 5));
  note_append(make_note(half_tone_time, NOTE_F, 4));

  note_append(make_note(half_tone_time, NOTE_A, 4));
  note_append(make_note(half_tone_time, NOTE_F, 4));
  note_append(make_note(half_tone_time, NOTE_A, 4));
  note_append(make_note(half_tone_time/2.0, NOTE_B, 4));
  note_append(make_note(half_tone_time/2.0, NOTE_C, 5));

  note_append(make_note(half_tone_time, NOTE_B, 4));
  note_append(make_note(half_tone_time, NOTE_GIS, 4));
  note_append(make_note(half_tone_time, NOTE_E, 4));
  note_append(make_note(half_tone_time, NOTE_G, 4));

  note_append(make_note(half_tone_time, NOTE_D, 4));
  note_append(make_note(half_tone_time, NOTE_F, 4));
  note_append(make_note(half_tone_time, NOTE_E, 4));
  note_append(make_note(half_tone_time, NOTE_D, 4));
}
