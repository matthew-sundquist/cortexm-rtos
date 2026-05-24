
.syntax unified
.thumb

.global PendSV_Handler
.type PendSV_Handler, %function

.extern os_started
.extern cur_process
.extern next_process

PendSV_Handler:
    CPSID i // disable interrupts

    LDR r0, =os_started // check if this is the first task
    LDR r1, [r0]

    CMP r1, #0
    BNE save_state // already started

    MOVS r1, #1
    STR r1, [r0]

	// need these for increment_process
    LDR r0, =cur_process // &cur_process
	LDR r0, [r0] // cur_process

    B restore_state

// push R11-R4 onto the cur stack
save_state:
	LDR r0, =cur_process // &cur_process
	LDR r1, [r0] // cur_process

	MRS r2, psp

	STMDB r2!, {r4-r11}

	STR r2, [r1]

restore_state:
    // load next task stack pointer
    LDR r0, =next_process
    LDR r1, [r0]
    LDR r2, [r1]          // r0 = next_process->sp

    MSR psp, r2 // set PSP to new task sp

    ISB

    LDMIA r2!, {r4-r11} // restore r4-r11 FIRST (because they are on top of stack)

    MSR psp, r2

increment_process:

	LDR r0, =cur_process
	LDR r1, =next_process
	LDR r1, [r1]
	STR r1, [r0]

	LDR lr, =0xFFFFFFFD
	CPSIE i
	BX lr
