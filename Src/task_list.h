/*
 * task_list.h
 *
 *  Created on: May 24, 2026
 *      Author: Matthew Sundquist
 */
#ifndef TASK_LIST_H_
#define TASK_LIST_H_

#include "tcb.h"
#include "stdint.h"

typedef struct task_list
{
	tcb_t *head;
	tcb_t *tail;
	uint8_t size;
} task_list_t;

uint8_t init_task_list(task_list_t *rl); // init struct
uint8_t push_task(task_list_t *rl, tcb_t *task);
uint8_t pop_task(task_list_t *rl); // pops from task


#endif /* TASK_LIST_H_ */
