/* Copyright 2025 NXP                                                   */
/* SPDX-License-Identifier: BSD-3-Clause                                     */

	.syntax unified
	.arch armv8-m.main
    .thumb
	.align 2
    .global UsageFault_Handler
	.type   UsageFault_Handler, %function

UsageFault_Handler:
	mrs r0, MSP
	mrs r1, PSP
	push {r0, lr}
#if defined(CONFIG_EXTRA_EXCEPTION_INFO)
	/* Build _callee_saved_t. To match the struct
	 * definition we push the psp & then r11-r4
	 */
	push { r1, r2 }
#if defined(CONFIG_ARMV6_M_ARMV8_M_BASELINE)
	mov  r3, r11
	mov  r2, r10
	push {r2, r3}
	mov  r3, r9
	mov  r2, r8
	push {r2, r3}
	push {r4-r7}
#elif defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
	push {r4-r11}
#endif
	mov  r3, sp /* pointer to _callee_saved_t */
#endif /* CONFIG_EXTRA_EXCEPTION_INFO */
	mov r2, lr /* EXC_RETURN */

	bl fault_capture
    ldr r0, =0xE000ED0C
    ldr r1, =0x05FA0004
    str r1, [r0]



    .end
