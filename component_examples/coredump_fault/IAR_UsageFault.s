; Copyright 2025 NXP
; SPDX-License-Identifier: BSD-3-Clause

;IAR UsageFault_Handler.s

;    AREA |.text|, CODE, READONLY
    SECTION RO:CODE:NOROOT(2)
    THUMB

    EXTERN fault_capture
    EXPORT UsageFault_Handler

UsageFault_Handler
    MRS R0, MSP
    MRS R1, PSP
    PUSH {R0, LR}
#if defined(CONFIG_EXTRA_EXCEPTION_INFO)
    PUSH {R1, R2}
#if defined(CONFIG_ARMV6_M_ARMV8_M_BASELINE)
    MOV  R3, R11
    MOV  R2, R10
    PUSH {R2, R3}
    MOV  R3, R9
    MOV  R2, R8
    PUSH {R2, R3}
    PUSH {R4-R7}
#elif defined(CONFIG_ARMV7_M_ARMV8_M_MAINLINE)
    PUSH {R4-R11}
#endif
    MOV  R3, SP
#endif
    MOV R2, LR
    BL fault_capture
    LDR R0, =0xE000ED0C
    LDR R1, =0x05FA0004
    STR R1, [R0]

    END
