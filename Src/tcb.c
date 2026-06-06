
#include "tcb.h"
#include "string.h"
#include <stm32l4xx.h>

uint32_t pid = 1;

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


// sp just needs to be allocated memory pointing to the lowest memory address of the stack (bottom)
void task_init(tcb_t *task, void(*entry)(void), uint32_t *arg, uint8_t priority, uint32_t *sp, char *name)
{

	__disable_irq(); // if this is not disabled two processes could be created with the same PID
	*task = (tcb_t){
			.sp = init_stack(sp, entry, arg),
			.priority = priority,
			.state = NONE,
			.wake_tick = 0,
			.prev = NULL,
			.next = NULL,
			.pid = pid
	};

	pid++;
	__enable_irq();

	strncpy(task->name, name, sizeof(task->name));

	task->name[sizeof(task->name)-1] = '\0';

}
