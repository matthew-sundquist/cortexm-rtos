
#include <stdint.h>
#include <stdbool.h>

#include "dma.h"
#include "stmmac.h"
#include "netbuf.h"

#define NUM_RX_DESCRIPTORS 4
#define NUM_TX_DESCRIPTORS 4

#define MAX_BUFFER_SIZE 0x1FFF

#define OWN_BIT (1U << 31)
#define TER_BIT (1U << 21)
#define RER_BIT (1U << 15)

typedef struct eth_dma_descriptor
{
    uint32_t desc[4];
    netbuf_t *buf_owner;
} eth_dma_descriptor_t;

static void dma_rx_init();

static void dma_tx_init();

static bool tx_desc_put_buffer(const void *data, size_t len);

static size_t rx_desc_get_buffer(const void **data);

static eth_dma_descriptor_t tx_descriptors[NUM_TX_DESCRIPTORS];
static eth_dma_descriptor_t rx_descriptors[NUM_RX_DESCRIPTORS];

static eth_dma_descriptor_t *rx_current;
static eth_dma_descriptor_t *tx_current;

void ethernet_dma_init()
{
    dma_rx_init();
    dma_tx_init();
}

static void dma_rx_init()
{
    rx_current = rx_descriptors[0];

    rx_descriptors[NUM_RX_DESCRIPTORS - 1].desc[1] |= RER_BIT; // check back on this, could be wrong

    ETH_DMAOMR |= ETH_DMAOMR_DTCEFD; // disable auto dropping IP due to checksum error
    
    ETH_DMAOMR |= ETH_DMAOMR_RSF; // only read from DMA if have full frame

    ETH_DMAOMR |= ETH_DMAOMR_DFRF; // disable flushing of recv frames
    // ETH_DMAOMR should be last
    
    ETH_DMAOMR |= ETH_DMAOMR_SR; // start recv
}

static void dma_tx_init()
{
    tx_current = tx_descriptors[0];

    tx_descriptors[NUM_TX_DESCRIPTORS - 1].desc[0] |= OWN_BIT;
    
    ETH_DMAOMR |= ETH_DMAOMR_TSF; // transmission starts when full frame in FIFO

    ETH_DMAOMR |= ETH_DMAOMR_ST; // start transmission
}

static bool tx_desc_put_buffer(const void *data, size_t len)
{
    if (tx_current->desc[0] & OWN_BIT)
    {
        return false;
    }

    if (len > MAX_BUFFER_SIZE)
    {
        return false;
    }

    tx_current->desc[1] = len;
    tx_current->desc[2] = (uint32_t)data;

    tx_current->desc[0] |= OWN_BIT;

    tx_current++;

    if (tx_current > tx_descriptors[NUM_TX_DESCRIPTORS - 1])
    {
        tx_current = tx_descriptors[0];
    }

    return true;
}

static size_t rx_desc_get_buffer(const void **data)
{
    size_t recvd_bytes = 0;

    if (rx_current->desc[0] & OWN_BIT)
    {
        return recvd_bytes;
    }

    if ()
}
