#ifndef TCB_H
#define TCB_H

#include "stdint.h"

typedef struct tcb {
	uint32_t *sp;
	uint8_t priority;
	char name[16];

	struct tcb *next;
} tcb_t;

#endif
