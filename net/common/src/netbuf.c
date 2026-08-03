
#include <stddef.h>
#include <string.h>

#include "assert.h"
#include "mem.h"
#include "netbuf.h"
#include "netqueue.h"

#define MAX_CONNECTIONS 3

static uint32_t cur_connections = 0xFF; // all connections available

static netqueue_t netbuf_pool;

static inline void netbuf_pool_push(netbuf_t *nbuf);

static inline netbuf_t *netbuf_pool_pop();

static netbuf_t nbuf_pool[MAX_CONNECTIONS];

static netbuf_t *netbuf_alloc() { return netbuf_pool_pop(); }

bool netbuf_free(netbuf_t *nbuf) {
  if (!nbuf || !nbuf->buf) {
    return false;
  }

  netbuf_pool_push(nbuf);

  return true;
}

bool netbuf_reset(netbuf_t *nbuf) {
  if (!nbuf || !nbuf->buf) {
    return false;
  }

  nbuf->cap = MAX_PAYLOAD_SIZE_BYTES;

  nbuf->head = nbuf->buf + MAX_PAYLOAD_SIZE_BYTES;

  nbuf->len = 0;

  return true;
}

bool netbuf_push(netbuf_t *nbuf, uint8_t *data, uint32_t len) {
  if (!nbuf || !data || len <= 0) {
    return false;
  }

  if (len + nbuf->len >= nbuf->cap) {
    return false;
  }

  memcpy(nbuf->head - len, data, len);

  nbuf->head -= len;

  return true;
}

uint32_t netbuf_len(netbuf_t *nbuf) {
  if (!nbuf) {
    return 0;
  }

  return nbuf->len;
}

uint8_t *netbuf_data(netbuf_t *nbuf) {
  if (!nbuf) {
    return NULL;
  }

  return nbuf->buf;
}

static inline void netbuf_pool_push(netbuf_t *nbuf) {
  ASSERT(nbuf != NULL);

  netqueue_push(&netbuf_pool, nbuf);
}

static inline netbuf_t *netbuf_pool_pop() {
  netbuf_t *nb;
  netqueue_pop(&netbuf_pool, nb);
  return nb;
}
