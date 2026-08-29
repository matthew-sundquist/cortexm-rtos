
#include <stdint.h>

#include "stmmac.h"
#include "driver.h"

static void tx_init();

static void rx_init();

static void enable_loopback();

void ethernet_mac_init()
{
    ETH_MACCR |= ETH_MACCR_FES; // 100 Mbit/s
    
    ETH_MACCR |= ETH_MACCR_DM; // enable duplex mode
}


static void tx_init()
{
    ETH_MACCR &= ~ETH_MACCR_JD; // disable jabber, can send 16384 bytes
    
    // TODO: configure interframe gap
    
    // TODO: configure CSD

    ETH_MACCR |= ETH_MACCR_RD; // come back to this, disabling retries now for simplicity
    
    // no need to configure Back off limit, only applies to half duplex mode

    ETH_MACCR |= ETH_MACCR_TE; // enable transmitter
}

static void rx_init()
{
    ETH_MACCR |= ETH_MACCR_CSTF; // enable crc stripping

    ETH_MACCR &= ~ETH_MACCR_WD; // disable watchdog, can recv 16384 bytes
    
    ETH_MACCR &= ~ETH_MACCR_IPCO; // disable ipv4 checksum offload

    ETH_MACCR |= ETH_MACCR_APCS; // enable strip of crc/pad for frame with len < 1500 bytes

    ETH_MACCR |= ETH_MACCR_RE; // enable receiver
}

static void enable_loopback()
{
    ETH_MACCR |= ETH_MACCR_LM; // enable loopback

    // need to enable RX_CLK for loopback to work
}
