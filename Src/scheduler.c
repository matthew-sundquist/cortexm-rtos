
#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"

uint8_t init_scheduler(scheduler_t* sch_inst, uint8_t num_priorities)
{
	if (sch_inst == NULL)
	{
		return 1;
	}

	if (num_priorities > MAX_PRIORITIES)
	{
		return 2;
	}

	sch_inst->num_priorities = num_priorities;
	sch_inst->ready_bitmap = 0;
	sch_inst->cur_task = NULL;

	for (int i = 0; i < num_priorities; i++)
	{
		init_ready_list(&(sch_inst->ready_lists[i]));
	}

	return 0;
}

uint8_t add_task(scheduler_t* sch_inst, tcb_t *task)
{
	if (sch_inst == NULL || task == NULL)
	{
		return 1;
	}

	if (task->priority > sch_inst->num_priorities - 1)
	{
		return 2;
	}

	if (push_task(&(sch_inst->ready_lists[task->priority]), task) != 0)
	{
		return 3;
	}

	sch_inst->ready_bitmap |= (1U << task->priority); // task is assumed to be ready when added

	return 0;
}

// removes from front
uint8_t remove_task(scheduler_t* sch_inst, uint8_t priority)
{
	if (sch_inst == NULL)
	{
		return 1;
	}

	if (priority > sch_inst->num_priorities - 1)
	{
		return 2;
	}

	if (pop_task(&(sch_inst->ready_lists[priority])) != 0)
	{
		return 3;
	}

	if (sch_inst->ready_lists[priority].__builtin_clzsize == 0)
	{
		sch_inst->ready_bitmap &= ~(1U << priority); // remove from ready list
	}

	return 0;
}

uint8_t select_task(scheduler_t* sch_inst)
{
	if (sch_inst == NULL)
	{
		return 1;
	}

	uint32_t prio = 31 - __builtin_clz(sch_inst->ready_bitmap); // gets the leading zeros

	if (prio < 0 || prio > sch_inst->num_priorities - 1) // no tasks ready
	{
		return 2;
	}

	sch_inst->cur_task = sch_inst->ready_lists[prio].head;

	if (remove_task(sch_inst, prio) != 0)
	{
		return 3; // BAD, SCHEDULER IN WEIRD STATE
	}

	return 0;
}
