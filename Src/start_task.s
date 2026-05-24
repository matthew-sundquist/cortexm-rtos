
.syntax unified
.thumb

.global PendSV_Handler
.type PendSV_Handler, %function

.extern os_started
.extern cur_process
.extern next_process

PendSV_Handler:
    CPSID i

    // load next task stack pointer
    LDR r1, =next_process
    LDR r1, [r1]
    LDR r0, [r1]          // r0 = next_process->sp

    // restore r4-r11 FIRST (because they are on top of stack)
    LDMIA r0!, {r4-r11}

    MSR psp, r0

    CPSIE i

	LDR lr, =0xFFFFFFFD
	BX lr
    //LDR lr, =0xFFFFFFFD
    //BX lr
