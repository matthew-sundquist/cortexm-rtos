#ifndef ETHERNET_DRIVER_H
#define ETHERNET_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#include "frame.h"
#include "netqueue.h"
#include "stmmac.h"

void ethernet_mac_init();

// returns bytes sent/recvd
size_t ethernet_send(const uint8_t *data, size_t len);

size_t ethernet_recv(uint8_t *data);

#endif
