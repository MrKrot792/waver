#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "wave.h"
#include "notes.h"

#define TAG(s)					\
  (uint32_t)(s[3] << 24 | s[2] << 16 |		\
	     s[1] << 8  | s[0])

typedef struct {
  uint16_t channels;
  uint32_t file_frequency;
  uint16_t bits_per_sample;
} SampleDataInfo;

#define FREQ 440

float wave(float x) {
  float r = 0;
  float w = 2 * M_PI * 440;

  r = wave_sine(x, w, NULL);
  
  return r;
}

float f(size_t i, float time) {
  (void)i;
  float w = wave(time);
  return w;
}

typedef struct {
  uint32_t tag;
  uint32_t size;
  uint16_t audio_format;
  uint16_t channels;
  uint32_t file_frequency;
  uint32_t byte_per_sec;
  uint16_t byte_per_bloc;
  uint16_t bits_per_sample;
} Fmt;

Fmt fmt_make(SampleDataInfo* info, uint32_t file_frequency) {
  Fmt r = {0};
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

void fmt_write(const Fmt* fmt, FILE* fd) {
  fwrite(fmt, sizeof(Fmt), 1, fd);
}

typedef struct {
  uint32_t bloc_id;
  uint32_t size;
  float* data;
} SampleData;

SampleData sample_make(SampleDataInfo* info) {
  SampleData r = {0};
  r.bloc_id = TAG("data");

  const uint32_t file_time = 42;
  const size_t data_size =
    info->file_frequency * file_time * info->channels *
    (info->bits_per_sample / 8);
  
  r.size = data_size;
  r.data = malloc(data_size);

  for (size_t i = 0; i < data_size / (info->bits_per_sample / 8); i++) {
    const float time = (float)i / (float)info->file_frequency;
    r.data[i] = f(i, time);
  }

  return r;
}

void sample_write(const SampleData* sample, FILE* fd) {
  fwrite(sample, sizeof(SampleData) - sizeof(float*), 1, fd);
  fwrite(sample->data, sample->size, 1, fd);
}

typedef struct {
  uint32_t tag;
  uint32_t size;
  uint32_t file_format;

  Fmt fmt;
  SampleData data;
} Riff;

Riff riff_make(uint32_t file_frequency) {
  Riff r = {0};
  r.tag = TAG("RIFF");
  r.file_format = TAG("WAVE");

  SampleDataInfo info = {0};
  r.fmt  = fmt_make(&info, file_frequency);
  r.data = sample_make(&info);

  r.size = r.fmt.size + r.data.size;

  return r;
}

void riff_write(const Riff* riff, FILE* fd) {
  fwrite(riff, 4, 3, fd);
  fmt_write(&riff->fmt, fd);
  sample_write(&riff->data, fd);
}

int main(int argc, char* argv[]) {
  if (argc <2) {
    fprintf(stderr, "Not enough arguments\n");
    return -1;
  }

  printf("[WAVS] Generating the soundwave...\n");
  Riff riff = riff_make(44100);
  printf("[WAVS] Finished!\n");

  printf("[WAVS] Writing the sounds wave to a file...\n");
  FILE* fd = fopen(argv[1], "w");
  riff_write(&riff, fd);
  fclose(fd);
  printf("[WAVS] Finished!\n");
}
