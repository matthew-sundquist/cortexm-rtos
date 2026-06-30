
#include "ring_buffer.h"
#include <string.h>

static inline size_t get_total_elements(const uint32_t head, const uint32_t tail, const size_t size);

static inline size_t get_free_space(const uint32_t head, const uint32_t tail, const size_t size);

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

	return true;
}

bool ring_buffer_write_byte(ring_buffer_t* rb, const uint8_t data)
{
	const static uint8_t size = 1;
	if (!rb)
	{
		return false;
	}


	if (rb->head + size == rb->tail)
	{
		// full case
		return false;
	}

	rb->buf[rb->head] = data;
	rb->head = (rb->head + size) % rb->size;
	return true;
}

bool ring_buffer_write(ring_buffer_t *rb, const uint8_t *data, size_t size)
{
	if (!rb || !data)
	{
		return false;
	}

	if (size > get_free_space(rb->head, rb->tail, rb->size))
	{
		return false;
	}

	// split into two memcpy's each copying one contigous memory space
	size_t len_first_seg = rb->size - rb->head;
	memcpy(&rb->buf[rb->head], data, len_first_seg);
	memcpy(&rb->buf[0], data + len_first_seg, size - len_first_seg);

	rb->head = (rb->head + size) % rb->size;
	return true;
}

bool ring_buffer_pop_byte(ring_buffer_t* rb, uint8_t* data);

bool ring_buffer_pop(ring_buffer_t* rb, uint8_t *data, size_t size);

static inline size_t get_free_space(const uint32_t head, const uint32_t tail, const size_t size)
{
	return size - get_total_elements(head, tail, size);
}

static inline size_t get_total_elements(const uint32_t head, const uint32_t tail, const size_t size)
{
	return (head + size - tail) % size;
}
