// Here, by "input" i mean anything that can be an LFO or something
// LFO-like. So like a knob.
#pragma once

#include <stdint.h>

typedef enum {
  INPUT_KIND_LFO,
  INPUT_KIND_MIXER,
  INPUT_KIND_POINTER,
  
  INPUT_KIND_STATIC, // Special input kind. Doesn't need allocation.
} input_kind_t;

typedef struct {
  input_kind_t kind;
  uint32_t references;
  float static_number; // To avoid allocating memory if it's just a number 
  void* data;
} input_t;

float input_get(const input_t* input);
void input_tick(input_t* input);
void input_deinit(input_t* input);
input_t* input_clone(input_t* input);
