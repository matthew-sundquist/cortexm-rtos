
#ifndef NETQUEUE_H
#define NETQUEUE_H

#include "netbuf.h"
#include "semaphore.h"
#include "ts_queue.h"

typedef struct netqueue {
  ts_queue_t queue;
  semaphore_t sem;
} netqueue_t;

bool netqueue_init(netqueue_t *nq);
bool netqueue_push(netqueue_t *nq, netbuf_t *nb);
bool netqueue_pop(netqueue_t *nq, netbuf_t *nb);

#endif
