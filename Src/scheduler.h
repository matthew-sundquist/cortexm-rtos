/*
 * scheduler.h
 *
 *  Created on: May 27, 2026
 *      Author: Matthew Sundquist
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>
#include "ready_list.h"
#include "tcb.h"

#define MAX_PRIORITIES 15

extern tcb_t* cur_process;
extern tcb_t* next_process;

extern uint64_t num_scheduler_ticks;

typedef struct scheduler {
	tcb_t *cur_task;
	ready_list_t ready_lists[MAX_PRIORITIES];
	uint8_t num_priorities;
	uint32_t ready_bitmap;
} scheduler_t;

uint8_t init_scheduler(uint8_t num_priorities);
uint8_t add_task(tcb_t *task);
uint8_t remove_task(uint8_t priority);
uint8_t select_task();

tcb_t* get_cur_task();

void scheduler_tick();


#endif /* SCHEDULER_H_ */
