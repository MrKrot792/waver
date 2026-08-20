#include <stdlib.h>

#include "inputs/inputs.h"
#include "oscillator.h"

typedef float (*input_fn)(const void* input);

static float static_get(const void* input) {
  return *(float*)input;
}

static float lfo_get(const void* input) {
  return oscillator_get((const oscillator_t*)input);
}

typedef struct {
  input_t* i1;
  input_t* i1_volume;
  input_t* i2;
  input_t* i2_volume;
} input_mixer_t;

static float mixer_get(const void* input) {
  const input_mixer_t* i = input;
  return input_get(i->i1) * input_get(i->i1_volume) +
         input_get(i->i2) * input_get(i->i2_volume);
}

static float pointer_get(const void* input) {
  return **(float**)input;
}

static const input_fn input_kind_to_fn[] = {
  static_get, lfo_get, mixer_get, pointer_get
};

typedef void (*input_deinit_fn)(void* data);

static void simple_deinit(void* d) { free(d); }
static void lfo_deinit(void* d) {
  oscillator_deinit((oscillator_t*)d);
  free(d);
}

static void mixer_deinit(void* d) {
  input_mixer_t* i = d;
  input_deinit(i->i1);
  input_deinit(i->i1_volume);
  input_deinit(i->i2);
  input_deinit(i->i2_volume);
  free(i);
}

static const input_deinit_fn input_kind_to_deinit[] = {
  simple_deinit, lfo_deinit, mixer_deinit, simple_deinit
};

typedef void (*input_tick_fn)(void* data);

static void no_tick(void* d) { (void)d; }
static void lfo_tick(void* d) {
  oscillator_t* o = d;
  oscillator_tick(o);
}

static void mixer_tick(void* d) {
  input_mixer_t* i = d;
  input_tick(i->i1);
  input_tick(i->i1_volume);
  input_tick(i->i2);
  input_tick(i->i2_volume);
}

static const input_tick_fn input_kind_to_tick[] = {
  no_tick, lfo_tick, mixer_tick, no_tick
};

void input_tick(input_t* input) {
  input_kind_to_tick[input->kind](input->data);
}

float input_get(const input_t* input) {
  return input_kind_to_fn[input->kind](input->data);
}

void input_deinit(input_t* input) {
  input->references--;
  if (input->references == 0) {
    input_kind_to_deinit[input->kind](input->data);
  }
}

input_t* input_clone(input_t* input) {
  input->references++;
  return input;
}
