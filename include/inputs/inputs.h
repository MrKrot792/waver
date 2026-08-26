#pragma once

#include <stdint.h>

typedef enum {
  INPUT_KIND_LFO,
  INPUT_KIND_MIXER,
  INPUT_KIND_POINTER,
  INPUT_KIND_STATIC,
} input_kind_t;

// Base structure. input_*_t will be cast to this.
typedef struct {
  input_kind_t kind;
  uint32_t references;
} input_info_t;

// Base structure, input_state_*_t will be cast to this.
typedef struct {
  // Everytime we tick, this goes up. Until it is equal to
  // `references`, only then we actually call the tick function.
  uint32_t tick_number;
} input_inner_t;

// Just a void*
typedef void input_user_data_t;

// Valid as long as input_info_t is valid is valid
typedef struct {
  const input_info_t* info;
  uint64_t time; // advances each sample (e.g. 44100 times/s)
  input_user_data_t* user_data;
  input_inner_t* inner;
} input_t;

void input_build(input_t* input,
		 const input_info_t* info,
		 const input_user_data_t* user_data);
float input_get(const input_t* input);
void  input_tick(input_t* input);
float input_get_time(const input_t* input);
// `new_user_data` can be NULL, which will leave it as it is
void input_reset(input_t* input, const input_user_data_t* new_user_data);
void input_deinit(input_t* input);

input_info_t* input_info_clone(input_info_t* input);
