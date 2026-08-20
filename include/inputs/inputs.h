// Here, by "input" i mean anything that can be an LFO or something
// LFO-like. So like a knob.
#pragma once

#include <stdint.h>

typedef enum {
  INPUT_KIND_STATIC,
  INPUT_KIND_LFO,
  INPUT_KIND_MIXER,
} input_kind_t;

typedef struct {
  input_kind_t kind;
  uint32_t references;
  void* data;
} input_t;

float input_get(const input_t* input);
void input_tick(input_t* input);
// gotta `free` the pointer afterwards if you allocated it on hea
void input_deinit(input_t* input);
input_t* input_clone(input_t* input);
