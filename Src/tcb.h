#ifndef TCB_H
#define TCB_H

#include "stdint.h"

typedef enum {
	READY,
	RUNNING,
	BLOCKED
} task_state_t;

typedef struct tcb {
	uint32_t *sp;
	uint8_t priority;
	char name[16];

	task_state_t state;

	struct tcb *next;
	struct tcb *prev;
} tcb_t;

#endif
