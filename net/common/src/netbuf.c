
#include <stddef.h>

#include "netbuf.h"
#include "assert.h"

#define MAX_CONNECTIONS 3
#define MAX_PAYLOAD_SIZE_BYTES 1000

static uint32_t cur_connections = 0;

static uint8_t buffers[MAX_CONNECTIONS][MAX_PAYLOAD_SIZE_BYTES];


netbuf_t *netbuf_alloc()
{
    if (cur_connections >= MAX_CONNECTIONS)
    {
        return NULL;
    }
    netbuf_t nbuf;

    nbuf.buf = get_available_buf();
}


// does not check if buf is out of range
static inline uint8_t *get_available_buf()
{
	
}
