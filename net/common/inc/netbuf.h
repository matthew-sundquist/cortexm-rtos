
#ifndef NETBUF_H
#define NETBUF_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "ts_queue.h"

#define MAX_PAYLOAD_SIZE_BYTES 1000

typedef struct netbuf {
  queue_link_t link;
  uint8_t buf[MAX_PAYLOAD_SIZE_BYTES];
  uint8_t *head;
  uint8_t *tail;
  uint32_t cap;
  uint32_t len;
} netbuf_t;

bool netbuf_free(netbuf_t *nbuf);

netbuf_t *netbuf_alloc();

bool netbuf_push_front(netbuf_t *nbuf, const uint8_t *data, size_t len);
bool netbuf_push_back(netbuf_t *nbuf, const uint8_t *data, size_t len);

bool netbuf_pop_front(netbuf_t *nbuf, uint8_t *data, size_t len);
bool netbuf_pop_back(netbuf_t *nbuf, uint8_t *data, size_t len);

uint32_t netbuf_len(netbuf_t *nbuf);
uint8_t *netbuf_data(netbuf_t *nbuf);

void netbufs_init();

#endif
