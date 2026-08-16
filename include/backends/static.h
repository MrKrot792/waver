#pragma once

#include <stdint.h>

#include "backends.h"
#include "wave.h"
#include "samples.h"

void backend_static_generate(
 complete_sample_t** samples,
 uint32_t* count,
 backend_user_data_t user_data
);
