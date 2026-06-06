

#include <stdint.h>
#include "tcb.h"
#include "scheduler.h"


extern uint32_t pid;

int task_create(tcb_t *task, void(*entry)(void), uint32_t *arg, uint8_t priority, uint32_t *sp, char *name);
