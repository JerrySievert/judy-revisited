#pragma once

#include <stdint.h>

#define MAX_JUDY_SIZE 1024
#define TYPE_BUFFER 0
#define TYPE_STRING 1
#define TYPE_NUMBER 2

#ifndef bool
#define bool char
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

struct store {
  uint16_t length;
  void *ptr;
  uint8_t type;
};

void *jg_init(uint16_t, uint16_t);
bool jg_set(void *, uint8_t *, uint8_t *, uint16_t, uint8_t);
struct store *jg_get(void *, uint8_t *);
void jg_delete(void *, uint8_t *);
void jg_close (void *);
