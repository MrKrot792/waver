#pragma once
 
#include "wave.h"
#include "inputs/inputs.h"

typedef struct {
  input_t frequency;
  input_t amplitude;
  wave_t  wave;
} oscillator_t;

float oscillator_get(const oscillator_t* osc);
void  oscillator_tick(oscillator_t* osc);
void  oscillator_reset(oscillator_t* osc);
void  oscillator_deinit(oscillator_t* osc);
