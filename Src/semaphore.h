/*
 * semaphore.h
 *
 *  Created on: Jun 7, 2026
 *      Author: Matthew Sundquist
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "task_queue.h"
#include "stdint.h"

/*
 * Semaphore API:
 *
 * sem_init : init the sem
 *
 * sem_aquire : decrement counter, if cant decrement counter, put into blocked list and reschedule
 *
 * sem_release : increment counter, add head of queue to ready list, maybe reschedule, one path of execution
 *
 */
typedef struct {

	uint32_t max_count;
	uint32_t count;

	task_queue_t blocked_tasks;
} semaphore_t;

void sem_init(semaphore_t *sem, uint32_t max_count, uint32_t init_count);

void sem_aquire(semaphore_t *sem);

void sem_release(semaphore_t *sem);


#endif /* SEMAPHORE_H_ */
