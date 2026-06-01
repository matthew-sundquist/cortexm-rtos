
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scheduler.h"

static scheduler_t sch_inst;

tcb_t* cur_process = NULL;
tcb_t* next_process = NULL;
uint64_t sch_ticks = 0;


uint8_t init_scheduler(uint8_t num_priorities)
{

	if (num_priorities > MAX_PRIORITIES || num_priorities < 1)
	{
		return 1;
	}

	sch_inst.num_priorities = num_priorities;
	sch_inst.ready_bitmap = 0;
	sch_inst.cur_task = NULL;

	for (int i = 0; i < num_priorities; i++)
	{
		init_task_list(&(sch_inst.ready_lists[i]));
	}

	init_task_list(&(sch_inst.blocked_list));

	return 0;
}

uint8_t add_task_to_ready(tcb_t *task)
{
	if (task == NULL)
	{
		return 1;
	}

	if (task->priority > sch_inst.num_priorities - 1)
	{
		return 2;
	}

	if (push_task(&(sch_inst.ready_lists[task->priority]), task) != 0)
	{
		return 3;
	}

	task->state = READY;

	sch_inst.ready_bitmap |= (1U << task->priority); // task is assumed to be ready when added

	return 0;
}

// removes from front
uint8_t remove_task_from_ready(uint8_t priority)
{

	if (priority > sch_inst.num_priorities - 1)
	{
		return 2;
	}

	if (pop_task(&(sch_inst.ready_lists[priority])) != 0)
	{
		return 3;
	}

	if (sch_inst.ready_lists[priority].size == 0)
	{
		sch_inst.ready_bitmap &= ~(1U << priority); // remove from ready list
	}

	return 0;
}

uint8_t add_task_to_blocked(tcb_t *task)
{
	if (task == NULL)
	{
		return 1;
	}


	return 0;
}

uint8_t select_task()
{

	uint32_t prio = 31 - __builtin_clz(sch_inst.ready_bitmap); // gets the leading zeros

	if (prio < 0 || prio > sch_inst.num_priorities - 1) // no tasks ready
	{
		return 1;
	}

	sch_inst.cur_task = sch_inst.ready_lists[prio].head;

	if (remove_task_from_ready(prio) != 0) // updates bitmap
	{
		return 2; // BAD, SCHEDULER IN WEIRD STATE
	}

	sch_inst.cur_task->state = RUNNING;

	return 0;
}

tcb_t* get_cur_task()
{
	return sch_inst.cur_task;
}

void scheduler_tick()
{
	add_task_to_ready(sch_inst.cur_task); // put the current running task back to ready

	uint8_t state = select_task(); // put the highest priority ready task into cur_task
	if (state == 1)
	{
		// no tasks ready : do not switch task
	}
	else if (state == 2)
	{
		// weird state : somehow recover?
	}
	else
	{
		next_process = sch_inst.cur_task; // set next process
	}

	sch_ticks++;
}

