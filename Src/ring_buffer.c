
#include "ring_buffer.h"


bool ring_buffer_init(ring_buffer_t* rb, uint8_t *buf, size_t size)
{
	if (!rb || !buf || size <= 0)
	{
		return false;
	}

	rb->buf = buf;
	rb->size = size;
	rb->head = 0;
	rb->tail = 0;
}

bool ring_buffer_write_byte(ring_buffer_t* rb, const uint8_t data)
{
	if (!rb)
	{
		return false;
	}

	uint32_t write_idx = (rb->head + 1) % rb->size;

	if (write_idx == rb->tail)
	{
		// full case
		return false;
	}

	rb->data[write_idx] = data;
	rb->head = write_idx;
	return true;
}

bool ring_buffer_write(ring_buffer_t *rb, const uint8_t *data, size_t size)
{
	if (!rb)
	{
		return false;
	}

	uint32_t write_end_idx = (rb->head+size) % rb->size;

	if (rb->head < rb->tail && write_end_idx > rb->tail)
	{
		// not enough room
		return false;
	}

	// need to handle the wrap case
}

bool ring_buffer_pop_byte(ring_buffer_t* rb, uint8_t* data);

bool ring_buffer_pop(ring_buffer_t* rb, uint8_t *data, size_t size);
