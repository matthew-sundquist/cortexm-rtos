
#ifndef NETBUF_H
#define NETBUF_H

#include <stdbool.h>
#include <stdint.h>

#include "ts_queue.h"

#define MAX_PAYLOAD_SIZE_BYTES 1000

typedef struct netbuf {
    queue_link_t link;
	uint8_t buf[MAX_PAYLOAD_SIZE_BYTES];
    uint8_t *head;
	uint32_t cap;
	uint32_t len;
} netbuf_t;

bool netbuf_free(netbuf_t *nbuf);
void netbuf_reset(netbuf_t *nbuf);
bool netbuf_push(netbuf_t *nbuf, uint8_t *data, uint32_t len);

uint32_t netbuf_len(netbuf_t *nbuf);
uint8_t netbuf_data(netbuf_t *nbuf);

bool netbuf_reset(netbuf_t *nbuf);

#endif
