#pragma once
#include <stdint.h>

#include "samples.h"

typedef void (*backend_user_data_deinit_fn)(void* d);

typedef struct {
  backend_user_data_deinit_fn deinit_fn;
  void* data;
} backend_user_data_t;

typedef struct {
  clip_t* clips;
  uint32_t count;
} backend_data_t;

/// Params:
/// 1. data: Output parameter. The implementation must
///    populate `data->samples` with a real, non-NULL pointers to samples
///    the implementation generated. The implementation must also
///    populate `data->count` with a count of samples, provided in
///    `data->samples`.
/// 2. user_data: Input parameter. Used to pass arbitrary user data,
///    that isn't standard for all backends. For example: a file to
///    parse.
typedef void(*backend_generate_fn)(backend_data_t* data,
				   backend_user_data_t user_data);

void backend_set_backend(backend_generate_fn backend);
void backend_generate(backend_user_data_t user_data);
backend_data_t backend_get_data();
