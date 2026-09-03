
#include <stdint.h>
#include <stdbool.h>

#include "assert.h"
#include "dma.h"
#include "stmmac.h"
#include "netbuf.h"

#define NUM_RX_DESCRIPTORS 4
#define NUM_TX_DESCRIPTORS 4

#define MAX_TX_BUF_LEN 0x1FFF

#define OWN_BIT (1U << 31)
#define TER_BIT (1U << 21)
#define RER_BIT (1U << 15)
#define TCH_BIT (1U << 20)
#define IC_BIT (1U << 30)
#define LS_BIT (1U << 29)
#define FS_BIT (1U << 28)

typedef struct eth_dma_descriptor
{
    uint32_t desc[4];
    netbuf_t *buf_owner;
} eth_dma_descriptor_t;

static void dma_rx_init();

static void dma_tx_init();

static inline void dma_tx_advance(eth_dma_descriptor_t *tx_desc);

static inline void dma_rx_advance(eth_dma_descriptor_t *rx_desc);

static inline void dma_put_tx_buf(const void *data, size_t len);

static inline size_t dma_get_rx_buf(void **data);

static inline bool dma_tx_is_last_desc(const eth_dma_descriptor_t *tx_desc);

static inline bool dma_rx_is_last_desc(const eth_dma_descriptor_t *rx_desc);

static eth_dma_descriptor_t tx_descriptors[NUM_TX_DESCRIPTORS];
static eth_dma_descriptor_t rx_descriptors[NUM_RX_DESCRIPTORS];

static eth_dma_descriptor_t *rx_current;
static eth_dma_descriptor_t *tx_current;
static eth_dma_descriptor_t *tx_reclaim;

void ethernet_dma_init()
{
    dma_rx_init();
    dma_tx_init();
}


dma_status_t ethernet_dma_put(netbuf_t *nbuf)
{
    ASSERT(nbuf != NULL);
    ASSERT(nbuf->len > 0); // not sure if this should be here
    ASSERT(nbuf->buf != NULL); // not sure if this should be here
    
    if (tx_current->desc[0] & OWN_BIT)
    {
        return DMA_BUSY;
    }

    // not supporting packets stretching across packets at the moment
    if (nbuf->len > MAX_TX_BUF_LEN)
    {
        return DMA_UNSUPPORTED;
    }

    dma_put_tx_buf(nbuf->buf, nbuf->len);

    tx_current->buf_owner = nbuf;

    tx_current->desc[0] |= IC_BIT | LS_BIT | FS_BIT | TCH_BIT;

    if (dma_tx_is_last_desc(tx_current))
    {
        tx_current->desc[0] |= TER_BIT;
    }

    dma_tx_advance(tx_current);

    return DMA_OK;
}

// this implementation is wrong for anything over simple testing
// this function is intended to take ownership of the netbuf stored in the 
// rx descriptor in DMA. The buffer that is read in this function needs to be replaced
// with another buffer somehow. Consider using netbuf_alloc.
dma_status_t ethernet_dma_get(netbuf_t **nbuf)
{
    ASSERT(nbuf != NULL);

    if (rx_current->desc[0] & OWN_BIT)
    {
        return DMA_ERROR; // maybe should be an assertion, or expand return type to support
    }

    // this expression gets rid of the need for calling dma_get_rx_buf
    *nbuf = rx_current->buf_owner;

    (*nbuf)->len = rx_current->desc[1] & 0x1FFF; // buffer 1 len
    
    dma_rx_advance(rx_current);

    return DMA_OK;
}

static void dma_rx_init()
{
    rx_current = &rx_descriptors[0];

    rx_descriptors[NUM_RX_DESCRIPTORS - 1].desc[1] |= RER_BIT; // check back on this, could be wrong

    ETH_DMAOMR |= ETH_DMAOMR_DTCEFD; // disable auto dropping IP due to checksum error
    
    ETH_DMAOMR |= ETH_DMAOMR_RSF; // only read from DMA if have full frame

    ETH_DMAOMR |= ETH_DMAOMR_DFRF; // disable flushing of recv frames
    // ETH_DMAOMR should be last
    
    ETH_DMAOMR |= ETH_DMAOMR_SR; // start recv
}

static void dma_tx_init()
{
    tx_current = &tx_descriptors[0];
    tx_reclaim = &tx_descriptors[0];

    for (uint8_t i = 0; i < NUM_TX_DESCRIPTORS - 1; i++)
    {
        tx_descriptors[i].desc[3] = (uint32_t)(&tx_descriptors[i+1].desc[0]);
    }

    ETH_DMAOMR |= ETH_DMAOMR_TSF; // transmission starts when full frame in FIFO

    ETH_DMAOMR |= ETH_DMAOMR_ST; // start transmission
}

static inline void dma_put_tx_buf(const void *data, size_t len)
{
    tx_current->desc[1] = len;
    tx_current->desc[2] = (uint32_t)data;
}

static inline size_t dma_get_rx_buf(void **data)
{
    *data = (void *)rx_current->desc[2];
    return (size_t)rx_current->desc[1] & 0x1FFF;
}


static inline void dma_rx_advance(eth_dma_descriptor_t *rx_desc)
{
    ASSERT(rx_desc != NULL);
    if (rx_desc->desc[1] & RER_BIT)
    {
        rx_desc = &rx_descriptors[0];
        return;
    }

    rx_current++;
}

static inline void dma_tx_advance(eth_dma_descriptor_t *tx_desc)
{
    ASSERT(tx_desc != NULL);
    if (tx_desc->desc[0] & TER_BIT)
    {
        tx_desc = &tx_descriptors[0];
        return;
    }

    tx_current++;
}


static inline bool dma_tx_is_last_desc(const eth_dma_descriptor_t *tx_desc)
{
    ASSERT(tx_desc != NULL);

    return tx_desc == &(tx_descriptors[NUM_TX_DESCRIPTORS - 1]);
}

static inline bool dma_rx_is_last_desc(const eth_dma_descriptor_t *rx_desc)
{
    ASSERT(rx_desc != NULL);

    return rx_desc == &(rx_descriptors[NUM_RX_DESCRIPTORS - 1]);
}
