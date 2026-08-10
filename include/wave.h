#pragma once

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

/// user_data must be a pointer to float, or NULL.
/// If it's a float, then it indicates the duty cycle of the wave. If
/// it's NULL, then the duty cycle is assumed to be 50%.
float wave_square(float x, float w, void* d);
/// user_data is not used.
float wave_sine  (float x, float w, void* d);
