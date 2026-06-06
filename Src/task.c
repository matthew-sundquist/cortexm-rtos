
#include "task.h"
#include <stdlib.h>

uint32_t pid = 0;
// assumes scheduler is already initialized
int task_create(tcb_t *task, void(*entry)(void), uint32_t *arg, uint8_t priority, uint32_t *sp, char *name)
{
	if (task == NULL || entry == NULL || arg == NULL || priority >= MAX_PRIORITIES || sp == NULL || name == NULL)
	{
		return 1;
	}

	task_init(task, entry, arg, priority, sp, name, pid);

	task_add_ready(task);

	return 0;
}
