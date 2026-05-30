
#include <stdlib.h>
#include <stm32l4xx.h>
#include "unit_tests.h"
#include "../src/tcb.h"
#include "../src/ready_list.h"
#include "../src/scheduler.h"

void scheduler_tests()
{
	tcb_t t1;
	tcb_t t2;
	tcb_t t3;

	t1.priority = 1;
	t2.priority = 1;
	t3.priority = 1;

	if (init_scheduler(0) != 1 || init_scheduler(-1) != 1) // -1 overflow to highest allowed priority
	{
		__BKPT(0);
	}

	if (init_scheduler(5) != 0)
	{
		__BKPT(0);
	}

	if (get_cur_task() != NULL)
	{
		__BKPT(0);
	}

	if (add_task(NULL) != 1)
	{
		__BKPT(0);
	}

	if (add_task(&t1) != 0)
	{
		__BKPT(0);
	}

	if (select_task() != 0)
	{
		__BKPT(0);
	}

	if (get_cur_task() != &t1)
	{
		__BKPT(0);
	}

	if (remove_task(1) != 3)
	{
		__BKPT(0);
	}

	if (add_task(&t2) != 0 || add_task(&t3) != 0)
	{
		__BKPT(0);
	}

	if (remove_task(1) != 0)
	{
		__BKPT(0);
	}

	if (remove_task(1) != 0)
	{
		__BKPT(0);
	}

	if (remove_task(1) != 3)
	{
		__BKPT(0);
	}

	add_task(&t2);

	if (remove_task(3) != 3)
	{
		__BKPT(0);
	}

	tcb_t t4;

	t4.priority = 10;

	if (add_task(&t4) != 2)
	{
		__BKPT(0);
	}

	t4.priority = 4;

	if (add_task(&t4) != 0)
	{
		__BKPT(0);
	}

	tcb_t t5;

	t5.priority = 3;

	add_task(&t5);

	if (select_task() != 0)
	{
		__BKPT(0);
	}


	if (get_cur_task() != &t4)
	{
		__BKPT(0);
	}

}
