
#include <stdint.h>

#include "stmmac.h"
#include "driver.h"


#define GOTO_ADDR(x) (*(volatile uint32_t *)(x))

void ethernet_mac_init()
{
#ifdef STMMAC

    GOTO_ADDR(ETH_MAC_BASE_ADDR + ETH_MACCR_OFFSET);

#endif
}
