#if FEATURE_TUI
#pragma once

#include <stdint.h>

#include "backends/backends.h"

void tui_init();
// Starts a loop, which lasts until the user presses either C-c or q.
void tui_loop(backend_data_t clip);
void tui_deinit();
#endif // FEATURE_TUI
