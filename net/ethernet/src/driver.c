
#include <stdint.h>

#include "stmmac.h"
#include "driver.h"

static void mac_tx_init();

static void mac_rx_init();

static void mac_enable_loopback();

static void dma_rx_init();

void ethernet_mac_init()
{
    RCC_AHB1ENR |= RCC_AHB1ENR_ETHMACEN;

    (void)RCC_AHB1ENR;

    ETH_MACCR |= ETH_MACCR_FES; // 100 Mbit/s
    
    ETH_MACCR |= ETH_MACCR_DM; // enable duplex mode

    mac_tx_init();
    mac_rx_init();
    mac_enable_loopback(); // for testing
}


size_t ethernet_send(const uint8_t *data, size_t len) // should not copy
{
    return 0;
}

size_t ethernet_recv(uint8_t *data) // should just do data = dma ptr
{
    return 0;
}

static void mac_tx_init()
{
    RCC_AHB1ENR |= RCC_AHB1ENR_ETHMACTXEN;
    
    (void)RCC_AHB1ENR;

    ETH_MACCR &= ~ETH_MACCR_JD; // disable jabber, can send 16384 bytes
    
    // TODO: configure interframe gap
    
    // TODO: configure CSD

    ETH_MACCR |= ETH_MACCR_RD; // come back to this, disabling retries now for simplicity
    
    // no need to configure Back off limit, only applies to half duplex mode

    ETH_MACCR |= ETH_MACCR_TE; // enable transmitter
}

static void mac_rx_init()
{
    RCC_AHB1ENR |= RCC_AHB1ENR_ETHMACRXEN;

    (void)RCC_AHB1ENR;

    ETH_MACCR |= ETH_MACCR_CSTF; // enable crc stripping

    ETH_MACCR &= ~ETH_MACCR_WD; // disable watchdog, can recv 16384 bytes
    
    ETH_MACCR &= ~ETH_MACCR_IPCO; // disable ipv4 checksum offload

    ETH_MACCR |= ETH_MACCR_APCS; // enable strip of crc/pad for frame with len < 1500 bytes

    ETH_MACCR |= ETH_MACCR_RE; // enable receiver
}

static void mac_enable_loopback()
{
    ETH_MACCR |= ETH_MACCR_LM; // enable loopback
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
