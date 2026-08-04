
#ifndef TS_QUEUE_H
#define TS_QUEUE_H

#include "mutex.h"
#include <stdbool.h>

// to avoid dynamic allocation, user defined structs embed a queue_link so that
// queue operations can be used on them
typedef struct queue_link {
  struct queue_link *next;
} queue_link_t;

typedef struct ts_queue {
  queue_link_t *head;
  queue_link_t *tail;
  mutex_t mut;
} ts_queue_t;

// user should use the void *data field to encapsulate their own struct
// with any fields they need, rather than adding a length field to the node
// struct

bool ts_queue_init(ts_queue_t *q);
bool ts_queue_push(ts_queue_t *q, queue_link_t *ql);
queue_link_t *ts_queue_pop(ts_queue_t *q);

#endif
