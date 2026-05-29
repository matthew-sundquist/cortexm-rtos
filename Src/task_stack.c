
#include "task_stack.h"


uint32_t *init_stack(uint32_t *sp, void (*entry)(void), uint32_t *arg)
{
	*(--sp) = 0x01000000; // pXSR (thumb bit set)
	*(--sp) = (uint32_t)(entry); // PC
	*(--sp) = (uint32_t)0xFFFFFFFD; // LR
	*(--sp) = (uint32_t)0; // R12
	*(--sp) = (uint32_t)0; // R3
	*(--sp) = (uint32_t)0; // R2
	*(--sp) = (uint32_t)0; // R1
	*(--sp) = (uint32_t)arg; // R0

	*(--sp) = (uint32_t)0; // R11
	*(--sp) = (uint32_t)0; // R10
	*(--sp) = (uint32_t)0; // R9
	*(--sp) = (uint32_t)0; // R8
	*(--sp) = (uint32_t)0; // R7
	*(--sp) = (uint32_t)0; // R6
	*(--sp) = (uint32_t)0; // R5
	*(--sp) = (uint32_t)0; // R4

	return sp;
}
