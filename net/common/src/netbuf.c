
#include <stddef.h>
#include <string.h>

#include "assert.h"
#include "mem.h"
#include "netbuf.h"
#include "netqueue.h"

#define NETBUF_POOL_SIZE 3

static netqueue_t netbuf_pool;

static inline void netbuf_pool_push(netbuf_t *nbuf);

static inline netbuf_t *netbuf_pool_pop();

static void netbuf_reset(netbuf_t *nbuf);

static netbuf_t nbuf_pool[NETBUF_POOL_SIZE];

netbuf_t *netbuf_alloc() { return netbuf_pool_pop(); }

static void init_pool();

void netbufs_init()
{
    init_pool();
}

bool netbuf_free(netbuf_t *nbuf) {
  if (!nbuf) {
    return false;
  }

  netbuf_pool_push(nbuf);

  return true;
}

static void netbuf_reset(netbuf_t *nbuf) {

  ASSERT(nbuf != NULL);

  nbuf->cap = MAX_PAYLOAD_SIZE_BYTES;

  nbuf->head = nbuf->buf + MAX_PAYLOAD_SIZE_BYTES;

  nbuf->len = 0;
}

bool netbuf_push(netbuf_t *nbuf, const uint8_t *data, uint32_t len) {
  if (!nbuf || !data || len <= 0) {
    return false;
  }

  if (len + nbuf->len >= nbuf->cap) {
    return false;
  }

  memcpy(nbuf->head - len, data, len);

  nbuf->head -= len;

  nbuf->len += len;

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

static inline netbuf_t *netbuf_pool_pop() 
{
  return netqueue_pop(&netbuf_pool);
}

static void init_pool()
{
   netqueue_init(&netbuf_pool);

   for (int i = 0; i < NETBUF_POOL_SIZE; i++)
   {
       netbuf_reset(&nbuf_pool[i]);
       netqueue_push(&netbuf_pool, &nbuf_pool[i]);
   }
}
