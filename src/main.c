#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>

#include "config.h"
#include "interface.h"
#include "wave.h"
#include "samples.h"

#include "inputs/inputs.h"
#include "inputs/inits.h"

#include "backends/static.h"

// float wave(float x) {
//   float r = 0;

//   for (size_t i = 0; i < result_samples_count; i++) {
//     if (x < result_samples[i].start) continue;
//     if (x > result_samples[i].end)   continue;
//   }

//   return r;
// }

// float f(size_t i, float time) {
//   (void)i;
//   float w = wave(time);
//   return w;
// }

#define TAG(s)					\
  (uint32_t)(s[3] << 24 | s[2] << 16 |		\
	     s[1] << 8  | s[0])

typedef struct {
  uint16_t channels;
  uint32_t file_frequency;
  uint16_t bits_per_sample;
} sample_data_info_t;

typedef struct {
  uint32_t tag;
  uint32_t size;
  uint16_t audio_format;
  uint16_t channels;
  uint32_t file_frequency;
  uint32_t byte_per_sec;
  uint16_t byte_per_bloc;
  uint16_t bits_per_sample;
} fmt_t;

fmt_t fmt_make(sample_data_info_t* info, uint32_t file_frequency) {
  fmt_t r = {0};
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

void fmt_write(const fmt_t* format, FILE* fd) {
  fwrite(format, sizeof(fmt_t), 1, fd);
}

typedef struct {
  uint32_t bloc_id;
  uint32_t size;
  float* data;
} sample_data_t;

sample_data_t sample_make(sample_data_info_t* info) {
  sample_data_t r = {0};
  r.bloc_id = TAG("data");

  const uint32_t file_time = 20;
  const size_t data_size =
    info->file_frequency * file_time * info->channels *
    (info->bits_per_sample / 8);
  
  r.size = data_size;
  r.data = malloc(data_size);
  
  wave_set_sample_rate(info->file_frequency);

  // backend_static_generate(&result_samples,
  // 			  &result_samples_count,
  // 			  (backend_user_data_t){0})

  input_t one;
  input_init_static(&one, 1.f);
  
  input_t dc;
  input_init_static(&dc, 880.f);

  float timep = 1.f;
  
  input_t freq;
  input_init_pointer(&freq, &timep);

  input_t ampl;
  input_init_static(&ampl, 440.f);

  input_t frequency;
  input_init_lfo(&frequency, (oscillator_t){
    &freq,
    &ampl,
    (wave_t) {wave_triangle, wave_triangle_user_data(1), {0}},
  });
  
  input_t f;
  input_init_mixer(&f, (input_mixer_t){
    &frequency, input_clone(&one),
    &dc,        input_clone(&one)
  });
  
  input_t i;
  input_init_lfo(&i, (oscillator_t){
    &f, &one,
    (wave_t){
      .function = wave_square,
      .user_data = {0},
      .state = {0},
    },
  });
  
  for (size_t n = 0; n < data_size / (info->bits_per_sample / 8); n++) {
    //const float time = (float)n / (float)info->file_frequency;
    timep = 60;
    r.data[n] = 0.5 * input_get(&i);
    input_tick(&i);
  }
  
  input_deinit(&i);

  return r;
}

void sample_write(const sample_data_t* sample, FILE* fd) {
  fwrite(sample, sizeof(sample_data_t) - sizeof(float*), 1, fd);
  fwrite(sample->data, sample->size, 1, fd);
}

typedef struct {
  uint32_t tag;
  uint32_t size;
  uint32_t file_format;

  fmt_t format;
  sample_data_t data;
} riff_t;

riff_t riff_make(uint32_t file_frequency) {
  riff_t r = {0};
  r.tag = TAG("RIFF");
  r.file_format = TAG("WAVE");

  sample_data_info_t info = {0};
  r.format = fmt_make(&info, file_frequency);
  r.data   = sample_make(&info);

  r.size = r.format.size + r.data.size;

  return r;
}

void riff_write(const riff_t* riff, FILE* fd) {
  fwrite(riff, 4, 3, fd);
  fmt_write(&riff->format, fd);
  sample_write(&riff->data, fd);
}

typedef struct {
  const char* exe_name;
  const char* output_file_path;
  bool tui_mode;
  bool print_usage;
  bool verbose;
} arguments_t;

arguments_t parse_arguments(int argc, char* argv[]) {
  arguments_t args = {0};
  args.exe_name = argv[0];

  static struct option long_options[] = {
    {"help",    0, 0, 'h'},
    {"output",  1, 0, 'o'},
    {"tui",     0, 0, 't'},
    {"verbose", 0, 0, 'v'},
    {0,         0, 0,  0 }
  };

  for (;;) {
    int option_index;
    int c = getopt_long(argc, argv, "ho:tv",
			long_options, &option_index);
    if (c == -1) break;
    
    switch (c) {
    case 'h':
      args.print_usage = true;
      break;

    case 't':
      args.tui_mode = true;
      break;

    case 'o':
      args.output_file_path = optarg;
      break;

    case 'v':
      args.verbose = true;
      break;

    case '?':
      fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
      exit(-1);
      break;

    default:
      fprintf(stderr, "Something went wrong, code 0%x\n", c);
      break;
    }
  }

  if (args.output_file_path == NULL)
    args.output_file_path = "a.out";

  return args;
}

void print_usage(const char* exe_name, int (*print_fn)(const char *format, ...)) {
  print_fn("%s: [OPTIONS] -o FILE\n", exe_name);
  print_fn("OPTIONS:\n");
  print_fn("  -o --output [OUTPUT]  Specify the output file.\n"); 
  print_fn("  -h --help             Prints this help message.\n");
  print_fn("  -t --tui              Enable TUI mode. Must be compiled with FEATURE_TUI.\n");
  print_fn("  -v --verbose          Enables verbose logging.\n");
}

int main(int argc, char* argv[]) {
  arguments_t args = parse_arguments(argc, argv);

  if (args.print_usage) {
    print_usage(args.exe_name, printf);
    return 0;
  }

  if (args.tui_mode) {
#if FEATURE_TUI == 1
    tui_init();
    tui_loop(NULL, 0);
    tui_deinit();
#else
    fprintf(stderr, "%s: ERROR: Compiled without FEATURE_TUI.\n", args.exe_name);
#endif
    
  } else {
    printf("Exporting...\n");
    if (args.verbose) printf("[WAVS] Generating the soundwave...\n");
    riff_t riff = riff_make(44100);
    if (args.verbose) printf("[WAVS] Finished!\n");

    if (args.verbose)
      printf("[WAVS] Writing the sounds wave to a file %s...\n",
	     args.output_file_path);
    FILE* fd = fopen(args.output_file_path, "w");
    riff_write(&riff, fd);
    fclose(fd);
    if (args.verbose) printf("[WAVS] Finished!\n");
  }
}
