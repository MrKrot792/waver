#include "inputs.h"
#include "oscillator.h"

typedef float (*input_fn)(void* input);

static float static_get(void* input) {
  return *(float*)input;
}

static float lfo_get(void* input) {
  return oscillator_get((oscillator_t*)input);
}

typedef struct {
  input_t* i1;
  input_t* i1_volume;
  input_t* i2;
  input_t* i2_volume;
} input_mixer_t;

static float mixer_get(void* input) {
  input_mixer_t* i = input;
  return input_get(i->i1) * input_get(i->i1_volume) +
         input_get(i->i2) * input_get(i->i2_volume);
}

static const input_fn input_kind_to_fn[] = {
  static_get, lfo_get, mixer_get,
};

typedef void (*input_deinit_fn)(void* data);

static void do_not_deinit(void* d) {(void)d;}
static void lfo_deinit(void* d) {
  oscillator_deinit((oscillator_t*)d)
}

static void mixer_deinit(void* d) {
  input_mixer_t* i = d;
  input_deinit(d->i1);
  input_deinit(d->i1_volume);
  input_deinit(d->i2);
  input_deinit(d->i2_volume);
}

static const input_deinit_fn input_kind_to_deinit[] = {
  do_not_deinit, lfo_deinit, mixer_deinit,
};

typedef enum {
  INPUT_KIND_STATIC,
  INPUT_KIND_LFO,
  INPUT_KIND_MIXER,
} input_kind_t;

struct input_t {
  input_kind_t kind;
  uint32_t references;
  void* data;
};

float input_get(input_t* input) {
  return input_kind_to_fn[input->kind](input->data);
}

void input_deinit(input_t* input) {
  input.references--;
  if (input.references == 0) {
    input_kind_to_deinit[input->kind](input->data);
  }
}

input_t* input_clone(input_t* input) {
  input->references++;
  return input;
}

void input_init_static(input_t* input, float value) {
  float* p = malloc(sizeof(float));
  *p = value;
  *input = (input_t){
    .kind = INPUT_KIND_STATIC,
    .references = 1,
    .data = p,
  };
}

void input_init_lfo(input_t* input, oscillator_t osc) {
  float* p = malloc(sizeof(oscillator_t));
  *p = osc;
  *input = (input_t){
    .kind = INPUT_KIND_LFO,
    .references = 1,
    .data = p,
  };
}

void input_init_mixer(input_t* input,
		      input_t* i1, input_t* i1_volume,
		      input_t* i2, input_t* i2_volume) {
  float* p = malloc(sizeof(input_mixer_t));
  *p = (input_mixer_t){i1, i1_volume, i2, i2_volume};
  *input = (input_t){
    .kind = INPUT_KIND_LFO,
    .references = 1,
    .data = p,
  };
}
