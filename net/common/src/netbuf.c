
#include <stddef.h>
#include <string.h>

#include "assert.h"
#include "mem.h"
#include "netbuf.h"
#include "netqueue.h"

#define NETBUF_POOL_SIZE 3

typedef enum {
    NETBUF_FRONT,
    NETBUF_BACK
} netbuf_side_t;

static netqueue_t netbuf_pool;

static inline void netbuf_pool_push(netbuf_t *nbuf);

static inline netbuf_t *netbuf_pool_pop();

static void netbuf_reset(netbuf_t *nbuf);

static netbuf_t nbuf_pool[NETBUF_POOL_SIZE];

netbuf_t *netbuf_alloc() { return netbuf_pool_pop(); }

static void init_pool();

static bool netbuf_remove(netbuf_t *nbuf, uint8_t *data, size_t len, netbuf_side_t side);

static bool netbuf_insert(netbuf_t *nbuf, const uint8_t *data, size_t len, netbuf_side_t side);

void netbufs_init()
{
    init_pool();
}

bool netbuf_push_front(netbuf_t *nbuf, const uint8_t *data, size_t len)
{
    return netbuf_insert(nbuf, data, len, NETBUF_FRONT);
}

bool netbuf_push_back(netbuf_t *nbuf, const uint8_t *data, size_t len)
{
    return netbuf_insert(nbuf, data, len, NETBUF_BACK);
}

bool netbuf_pop_front(netbuf_t *nbuf, uint8_t *data, size_t len)
{
    return netbuf_remove(nbuf, data, len, NETBUF_FRONT);
}

bool netbuf_pop_back(netbuf_t *nbuf, uint8_t *data, size_t len)
{
    return netbuf_remove(nbuf, data, len, NETBUF_BACK);
}

bool netbuf_free(netbuf_t *nbuf) {
  ASSERT(nbuf != NULL);

  netbuf_pool_push(nbuf);

  return true;
}

static void netbuf_reset(netbuf_t *nbuf) {

  ASSERT(nbuf != NULL);

  nbuf->cap = MAX_PAYLOAD_SIZE_BYTES;

  nbuf->head = nbuf->buf + MAX_PAYLOAD_SIZE_BYTES;
  nbuf->tail = nbuf->head;

  nbuf->len = 0;
}

uint32_t netbuf_len(netbuf_t *nbuf) {
    ASSERT(nbuf != NULL);

  return nbuf->len;
}

uint8_t *netbuf_data(netbuf_t *nbuf) 
{
  ASSERT(nbuf != NULL);

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

static bool netbuf_remove(netbuf_t *nbuf, uint8_t *data, size_t len, netbuf_side_t side)
{
    ASSERT(nbuf != NULL);
    ASSERT(data != NULL);

    ASSERT(nbuf->head <= nbuf->tail);

    if (nbuf->len < len)
    {
        return false;
    }

    if (side == NETBUF_FRONT)
    {
        memcpy(data, nbuf->head, len);
        nbuf->head += len;
    }

    else
    {
        memcpy(data, nbuf->tail - len, len);
        nbuf->tail -= len;
    }

    nbuf->len -= len;

    return true;
}

static bool netbuf_insert(netbuf_t *nbuf, const uint8_t *data, size_t len, netbuf_side_t side)
{
    ASSERT(nbuf != NULL);
    ASSERT(data != NULL);
    
    ASSERT(nbuf->head <= nbuf->tail);

    if (nbuf->len + len > nbuf->cap)
    {
        return false;
    }

    if (side == NETBUF_FRONT)
    {
        if (nbuf->head - len < nbuf->buf)
        {
            return false; // head goes over the start of buf
        }

        memcpy(nbuf->head - len, data, len);
        nbuf->tail -= len;
    }

    else
    {
        if (nbuf->tail + len > nbuf->buf + nbuf->cap)
        {
            return false; // tail going over back
        }

        memcpy(nbuf->tail, data, len);
        nbuf->tail += len;
    }

    nbuf->len += len;

    return true;
}

