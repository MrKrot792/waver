#include "backends/backends.h"
#include "samples.h"

static backend_generate_fn global_backend;
static backend_data_t      global_backend_data;

void backend_set_backend(backend_generate_fn backend) {
  global_backend = backend;
}

void backend_generate(backend_user_data_t user_data) {
  global_backend(&global_backend_data, user_data);
}

backend_data_t backend_get_data() {
  return global_backend_data;
}
