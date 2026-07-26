
#include <stddef.h>
#include <string.h>

#include "netbuf.h"
#include "assert.h"

#define MAX_CONNECTIONS 3

static uint32_t cur_connections = 0xFF; // all connections available

static uint8_t buffers[MAX_CONNECTIONS][MAX_PAYLOAD_SIZE_BYTES];

static inline bool get_available_buf(uint8_t *buf);

typedef struct netbuf_pool {
    netbuf_t *free_list;
    mutex_t lock;
} netbuf_pool_t;

static inline void netbuf_pool_push(netbuf_t *nbuf);

static inline netbuf_t *netbuf_pool_pop();

static netbuf_t nbuf_pool[MAX_CONNECTIONS];

static netbuf_pool_t netbuf_manager;

static netbuf_t *netbuf_alloc(netbuf_pool_t *pool)
{
    uint8_t *buf;
    // if cur connections is all 1, then the connections are available
    if (!get_available_buf(buf))
    {
        return NULL;
    }

    netbuf_t nbuf;

    nbuf.buf = buf;

    return nbuf;
}

bool netbuf_free(netbuf_t *nbuf)
{
    if (!nbuf || !nbuf->buf)
    {
        return false;
    }

    uint32_t buf_num = (nbuf->buf - buffers) / MAX_PAYLOAD_SIZE_BYTES;

    cur_connections |= buf_num;

    return true;
}


// does not check if buf is out of range
static netbuf_t *get_available_buf()
{
    netbuf_node_t n = netbuf_manager.free_list; // get head of free list
    
    if (!n)
    {
        return NULL;
    }

    
}

bool netbuf_reset(netbuf_t *nbuf)
{
    if (!nbuf || !nbuf->buf)
    {
        return false;
    }

    nbuf->cap = MAX_PAYLOAD_SIZE_BYTES;

    nbuf->head = nbuf->buf + MAX_PAYLOAD_SIZE_BYTES;

    nbuf->len = 0;

    return true;
}

bool netbuf_push(netbuf_t *nbuf, uint8_t *data, uint32_t len)
{
    if (!nbuf || !data || len <= 0)
    {
        return false;
    }

    if (len + nbuf->len >= nbuf->cap)
    {
        return false;
    }

    memcpy(nbuf->head - len, data, len);

    nbuf->head -= len;



    return true;
}

uint32_t netbuf_len(netbuf_t *nbuf)
{
    if (!nbuf)
    {
        return 0;
    }

    return nbuf->len;
}

uint8_t *netbuf_data(netbuf_t *nbuf)
{
    if (!nbuf)
    {
        return NULL;
    }

    return nbuf->buf;
}

static inline void netbuf_pool_push(netbuf_t *nbuf)
{

}
