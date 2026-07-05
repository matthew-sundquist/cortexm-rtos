
#include "mutex.h"
#include "scheduler.h"
#include "task_queue.h"
#include <stm32l4xx.h>
#include "tcb.h"
#include "assert.h"

void mutex_init(mutex_t *mut)
{
	mut->owner_pid = 0;
	init_task_queue(&(mut->delayed_tasks));
}

void mutex_aquire(mutex_t *mut)
{
	/*
	 * Algorithm psuedocode:
	 * check if mutex is currently being used
	 * if true: add into blocked list
	 * else: set pid
	 */

	// cur task's pid is always valid
	do {
		uint32_t result = __LDREXW(&(mut->owner_pid));

		if (result == 0) // is free
		{
			if (__STREXW(get_running_pid(), &(mut->owner_pid)) == 1)
			{
				continue; // failed
			}

			return; // success case
		}

		else // even if this is our pid, we do not care, correct usage is not to try to aquire the mutex after you already have it
		{
			// this must add to this mutex's delayed list

			__CLREX();

			tcb_t *cur_task = get_cur_task();

			__disable_irq();

			task_block(cur_task, BLOCKED);

			task_push(&(mut->delayed_tasks), cur_task);

			__enable_irq();

			scheduler_tick();
		}
	} while (1);
}

void mutex_release(mutex_t *mut)
{
	if (get_running_pid() != mut->owner_pid)
	{
		return;
	}

	// steps: if task in delayed list,pop a task from the delayed list and add back to ready list

	__disable_irq();
	if (mut->delayed_tasks.size > 0)
	{
		tcb_t *task = task_pop(&(mut->delayed_tasks));

		task_add_ready(task);
	}

	mut->owner_pid = 0;
	__enable_irq();
}
