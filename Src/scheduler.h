/*
 * scheduler.h
 *
 *  Created on: May 27, 2026
 *      Author: Matthew Sundquist
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_


#include <stdint.h>
#include <stdbool.h>
#include "task_queue.h"
#include "tcb.h"

#define MAX_PRIORITIES 15

extern tcb_t* cur_process;
extern tcb_t* next_process;

extern uint64_t num_scheduler_ticks;

typedef struct scheduler {
	tcb_t *cur_task;
	task_queue_t ready_lists[MAX_PRIORITIES];
	task_queue_t blocked_list;
	task_queue_t delayed_list;
	uint32_t ready_bitmap;
} scheduler_t;

void task_add_ready(tcb_t *task);
tcb_t *task_pop_ready();

void task_block(tcb_t *task, task_state_t state);
void task_unblock(tcb_t *task);

void task_sleep(uint32_t ticks_asleep);

#ifdef DEBUG
tcb_t* get_cur_task();

scheduler_t *get_scheduler();
#endif

void init_scheduler();
void scheduler_tick();
void handle_blocked();


#endif /* SCHEDULER_H_ */
