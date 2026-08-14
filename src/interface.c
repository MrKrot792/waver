#include "config.h"
#if FEATURE_TUI

#define TB_IMPL
#include "termbox2.h"
#include "interface.h"

void tui_init() {
  tb_init();
}

void tui_loop(complete_sample_t* samples, uint32_t sample_count) {
  tb_printf(0, 0, 0, 0, "Hello, world!");
  tb_present();
  struct tb_event event = {0};
  tb_poll_event(&event);
  
}

void tui_deinit() {
  tb_shutdown();
}
#endif
