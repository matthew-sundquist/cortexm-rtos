
#include <stdbool.h>
#include <stdint.h>

typedef struct netbuf {
	uint8_t *buf;
	uint32_t cap;
	uint32_t len;
} netbuf_t;


netbuf_t *netbuf_alloc(void);
bool netbuf_free(netbuf_t *nbuf);
void netbuf_init(netbuf_t *nbuf);
bool netbuf_push(netbuf_t *nbuf, uint8_t *data, uint32_t len);
bool netbuf_pop(netbuf_t *nbuf, uint8_t *data, uint32_t len);
