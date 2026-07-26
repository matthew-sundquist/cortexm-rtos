
#include <stdbool.h>
#include <stdint.h>

typedef struct netbuf {
	uint8_t *buf;
    uint8_t *head;
	uint32_t cap;
	uint32_t len;
} netbuf_t;


netbuf_t *netbuf_alloc(void);
bool netbuf_free(netbuf_t *nbuf);
void netbuf_reset(netbuf_t *nbuf);
bool netbuf_push(netbuf_t *nbuf, uint8_t *data, uint32_t len);

uint32_t netbuf_len(netbuf_t *nbuf);
uint8_t netbuf_data(netbuf_t *nbuf);

bool netbuf_reset(netbuf_t *nbuf);
