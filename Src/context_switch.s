

.syntax unified
.thumb

.global context_switch
.type context_switch, %function

context_switch:
	mrs r2, psp
	stmdb r2!, {r4-r11}
	str r2, [r0]

	ldr r2, [r1]
	ldmia r2!, {r4-r11}
	msr psp, r2

	bx lr
