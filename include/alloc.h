#pragma once
#include <stdint.h>

void  w_alloc_init();
void* w_malloc(size_t size);
void  w_free(void* p);
void* w_realloc(void* p, size_t size);
void  w_alloc_log();
