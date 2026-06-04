#include "../src/config.h"

#ifdef UNIT_TESTS
#include <stdlib.h>
#include <stm32l4xx.h>
#include <stdbool.h>
#include "unit_tests.h"
#include "../src/tcb.h"
#include "../src/scheduler.h"
#include "../src/task_queue.h"
#include "../src/assert.h"

void test_scheduler_init()
{
	init_scheduler(); // not any tests to be done here

	scheduler_t *s = get_scheduler();

	for (int i = 0; i < MAX_PRIORITIES; i++)
	{
		ASSERT(s->ready_lists[i].head == NULL);
		ASSERT(s->ready_lists[i].tail == NULL);
		ASSERT(s->ready_lists[i].size == 0);
	}

	ASSERT(s->cur_task == NULL);
	ASSERT(s->ready_bitmap == 0);

	ASSERT(s->blocked_list.head == NULL);
	ASSERT(s->blocked_list.tail == NULL);
	ASSERT(s->blocked_list.size == 0);

	ASSERT(s->delayed_list.head == NULL);
	ASSERT(s->delayed_list.tail == NULL);
	ASSERT(s->delayed_list.size == 0);
}

void test_task_add_ready()
{
	init_scheduler(); // re-init the scheduler, get a clean state

	tcb_t task_1;
	task_1.priority = 0;
	task_1.state = BLOCKED;

	task_add_ready(&task_1);

	tcb_t task_2;
	task_2.priority = 1;

	task_add_ready(&task_2);

	scheduler_t *s = get_scheduler();

	ASSERT(task_1.state == READY);
	ASSERT(s->ready_lists[task_1.priority].size == 1);
	ASSERT(s->ready_lists[task_1.priority].head == &task_1);
	ASSERT(s->ready_lists[task_1.priority].tail == &task_1);
	ASSERT((s->ready_bitmap & 0x1) == 0x1);


	ASSERT(task_2.state == READY);
	ASSERT(s->ready_lists[task_2.priority].size == 1);
	ASSERT(s->ready_lists[task_2.priority].head == &task_2);
	ASSERT(s->ready_lists[task_2.priority].tail == &task_2);
	ASSERT((s->ready_bitmap & 0x02) == 0x02);

	tcb_t task_3;
	task_3.priority = 0;

	task_add_ready(&task_3);

	ASSERT(task_3.state == READY);
	ASSERT(s->ready_lists[task_3.priority].size == 2);
	ASSERT(s->ready_lists[task_3.priority].head == &task_1);
	ASSERT(s->ready_lists[task_3.priority].tail == &task_3);
	ASSERT((s->ready_bitmap & 0x1) == 0x1);

	ASSERT(s->ready_bitmap == 0x3);
}

void test_task_pop_ready()
{
	init_scheduler(); // re-init the scheduler, get a clean state

	tcb_t task_1;
	task_1.priority = 0;

	task_add_ready(&task_1);

	tcb_t task_2;
	task_2.priority = 1;

	task_add_ready(&task_2);

	scheduler_t *s = get_scheduler();

	ASSERT(task_pop_ready() == &task_2);
	ASSERT(s->ready_bitmap == 0x1);
	ASSERT(s->ready_lists[task_2.priority].size == 0);
	ASSERT(s->ready_lists[task_2.priority].head == NULL);
	ASSERT(s->ready_lists[task_2.priority].tail == NULL);

	ASSERT(task_pop_ready() == &task_1);
	ASSERT(s->ready_lists[task_1.priority].size == 0);
	ASSERT(s->ready_lists[task_1.priority].head == NULL);
	ASSERT(s->ready_lists[task_1.priority].tail == NULL);
	ASSERT(s->ready_bitmap == 0x0);

	ASSERT(task_pop_ready() == NULL);
}

void test_task_block()
{
	init_scheduler(); // re-init the scheduler, get a clean state

	tcb_t task_1;
	task_1.priority = 1;

	task_add_ready(&task_1);

	task_block(&task_1);

	scheduler_t *s = get_scheduler();

	ASSERT(s->ready_lists[task_1.priority].size == 0);
	ASSERT(s->ready_lists[task_1.priority].head == NULL);
	ASSERT(s->ready_lists[task_1.priority].tail == NULL);
	ASSERT(s->ready_bitmap == 0x0);

	ASSERT(s->blocked_list.size == 1);
	ASSERT(s->blocked_list.head == &task_1);
	ASSERT(s->blocked_list.tail == &task_1);

	ASSERT(task_1.state == BLOCKED);
}

void test_task_unblock()
{
	init_scheduler(); // re-init the scheduler, get a clean state

	tcb_t task_1;
	task_1.priority = 1;

	task_add_ready(&task_1);

	task_block(&task_1);

	task_unblock(&task_1);

	scheduler_t *s = get_scheduler();

	ASSERT(s->blocked_list.size == 0);
	ASSERT(s->blocked_list.head == NULL);
	ASSERT(s->blocked_list.tail == NULL);

	ASSERT(s->ready_lists[task_1.priority].size == 1);
	ASSERT(s->ready_lists[task_1.priority].head == &task_1);
	ASSERT(s->ready_lists[task_1.priority].tail == &task_1);
	ASSERT(s->ready_bitmap == 0x02);

	ASSERT(task_1.state == READY);
}

void test_task_delay()
{
	init_scheduler(); // re-init the scheduler, get a clean state

	tcb_t task_1;
	task_1.priority = 1;

	task_add_ready(&task_1);

	task_delay(&task_1);

	scheduler_t *s = get_scheduler();

	ASSERT(s->ready_lists[task_1.priority].size == 0);
	ASSERT(s->ready_lists[task_1.priority].head == NULL);
	ASSERT(s->ready_lists[task_1.priority].tail == NULL);

	ASSERT(s->delayed_list.size == 1);
	ASSERT(s->delayed_list.head == &task_1);
	ASSERT(s->delayed_list.tail == &task_1);

	ASSERT(task_1.state == DELAYED);
}

test_case_t scheduler_test_cases[] =
{
		{"scheduler init", test_scheduler_init},
		{"task add ready", test_task_add_ready},
		{"task pop ready", test_task_pop_ready},
		{"task block", test_task_block},
		{"task unblock", test_task_unblock},
		{"task block", test_task_delay}

};

// need to create an override on the ASSERT macro to make it better for testing
void scheduler_tests()
{
	int num_tests = sizeof(scheduler_test_cases) / sizeof(scheduler_test_cases[0]);

	for (int i = 0; i < num_tests; i++)
	{
		uint32_t prev_errors = num_errors;

		scheduler_test_cases[i].fn();

		if (prev_errors != num_errors)
		{
			__BKPT(0);
		}
	}
}
#endif
