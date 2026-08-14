#pragma once
#include <stdint.h>

#include "samples.h"

typedef void (*backend_user_data_deinit_fn)(void* d);

typedef struct {
  backend_user_data_deinit_fn deinit_fn;
  void* data;
} backend_user_data_t;

/// Params:
/// 1. samples: Output parameter. The implementation must populate it
///    with a real, non-NULL pointers to samples the implementation
///    generated.
/// 2. count: Output parameter. The implementation must populate it
///    with a count of samples, provided in `samples`.
/// 3. user_data: Input parameter. Used to pass arbitrary user data,
///    that isn't standard for all backends. Example: file to parse.
typedef void(*backend_generate_fn)(complete_sample_t** samples,
				   uint32_t* count,
				   backend_user_data_t user_data);
