#include <stdlib.h>
#include <assert.h>

#include "inputs/inputs.h"
#include "inputs/inits.h"
#include "oscillator.h"

typedef float (*input_get_fn)(const input_t* input);

static float static_get(const input_t* input) {
  return ((const input_info_static_t*)input->info)->number;
}

typedef struct {
  uint32_t tick_number;
  oscillator_t osc;
} input_lfo_t;

static float lfo_get(const input_t* input) {
  return oscillator_get(&((const input_lfo_t*)(input->inner))->osc);
}

typedef struct {
  uint32_t tick_number;
  input_t i1;
  input_t i2;
} input_mixer_t;

static float mixer_get(const input_t* input) {
  const input_mixer_t* i = input->inner;
  if (((const input_info_mixer_t*)input->info)->mode == INPUT_MIXER_MODE_ADD)
    return input_get(&i->i1) + input_get(&i->i2);
  else
    return input_get(&i->i1) * input_get(&i->i2);
}

static float pointer_get(const input_t* input) {
  return *((const input_info_pointer_t*)input->info)->ptr;
}

// TODO: Dynamic runtime creation of input kinds 
static const input_get_fn input_kind_to_fn[] = {
  static_get, lfo_get, mixer_get, pointer_get
};

float input_get(const input_t* input) {
  return input_kind_to_fn[input->info->kind](input->inner);
}

typedef void (*input_tick_fn)(input_t* data);

static void no_tick(input_t* d) { (void)d; }
static void lfo_tick(input_t* d) {
  input_lfo_t* o = d;
  oscillator_tick(&o->osc);
}

static void mixer_tick(input_t* d) {
  input_mixer_t* i = d;
  input_tick(&i->i1);
  input_tick(&i->i2);
}

static const input_tick_fn input_kind_to_tick[] = {
  no_tick, lfo_tick, mixer_tick, no_tick
};

void input_tick(input_t* input) {
  input->inner->tick_number++;
  if (input->inner->tick_number == input->info->references) {
    input->inner->tick_number = 0;
    input_kind_to_tick[input->info->kind](input);
  }
}

typedef void (*input_deinit_fn)(input_t* data);

static void basic_deinit(input_t* d) { free(d->inner); }
static void lfo_deinit(input_t* d) {
  oscillator_deinit(&((input_lfo_t*)d->inner)->osc);
  free(d->inner);
}

static void mixer_deinit(input_t* d) {
  input_mixer_t* i = d->inner;
  input_deinit(&i->i1);
  input_deinit(&i->i2);
  free(d->inner);
}

static const input_deinit_fn input_kind_to_deinit[] = {
  basic_deinit, lfo_deinit, mixer_deinit, basic_deinit
};

void input_deinit(input_t* input) {
  if (input->info->references == 0) return;
  input->info->references--;
  if (input->info->references == 0) {
    input_kind_to_deinit[input->info->kind](input);
  }
}

input_info_t* input_info_clone(input_info_t* input) {
  input->references++;
  return input;
}

////////////////////////////////////////////////////////////

typedef void (*state_inner_alloc_fn)(input_inner_t** inner,
				     const input_info_t* info);

void basic_alloc(input_inner_t** inner, const input_info_t* info) {
  *inner = malloc(sizeof(input_inner_t));
  (*inner)->tick_number = 0;
}

void lfo_alloc(input_inner_t** inner, const input_info_t* i) {
  input_lfo_t* r = malloc(sizeof(input_lfo_t));
  r->tick_number = 0;
  input_lfo_t* info = (void*)i;

  input_t f, a;
  input_build(&f, &info->osc.frequency, NULL);
  input_build(&a, &info->osc.amplitude, NULL);
  
  r->osc = (oscillator_t){
    .frequency = f,
    .amplitude = a,
    .wave = (wave_t){
      .info = &info->osc.wave,
    },
  };

  *inner = (void*)r;
}

// TODO: Add user data
void mixer_alloc(input_inner_t** inner, const input_info_t* i) {
  input_mixer_t* r = malloc(sizeof(input_mixer_t));
  r->tick_number = 0;
  input_info_mixer_t* info = (void*)i;

  input_t i1, i2;
  
  input_build(&i1, info->i1, NULL);
  input_build(&i2, info->i2, NULL);

  r->i1 = i1;
  r->i2 = i2;
  
  *inner = (void*)r;
}

static const state_inner_alloc_fn kind_to_alloc_fn[] = {
  basic_alloc, lfo_alloc, mixer_alloc, basic_alloc
};

void input_build(input_t* state,
		 const input_info_t* info,
		 const input_user_data_t* user_data) {
  state->info = info;
  state->time = 0;
  state->user_data = user_data;
  kind_to_alloc_fn[info->kind](&state->inner, info);
}

typedef void (*reset_fn)(input_inner_t* inner);

void no_reset(input_inner_t* inner) { (void)inner; }

void lfo_reset(input_inner_t* inner) {
  oscillator_reset(&(((input_lfo_t*)inner)->osc));
}

void mixer_reset(input_inner_t* inner) {
  input_mixer_t* i = (input_inner_t*)inner;
  input_reset(&i->i1, NULL);
  input_reset(&i->i2, NULL);
}

static const reset_fn kind_to_reset_fn[] = {
  no_reset, lfo_reset, mixer_reset, no_reset
};

void input_reset(input_t* input, input_user_data_t* new_user_data) {
  if (new_user_data != NULL) {
    if (input->user_data != NULL) free(input->user_data);
    input->user_data = new_user_data;
  }
  input->time = 0;
  kind_to_reset_fn[input->info->kind](input->inner);
}
