
#include <stdint.h>
#include <stdbool.h>

#include "dma.h"
#include "stmmac.h"

#define NUM_RX_DESCRIPTORS 4
#define NUM_TX_DESCRIPTORS 4

#define OWN_BIT (1 << 31)

static void dma_rx_init();

static void dma_tx_init();

static bool tx_desc_put_buffer(const void *data, uint16_t len);

static uint32_t tx_descriptors[NUM_TX_DESCRIPTORS];
static uint32_t rx_descriptors[NUM_RX_DESCRIPTORS];

void ethernet_dma_init()
{
    dma_rx_init();
    dma_tx_init();
}

static void dma_rx_init()
{

    ETH_DMAOMR |= ETH_DMAOMR_DTCEFD; // disable auto dropping IP due to checksum error
    
    ETH_DMAOMR |= ETH_DMAOMR_RSF; // only read from DMA if have full frame

    ETH_DMAOMR |= ETH_DMAOMR_DFRF; // disable flushing of recv frames
    // ETH_DMAOMR should be last
    
    ETH_DMAOMR |= ETH_DMAOMR_SR; // start recv
}

static void dma_tx_init()
{

    ETH_DMAOMR |= ETH_DMAOMR_TSF; // transmission starts when full frame in FIFO

    ETH_DMAOMR |= ETH_DMAOMR_ST; // start transmission
}

static bool tx_desc_put_buffer(const void *data, uint16_t len)
{
    if (tx_descriptors[0] & OWN_BIT) // last desc hasnt been sent yet
    {
        return false;
    }

    if (len > 0x1FFF) // length over max for one descriptor
    {
        return false;
    }

    tx_descriptors[1] = len;

    tx_descriptors[2] = (uint32_t)data;

    tx_descriptors[0] |= OWN_BIT;

    return true;
}
