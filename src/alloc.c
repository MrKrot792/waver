#include "alloc.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

typedef struct {
  void*  k;
  size_t v;
} pair_t;

#define PAIR_NULL ((pair_t){NULL, 0})

#define TABLE_SIZE 128

static size_t table_size;
static pair_t memory_table[TABLE_SIZE] = {0};

static void push(pair_t p) {
  table_size++;
  for (size_t i = 0; i < TABLE_SIZE; i++) {
    if (memory_table[i].k == NULL) { memory_table[i] = p; return; }
    if (memory_table[i].k == p.k)  { memory_table[i] = p; return; }
  }

  assert(false);
}

static pair_t pop(void* k) {
  table_size--;
  for (size_t i = 0; i < TABLE_SIZE; i++) {
    if (memory_table[i].k == k) {
      pair_t t = memory_table[i];
      memory_table[i] = PAIR_NULL;
      return t;
    }
  }
  
  return PAIR_NULL;
}

void w_alloc_init() {
  for (size_t i = 0; i < TABLE_SIZE; i++) {
    memory_table[i] = PAIR_NULL;
  }
}

void* w_malloc(size_t size) {
  void* p = malloc(size);
  push((pair_t){p, size});
  return p;
}

void w_free(void* p) {
  pop(p);
  free(p);
}

void* w_realloc(void* p_i, size_t size) {
  void* p = realloc(p_i, size);
  pop(p_i);
  push((pair_t){p, size});
  return p;
}

void w_alloc_log() {
  if (table_size == 0) {
    printf("[DEBUG] No allocated memory/memory leaks.\n");
    return;
  }
  
  size_t total = 0;
  printf("[DEBUG] Allocated memory:\n");
  for (size_t i = 0; i < TABLE_SIZE; i++) {
    if (memory_table[i].k != NULL) {
      printf("[DEBUG]  %p: %zu bytes;\n", memory_table[i].k, memory_table[i].v);
      total += memory_table[i].v;
    }
  }
  printf("[DEBUG] Total allocated memory: %zu bytes.\n", total);
}
