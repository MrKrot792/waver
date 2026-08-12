#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "wave.h"
#include "samples.h"

typedef struct {
  sample s;
  sample_state st;
  float start;
  float end;
} complete_sample;

complete_sample* notes = NULL;
uint32_t max_note = 0;

float wave(float x) {
  float r = 0;

  for (size_t i = 0; i < max_note; i++) {
    if (x < notes[i].start) continue;
    if (x > notes[i].end)   continue;
    r += sample_next(notes[i].s, &notes[i].st, true);
  }

  return r;
}

float f(size_t i, float time) {
  (void)i;
  float w = wave(time);
  return w;
}

#define TAG(s)					\
  (uint32_t)(s[3] << 24 | s[2] << 16 |		\
	     s[1] << 8  | s[0])

typedef struct {
  uint16_t channels;
  uint32_t file_frequency;
  uint16_t bits_per_sample;
} sample_data_info;

typedef struct {
  uint32_t tag;
  uint32_t size;
  uint16_t audio_format;
  uint16_t channels;
  uint32_t file_frequency;
  uint32_t byte_per_sec;
  uint16_t byte_per_bloc;
  uint16_t bits_per_sample;
} fmt;

fmt fmt_make(sample_data_info* info, uint32_t file_frequency) {
  fmt r = {0};
  r.tag = TAG("fmt ");
  r.size = 0x10;
  r.audio_format = 3; // 1 - int, 3 - float
  r.channels = 1;
  r.file_frequency  = file_frequency;
  r.bits_per_sample = 32;
  r.byte_per_bloc   = r.channels * r.bits_per_sample / 8;
  r.byte_per_sec    = r.byte_per_bloc * r.file_frequency;

  info->channels        = r.channels;
  info->file_frequency  = file_frequency;
  info->bits_per_sample = r.bits_per_sample;

  return r;
}

void fmt_write(const fmt* format, FILE* fd) {
  fwrite(format, sizeof(fmt), 1, fd);
}

typedef struct {
  uint32_t bloc_id;
  uint32_t size;
  float* data;
} sample_data;

sample make_note(float duration, note n, uint32_t octave) {
  max_note++;
  return (sample){
    frequency_by_note(n, octave),
    1.0,
    {0},
    wave_triangle,
    (modulation){0.05, 1, 0.05, 0.2},
    duration,
  };
}

void note_append(sample s) {
  static uint32_t last = 0;
  static float last_time = 0;
  notes[last] = (complete_sample){s, {0}, last_time, last_time + s.duration};
  last_time += s.duration;
  last++;
}

sample_data sample_make(sample_data_info* info) {
  sample_data r = {0};
  r.bloc_id = TAG("data");

  const uint32_t file_time = 20;
  const size_t data_size =
    info->file_frequency * file_time * info->channels *
    (info->bits_per_sample / 8);
  
  r.size = data_size;
  r.data = malloc(data_size);

  float pbm = 104;

  float half_tone_time = 60.0 / pbm;

  notes = malloc(128 * sizeof(complete_sample));

  notes[0] = (complete_sample){
    (sample){220, 1, {0}, wave_triangle, modulation_basic, 10.0},
    (sample_state){0},
    0,
    10,
  };
  
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

  set_sample_rate(info->file_frequency);
  
  for (size_t i = 0; i < data_size / (info->bits_per_sample / 8); i++) {
    const float time = (float)i / (float)info->file_frequency;
    r.data[i] = (float)f(i, time);
  }

  return r;
}

void sample_write(const sample_data* sample, FILE* fd) {
  fwrite(sample, sizeof(sample_data) - sizeof(float*), 1, fd);
  fwrite(sample->data, sample->size, 1, fd);
}

typedef struct {
  uint32_t tag;
  uint32_t size;
  uint32_t file_format;

  fmt format;
  sample_data data;
} riff;

riff riff_make(uint32_t file_frequency) {
  riff r = {0};
  r.tag = TAG("RIFF");
  r.file_format = TAG("WAVE");

  sample_data_info info = {0};
  r.format = fmt_make(&info, file_frequency);
  r.data   = sample_make(&info);

  r.size = r.format.size + r.data.size;

  return r;
}

void riff_write(const riff* riff, FILE* fd) {
  fwrite(riff, 4, 3, fd);
  fmt_write(&riff->format, fd);
  sample_write(&riff->data, fd);
}

int main(int argc, char* argv[]) {
  if (argc <2) {
    fprintf(stderr, "Not enough arguments\n");
    return -1;
  }

  printf("[WAVS] Generating the soundwave...\n");
  riff riff = riff_make(44100);
  printf("[WAVS] Finished!\n");

  printf("[WAVS] Writing the sounds wave to a file...\n");
  FILE* fd = fopen(argv[1], "w");
  riff_write(&riff, fd);
  fclose(fd);
  printf("[WAVS] Finished!\n");
}
