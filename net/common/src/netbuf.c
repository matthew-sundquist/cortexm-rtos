
#include <stddef.h>
#include <string.h>

#include "netbuf.h"
#include "assert.h"

#define MAX_CONNECTIONS 3
#define MAX_PAYLOAD_SIZE_BYTES 1000

static uint32_t cur_connections = 0xFF; // all connections available

static uint8_t buffers[MAX_CONNECTIONS][MAX_PAYLOAD_SIZE_BYTES];

static inline bool get_available_buf(uint8_t *buf);

netbuf_t netbuf_alloc()
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
static inline bool get_available_buf(uint8_t *buf)
{
    uint32_t all_cons = cur_connections & MAX_CONNECTIONS;

    if (!all_cons || !buf)
    {
        return false;
    }
    
    uint32_t shift_amount = 0;

    while (all_cons != 1)
    {
        shift_amount++;
        all_cons >>= 1;
    }

    buf = buffers[shift_amount];

    cur_connections &= ~(1 << shift_amount);

    return true;
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
