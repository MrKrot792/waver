#if FEATURE_TUI
#pragma once

#include <stdint.h>

#include "samples.h"

void tui_init();
// Starts a loop, which lasts until the user presses either C-c or q.
void tui_loop(sample_t* samples, uint32_t sample_count);
void tui_deinit();
#endif // FEATURE_TUI
