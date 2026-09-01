
#include <stdint.h>

#include "dma.h"
#include "stmmac.h"

static void dma_rx_init();

static void dma_tx_init();

static uint32_t tx_descriptors[4] __attribute__((aligned(4)));

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
