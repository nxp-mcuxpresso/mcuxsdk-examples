; -------------------------------------------------------------------------
;  @file:    startup_MCXA276.s
;  @purpose: CMSIS Cortex-M33 Core Device Startup File
;            MCXA276
;  @version: 1.0
;  @date:    2024-11-21
;  @build:   b241224
; -------------------------------------------------------------------------
;
; Copyright 1997-2016 Freescale Semiconductor, Inc.
; Copyright 2016-2024 NXP
; SPDX-License-Identifier: BSD-3-Clause
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__iar_init$$done:              ; The vector table is not needed
                      ; until after copy initialization is done

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler                                   ;NMI Handler
        DCD     HardFault_Handler                             ;Hard Fault Handler
        DCD     MemManage_Handler                             ;MPU Fault Handler
        DCD     BusFault_Handler                              ;Bus Fault Handler
        DCD     UsageFault_Handler                            ;Usage Fault Handler
__vector_table_0x1c
        DCD     SecureFault_Handler                           ;Secure Fault Handler
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     SVC_Handler                                   ;SVCall Handler
        DCD     DebugMon_Handler                              ;Debug Monitor Handler
        DCD     0                                             ;Reserved
        DCD     PendSV_Handler                                ;PendSV Handler
        DCD     SysTick_Handler                               ;SysTick Handler

                                                              ;External Interrupts
        DCD     Reserved16_IRQHandler                         ;OR IRQ1 to IRQ53
        DCD     CMC_IRQHandler                                ;Core Mode Controller interrupt
        DCD     DMA_CH0_IRQHandler                            ;DMA3_0_CH0 error or transfer complete
        DCD     DMA_CH1_IRQHandler                            ;DMA3_0_CH1 error or transfer complete
        DCD     DMA_CH2_IRQHandler                            ;DMA3_0_CH2 error or transfer complete
        DCD     DMA_CH3_IRQHandler                            ;DMA3_0_CH3 error or transfer complete
        DCD     DMA_CH4_IRQHandler                            ;DMA3_0_CH4 error or transfer complete
        DCD     DMA_CH5_IRQHandler                            ;DMA3_0_CH5 error or transfer complete
        DCD     DMA_CH6_IRQHandler                            ;DMA3_0_CH6 error or transfer complete
        DCD     DMA_CH7_IRQHandler                            ;DMA3_0_CH7 error or transfer complete
        DCD     ERM0_SINGLE_BIT_IRQHandler                    ;ERM Single Bit error interrupt
        DCD     ERM0_MULTI_BIT_IRQHandler                     ;ERM Multi Bit error interrupt
        DCD     FMU0_IRQHandler                               ;Flash Management Unit interrupt
        DCD     GLIKEY0_IRQHandler                            ;GLIKEY Interrupt
        DCD     MBC0_IRQHandler                               ;MBC secure violation interrupt
        DCD     SCG0_IRQHandler                               ;System Clock Generator interrupt
        DCD     SPC0_IRQHandler                               ;System Power Controller interrupt
        DCD     TDET_IRQHandler                               ;TDET interrrupt
        DCD     WUU0_IRQHandler                               ;Wake Up Unit interrupt
        DCD     CAN0_IRQHandler                               ;Controller Area Network 0 interrupt
        DCD     CAN1_IRQHandler                               ;Controller Area Network 1 interrupt
        DCD     Reserved37_IRQHandler                         ;xxx Interrupt 37
        DCD     Reserved38_IRQHandler                         ;xxx Interrupt 38
        DCD     FLEXIO_IRQHandler                             ;Flexible Input/Output interrupt
        DCD     I3C0_IRQHandler                               ;Improved Inter Integrated Circuit interrupt 0
        DCD     Reserved41_IRQHandler                         ;xxx Interrupt 41
        DCD     LPI2C0_IRQHandler                             ;Low-Power Inter Integrated Circuit 0 interrupt
        DCD     LPI2C1_IRQHandler                             ;Low-Power Inter Integrated Circuit 1 interrupt
        DCD     LPSPI0_IRQHandler                             ;Low-Power Serial Peripheral Interface 0 interrupt
        DCD     LPSPI1_IRQHandler                             ;Low-Power Serial Peripheral Interface 1 interrupt
        DCD     Reserved46_IRQHandler                         ;xxx Interrupt 46
        DCD     LPUART0_IRQHandler                            ;Low-Power Universal Asynchronous Receive/Transmit 0 interrupt
        DCD     LPUART1_IRQHandler                            ;Low-Power Universal Asynchronous Receive/Transmit 1 interrupt
        DCD     LPUART2_IRQHandler                            ;Low-Power Universal Asynchronous Receive/Transmit 2 interrupt
        DCD     LPUART3_IRQHandler                            ;Low-Power Universal Asynchronous Receive/Transmit 3 interrupt
        DCD     LPUART4_IRQHandler                            ;Low-Power Universal Asynchronous Receive/Transmit 4 interrupt
        DCD     USB0_IRQHandler                               ;Universal Serial Bus - Full Speed interrupt
        DCD     Reserved53_IRQHandler                         ;xxx Interrupt 53
        DCD     CDOG0_IRQHandler                              ;Code Watchdog Timer 0 interrupt
        DCD     CTIMER0_IRQHandler                            ;Standard counter/timer 0 interrupt
        DCD     CTIMER1_IRQHandler                            ;Standard counter/timer 1 interrupt
        DCD     CTIMER2_IRQHandler                            ;Standard counter/timer 2 interrupt
        DCD     CTIMER3_IRQHandler                            ;Standard counter/timer 3 interrupt
        DCD     CTIMER4_IRQHandler                            ;Standard counter/timer 4 interrupt
        DCD     FLEXPWM0_RELOAD_ERROR_IRQHandler              ;FlexPWM0_reload_error interrupt
        DCD     FLEXPWM0_FAULT_IRQHandler                     ;FlexPWM0_fault interrupt
        DCD     FLEXPWM0_SUBMODULE0_IRQHandler                ;FlexPWM0 Submodule 0 capture/compare/reload interrupt
        DCD     FLEXPWM0_SUBMODULE1_IRQHandler                ;FlexPWM0 Submodule 1 capture/compare/reload interrupt
        DCD     FLEXPWM0_SUBMODULE2_IRQHandler                ;FlexPWM0 Submodule 2 capture/compare/reload interrupt
        DCD     FLEXPWM0_SUBMODULE3_IRQHandler                ;FlexPWM0 Submodule 3 capture/compare/reload interrupt
        DCD     EQDC0_COMPARE_IRQHandler                      ;Compare
        DCD     EQDC0_HOME_IRQHandler                         ;Home
        DCD     EQDC0_WATCHDOG_IRQHandler                     ;Watchdog / Simultaneous A and B Change
        DCD     EQDC0_INDEX_IRQHandler                        ;Index / Roll Over / Roll Under
        DCD     FREQME0_IRQHandler                            ;Frequency Measurement interrupt
        DCD     LPTMR0_IRQHandler                             ;Low Power Timer 0 interrupt
        DCD     Reserved72_IRQHandler                         ;xxx Interrupt 72
        DCD     OS_EVENT_IRQHandler                           ;OS event timer interrupt
        DCD     WAKETIMER0_IRQHandler                         ;Wake Timer Interrupt
        DCD     UTICK0_IRQHandler                             ;Micro-Tick Timer interrupt
        DCD     WWDT0_IRQHandler                              ;Windowed Watchdog Timer 0 interrupt
        DCD     Reserved77_IRQHandler                         ;xxx Interrupt 77
        DCD     ADC0_IRQHandler                               ;Analog-to-Digital Converter 0 interrupt
        DCD     ADC1_IRQHandler                               ;Analog-to-Digital Converter 1 interrupt
        DCD     CMP0_IRQHandler                               ;Comparator 0 interrupt
        DCD     CMP1_IRQHandler                               ;Comparator 1 interrupt
        DCD     CMP2_IRQHandler                               ;Comparator 2 interrupt
        DCD     DAC0_IRQHandler                               ;Digital-to-Analog Converter 0 - General Purpose interrupt
        DCD     Reserved84_IRQHandler                         ;xxx Interrupt 84
        DCD     Reserved85_IRQHandler                         ;xxx Interrupt 85
        DCD     Reserved86_IRQHandler                         ;xxx Interrupt 86
        DCD     GPIO0_IRQHandler                              ;General Purpose Input/Output interrupt 0
        DCD     GPIO1_IRQHandler                              ;General Purpose Input/Output interrupt 1
        DCD     GPIO2_IRQHandler                              ;General Purpose Input/Output interrupt 2
        DCD     GPIO3_IRQHandler                              ;General Purpose Input/Output interrupt 3
        DCD     GPIO4_IRQHandler                              ;General Purpose Input/Output interrupt 4
        DCD     Reserved92_IRQHandler                         ;xxx Interrupt 92
        DCD     LPI2C2_IRQHandler                             ;Low-Power Inter Integrated Circuit 2 interrupt
        DCD     LPI2C3_IRQHandler                             ;Low-Power Inter Integrated Circuit 3 interrupt
        DCD     FLEXPWM1_RELOAD_ERROR_IRQHandler              ;FlexPWM1_reload_error interrupt
        DCD     FLEXPWM1_FAULT_IRQHandler                     ;FlexPWM1_fault interrupt
        DCD     FLEXPWM1_SUBMODULE0_IRQHandler                ;FlexPWM1 Submodule 0 capture/compare/reload interrupt
        DCD     FLEXPWM1_SUBMODULE1_IRQHandler                ;FlexPWM1 Submodule 1 capture/compare/reload interrupt
        DCD     FLEXPWM1_SUBMODULE2_IRQHandler                ;FlexPWM1 Submodule 2 capture/compare/reload interrupt
        DCD     FLEXPWM1_SUBMODULE3_IRQHandler                ;FlexPWM1 Submodule 3 capture/compare/reload interrupt
        DCD     EQDC1_COMPARE_IRQHandler                      ;Compare
        DCD     EQDC1_HOME_IRQHandler                         ;Home
        DCD     EQDC1_WATCHDOG_IRQHandler                     ;Watchdog / Simultaneous A and B Change
        DCD     EQDC1_INDEX_IRQHandler                        ;Index / Roll Over / Roll Under
        DCD     Reserved105_IRQHandler                        ;xxx Interrupt 105
        DCD     Reserved106_IRQHandler                        ;xxx Interrupt 106
        DCD     Reserved107_IRQHandler                        ;xxx Interrupt 107
        DCD     Reserved108_IRQHandler                        ;xxx Interrupt 108
        DCD     Reserved109_IRQHandler                        ;xxx Interrupt 109
        DCD     Reserved110_IRQHandler                        ;xxx Interrupt 110
        DCD     LPUART5_IRQHandler                            ;Low-Power Universal Asynchronous Receive/Transmit interrupt
        DCD     Reserved112_IRQHandler                        ;xxx Interrupt 112
        DCD     Reserved113_IRQHandler                        ;xxx Interrupt 113
        DCD     Reserved114_IRQHandler                        ;xxx Interrupt 114
        DCD     Reserved115_IRQHandler                        ;xxx Interrupt 115
        DCD     Reserved116_IRQHandler                        ;xxx Interrupt 116
        DCD     Reserved117_IRQHandler                        ;xxx Interrupt 117
        DCD     Reserved118_IRQHandler                        ;xxx Interrupt 118
        DCD     Reserved119_IRQHandler                        ;xxx Interrupt 119
        DCD     Reserved120_IRQHandler                        ;xxx Interrupt 120
        DCD     Reserved121_IRQHandler                        ;xxx Interrupt 121
        DCD     Reserved122_IRQHandler                        ;xxx Interrupt 122
        DCD     MAU_IRQHandler                                ;MAU interrupt
        DCD     SMARTDMA_IRQHandler                           ;SmartDMA interrupt
        DCD     CDOG1_IRQHandler                              ;Code Watchdog Timer 1 interrupt
        DCD     PKC_IRQHandler                                ;PKC interrupt
        DCD     SGI_IRQHandler                                ;SGI interrupt
        DCD     Reserved128_IRQHandler                        ;xxx Interrupt 128
        DCD     TRNG0_IRQHandler                              ;True Random Number Generator interrupt
        DCD     SECURE_ERR_IRQHandler                         ;Secure IP Error interrupt. It OR SGI, PKC, TRNG error together.
        DCD     Reserved131_IRQHandler                        ;xxx Interrupt 131
        DCD     ADC2_IRQHandler                               ;Analog-to-Digital Converter 2 interrupt
        DCD     ADC3_IRQHandler                               ;Analog-to-Digital Converter 3 interrupt
        DCD     Reserved134_IRQHandler                        ;xxx Interrupt 134
        DCD     RTC_IRQHandler                                ;RTC alarm interrupt
        DCD     RTC_1HZ_IRQHandler                            ;RTC 1Hz interrupt
        DCD     SLCD_IRQHandler                               ;SLCD frame start interrupt
__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        CPSID   I               ; Mask interrupts
        LDR     R0, =0xE000ED08
        LDR     R1, =__vector_table
        STR     R1, [R0]
        LDR     R2, [R1]
        MSR     MSP, R2
        LDR     R0, =sfb(CSTACK)
        MSR     MSPLIM, R0
        CPSIE   I               ; Unmask interrupts
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B .

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B .

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B .

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B .

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B .

        PUBWEAK SecureFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SecureFault_Handler
        B .

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B .

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B .

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B .

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B .

        PUBWEAK Reserved16_IRQHandler
        PUBWEAK Reserved16_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved16_IRQHandler
        LDR     R0, =Reserved16_DriverIRQHandler
        BX      R0

        PUBWEAK CMC_IRQHandler
        PUBWEAK CMC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CMC_IRQHandler
        LDR     R0, =CMC_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_CH0_IRQHandler
        PUBWEAK DMA_CH0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_CH0_IRQHandler
        LDR     R0, =DMA_CH0_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_CH1_IRQHandler
        PUBWEAK DMA_CH1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_CH1_IRQHandler
        LDR     R0, =DMA_CH1_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_CH2_IRQHandler
        PUBWEAK DMA_CH2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_CH2_IRQHandler
        LDR     R0, =DMA_CH2_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_CH3_IRQHandler
        PUBWEAK DMA_CH3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_CH3_IRQHandler
        LDR     R0, =DMA_CH3_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_CH4_IRQHandler
        PUBWEAK DMA_CH4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_CH4_IRQHandler
        LDR     R0, =DMA_CH4_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_CH5_IRQHandler
        PUBWEAK DMA_CH5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_CH5_IRQHandler
        LDR     R0, =DMA_CH5_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_CH6_IRQHandler
        PUBWEAK DMA_CH6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_CH6_IRQHandler
        LDR     R0, =DMA_CH6_DriverIRQHandler
        BX      R0

        PUBWEAK DMA_CH7_IRQHandler
        PUBWEAK DMA_CH7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_CH7_IRQHandler
        LDR     R0, =DMA_CH7_DriverIRQHandler
        BX      R0

        PUBWEAK ERM0_SINGLE_BIT_IRQHandler
        PUBWEAK ERM0_SINGLE_BIT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ERM0_SINGLE_BIT_IRQHandler
        LDR     R0, =ERM0_SINGLE_BIT_DriverIRQHandler
        BX      R0

        PUBWEAK ERM0_MULTI_BIT_IRQHandler
        PUBWEAK ERM0_MULTI_BIT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ERM0_MULTI_BIT_IRQHandler
        LDR     R0, =ERM0_MULTI_BIT_DriverIRQHandler
        BX      R0

        PUBWEAK FMU0_IRQHandler
        PUBWEAK FMU0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FMU0_IRQHandler
        LDR     R0, =FMU0_DriverIRQHandler
        BX      R0

        PUBWEAK GLIKEY0_IRQHandler
        PUBWEAK GLIKEY0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GLIKEY0_IRQHandler
        LDR     R0, =GLIKEY0_DriverIRQHandler
        BX      R0

        PUBWEAK MBC0_IRQHandler
        PUBWEAK MBC0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MBC0_IRQHandler
        LDR     R0, =MBC0_DriverIRQHandler
        BX      R0

        PUBWEAK SCG0_IRQHandler
        PUBWEAK SCG0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SCG0_IRQHandler
        LDR     R0, =SCG0_DriverIRQHandler
        BX      R0

        PUBWEAK SPC0_IRQHandler
        PUBWEAK SPC0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SPC0_IRQHandler
        LDR     R0, =SPC0_DriverIRQHandler
        BX      R0

        PUBWEAK TDET_IRQHandler
        PUBWEAK TDET_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
TDET_IRQHandler
        LDR     R0, =TDET_DriverIRQHandler
        BX      R0

        PUBWEAK WUU0_IRQHandler
        PUBWEAK WUU0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WUU0_IRQHandler
        LDR     R0, =WUU0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN0_IRQHandler
        PUBWEAK CAN0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN0_IRQHandler
        LDR     R0, =CAN0_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_IRQHandler
        PUBWEAK CAN1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved37_IRQHandler
        PUBWEAK Reserved37_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved37_IRQHandler
        LDR     R0, =Reserved37_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved38_IRQHandler
        PUBWEAK Reserved38_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved38_IRQHandler
        LDR     R0, =Reserved38_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXIO_IRQHandler
        PUBWEAK FLEXIO_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXIO_IRQHandler
        LDR     R0, =FLEXIO_DriverIRQHandler
        BX      R0

        PUBWEAK I3C0_IRQHandler
        PUBWEAK I3C0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I3C0_IRQHandler
        LDR     R0, =I3C0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved41_IRQHandler
        PUBWEAK Reserved41_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved41_IRQHandler
        LDR     R0, =Reserved41_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C0_IRQHandler
        PUBWEAK LPI2C0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C0_IRQHandler
        LDR     R0, =LPI2C0_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C1_IRQHandler
        PUBWEAK LPI2C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C1_IRQHandler
        LDR     R0, =LPI2C1_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI0_IRQHandler
        PUBWEAK LPSPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI0_IRQHandler
        LDR     R0, =LPSPI0_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI1_IRQHandler
        PUBWEAK LPSPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI1_IRQHandler
        LDR     R0, =LPSPI1_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved46_IRQHandler
        PUBWEAK Reserved46_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved46_IRQHandler
        LDR     R0, =Reserved46_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART0_IRQHandler
        PUBWEAK LPUART0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART0_IRQHandler
        LDR     R0, =LPUART0_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART1_IRQHandler
        PUBWEAK LPUART1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART1_IRQHandler
        LDR     R0, =LPUART1_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART2_IRQHandler
        PUBWEAK LPUART2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART2_IRQHandler
        LDR     R0, =LPUART2_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART3_IRQHandler
        PUBWEAK LPUART3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART3_IRQHandler
        LDR     R0, =LPUART3_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART4_IRQHandler
        PUBWEAK LPUART4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART4_IRQHandler
        LDR     R0, =LPUART4_DriverIRQHandler
        BX      R0

        PUBWEAK USB0_IRQHandler
        PUBWEAK USB0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USB0_IRQHandler
        LDR     R0, =USB0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved53_IRQHandler
        PUBWEAK Reserved53_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved53_IRQHandler
        LDR     R0, =Reserved53_DriverIRQHandler
        BX      R0

        PUBWEAK CDOG0_IRQHandler
        PUBWEAK CDOG0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CDOG0_IRQHandler
        LDR     R0, =CDOG0_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER0_IRQHandler
        PUBWEAK CTIMER0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER0_IRQHandler
        LDR     R0, =CTIMER0_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER1_IRQHandler
        PUBWEAK CTIMER1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER1_IRQHandler
        LDR     R0, =CTIMER1_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER2_IRQHandler
        PUBWEAK CTIMER2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER2_IRQHandler
        LDR     R0, =CTIMER2_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER3_IRQHandler
        PUBWEAK CTIMER3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER3_IRQHandler
        LDR     R0, =CTIMER3_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER4_IRQHandler
        PUBWEAK CTIMER4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER4_IRQHandler
        LDR     R0, =CTIMER4_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXPWM0_RELOAD_ERROR_IRQHandler
        PUBWEAK FLEXPWM0_RELOAD_ERROR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXPWM0_RELOAD_ERROR_IRQHandler
        LDR     R0, =FLEXPWM0_RELOAD_ERROR_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXPWM0_FAULT_IRQHandler
        PUBWEAK FLEXPWM0_FAULT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXPWM0_FAULT_IRQHandler
        LDR     R0, =FLEXPWM0_FAULT_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXPWM0_SUBMODULE0_IRQHandler
        PUBWEAK FLEXPWM0_SUBMODULE0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXPWM0_SUBMODULE0_IRQHandler
        LDR     R0, =FLEXPWM0_SUBMODULE0_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXPWM0_SUBMODULE1_IRQHandler
        PUBWEAK FLEXPWM0_SUBMODULE1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXPWM0_SUBMODULE1_IRQHandler
        LDR     R0, =FLEXPWM0_SUBMODULE1_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXPWM0_SUBMODULE2_IRQHandler
        PUBWEAK FLEXPWM0_SUBMODULE2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXPWM0_SUBMODULE2_IRQHandler
        LDR     R0, =FLEXPWM0_SUBMODULE2_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXPWM0_SUBMODULE3_IRQHandler
        PUBWEAK FLEXPWM0_SUBMODULE3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXPWM0_SUBMODULE3_IRQHandler
        LDR     R0, =FLEXPWM0_SUBMODULE3_DriverIRQHandler
        BX      R0

        PUBWEAK EQDC0_COMPARE_IRQHandler
        PUBWEAK EQDC0_COMPARE_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EQDC0_COMPARE_IRQHandler
        LDR     R0, =EQDC0_COMPARE_DriverIRQHandler
        BX      R0

        PUBWEAK EQDC0_HOME_IRQHandler
        PUBWEAK EQDC0_HOME_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EQDC0_HOME_IRQHandler
        LDR     R0, =EQDC0_HOME_DriverIRQHandler
        BX      R0

        PUBWEAK EQDC0_WATCHDOG_IRQHandler
        PUBWEAK EQDC0_WATCHDOG_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EQDC0_WATCHDOG_IRQHandler
        LDR     R0, =EQDC0_WATCHDOG_DriverIRQHandler
        BX      R0

        PUBWEAK EQDC0_INDEX_IRQHandler
        PUBWEAK EQDC0_INDEX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EQDC0_INDEX_IRQHandler
        LDR     R0, =EQDC0_INDEX_DriverIRQHandler
        BX      R0

        PUBWEAK FREQME0_IRQHandler
        PUBWEAK FREQME0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FREQME0_IRQHandler
        LDR     R0, =FREQME0_DriverIRQHandler
        BX      R0

        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK LPTMR0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPTMR0_IRQHandler
        LDR     R0, =LPTMR0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved72_IRQHandler
        PUBWEAK Reserved72_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved72_IRQHandler
        LDR     R0, =Reserved72_DriverIRQHandler
        BX      R0

        PUBWEAK OS_EVENT_IRQHandler
        PUBWEAK OS_EVENT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
OS_EVENT_IRQHandler
        LDR     R0, =OS_EVENT_DriverIRQHandler
        BX      R0

        PUBWEAK WAKETIMER0_IRQHandler
        PUBWEAK WAKETIMER0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WAKETIMER0_IRQHandler
        LDR     R0, =WAKETIMER0_DriverIRQHandler
        BX      R0

        PUBWEAK UTICK0_IRQHandler
        PUBWEAK UTICK0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UTICK0_IRQHandler
        LDR     R0, =UTICK0_DriverIRQHandler
        BX      R0

        PUBWEAK WWDT0_IRQHandler
        PUBWEAK WWDT0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WWDT0_IRQHandler
        LDR     R0, =WWDT0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved77_IRQHandler
        PUBWEAK Reserved77_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved77_IRQHandler
        LDR     R0, =Reserved77_DriverIRQHandler
        BX      R0

        PUBWEAK ADC0_IRQHandler
        PUBWEAK ADC0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADC0_IRQHandler
        LDR     R0, =ADC0_DriverIRQHandler
        BX      R0

        PUBWEAK ADC1_IRQHandler
        PUBWEAK ADC1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADC1_IRQHandler
        LDR     R0, =ADC1_DriverIRQHandler
        BX      R0

        PUBWEAK CMP0_IRQHandler
        PUBWEAK CMP0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CMP0_IRQHandler
        LDR     R0, =CMP0_DriverIRQHandler
        BX      R0

        PUBWEAK CMP1_IRQHandler
        PUBWEAK CMP1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CMP1_IRQHandler
        LDR     R0, =CMP1_DriverIRQHandler
        BX      R0

        PUBWEAK CMP2_IRQHandler
        PUBWEAK CMP2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CMP2_IRQHandler
        LDR     R0, =CMP2_DriverIRQHandler
        BX      R0

        PUBWEAK DAC0_IRQHandler
        PUBWEAK DAC0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DAC0_IRQHandler
        LDR     R0, =DAC0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved84_IRQHandler
        PUBWEAK Reserved84_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved84_IRQHandler
        LDR     R0, =Reserved84_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved85_IRQHandler
        PUBWEAK Reserved85_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved85_IRQHandler
        LDR     R0, =Reserved85_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved86_IRQHandler
        PUBWEAK Reserved86_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved86_IRQHandler
        LDR     R0, =Reserved86_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO0_IRQHandler
        PUBWEAK GPIO0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO0_IRQHandler
        LDR     R0, =GPIO0_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO1_IRQHandler
        PUBWEAK GPIO1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO1_IRQHandler
        LDR     R0, =GPIO1_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO2_IRQHandler
        PUBWEAK GPIO2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO2_IRQHandler
        LDR     R0, =GPIO2_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO3_IRQHandler
        PUBWEAK GPIO3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO3_IRQHandler
        LDR     R0, =GPIO3_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO4_IRQHandler
        PUBWEAK GPIO4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO4_IRQHandler
        LDR     R0, =GPIO4_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved92_IRQHandler
        PUBWEAK Reserved92_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved92_IRQHandler
        LDR     R0, =Reserved92_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C2_IRQHandler
        PUBWEAK LPI2C2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C2_IRQHandler
        LDR     R0, =LPI2C2_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C3_IRQHandler
        PUBWEAK LPI2C3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C3_IRQHandler
        LDR     R0, =LPI2C3_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXPWM1_RELOAD_ERROR_IRQHandler
        PUBWEAK FLEXPWM1_RELOAD_ERROR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXPWM1_RELOAD_ERROR_IRQHandler
        LDR     R0, =FLEXPWM1_RELOAD_ERROR_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXPWM1_FAULT_IRQHandler
        PUBWEAK FLEXPWM1_FAULT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXPWM1_FAULT_IRQHandler
        LDR     R0, =FLEXPWM1_FAULT_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXPWM1_SUBMODULE0_IRQHandler
        PUBWEAK FLEXPWM1_SUBMODULE0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXPWM1_SUBMODULE0_IRQHandler
        LDR     R0, =FLEXPWM1_SUBMODULE0_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXPWM1_SUBMODULE1_IRQHandler
        PUBWEAK FLEXPWM1_SUBMODULE1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXPWM1_SUBMODULE1_IRQHandler
        LDR     R0, =FLEXPWM1_SUBMODULE1_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXPWM1_SUBMODULE2_IRQHandler
        PUBWEAK FLEXPWM1_SUBMODULE2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXPWM1_SUBMODULE2_IRQHandler
        LDR     R0, =FLEXPWM1_SUBMODULE2_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXPWM1_SUBMODULE3_IRQHandler
        PUBWEAK FLEXPWM1_SUBMODULE3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXPWM1_SUBMODULE3_IRQHandler
        LDR     R0, =FLEXPWM1_SUBMODULE3_DriverIRQHandler
        BX      R0

        PUBWEAK EQDC1_COMPARE_IRQHandler
        PUBWEAK EQDC1_COMPARE_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EQDC1_COMPARE_IRQHandler
        LDR     R0, =EQDC1_COMPARE_DriverIRQHandler
        BX      R0

        PUBWEAK EQDC1_HOME_IRQHandler
        PUBWEAK EQDC1_HOME_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EQDC1_HOME_IRQHandler
        LDR     R0, =EQDC1_HOME_DriverIRQHandler
        BX      R0

        PUBWEAK EQDC1_WATCHDOG_IRQHandler
        PUBWEAK EQDC1_WATCHDOG_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EQDC1_WATCHDOG_IRQHandler
        LDR     R0, =EQDC1_WATCHDOG_DriverIRQHandler
        BX      R0

        PUBWEAK EQDC1_INDEX_IRQHandler
        PUBWEAK EQDC1_INDEX_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
EQDC1_INDEX_IRQHandler
        LDR     R0, =EQDC1_INDEX_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved105_IRQHandler
        PUBWEAK Reserved105_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved105_IRQHandler
        LDR     R0, =Reserved105_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved106_IRQHandler
        PUBWEAK Reserved106_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved106_IRQHandler
        LDR     R0, =Reserved106_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved107_IRQHandler
        PUBWEAK Reserved107_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved107_IRQHandler
        LDR     R0, =Reserved107_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved108_IRQHandler
        PUBWEAK Reserved108_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved108_IRQHandler
        LDR     R0, =Reserved108_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved109_IRQHandler
        PUBWEAK Reserved109_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved109_IRQHandler
        LDR     R0, =Reserved109_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved110_IRQHandler
        PUBWEAK Reserved110_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved110_IRQHandler
        LDR     R0, =Reserved110_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART5_IRQHandler
        PUBWEAK LPUART5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART5_IRQHandler
        LDR     R0, =LPUART5_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved112_IRQHandler
        PUBWEAK Reserved112_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved112_IRQHandler
        LDR     R0, =Reserved112_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved113_IRQHandler
        PUBWEAK Reserved113_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved113_IRQHandler
        LDR     R0, =Reserved113_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved114_IRQHandler
        PUBWEAK Reserved114_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved114_IRQHandler
        LDR     R0, =Reserved114_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved115_IRQHandler
        PUBWEAK Reserved115_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved115_IRQHandler
        LDR     R0, =Reserved115_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved116_IRQHandler
        PUBWEAK Reserved116_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved116_IRQHandler
        LDR     R0, =Reserved116_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved117_IRQHandler
        PUBWEAK Reserved117_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved117_IRQHandler
        LDR     R0, =Reserved117_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved118_IRQHandler
        PUBWEAK Reserved118_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved118_IRQHandler
        LDR     R0, =Reserved118_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved119_IRQHandler
        PUBWEAK Reserved119_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved119_IRQHandler
        LDR     R0, =Reserved119_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved120_IRQHandler
        PUBWEAK Reserved120_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved120_IRQHandler
        LDR     R0, =Reserved120_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved121_IRQHandler
        PUBWEAK Reserved121_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved121_IRQHandler
        LDR     R0, =Reserved121_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved122_IRQHandler
        PUBWEAK Reserved122_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved122_IRQHandler
        LDR     R0, =Reserved122_DriverIRQHandler
        BX      R0

        PUBWEAK MAU_IRQHandler
        PUBWEAK MAU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MAU_IRQHandler
        LDR     R0, =MAU_DriverIRQHandler
        BX      R0

        PUBWEAK SMARTDMA_IRQHandler
        PUBWEAK SMARTDMA_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SMARTDMA_IRQHandler
        LDR     R0, =SMARTDMA_DriverIRQHandler
        BX      R0

        PUBWEAK CDOG1_IRQHandler
        PUBWEAK CDOG1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CDOG1_IRQHandler
        LDR     R0, =CDOG1_DriverIRQHandler
        BX      R0

        PUBWEAK PKC_IRQHandler
        PUBWEAK PKC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PKC_IRQHandler
        LDR     R0, =PKC_DriverIRQHandler
        BX      R0

        PUBWEAK SGI_IRQHandler
        PUBWEAK SGI_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SGI_IRQHandler
        LDR     R0, =SGI_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved128_IRQHandler
        PUBWEAK Reserved128_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved128_IRQHandler
        LDR     R0, =Reserved128_DriverIRQHandler
        BX      R0

        PUBWEAK TRNG0_IRQHandler
        PUBWEAK TRNG0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
TRNG0_IRQHandler
        LDR     R0, =TRNG0_DriverIRQHandler
        BX      R0

        PUBWEAK SECURE_ERR_IRQHandler
        PUBWEAK SECURE_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SECURE_ERR_IRQHandler
        LDR     R0, =SECURE_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved131_IRQHandler
        PUBWEAK Reserved131_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved131_IRQHandler
        LDR     R0, =Reserved131_DriverIRQHandler
        BX      R0

        PUBWEAK ADC2_IRQHandler
        PUBWEAK ADC2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADC2_IRQHandler
        LDR     R0, =ADC2_DriverIRQHandler
        BX      R0

        PUBWEAK ADC3_IRQHandler
        PUBWEAK ADC3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ADC3_IRQHandler
        LDR     R0, =ADC3_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved134_IRQHandler
        PUBWEAK Reserved134_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved134_IRQHandler
        LDR     R0, =Reserved134_DriverIRQHandler
        BX      R0

        PUBWEAK RTC_IRQHandler
        PUBWEAK RTC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
RTC_IRQHandler
        LDR     R0, =RTC_DriverIRQHandler
        BX      R0

        PUBWEAK RTC_1HZ_IRQHandler
        PUBWEAK RTC_1HZ_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
RTC_1HZ_IRQHandler
        LDR     R0, =RTC_1HZ_DriverIRQHandler
        BX      R0

        PUBWEAK SLCD_IRQHandler
        PUBWEAK SLCD_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SLCD_IRQHandler
        LDR     R0, =SLCD_DriverIRQHandler
        BX      R0

Reserved16_DriverIRQHandler
CMC_DriverIRQHandler
DMA_CH0_DriverIRQHandler
DMA_CH1_DriverIRQHandler
DMA_CH2_DriverIRQHandler
DMA_CH3_DriverIRQHandler
DMA_CH4_DriverIRQHandler
DMA_CH5_DriverIRQHandler
DMA_CH6_DriverIRQHandler
DMA_CH7_DriverIRQHandler
ERM0_SINGLE_BIT_DriverIRQHandler
ERM0_MULTI_BIT_DriverIRQHandler
FMU0_DriverIRQHandler
GLIKEY0_DriverIRQHandler
MBC0_DriverIRQHandler
SCG0_DriverIRQHandler
SPC0_DriverIRQHandler
TDET_DriverIRQHandler
WUU0_DriverIRQHandler
CAN0_DriverIRQHandler
CAN1_DriverIRQHandler
Reserved37_DriverIRQHandler
Reserved38_DriverIRQHandler
FLEXIO_DriverIRQHandler
I3C0_DriverIRQHandler
Reserved41_DriverIRQHandler
LPI2C0_DriverIRQHandler
LPI2C1_DriverIRQHandler
LPSPI0_DriverIRQHandler
LPSPI1_DriverIRQHandler
Reserved46_DriverIRQHandler
LPUART0_DriverIRQHandler
LPUART1_DriverIRQHandler
LPUART2_DriverIRQHandler
LPUART3_DriverIRQHandler
LPUART4_DriverIRQHandler
USB0_DriverIRQHandler
Reserved53_DriverIRQHandler
CDOG0_DriverIRQHandler
CTIMER0_DriverIRQHandler
CTIMER1_DriverIRQHandler
CTIMER2_DriverIRQHandler
CTIMER3_DriverIRQHandler
CTIMER4_DriverIRQHandler
FLEXPWM0_RELOAD_ERROR_DriverIRQHandler
FLEXPWM0_FAULT_DriverIRQHandler
FLEXPWM0_SUBMODULE0_DriverIRQHandler
FLEXPWM0_SUBMODULE1_DriverIRQHandler
FLEXPWM0_SUBMODULE2_DriverIRQHandler
FLEXPWM0_SUBMODULE3_DriverIRQHandler
EQDC0_COMPARE_DriverIRQHandler
EQDC0_HOME_DriverIRQHandler
EQDC0_WATCHDOG_DriverIRQHandler
EQDC0_INDEX_DriverIRQHandler
FREQME0_DriverIRQHandler
LPTMR0_DriverIRQHandler
Reserved72_DriverIRQHandler
OS_EVENT_DriverIRQHandler
WAKETIMER0_DriverIRQHandler
UTICK0_DriverIRQHandler
WWDT0_DriverIRQHandler
Reserved77_DriverIRQHandler
ADC0_DriverIRQHandler
ADC1_DriverIRQHandler
CMP0_DriverIRQHandler
CMP1_DriverIRQHandler
CMP2_DriverIRQHandler
DAC0_DriverIRQHandler
Reserved84_DriverIRQHandler
Reserved85_DriverIRQHandler
Reserved86_DriverIRQHandler
GPIO0_DriverIRQHandler
GPIO1_DriverIRQHandler
GPIO2_DriverIRQHandler
GPIO3_DriverIRQHandler
GPIO4_DriverIRQHandler
Reserved92_DriverIRQHandler
LPI2C2_DriverIRQHandler
LPI2C3_DriverIRQHandler
FLEXPWM1_RELOAD_ERROR_DriverIRQHandler
FLEXPWM1_FAULT_DriverIRQHandler
FLEXPWM1_SUBMODULE0_DriverIRQHandler
FLEXPWM1_SUBMODULE1_DriverIRQHandler
FLEXPWM1_SUBMODULE2_DriverIRQHandler
FLEXPWM1_SUBMODULE3_DriverIRQHandler
EQDC1_COMPARE_DriverIRQHandler
EQDC1_HOME_DriverIRQHandler
EQDC1_WATCHDOG_DriverIRQHandler
EQDC1_INDEX_DriverIRQHandler
Reserved105_DriverIRQHandler
Reserved106_DriverIRQHandler
Reserved107_DriverIRQHandler
Reserved108_DriverIRQHandler
Reserved109_DriverIRQHandler
Reserved110_DriverIRQHandler
LPUART5_DriverIRQHandler
Reserved112_DriverIRQHandler
Reserved113_DriverIRQHandler
Reserved114_DriverIRQHandler
Reserved115_DriverIRQHandler
Reserved116_DriverIRQHandler
Reserved117_DriverIRQHandler
Reserved118_DriverIRQHandler
Reserved119_DriverIRQHandler
Reserved120_DriverIRQHandler
Reserved121_DriverIRQHandler
Reserved122_DriverIRQHandler
MAU_DriverIRQHandler
SMARTDMA_DriverIRQHandler
CDOG1_DriverIRQHandler
PKC_DriverIRQHandler
SGI_DriverIRQHandler
Reserved128_DriverIRQHandler
TRNG0_DriverIRQHandler
SECURE_ERR_DriverIRQHandler
Reserved131_DriverIRQHandler
ADC2_DriverIRQHandler
ADC3_DriverIRQHandler
Reserved134_DriverIRQHandler
RTC_DriverIRQHandler
RTC_1HZ_DriverIRQHandler
SLCD_DriverIRQHandler
DefaultISR
        B .

        END
