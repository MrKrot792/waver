#pragma once

#include <stdint.h>
#include <stdbool.h>

/// __IMPORTANT__ If the third parameter is NULL, the function must
/// assume some sane defaults.
/// 
/// Returns:
///   Wave amplitude in the current phase, which *must* be between
///   -1.0 and +1.0.
/// 
/// Params:
/// 1. phase     - Current phase in revolutions (1 rev = 360°).
/// 2. user_data - Optional additional user data.
/// 
typedef float (*wave_fn)(float phase, const void* user_data);

typedef void (*wave_user_data_deinit_fn)(void* data);

// TODO: Potentially add reference counting.
/// It is safe to null initialize this struct.
typedef struct {
  wave_user_data_deinit_fn deinit_fn;
  void* data;
} wave_user_data_t;

typedef struct {
  const wave_fn function;
  wave_user_data_t user_data;
  uint32_t references;
} wave_info_t;

typedef struct {
  wave_info_t* info;
  float phase;             // From 0 to 1. Loops.
  uint32_t periods_passed; // Increments each time phase loops.
} wave_t;

void wave_set_sample_rate(float s);

float wave_get(const wave_t* wave);
void  wave_tick(wave_t* wave, float frequency);
void  wave_deinit(wave_t* wave);
wave_info_t* wave_info_clone(wave_info_t* info);

// TODO: input_t in wave user data

/// User data must be a pointer to a float, or NULL.
/// If it's a float, then it indicates the duty cycle of the wave. If
/// it's NULL, then the duty cycle is assumed to be 50%.
float wave_square(float p, const void* d);
wave_user_data_t wave_square_user_data(float duty_cycle);
/// User data is not used.
float wave_sine (float p, const void* d);
/// User data must be a pointer to a float, or NULL.
/// If it's a float, then it indicates the displacement of the
/// triangle wave's peak. I. e. if it's -1.0, then the triangle wave
/// converts into a sawtooth wave. If it's +1.0 instead, it's also a
/// sawtooth wave, but the other way around.
///
/// i.e, -1:
/// 
///    |\    |\
///    | \   | \
/// \--|--\--|--\
///  \ |   \ |
///   \|    \|
///
/// 0:
///
///        /\
///       /  \
/// \----/----\----/
///  \  /      \  /
///   \/        \/
///
/// and +1:
///   /|    /|
///  / |   / |
/// /--|--/--|--/
///    | /   | /
///    |/    |/
float wave_triangle(float p, const void* d);
wave_user_data_t wave_triangle_user_data(float shape);
