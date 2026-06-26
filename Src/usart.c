

#include <stdlib.h>
#include "usart.h"
#include "assert.h"


void usart_init(usart_t *usart, USART_TypeDef *regs, const usart_config_t *config)
{
	if (!usart || !regs || !config)
	{
		return false;
	}

	regs->CR1 &= ~((1 << 0) | (1 << 12) | (1 << 28));
	regs->CR1 |= ((config->word_len & 0b01) << 12) | ((config->word_len & 0b10) << 28);

	regs->CR1 &= ~((1 << 10) | (1 << 9));
	if (config->parity_bit != UART_PARITY_NONE)
	{
		regs->CR1 |= ((1 << 10) | (config->parity_bit << 9));
	}

	regs->CR2 &= ~((0x3 << 12));
	regs->CR2 |= (config->stop_bits << 12);


	regs->BRR |=
}

static inline uint32_t compute_USARTDIV(const uint32_t baudrate, USART_TypeDef *regs)
{
	/*
	 * calculation for baud rate:
	 * if oversampling by 16: f_ck/USARTDIV
	 * if oversampling by 8: 2*f_ck/USARTDIV
	 */

	uint32_t br;
	if (regs->CR1 & (1 << 15))
	{
		// oversampling by 8


		return br;
	}

	return br;

}

static uint32_t get_uart_clk()
{
	return 16000000; // assumption for now, need to change later
}

// non-blocking
void usart_write_async(usart_t *usart, const void *buf, size_t len);

// non-blocking
void usart_read_async(usart_t *usart, void *buf, size_t len);
