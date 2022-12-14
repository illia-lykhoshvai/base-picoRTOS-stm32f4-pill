	.syntax unified
	
	/* Declarations */
	.global arch_enable_vfp
	.global arch_start_first_task
	.global arch_SYSTICK
	.global arch_PENDSV

	.global arch_compare_and_swap
	
	.extern picoRTOS_tick
	.extern picoRTOS_switch_context
	
	.macro SAVE_CONTEXT rS
	mrs \rS, psp		/* use rS as psp stack */
	stmdb \rS!, {r4-r11}	/* save registers r4-r11 */
	vstmdb \rS!, {s16-s31}	/* save fpu high registers */
	.endm

	.macro RESTORE_CONTEXT rS
	vldmia \rS!, {s16-s31}	/* restore fpu high registers */
	ldmia \rS!, {r4-r11}	/* restore registers r4-11 */
	msr psp, \rS		/* reclaim stack space */
	.endm
	
	.equ CPACR, 0xe000ed88
	.equ FPCCR, 0xe000ef34

	.align 1
	.thumb_func
	.type arch_enable_vfp, %function
arch_enable_vfp:
	/* enable CP10 & CP11 */
	ldr.w r0, =CPACR
	ldr r1, [r0]
	orr r1, r1, #0xf00000
	str r1, [r0]
	/* extended FPU frame stacking (no lazy frame) */
	ldr.w r0, =FPCCR
	movs r1, #0x80000000
	str r1, [r0]
	bx lr
	
	.align 1
	.thumb_func
	.type arch_start_first_task, %function
arch_start_first_task:
	RESTORE_CONTEXT r0
	/* switch to psp stack & activate FP */
	movs r0, #6
	msr CONTROL, r0
	isb

	pop {r0-r3, r12, lr}	/* pop the registers that are saved automatically */
	pop {r2, r3}		/* return address and xspr */

	/* pop fpu */
	mrs r4, psp
	add r4, r4, #72
	msr psp, r4

	cpsie if			/* enable interrupts */
	bx r2

	.align 1
	.thumb_func
	.type arch_SYSTICK, %function
arch_SYSTICK:
	push {lr}		/* store lr for interrupt return */
	SAVE_CONTEXT r0
	bl picoRTOS_tick
	RESTORE_CONTEXT r0
	pop {pc}	/* return */
	
	.align 1
	.thumb_func
	.type arch_PENDSV, %function
arch_PENDSV:
	push {lr}		/* store lr (interrupt return) */
	SAVE_CONTEXT r0
	bl picoRTOS_switch_context
	RESTORE_CONTEXT r0
	pop {pc}		/* return */

	.align 1
	.thumb_func
	.type arch_compare_and_swap, %function
arch_compare_and_swap:
	dmb ish
	ldrex r3, [r0]
	cmp r3, r1
	bne .arch_compare_and_swap_return
	strex r3, r2, [r0]
	cmp r3, #0
	bne .arch_compare_and_swap_return
	mov r0, r1
.arch_compare_and_swap_return:
	dmb ish
	bx lr
