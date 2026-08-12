#pragma once

#include <stdbool.h>

/// __IMPORTANT__ If the third parameter is NULL, the function must
/// assume some sane defaults.
/// 
/// Returns:
///   Wave amplitude at the current time, which *must* be between -1.0
/// and +1.0.
/// 
/// Params:
/// 1. time      - Current time in seconds.
/// 2. frequency - Wave's frequency.
/// 3. user_data - Optional additional user data.
/// 
typedef float (*wave_fn)(float time, float frequency, void* user_data);

typedef void (*wave_user_data_deinit_fn)(void* data);

// TODO: Rename everything to `type_t`
// TODO: Potentially add reference counting.
/// It is safe to null initialize this struct.
typedef struct {
  void* data;
  bool deinit_fn_present;
  wave_user_data_deinit_fn deinit_fn;
} wave_user_data;

/// User data must be a pointer to a float, or NULL.
/// If it's a float, then it indicates the duty cycle of the wave. If
/// it's NULL, then the duty cycle is assumed to be 50%.
float wave_square(float x, float w, void* d);
wave_user_data wave_square_user_data(float duty_cycle);
/// User data is not used.
float wave_sine (float x, float w, void* d);
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
float wave_triangle(float x, float w, void* d);
wave_user_data wave_triangle_user_data(float shape);
