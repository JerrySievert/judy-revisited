#include "glue.h"
#include "judy.h"
#include <string.h>
#include <stdlib.h>

void *jg_init (uint16_t size, uint16_t depth) {
  if (!size) {
    size = MAX_JUDY_SIZE;
  } else if (size > MAX_JUDY_SIZE) {
    return NULL;
  }

  return judy_open(size, depth);
}

bool jg_set (void *judy, uint8_t *key, uint8_t *value, uint16_t len, uint8_t type) {
  JudySlot *cell;

  cell = judy_cell((Judy *) judy, key, (size_t) strlen((const char *) key));

  if (cell && *cell) {
    struct store *data = (struct store *) *cell;

    if (data->type == TYPE_STRING) {
      if (data->length != 0 && data->ptr) {
        free(data->ptr);
      }
    }

    free(data);
  }

  if (cell) {
    struct store *data = (struct store *) malloc(sizeof(struct store));

    if (type == TYPE_STRING) {
      data->ptr = (void *) malloc(len);
      data->length = len;
      memcpy(data->ptr, value, len);
    }

    data->type = type;

    *cell = (JudySlot)data;

    return true;
  }

  return false;
}

struct store *jg_get(void *judy, uint8_t *key) {
  JudySlot *cell;

  cell = judy_slot((Judy *) judy, key, (size_t) strlen((const char *)key));

  if (cell) {
    struct store *data = (struct store *)*cell;

    return data;
  }

  return NULL;
}

void jg_delete(void *judy, uint8_t *key) {
  JudySlot *cell;

  cell = judy_slot((Judy *)judy, key, (size_t)strlen((const char *)key));

  if (cell) {
    struct store *data = (struct store *)*cell;
    if (data) {
      if (data->length != 0 && data->ptr) {
        free(data->ptr);
      }

      free(data);
    }

    judy_del((Judy *)judy);
  }
}

void jg_close (void *judy) {
  judy_close((Judy *) judy);
}
