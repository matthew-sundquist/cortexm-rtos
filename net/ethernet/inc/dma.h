#ifndef DMA_H
#define DMA_H

#include "netbuf.h"

typedef enum {
    DMA_OK,
    DMA_BUSY,
    DMA_ERROR,
    DMA_UNSUPPORTED
} dma_status_t;


void ethernet_dma_init();

dma_status_t ethernet_dma_put(const netbuf_t *nbuf);

// this func will set the len inside netbuf to the len inside the rx descriptor
// no need to return len here
dma_status_t ethernet_dma_get(netbuf_t **nbuf);

#endif
