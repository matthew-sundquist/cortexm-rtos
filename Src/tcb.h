#ifndef TCB_H
#define TCB_H

#include "stdint.h"
#define STACK_SIZE 256

extern uint32_t pid;

typedef enum {
	READY,
	RUNNING,
	BLOCKED,
	DELAYED,
	NONE
} task_state_t;

typedef struct tcb {
	uint32_t *sp;
	uint8_t priority;
	char name[16];

	task_state_t state;
	uint32_t wake_tick;

	uint32_t pid;

	struct tcb *next;
	struct tcb *prev;
} tcb_t;

void task_init(tcb_t *task, void(*entry)(void), uint32_t *arg, uint8_t priority, uint32_t *sp, char *name);

uint32_t *init_stack(uint32_t *sp, void (*entry)(void), uint32_t *arg);

#endif
