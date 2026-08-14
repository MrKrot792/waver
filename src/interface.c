#include "config.h"
#if FEATURE_TUI

#define TB_IMPL
#if defined(__clang__) && defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-function"
#  include "termbox2.h"
#  pragma GCC diagnostic pop
#else
#  include "termbox2.h"
#endif
#include "interface.h"

void tui_init() {
  tb_init();
}

void tui_loop(complete_sample_t* samples, uint32_t sample_count) {
  (void)samples;
  (void)sample_count;
  
  tb_printf(0, 0, 0, 0, "Hello, world!");
  tb_present();
  struct tb_event event = {0};
  tb_poll_event(&event);
  
}

void tui_deinit() {
  tb_shutdown();
}
#endif
