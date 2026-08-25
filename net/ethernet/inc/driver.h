#ifndef ETHERNET_DRIVER_H
#define ETHERNET_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#include "frame.h"
#include "netbuf.h"
#include "netqueue.h"
#include "stmmac.h"

void ethernet_mac_init();

#endif
