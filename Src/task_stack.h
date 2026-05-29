/*
 * task_stack.h
 *
 *  Created on: May 28, 2026
 *      Author: Matthew Sundquist
 */

#ifndef TASK_STACK_H_
#define TASK_STACK_H_
#include <stdint.h>


uint32_t *init_stack(uint32_t *sp, void (*entry)(void), uint32_t *arg);

#endif /* TASK_STACK_H_ */
