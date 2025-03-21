/* ------------------------------------------------------------------------- */
/*  @file:    startup_MCXA276.s                                              */
/*  @purpose: CMSIS Cortex-M33 Core Device Startup File                      */
/*            MCXA276                                                        */
/*  @version: 1.0                                                            */
/*  @date:    2024-11-21                                                     */
/*  @build:   b241224                                                        */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Copyright 1997-2016 Freescale Semiconductor, Inc.                         */
/* Copyright 2016-2024 NXP                                                   */
/* SPDX-License-Identifier: BSD-3-Clause                                     */
/*****************************************************************************/
/* Version: GCC for ARM Embedded Processors                                  */
/*****************************************************************************/
    .syntax unified
    .arch armv8-m.main
    .eabi_attribute Tag_ABI_align_preserved, 1 /*8-byte alignment */

    .section .isr_vector, "a"
    .align 2
    .globl __Vectors
__Vectors:
    .long   Image$$ARM_LIB_STACK$$ZI$$Limit                 /* Top of Stack */
    .long   Reset_Handler                                   /* Reset Handler */
    .long   NMI_Handler                                     /* NMI Handler*/
    .long   HardFault_Handler                               /* Hard Fault Handler*/
    .long   MemManage_Handler                               /* MPU Fault Handler*/
    .long   BusFault_Handler                                /* Bus Fault Handler*/
    .long   UsageFault_Handler                              /* Usage Fault Handler*/
    .long   SecureFault_Handler                             /* Secure Fault Handler*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   SVC_Handler                                     /* SVCall Handler*/
    .long   DebugMon_Handler                                /* Debug Monitor Handler*/
    .long   0                                               /* Reserved*/
    .long   PendSV_Handler                                  /* PendSV Handler*/
    .long   SysTick_Handler                                 /* SysTick Handler*/

                                                            /* External Interrupts*/
    .long   Reserved16_IRQHandler                           /* OR IRQ1 to IRQ53*/
    .long   CMC_IRQHandler                                  /* Core Mode Controller interrupt*/
    .long   DMA_CH0_IRQHandler                              /* DMA3_0_CH0 error or transfer complete*/
    .long   DMA_CH1_IRQHandler                              /* DMA3_0_CH1 error or transfer complete*/
    .long   DMA_CH2_IRQHandler                              /* DMA3_0_CH2 error or transfer complete*/
    .long   DMA_CH3_IRQHandler                              /* DMA3_0_CH3 error or transfer complete*/
    .long   DMA_CH4_IRQHandler                              /* DMA3_0_CH4 error or transfer complete*/
    .long   DMA_CH5_IRQHandler                              /* DMA3_0_CH5 error or transfer complete*/
    .long   DMA_CH6_IRQHandler                              /* DMA3_0_CH6 error or transfer complete*/
    .long   DMA_CH7_IRQHandler                              /* DMA3_0_CH7 error or transfer complete*/
    .long   ERM0_SINGLE_BIT_IRQHandler                      /* ERM Single Bit error interrupt*/
    .long   ERM0_MULTI_BIT_IRQHandler                       /* ERM Multi Bit error interrupt*/
    .long   FMU0_IRQHandler                                 /* Flash Management Unit interrupt*/
    .long   GLIKEY0_IRQHandler                              /* GLIKEY Interrupt */
    .long   MBC0_IRQHandler                                 /* MBC secure violation interrupt*/
    .long   SCG0_IRQHandler                                 /* System Clock Generator interrupt*/
    .long   SPC0_IRQHandler                                 /* System Power Controller interrupt*/
    .long   TDET_IRQHandler                                 /* TDET interrrupt*/
    .long   WUU0_IRQHandler                                 /* Wake Up Unit interrupt*/
    .long   CAN0_IRQHandler                                 /* Controller Area Network 0 interrupt*/
    .long   CAN1_IRQHandler                                 /* Controller Area Network 1 interrupt*/
    .long   Reserved37_IRQHandler                           /* xxx Interrupt 37*/
    .long   Reserved38_IRQHandler                           /* xxx Interrupt 38*/
    .long   FLEXIO_IRQHandler                               /* Flexible Input/Output interrupt*/
    .long   I3C0_IRQHandler                                 /* Improved Inter Integrated Circuit interrupt 0*/
    .long   Reserved41_IRQHandler                           /* xxx Interrupt 41*/
    .long   LPI2C0_IRQHandler                               /* Low-Power Inter Integrated Circuit 0 interrupt*/
    .long   LPI2C1_IRQHandler                               /* Low-Power Inter Integrated Circuit 1 interrupt*/
    .long   LPSPI0_IRQHandler                               /* Low-Power Serial Peripheral Interface 0 interrupt*/
    .long   LPSPI1_IRQHandler                               /* Low-Power Serial Peripheral Interface 1 interrupt*/
    .long   Reserved46_IRQHandler                           /* xxx Interrupt 46*/
    .long   LPUART0_IRQHandler                              /* Low-Power Universal Asynchronous Receive/Transmit 0 interrupt*/
    .long   LPUART1_IRQHandler                              /* Low-Power Universal Asynchronous Receive/Transmit 1 interrupt*/
    .long   LPUART2_IRQHandler                              /* Low-Power Universal Asynchronous Receive/Transmit 2 interrupt*/
    .long   LPUART3_IRQHandler                              /* Low-Power Universal Asynchronous Receive/Transmit 3 interrupt*/
    .long   LPUART4_IRQHandler                              /* Low-Power Universal Asynchronous Receive/Transmit 4 interrupt*/
    .long   USB0_IRQHandler                                 /* Universal Serial Bus - Full Speed interrupt*/
    .long   Reserved53_IRQHandler                           /* xxx Interrupt 53*/
    .long   CDOG0_IRQHandler                                /* Code Watchdog Timer 0 interrupt*/
    .long   CTIMER0_IRQHandler                              /* Standard counter/timer 0 interrupt*/
    .long   CTIMER1_IRQHandler                              /* Standard counter/timer 1 interrupt*/
    .long   CTIMER2_IRQHandler                              /* Standard counter/timer 2 interrupt*/
    .long   CTIMER3_IRQHandler                              /* Standard counter/timer 3 interrupt*/
    .long   CTIMER4_IRQHandler                              /* Standard counter/timer 4 interrupt*/
    .long   FLEXPWM0_RELOAD_ERROR_IRQHandler                /* FlexPWM0_reload_error interrupt*/
    .long   FLEXPWM0_FAULT_IRQHandler                       /* FlexPWM0_fault interrupt*/
    .long   FLEXPWM0_SUBMODULE0_IRQHandler                  /* FlexPWM0 Submodule 0 capture/compare/reload interrupt*/
    .long   FLEXPWM0_SUBMODULE1_IRQHandler                  /* FlexPWM0 Submodule 1 capture/compare/reload interrupt*/
    .long   FLEXPWM0_SUBMODULE2_IRQHandler                  /* FlexPWM0 Submodule 2 capture/compare/reload interrupt*/
    .long   FLEXPWM0_SUBMODULE3_IRQHandler                  /* FlexPWM0 Submodule 3 capture/compare/reload interrupt*/
    .long   EQDC0_COMPARE_IRQHandler                        /* Compare*/
    .long   EQDC0_HOME_IRQHandler                           /* Home*/
    .long   EQDC0_WATCHDOG_IRQHandler                       /* Watchdog / Simultaneous A and B Change*/
    .long   EQDC0_INDEX_IRQHandler                          /* Index / Roll Over / Roll Under*/
    .long   FREQME0_IRQHandler                              /* Frequency Measurement interrupt*/
    .long   LPTMR0_IRQHandler                               /* Low Power Timer 0 interrupt*/
    .long   Reserved72_IRQHandler                           /* xxx Interrupt 72*/
    .long   OS_EVENT_IRQHandler                             /* OS event timer interrupt*/
    .long   WAKETIMER0_IRQHandler                           /* Wake Timer Interrupt*/
    .long   UTICK0_IRQHandler                               /* Micro-Tick Timer interrupt*/
    .long   WWDT0_IRQHandler                                /* Windowed Watchdog Timer 0 interrupt*/
    .long   Reserved77_IRQHandler                           /* xxx Interrupt 77*/
    .long   ADC0_IRQHandler                                 /* Analog-to-Digital Converter 0 interrupt*/
    .long   ADC1_IRQHandler                                 /* Analog-to-Digital Converter 1 interrupt*/
    .long   CMP0_IRQHandler                                 /* Comparator 0 interrupt*/
    .long   CMP1_IRQHandler                                 /* Comparator 1 interrupt*/
    .long   CMP2_IRQHandler                                 /* Comparator 2 interrupt*/
    .long   DAC0_IRQHandler                                 /* Digital-to-Analog Converter 0 - General Purpose interrupt*/
    .long   Reserved84_IRQHandler                           /* xxx Interrupt 84*/
    .long   Reserved85_IRQHandler                           /* xxx Interrupt 85*/
    .long   Reserved86_IRQHandler                           /* xxx Interrupt 86*/
    .long   GPIO0_IRQHandler                                /* General Purpose Input/Output interrupt 0*/
    .long   GPIO1_IRQHandler                                /* General Purpose Input/Output interrupt 1*/
    .long   GPIO2_IRQHandler                                /* General Purpose Input/Output interrupt 2*/
    .long   GPIO3_IRQHandler                                /* General Purpose Input/Output interrupt 3*/
    .long   GPIO4_IRQHandler                                /* General Purpose Input/Output interrupt 4*/
    .long   Reserved92_IRQHandler                           /* xxx Interrupt 92*/
    .long   LPI2C2_IRQHandler                               /* Low-Power Inter Integrated Circuit 2 interrupt*/
    .long   LPI2C3_IRQHandler                               /* Low-Power Inter Integrated Circuit 3 interrupt*/
    .long   FLEXPWM1_RELOAD_ERROR_IRQHandler                /* FlexPWM1_reload_error interrupt*/
    .long   FLEXPWM1_FAULT_IRQHandler                       /* FlexPWM1_fault interrupt*/
    .long   FLEXPWM1_SUBMODULE0_IRQHandler                  /* FlexPWM1 Submodule 0 capture/compare/reload interrupt*/
    .long   FLEXPWM1_SUBMODULE1_IRQHandler                  /* FlexPWM1 Submodule 1 capture/compare/reload interrupt*/
    .long   FLEXPWM1_SUBMODULE2_IRQHandler                  /* FlexPWM1 Submodule 2 capture/compare/reload interrupt*/
    .long   FLEXPWM1_SUBMODULE3_IRQHandler                  /* FlexPWM1 Submodule 3 capture/compare/reload interrupt*/
    .long   EQDC1_COMPARE_IRQHandler                        /* Compare*/
    .long   EQDC1_HOME_IRQHandler                           /* Home*/
    .long   EQDC1_WATCHDOG_IRQHandler                       /* Watchdog / Simultaneous A and B Change*/
    .long   EQDC1_INDEX_IRQHandler                          /* Index / Roll Over / Roll Under*/
    .long   Reserved105_IRQHandler                          /* xxx Interrupt 105*/
    .long   Reserved106_IRQHandler                          /* xxx Interrupt 106*/
    .long   Reserved107_IRQHandler                          /* xxx Interrupt 107*/
    .long   Reserved108_IRQHandler                          /* xxx Interrupt 108*/
    .long   Reserved109_IRQHandler                          /* xxx Interrupt 109*/
    .long   Reserved110_IRQHandler                          /* xxx Interrupt 110*/
    .long   LPUART5_IRQHandler                              /* Low-Power Universal Asynchronous Receive/Transmit interrupt*/
    .long   Reserved112_IRQHandler                          /* xxx Interrupt 112*/
    .long   Reserved113_IRQHandler                          /* xxx Interrupt 113*/
    .long   Reserved114_IRQHandler                          /* xxx Interrupt 114*/
    .long   Reserved115_IRQHandler                          /* xxx Interrupt 115*/
    .long   Reserved116_IRQHandler                          /* xxx Interrupt 116*/
    .long   Reserved117_IRQHandler                          /* xxx Interrupt 117*/
    .long   Reserved118_IRQHandler                          /* xxx Interrupt 118*/
    .long   Reserved119_IRQHandler                          /* xxx Interrupt 119*/
    .long   Reserved120_IRQHandler                          /* xxx Interrupt 120*/
    .long   Reserved121_IRQHandler                          /* xxx Interrupt 121*/
    .long   Reserved122_IRQHandler                          /* xxx Interrupt 122*/
    .long   MAU_IRQHandler                                  /* MAU interrupt*/
    .long   SMARTDMA_IRQHandler                             /* SmartDMA interrupt*/
    .long   CDOG1_IRQHandler                                /* Code Watchdog Timer 1 interrupt*/
    .long   PKC_IRQHandler                                  /* PKC interrupt*/
    .long   SGI_IRQHandler                                  /* SGI interrupt*/
    .long   Reserved128_IRQHandler                          /* xxx Interrupt 128*/
    .long   TRNG0_IRQHandler                                /* True Random Number Generator interrupt*/
    .long   SECURE_ERR_IRQHandler                           /* Secure IP Error interrupt. It OR SGI, PKC, TRNG error together. */
    .long   Reserved131_IRQHandler                          /* xxx Interrupt 131*/
    .long   ADC2_IRQHandler                                 /* Analog-to-Digital Converter 2 interrupt*/
    .long   ADC3_IRQHandler                                 /* Analog-to-Digital Converter 3 interrupt*/
    .long   Reserved134_IRQHandler                          /* xxx Interrupt 134*/
    .long   RTC_IRQHandler                                  /* RTC alarm interrupt*/
    .long   RTC_1HZ_IRQHandler                              /* RTC 1Hz interrupt*/
    .long   SLCD_IRQHandler                                 /* SLCD frame start interrupt*/

    .size    __Vectors, . - __Vectors

    .text
    .thumb

/* Reset Handler */

    .thumb_func
    .align 2
    .weak    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    cpsid   i               /* Mask interrupts */
    .equ    VTOR, 0xE000ED08
    ldr     r0, =VTOR
    ldr     r1, =__Vectors
    str     r1, [r0]
    ldr     r2, [r1]
    msr     msp, r2
    ldr     r0, =Image$$ARM_LIB_STACK$$ZI$$Base
    msr     msplim, r0
    ldr     r0,=SystemInit
    blx     r0
    cpsie   i               /* Unmask interrupts */
    ldr     r0,=__main
    bx      r0

    .pool
    .size Reset_Handler, . - Reset_Handler

    .align  1
    .thumb_func
    .weak DefaultISR
    .type DefaultISR, %function
DefaultISR:
    b DefaultISR
    .size DefaultISR, . - DefaultISR

    .align 1
    .thumb_func
    .weak NMI_Handler
    .type NMI_Handler, %function
NMI_Handler:
    ldr   r0,=NMI_Handler
    bx    r0
    .size NMI_Handler, . - NMI_Handler

    .align 1
    .thumb_func
    .weak HardFault_Handler
    .type HardFault_Handler, %function
HardFault_Handler:
    ldr   r0,=HardFault_Handler
    bx    r0
    .size HardFault_Handler, . - HardFault_Handler

    .align 1
    .thumb_func
    .weak SVC_Handler
    .type SVC_Handler, %function
SVC_Handler:
    ldr   r0,=SVC_Handler
    bx    r0
    .size SVC_Handler, . - SVC_Handler

    .align 1
    .thumb_func
    .weak PendSV_Handler
    .type PendSV_Handler, %function
PendSV_Handler:
    ldr   r0,=PendSV_Handler
    bx    r0
    .size PendSV_Handler, . - PendSV_Handler

    .align 1
    .thumb_func
    .weak SysTick_Handler
    .type SysTick_Handler, %function
SysTick_Handler:
    ldr   r0,=SysTick_Handler
    bx    r0
    .size SysTick_Handler, . - SysTick_Handler

    .align 1
    .thumb_func
    .weak Reserved16_IRQHandler
    .type Reserved16_IRQHandler, %function
Reserved16_IRQHandler:
    ldr   r0,=Reserved16_DriverIRQHandler
    bx    r0
    .size Reserved16_IRQHandler, . - Reserved16_IRQHandler

    .align 1
    .thumb_func
    .weak CMC_IRQHandler
    .type CMC_IRQHandler, %function
CMC_IRQHandler:
    ldr   r0,=CMC_DriverIRQHandler
    bx    r0
    .size CMC_IRQHandler, . - CMC_IRQHandler

    .align 1
    .thumb_func
    .weak DMA_CH0_IRQHandler
    .type DMA_CH0_IRQHandler, %function
DMA_CH0_IRQHandler:
    ldr   r0,=DMA_CH0_DriverIRQHandler
    bx    r0
    .size DMA_CH0_IRQHandler, . - DMA_CH0_IRQHandler

    .align 1
    .thumb_func
    .weak DMA_CH1_IRQHandler
    .type DMA_CH1_IRQHandler, %function
DMA_CH1_IRQHandler:
    ldr   r0,=DMA_CH1_DriverIRQHandler
    bx    r0
    .size DMA_CH1_IRQHandler, . - DMA_CH1_IRQHandler

    .align 1
    .thumb_func
    .weak DMA_CH2_IRQHandler
    .type DMA_CH2_IRQHandler, %function
DMA_CH2_IRQHandler:
    ldr   r0,=DMA_CH2_DriverIRQHandler
    bx    r0
    .size DMA_CH2_IRQHandler, . - DMA_CH2_IRQHandler

    .align 1
    .thumb_func
    .weak DMA_CH3_IRQHandler
    .type DMA_CH3_IRQHandler, %function
DMA_CH3_IRQHandler:
    ldr   r0,=DMA_CH3_DriverIRQHandler
    bx    r0
    .size DMA_CH3_IRQHandler, . - DMA_CH3_IRQHandler

    .align 1
    .thumb_func
    .weak DMA_CH4_IRQHandler
    .type DMA_CH4_IRQHandler, %function
DMA_CH4_IRQHandler:
    ldr   r0,=DMA_CH4_DriverIRQHandler
    bx    r0
    .size DMA_CH4_IRQHandler, . - DMA_CH4_IRQHandler

    .align 1
    .thumb_func
    .weak DMA_CH5_IRQHandler
    .type DMA_CH5_IRQHandler, %function
DMA_CH5_IRQHandler:
    ldr   r0,=DMA_CH5_DriverIRQHandler
    bx    r0
    .size DMA_CH5_IRQHandler, . - DMA_CH5_IRQHandler

    .align 1
    .thumb_func
    .weak DMA_CH6_IRQHandler
    .type DMA_CH6_IRQHandler, %function
DMA_CH6_IRQHandler:
    ldr   r0,=DMA_CH6_DriverIRQHandler
    bx    r0
    .size DMA_CH6_IRQHandler, . - DMA_CH6_IRQHandler

    .align 1
    .thumb_func
    .weak DMA_CH7_IRQHandler
    .type DMA_CH7_IRQHandler, %function
DMA_CH7_IRQHandler:
    ldr   r0,=DMA_CH7_DriverIRQHandler
    bx    r0
    .size DMA_CH7_IRQHandler, . - DMA_CH7_IRQHandler

    .align 1
    .thumb_func
    .weak ERM0_SINGLE_BIT_IRQHandler
    .type ERM0_SINGLE_BIT_IRQHandler, %function
ERM0_SINGLE_BIT_IRQHandler:
    ldr   r0,=ERM0_SINGLE_BIT_DriverIRQHandler
    bx    r0
    .size ERM0_SINGLE_BIT_IRQHandler, . - ERM0_SINGLE_BIT_IRQHandler

    .align 1
    .thumb_func
    .weak ERM0_MULTI_BIT_IRQHandler
    .type ERM0_MULTI_BIT_IRQHandler, %function
ERM0_MULTI_BIT_IRQHandler:
    ldr   r0,=ERM0_MULTI_BIT_DriverIRQHandler
    bx    r0
    .size ERM0_MULTI_BIT_IRQHandler, . - ERM0_MULTI_BIT_IRQHandler

    .align 1
    .thumb_func
    .weak FMU0_IRQHandler
    .type FMU0_IRQHandler, %function
FMU0_IRQHandler:
    ldr   r0,=FMU0_DriverIRQHandler
    bx    r0
    .size FMU0_IRQHandler, . - FMU0_IRQHandler

    .align 1
    .thumb_func
    .weak GLIKEY0_IRQHandler
    .type GLIKEY0_IRQHandler, %function
GLIKEY0_IRQHandler:
    ldr   r0,=GLIKEY0_DriverIRQHandler
    bx    r0
    .size GLIKEY0_IRQHandler, . - GLIKEY0_IRQHandler

    .align 1
    .thumb_func
    .weak MBC0_IRQHandler
    .type MBC0_IRQHandler, %function
MBC0_IRQHandler:
    ldr   r0,=MBC0_DriverIRQHandler
    bx    r0
    .size MBC0_IRQHandler, . - MBC0_IRQHandler

    .align 1
    .thumb_func
    .weak SCG0_IRQHandler
    .type SCG0_IRQHandler, %function
SCG0_IRQHandler:
    ldr   r0,=SCG0_DriverIRQHandler
    bx    r0
    .size SCG0_IRQHandler, . - SCG0_IRQHandler

    .align 1
    .thumb_func
    .weak SPC0_IRQHandler
    .type SPC0_IRQHandler, %function
SPC0_IRQHandler:
    ldr   r0,=SPC0_DriverIRQHandler
    bx    r0
    .size SPC0_IRQHandler, . - SPC0_IRQHandler

    .align 1
    .thumb_func
    .weak TDET_IRQHandler
    .type TDET_IRQHandler, %function
TDET_IRQHandler:
    ldr   r0,=TDET_DriverIRQHandler
    bx    r0
    .size TDET_IRQHandler, . - TDET_IRQHandler

    .align 1
    .thumb_func
    .weak WUU0_IRQHandler
    .type WUU0_IRQHandler, %function
WUU0_IRQHandler:
    ldr   r0,=WUU0_DriverIRQHandler
    bx    r0
    .size WUU0_IRQHandler, . - WUU0_IRQHandler

    .align 1
    .thumb_func
    .weak CAN0_IRQHandler
    .type CAN0_IRQHandler, %function
CAN0_IRQHandler:
    ldr   r0,=CAN0_DriverIRQHandler
    bx    r0
    .size CAN0_IRQHandler, . - CAN0_IRQHandler

    .align 1
    .thumb_func
    .weak CAN1_IRQHandler
    .type CAN1_IRQHandler, %function
CAN1_IRQHandler:
    ldr   r0,=CAN1_DriverIRQHandler
    bx    r0
    .size CAN1_IRQHandler, . - CAN1_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved37_IRQHandler
    .type Reserved37_IRQHandler, %function
Reserved37_IRQHandler:
    ldr   r0,=Reserved37_DriverIRQHandler
    bx    r0
    .size Reserved37_IRQHandler, . - Reserved37_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved38_IRQHandler
    .type Reserved38_IRQHandler, %function
Reserved38_IRQHandler:
    ldr   r0,=Reserved38_DriverIRQHandler
    bx    r0
    .size Reserved38_IRQHandler, . - Reserved38_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXIO_IRQHandler
    .type FLEXIO_IRQHandler, %function
FLEXIO_IRQHandler:
    ldr   r0,=FLEXIO_DriverIRQHandler
    bx    r0
    .size FLEXIO_IRQHandler, . - FLEXIO_IRQHandler

    .align 1
    .thumb_func
    .weak I3C0_IRQHandler
    .type I3C0_IRQHandler, %function
I3C0_IRQHandler:
    ldr   r0,=I3C0_DriverIRQHandler
    bx    r0
    .size I3C0_IRQHandler, . - I3C0_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved41_IRQHandler
    .type Reserved41_IRQHandler, %function
Reserved41_IRQHandler:
    ldr   r0,=Reserved41_DriverIRQHandler
    bx    r0
    .size Reserved41_IRQHandler, . - Reserved41_IRQHandler

    .align 1
    .thumb_func
    .weak LPI2C0_IRQHandler
    .type LPI2C0_IRQHandler, %function
LPI2C0_IRQHandler:
    ldr   r0,=LPI2C0_DriverIRQHandler
    bx    r0
    .size LPI2C0_IRQHandler, . - LPI2C0_IRQHandler

    .align 1
    .thumb_func
    .weak LPI2C1_IRQHandler
    .type LPI2C1_IRQHandler, %function
LPI2C1_IRQHandler:
    ldr   r0,=LPI2C1_DriverIRQHandler
    bx    r0
    .size LPI2C1_IRQHandler, . - LPI2C1_IRQHandler

    .align 1
    .thumb_func
    .weak LPSPI0_IRQHandler
    .type LPSPI0_IRQHandler, %function
LPSPI0_IRQHandler:
    ldr   r0,=LPSPI0_DriverIRQHandler
    bx    r0
    .size LPSPI0_IRQHandler, . - LPSPI0_IRQHandler

    .align 1
    .thumb_func
    .weak LPSPI1_IRQHandler
    .type LPSPI1_IRQHandler, %function
LPSPI1_IRQHandler:
    ldr   r0,=LPSPI1_DriverIRQHandler
    bx    r0
    .size LPSPI1_IRQHandler, . - LPSPI1_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved46_IRQHandler
    .type Reserved46_IRQHandler, %function
Reserved46_IRQHandler:
    ldr   r0,=Reserved46_DriverIRQHandler
    bx    r0
    .size Reserved46_IRQHandler, . - Reserved46_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART0_IRQHandler
    .type LPUART0_IRQHandler, %function
LPUART0_IRQHandler:
    ldr   r0,=LPUART0_DriverIRQHandler
    bx    r0
    .size LPUART0_IRQHandler, . - LPUART0_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART1_IRQHandler
    .type LPUART1_IRQHandler, %function
LPUART1_IRQHandler:
    ldr   r0,=LPUART1_DriverIRQHandler
    bx    r0
    .size LPUART1_IRQHandler, . - LPUART1_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART2_IRQHandler
    .type LPUART2_IRQHandler, %function
LPUART2_IRQHandler:
    ldr   r0,=LPUART2_DriverIRQHandler
    bx    r0
    .size LPUART2_IRQHandler, . - LPUART2_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART3_IRQHandler
    .type LPUART3_IRQHandler, %function
LPUART3_IRQHandler:
    ldr   r0,=LPUART3_DriverIRQHandler
    bx    r0
    .size LPUART3_IRQHandler, . - LPUART3_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART4_IRQHandler
    .type LPUART4_IRQHandler, %function
LPUART4_IRQHandler:
    ldr   r0,=LPUART4_DriverIRQHandler
    bx    r0
    .size LPUART4_IRQHandler, . - LPUART4_IRQHandler

    .align 1
    .thumb_func
    .weak USB0_IRQHandler
    .type USB0_IRQHandler, %function
USB0_IRQHandler:
    ldr   r0,=USB0_DriverIRQHandler
    bx    r0
    .size USB0_IRQHandler, . - USB0_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved53_IRQHandler
    .type Reserved53_IRQHandler, %function
Reserved53_IRQHandler:
    ldr   r0,=Reserved53_DriverIRQHandler
    bx    r0
    .size Reserved53_IRQHandler, . - Reserved53_IRQHandler

    .align 1
    .thumb_func
    .weak CDOG0_IRQHandler
    .type CDOG0_IRQHandler, %function
CDOG0_IRQHandler:
    ldr   r0,=CDOG0_DriverIRQHandler
    bx    r0
    .size CDOG0_IRQHandler, . - CDOG0_IRQHandler

    .align 1
    .thumb_func
    .weak CTIMER0_IRQHandler
    .type CTIMER0_IRQHandler, %function
CTIMER0_IRQHandler:
    ldr   r0,=CTIMER0_DriverIRQHandler
    bx    r0
    .size CTIMER0_IRQHandler, . - CTIMER0_IRQHandler

    .align 1
    .thumb_func
    .weak CTIMER1_IRQHandler
    .type CTIMER1_IRQHandler, %function
CTIMER1_IRQHandler:
    ldr   r0,=CTIMER1_DriverIRQHandler
    bx    r0
    .size CTIMER1_IRQHandler, . - CTIMER1_IRQHandler

    .align 1
    .thumb_func
    .weak CTIMER2_IRQHandler
    .type CTIMER2_IRQHandler, %function
CTIMER2_IRQHandler:
    ldr   r0,=CTIMER2_DriverIRQHandler
    bx    r0
    .size CTIMER2_IRQHandler, . - CTIMER2_IRQHandler

    .align 1
    .thumb_func
    .weak CTIMER3_IRQHandler
    .type CTIMER3_IRQHandler, %function
CTIMER3_IRQHandler:
    ldr   r0,=CTIMER3_DriverIRQHandler
    bx    r0
    .size CTIMER3_IRQHandler, . - CTIMER3_IRQHandler

    .align 1
    .thumb_func
    .weak CTIMER4_IRQHandler
    .type CTIMER4_IRQHandler, %function
CTIMER4_IRQHandler:
    ldr   r0,=CTIMER4_DriverIRQHandler
    bx    r0
    .size CTIMER4_IRQHandler, . - CTIMER4_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXPWM0_RELOAD_ERROR_IRQHandler
    .type FLEXPWM0_RELOAD_ERROR_IRQHandler, %function
FLEXPWM0_RELOAD_ERROR_IRQHandler:
    ldr   r0,=FLEXPWM0_RELOAD_ERROR_DriverIRQHandler
    bx    r0
    .size FLEXPWM0_RELOAD_ERROR_IRQHandler, . - FLEXPWM0_RELOAD_ERROR_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXPWM0_FAULT_IRQHandler
    .type FLEXPWM0_FAULT_IRQHandler, %function
FLEXPWM0_FAULT_IRQHandler:
    ldr   r0,=FLEXPWM0_FAULT_DriverIRQHandler
    bx    r0
    .size FLEXPWM0_FAULT_IRQHandler, . - FLEXPWM0_FAULT_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXPWM0_SUBMODULE0_IRQHandler
    .type FLEXPWM0_SUBMODULE0_IRQHandler, %function
FLEXPWM0_SUBMODULE0_IRQHandler:
    ldr   r0,=FLEXPWM0_SUBMODULE0_DriverIRQHandler
    bx    r0
    .size FLEXPWM0_SUBMODULE0_IRQHandler, . - FLEXPWM0_SUBMODULE0_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXPWM0_SUBMODULE1_IRQHandler
    .type FLEXPWM0_SUBMODULE1_IRQHandler, %function
FLEXPWM0_SUBMODULE1_IRQHandler:
    ldr   r0,=FLEXPWM0_SUBMODULE1_DriverIRQHandler
    bx    r0
    .size FLEXPWM0_SUBMODULE1_IRQHandler, . - FLEXPWM0_SUBMODULE1_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXPWM0_SUBMODULE2_IRQHandler
    .type FLEXPWM0_SUBMODULE2_IRQHandler, %function
FLEXPWM0_SUBMODULE2_IRQHandler:
    ldr   r0,=FLEXPWM0_SUBMODULE2_DriverIRQHandler
    bx    r0
    .size FLEXPWM0_SUBMODULE2_IRQHandler, . - FLEXPWM0_SUBMODULE2_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXPWM0_SUBMODULE3_IRQHandler
    .type FLEXPWM0_SUBMODULE3_IRQHandler, %function
FLEXPWM0_SUBMODULE3_IRQHandler:
    ldr   r0,=FLEXPWM0_SUBMODULE3_DriverIRQHandler
    bx    r0
    .size FLEXPWM0_SUBMODULE3_IRQHandler, . - FLEXPWM0_SUBMODULE3_IRQHandler

    .align 1
    .thumb_func
    .weak EQDC0_COMPARE_IRQHandler
    .type EQDC0_COMPARE_IRQHandler, %function
EQDC0_COMPARE_IRQHandler:
    ldr   r0,=EQDC0_COMPARE_DriverIRQHandler
    bx    r0
    .size EQDC0_COMPARE_IRQHandler, . - EQDC0_COMPARE_IRQHandler

    .align 1
    .thumb_func
    .weak EQDC0_HOME_IRQHandler
    .type EQDC0_HOME_IRQHandler, %function
EQDC0_HOME_IRQHandler:
    ldr   r0,=EQDC0_HOME_DriverIRQHandler
    bx    r0
    .size EQDC0_HOME_IRQHandler, . - EQDC0_HOME_IRQHandler

    .align 1
    .thumb_func
    .weak EQDC0_WATCHDOG_IRQHandler
    .type EQDC0_WATCHDOG_IRQHandler, %function
EQDC0_WATCHDOG_IRQHandler:
    ldr   r0,=EQDC0_WATCHDOG_DriverIRQHandler
    bx    r0
    .size EQDC0_WATCHDOG_IRQHandler, . - EQDC0_WATCHDOG_IRQHandler

    .align 1
    .thumb_func
    .weak EQDC0_INDEX_IRQHandler
    .type EQDC0_INDEX_IRQHandler, %function
EQDC0_INDEX_IRQHandler:
    ldr   r0,=EQDC0_INDEX_DriverIRQHandler
    bx    r0
    .size EQDC0_INDEX_IRQHandler, . - EQDC0_INDEX_IRQHandler

    .align 1
    .thumb_func
    .weak FREQME0_IRQHandler
    .type FREQME0_IRQHandler, %function
FREQME0_IRQHandler:
    ldr   r0,=FREQME0_DriverIRQHandler
    bx    r0
    .size FREQME0_IRQHandler, . - FREQME0_IRQHandler

    .align 1
    .thumb_func
    .weak LPTMR0_IRQHandler
    .type LPTMR0_IRQHandler, %function
LPTMR0_IRQHandler:
    ldr   r0,=LPTMR0_DriverIRQHandler
    bx    r0
    .size LPTMR0_IRQHandler, . - LPTMR0_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved72_IRQHandler
    .type Reserved72_IRQHandler, %function
Reserved72_IRQHandler:
    ldr   r0,=Reserved72_DriverIRQHandler
    bx    r0
    .size Reserved72_IRQHandler, . - Reserved72_IRQHandler

    .align 1
    .thumb_func
    .weak OS_EVENT_IRQHandler
    .type OS_EVENT_IRQHandler, %function
OS_EVENT_IRQHandler:
    ldr   r0,=OS_EVENT_DriverIRQHandler
    bx    r0
    .size OS_EVENT_IRQHandler, . - OS_EVENT_IRQHandler

    .align 1
    .thumb_func
    .weak WAKETIMER0_IRQHandler
    .type WAKETIMER0_IRQHandler, %function
WAKETIMER0_IRQHandler:
    ldr   r0,=WAKETIMER0_DriverIRQHandler
    bx    r0
    .size WAKETIMER0_IRQHandler, . - WAKETIMER0_IRQHandler

    .align 1
    .thumb_func
    .weak UTICK0_IRQHandler
    .type UTICK0_IRQHandler, %function
UTICK0_IRQHandler:
    ldr   r0,=UTICK0_DriverIRQHandler
    bx    r0
    .size UTICK0_IRQHandler, . - UTICK0_IRQHandler

    .align 1
    .thumb_func
    .weak WWDT0_IRQHandler
    .type WWDT0_IRQHandler, %function
WWDT0_IRQHandler:
    ldr   r0,=WWDT0_DriverIRQHandler
    bx    r0
    .size WWDT0_IRQHandler, . - WWDT0_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved77_IRQHandler
    .type Reserved77_IRQHandler, %function
Reserved77_IRQHandler:
    ldr   r0,=Reserved77_DriverIRQHandler
    bx    r0
    .size Reserved77_IRQHandler, . - Reserved77_IRQHandler

    .align 1
    .thumb_func
    .weak ADC0_IRQHandler
    .type ADC0_IRQHandler, %function
ADC0_IRQHandler:
    ldr   r0,=ADC0_DriverIRQHandler
    bx    r0
    .size ADC0_IRQHandler, . - ADC0_IRQHandler

    .align 1
    .thumb_func
    .weak ADC1_IRQHandler
    .type ADC1_IRQHandler, %function
ADC1_IRQHandler:
    ldr   r0,=ADC1_DriverIRQHandler
    bx    r0
    .size ADC1_IRQHandler, . - ADC1_IRQHandler

    .align 1
    .thumb_func
    .weak CMP0_IRQHandler
    .type CMP0_IRQHandler, %function
CMP0_IRQHandler:
    ldr   r0,=CMP0_DriverIRQHandler
    bx    r0
    .size CMP0_IRQHandler, . - CMP0_IRQHandler

    .align 1
    .thumb_func
    .weak CMP1_IRQHandler
    .type CMP1_IRQHandler, %function
CMP1_IRQHandler:
    ldr   r0,=CMP1_DriverIRQHandler
    bx    r0
    .size CMP1_IRQHandler, . - CMP1_IRQHandler

    .align 1
    .thumb_func
    .weak CMP2_IRQHandler
    .type CMP2_IRQHandler, %function
CMP2_IRQHandler:
    ldr   r0,=CMP2_DriverIRQHandler
    bx    r0
    .size CMP2_IRQHandler, . - CMP2_IRQHandler

    .align 1
    .thumb_func
    .weak DAC0_IRQHandler
    .type DAC0_IRQHandler, %function
DAC0_IRQHandler:
    ldr   r0,=DAC0_DriverIRQHandler
    bx    r0
    .size DAC0_IRQHandler, . - DAC0_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved84_IRQHandler
    .type Reserved84_IRQHandler, %function
Reserved84_IRQHandler:
    ldr   r0,=Reserved84_DriverIRQHandler
    bx    r0
    .size Reserved84_IRQHandler, . - Reserved84_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved85_IRQHandler
    .type Reserved85_IRQHandler, %function
Reserved85_IRQHandler:
    ldr   r0,=Reserved85_DriverIRQHandler
    bx    r0
    .size Reserved85_IRQHandler, . - Reserved85_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved86_IRQHandler
    .type Reserved86_IRQHandler, %function
Reserved86_IRQHandler:
    ldr   r0,=Reserved86_DriverIRQHandler
    bx    r0
    .size Reserved86_IRQHandler, . - Reserved86_IRQHandler

    .align 1
    .thumb_func
    .weak GPIO0_IRQHandler
    .type GPIO0_IRQHandler, %function
GPIO0_IRQHandler:
    ldr   r0,=GPIO0_DriverIRQHandler
    bx    r0
    .size GPIO0_IRQHandler, . - GPIO0_IRQHandler

    .align 1
    .thumb_func
    .weak GPIO1_IRQHandler
    .type GPIO1_IRQHandler, %function
GPIO1_IRQHandler:
    ldr   r0,=GPIO1_DriverIRQHandler
    bx    r0
    .size GPIO1_IRQHandler, . - GPIO1_IRQHandler

    .align 1
    .thumb_func
    .weak GPIO2_IRQHandler
    .type GPIO2_IRQHandler, %function
GPIO2_IRQHandler:
    ldr   r0,=GPIO2_DriverIRQHandler
    bx    r0
    .size GPIO2_IRQHandler, . - GPIO2_IRQHandler

    .align 1
    .thumb_func
    .weak GPIO3_IRQHandler
    .type GPIO3_IRQHandler, %function
GPIO3_IRQHandler:
    ldr   r0,=GPIO3_DriverIRQHandler
    bx    r0
    .size GPIO3_IRQHandler, . - GPIO3_IRQHandler

    .align 1
    .thumb_func
    .weak GPIO4_IRQHandler
    .type GPIO4_IRQHandler, %function
GPIO4_IRQHandler:
    ldr   r0,=GPIO4_DriverIRQHandler
    bx    r0
    .size GPIO4_IRQHandler, . - GPIO4_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved92_IRQHandler
    .type Reserved92_IRQHandler, %function
Reserved92_IRQHandler:
    ldr   r0,=Reserved92_DriverIRQHandler
    bx    r0
    .size Reserved92_IRQHandler, . - Reserved92_IRQHandler

    .align 1
    .thumb_func
    .weak LPI2C2_IRQHandler
    .type LPI2C2_IRQHandler, %function
LPI2C2_IRQHandler:
    ldr   r0,=LPI2C2_DriverIRQHandler
    bx    r0
    .size LPI2C2_IRQHandler, . - LPI2C2_IRQHandler

    .align 1
    .thumb_func
    .weak LPI2C3_IRQHandler
    .type LPI2C3_IRQHandler, %function
LPI2C3_IRQHandler:
    ldr   r0,=LPI2C3_DriverIRQHandler
    bx    r0
    .size LPI2C3_IRQHandler, . - LPI2C3_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXPWM1_RELOAD_ERROR_IRQHandler
    .type FLEXPWM1_RELOAD_ERROR_IRQHandler, %function
FLEXPWM1_RELOAD_ERROR_IRQHandler:
    ldr   r0,=FLEXPWM1_RELOAD_ERROR_DriverIRQHandler
    bx    r0
    .size FLEXPWM1_RELOAD_ERROR_IRQHandler, . - FLEXPWM1_RELOAD_ERROR_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXPWM1_FAULT_IRQHandler
    .type FLEXPWM1_FAULT_IRQHandler, %function
FLEXPWM1_FAULT_IRQHandler:
    ldr   r0,=FLEXPWM1_FAULT_DriverIRQHandler
    bx    r0
    .size FLEXPWM1_FAULT_IRQHandler, . - FLEXPWM1_FAULT_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXPWM1_SUBMODULE0_IRQHandler
    .type FLEXPWM1_SUBMODULE0_IRQHandler, %function
FLEXPWM1_SUBMODULE0_IRQHandler:
    ldr   r0,=FLEXPWM1_SUBMODULE0_DriverIRQHandler
    bx    r0
    .size FLEXPWM1_SUBMODULE0_IRQHandler, . - FLEXPWM1_SUBMODULE0_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXPWM1_SUBMODULE1_IRQHandler
    .type FLEXPWM1_SUBMODULE1_IRQHandler, %function
FLEXPWM1_SUBMODULE1_IRQHandler:
    ldr   r0,=FLEXPWM1_SUBMODULE1_DriverIRQHandler
    bx    r0
    .size FLEXPWM1_SUBMODULE1_IRQHandler, . - FLEXPWM1_SUBMODULE1_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXPWM1_SUBMODULE2_IRQHandler
    .type FLEXPWM1_SUBMODULE2_IRQHandler, %function
FLEXPWM1_SUBMODULE2_IRQHandler:
    ldr   r0,=FLEXPWM1_SUBMODULE2_DriverIRQHandler
    bx    r0
    .size FLEXPWM1_SUBMODULE2_IRQHandler, . - FLEXPWM1_SUBMODULE2_IRQHandler

    .align 1
    .thumb_func
    .weak FLEXPWM1_SUBMODULE3_IRQHandler
    .type FLEXPWM1_SUBMODULE3_IRQHandler, %function
FLEXPWM1_SUBMODULE3_IRQHandler:
    ldr   r0,=FLEXPWM1_SUBMODULE3_DriverIRQHandler
    bx    r0
    .size FLEXPWM1_SUBMODULE3_IRQHandler, . - FLEXPWM1_SUBMODULE3_IRQHandler

    .align 1
    .thumb_func
    .weak EQDC1_COMPARE_IRQHandler
    .type EQDC1_COMPARE_IRQHandler, %function
EQDC1_COMPARE_IRQHandler:
    ldr   r0,=EQDC1_COMPARE_DriverIRQHandler
    bx    r0
    .size EQDC1_COMPARE_IRQHandler, . - EQDC1_COMPARE_IRQHandler

    .align 1
    .thumb_func
    .weak EQDC1_HOME_IRQHandler
    .type EQDC1_HOME_IRQHandler, %function
EQDC1_HOME_IRQHandler:
    ldr   r0,=EQDC1_HOME_DriverIRQHandler
    bx    r0
    .size EQDC1_HOME_IRQHandler, . - EQDC1_HOME_IRQHandler

    .align 1
    .thumb_func
    .weak EQDC1_WATCHDOG_IRQHandler
    .type EQDC1_WATCHDOG_IRQHandler, %function
EQDC1_WATCHDOG_IRQHandler:
    ldr   r0,=EQDC1_WATCHDOG_DriverIRQHandler
    bx    r0
    .size EQDC1_WATCHDOG_IRQHandler, . - EQDC1_WATCHDOG_IRQHandler

    .align 1
    .thumb_func
    .weak EQDC1_INDEX_IRQHandler
    .type EQDC1_INDEX_IRQHandler, %function
EQDC1_INDEX_IRQHandler:
    ldr   r0,=EQDC1_INDEX_DriverIRQHandler
    bx    r0
    .size EQDC1_INDEX_IRQHandler, . - EQDC1_INDEX_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved105_IRQHandler
    .type Reserved105_IRQHandler, %function
Reserved105_IRQHandler:
    ldr   r0,=Reserved105_DriverIRQHandler
    bx    r0
    .size Reserved105_IRQHandler, . - Reserved105_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved106_IRQHandler
    .type Reserved106_IRQHandler, %function
Reserved106_IRQHandler:
    ldr   r0,=Reserved106_DriverIRQHandler
    bx    r0
    .size Reserved106_IRQHandler, . - Reserved106_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved107_IRQHandler
    .type Reserved107_IRQHandler, %function
Reserved107_IRQHandler:
    ldr   r0,=Reserved107_DriverIRQHandler
    bx    r0
    .size Reserved107_IRQHandler, . - Reserved107_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved108_IRQHandler
    .type Reserved108_IRQHandler, %function
Reserved108_IRQHandler:
    ldr   r0,=Reserved108_DriverIRQHandler
    bx    r0
    .size Reserved108_IRQHandler, . - Reserved108_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved109_IRQHandler
    .type Reserved109_IRQHandler, %function
Reserved109_IRQHandler:
    ldr   r0,=Reserved109_DriverIRQHandler
    bx    r0
    .size Reserved109_IRQHandler, . - Reserved109_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved110_IRQHandler
    .type Reserved110_IRQHandler, %function
Reserved110_IRQHandler:
    ldr   r0,=Reserved110_DriverIRQHandler
    bx    r0
    .size Reserved110_IRQHandler, . - Reserved110_IRQHandler

    .align 1
    .thumb_func
    .weak LPUART5_IRQHandler
    .type LPUART5_IRQHandler, %function
LPUART5_IRQHandler:
    ldr   r0,=LPUART5_DriverIRQHandler
    bx    r0
    .size LPUART5_IRQHandler, . - LPUART5_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved112_IRQHandler
    .type Reserved112_IRQHandler, %function
Reserved112_IRQHandler:
    ldr   r0,=Reserved112_DriverIRQHandler
    bx    r0
    .size Reserved112_IRQHandler, . - Reserved112_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved113_IRQHandler
    .type Reserved113_IRQHandler, %function
Reserved113_IRQHandler:
    ldr   r0,=Reserved113_DriverIRQHandler
    bx    r0
    .size Reserved113_IRQHandler, . - Reserved113_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved114_IRQHandler
    .type Reserved114_IRQHandler, %function
Reserved114_IRQHandler:
    ldr   r0,=Reserved114_DriverIRQHandler
    bx    r0
    .size Reserved114_IRQHandler, . - Reserved114_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved115_IRQHandler
    .type Reserved115_IRQHandler, %function
Reserved115_IRQHandler:
    ldr   r0,=Reserved115_DriverIRQHandler
    bx    r0
    .size Reserved115_IRQHandler, . - Reserved115_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved116_IRQHandler
    .type Reserved116_IRQHandler, %function
Reserved116_IRQHandler:
    ldr   r0,=Reserved116_DriverIRQHandler
    bx    r0
    .size Reserved116_IRQHandler, . - Reserved116_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved117_IRQHandler
    .type Reserved117_IRQHandler, %function
Reserved117_IRQHandler:
    ldr   r0,=Reserved117_DriverIRQHandler
    bx    r0
    .size Reserved117_IRQHandler, . - Reserved117_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved118_IRQHandler
    .type Reserved118_IRQHandler, %function
Reserved118_IRQHandler:
    ldr   r0,=Reserved118_DriverIRQHandler
    bx    r0
    .size Reserved118_IRQHandler, . - Reserved118_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved119_IRQHandler
    .type Reserved119_IRQHandler, %function
Reserved119_IRQHandler:
    ldr   r0,=Reserved119_DriverIRQHandler
    bx    r0
    .size Reserved119_IRQHandler, . - Reserved119_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved120_IRQHandler
    .type Reserved120_IRQHandler, %function
Reserved120_IRQHandler:
    ldr   r0,=Reserved120_DriverIRQHandler
    bx    r0
    .size Reserved120_IRQHandler, . - Reserved120_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved121_IRQHandler
    .type Reserved121_IRQHandler, %function
Reserved121_IRQHandler:
    ldr   r0,=Reserved121_DriverIRQHandler
    bx    r0
    .size Reserved121_IRQHandler, . - Reserved121_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved122_IRQHandler
    .type Reserved122_IRQHandler, %function
Reserved122_IRQHandler:
    ldr   r0,=Reserved122_DriverIRQHandler
    bx    r0
    .size Reserved122_IRQHandler, . - Reserved122_IRQHandler

    .align 1
    .thumb_func
    .weak MAU_IRQHandler
    .type MAU_IRQHandler, %function
MAU_IRQHandler:
    ldr   r0,=MAU_DriverIRQHandler
    bx    r0
    .size MAU_IRQHandler, . - MAU_IRQHandler

    .align 1
    .thumb_func
    .weak SMARTDMA_IRQHandler
    .type SMARTDMA_IRQHandler, %function
SMARTDMA_IRQHandler:
    ldr   r0,=SMARTDMA_DriverIRQHandler
    bx    r0
    .size SMARTDMA_IRQHandler, . - SMARTDMA_IRQHandler

    .align 1
    .thumb_func
    .weak CDOG1_IRQHandler
    .type CDOG1_IRQHandler, %function
CDOG1_IRQHandler:
    ldr   r0,=CDOG1_DriverIRQHandler
    bx    r0
    .size CDOG1_IRQHandler, . - CDOG1_IRQHandler

    .align 1
    .thumb_func
    .weak PKC_IRQHandler
    .type PKC_IRQHandler, %function
PKC_IRQHandler:
    ldr   r0,=PKC_DriverIRQHandler
    bx    r0
    .size PKC_IRQHandler, . - PKC_IRQHandler

    .align 1
    .thumb_func
    .weak SGI_IRQHandler
    .type SGI_IRQHandler, %function
SGI_IRQHandler:
    ldr   r0,=SGI_DriverIRQHandler
    bx    r0
    .size SGI_IRQHandler, . - SGI_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved128_IRQHandler
    .type Reserved128_IRQHandler, %function
Reserved128_IRQHandler:
    ldr   r0,=Reserved128_DriverIRQHandler
    bx    r0
    .size Reserved128_IRQHandler, . - Reserved128_IRQHandler

    .align 1
    .thumb_func
    .weak TRNG0_IRQHandler
    .type TRNG0_IRQHandler, %function
TRNG0_IRQHandler:
    ldr   r0,=TRNG0_DriverIRQHandler
    bx    r0
    .size TRNG0_IRQHandler, . - TRNG0_IRQHandler

    .align 1
    .thumb_func
    .weak SECURE_ERR_IRQHandler
    .type SECURE_ERR_IRQHandler, %function
SECURE_ERR_IRQHandler:
    ldr   r0,=SECURE_ERR_DriverIRQHandler
    bx    r0
    .size SECURE_ERR_IRQHandler, . - SECURE_ERR_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved131_IRQHandler
    .type Reserved131_IRQHandler, %function
Reserved131_IRQHandler:
    ldr   r0,=Reserved131_DriverIRQHandler
    bx    r0
    .size Reserved131_IRQHandler, . - Reserved131_IRQHandler

    .align 1
    .thumb_func
    .weak ADC2_IRQHandler
    .type ADC2_IRQHandler, %function
ADC2_IRQHandler:
    ldr   r0,=ADC2_DriverIRQHandler
    bx    r0
    .size ADC2_IRQHandler, . - ADC2_IRQHandler

    .align 1
    .thumb_func
    .weak ADC3_IRQHandler
    .type ADC3_IRQHandler, %function
ADC3_IRQHandler:
    ldr   r0,=ADC3_DriverIRQHandler
    bx    r0
    .size ADC3_IRQHandler, . - ADC3_IRQHandler

    .align 1
    .thumb_func
    .weak Reserved134_IRQHandler
    .type Reserved134_IRQHandler, %function
Reserved134_IRQHandler:
    ldr   r0,=Reserved134_DriverIRQHandler
    bx    r0
    .size Reserved134_IRQHandler, . - Reserved134_IRQHandler

    .align 1
    .thumb_func
    .weak RTC_IRQHandler
    .type RTC_IRQHandler, %function
RTC_IRQHandler:
    ldr   r0,=RTC_DriverIRQHandler
    bx    r0
    .size RTC_IRQHandler, . - RTC_IRQHandler

    .align 1
    .thumb_func
    .weak RTC_1HZ_IRQHandler
    .type RTC_1HZ_IRQHandler, %function
RTC_1HZ_IRQHandler:
    ldr   r0,=RTC_1HZ_DriverIRQHandler
    bx    r0
    .size RTC_1HZ_IRQHandler, . - RTC_1HZ_IRQHandler

    .align 1
    .thumb_func
    .weak SLCD_IRQHandler
    .type SLCD_IRQHandler, %function
SLCD_IRQHandler:
    ldr   r0,=SLCD_DriverIRQHandler
    bx    r0
    .size SLCD_IRQHandler, . - SLCD_IRQHandler


/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler  handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler    MemManage_Handler
    def_irq_handler    BusFault_Handler
    def_irq_handler    UsageFault_Handler
    def_irq_handler    SecureFault_Handler
    def_irq_handler    DebugMon_Handler
    def_irq_handler    Reserved16_DriverIRQHandler
    def_irq_handler    CMC_DriverIRQHandler
    def_irq_handler    DMA_CH0_DriverIRQHandler
    def_irq_handler    DMA_CH1_DriverIRQHandler
    def_irq_handler    DMA_CH2_DriverIRQHandler
    def_irq_handler    DMA_CH3_DriverIRQHandler
    def_irq_handler    DMA_CH4_DriverIRQHandler
    def_irq_handler    DMA_CH5_DriverIRQHandler
    def_irq_handler    DMA_CH6_DriverIRQHandler
    def_irq_handler    DMA_CH7_DriverIRQHandler
    def_irq_handler    ERM0_SINGLE_BIT_DriverIRQHandler
    def_irq_handler    ERM0_MULTI_BIT_DriverIRQHandler
    def_irq_handler    FMU0_DriverIRQHandler
    def_irq_handler    GLIKEY0_DriverIRQHandler
    def_irq_handler    MBC0_DriverIRQHandler
    def_irq_handler    SCG0_DriverIRQHandler
    def_irq_handler    SPC0_DriverIRQHandler
    def_irq_handler    TDET_DriverIRQHandler
    def_irq_handler    WUU0_DriverIRQHandler
    def_irq_handler    CAN0_DriverIRQHandler
    def_irq_handler    CAN1_DriverIRQHandler
    def_irq_handler    Reserved37_DriverIRQHandler
    def_irq_handler    Reserved38_DriverIRQHandler
    def_irq_handler    FLEXIO_DriverIRQHandler
    def_irq_handler    I3C0_DriverIRQHandler
    def_irq_handler    Reserved41_DriverIRQHandler
    def_irq_handler    LPI2C0_DriverIRQHandler
    def_irq_handler    LPI2C1_DriverIRQHandler
    def_irq_handler    LPSPI0_DriverIRQHandler
    def_irq_handler    LPSPI1_DriverIRQHandler
    def_irq_handler    Reserved46_DriverIRQHandler
    def_irq_handler    LPUART0_DriverIRQHandler
    def_irq_handler    LPUART1_DriverIRQHandler
    def_irq_handler    LPUART2_DriverIRQHandler
    def_irq_handler    LPUART3_DriverIRQHandler
    def_irq_handler    LPUART4_DriverIRQHandler
    def_irq_handler    USB0_DriverIRQHandler
    def_irq_handler    Reserved53_DriverIRQHandler
    def_irq_handler    CDOG0_DriverIRQHandler
    def_irq_handler    CTIMER0_DriverIRQHandler
    def_irq_handler    CTIMER1_DriverIRQHandler
    def_irq_handler    CTIMER2_DriverIRQHandler
    def_irq_handler    CTIMER3_DriverIRQHandler
    def_irq_handler    CTIMER4_DriverIRQHandler
    def_irq_handler    FLEXPWM0_RELOAD_ERROR_DriverIRQHandler
    def_irq_handler    FLEXPWM0_FAULT_DriverIRQHandler
    def_irq_handler    FLEXPWM0_SUBMODULE0_DriverIRQHandler
    def_irq_handler    FLEXPWM0_SUBMODULE1_DriverIRQHandler
    def_irq_handler    FLEXPWM0_SUBMODULE2_DriverIRQHandler
    def_irq_handler    FLEXPWM0_SUBMODULE3_DriverIRQHandler
    def_irq_handler    EQDC0_COMPARE_DriverIRQHandler
    def_irq_handler    EQDC0_HOME_DriverIRQHandler
    def_irq_handler    EQDC0_WATCHDOG_DriverIRQHandler
    def_irq_handler    EQDC0_INDEX_DriverIRQHandler
    def_irq_handler    FREQME0_DriverIRQHandler
    def_irq_handler    LPTMR0_DriverIRQHandler
    def_irq_handler    Reserved72_DriverIRQHandler
    def_irq_handler    OS_EVENT_DriverIRQHandler
    def_irq_handler    WAKETIMER0_DriverIRQHandler
    def_irq_handler    UTICK0_DriverIRQHandler
    def_irq_handler    WWDT0_DriverIRQHandler
    def_irq_handler    Reserved77_DriverIRQHandler
    def_irq_handler    ADC0_DriverIRQHandler
    def_irq_handler    ADC1_DriverIRQHandler
    def_irq_handler    CMP0_DriverIRQHandler
    def_irq_handler    CMP1_DriverIRQHandler
    def_irq_handler    CMP2_DriverIRQHandler
    def_irq_handler    DAC0_DriverIRQHandler
    def_irq_handler    Reserved84_DriverIRQHandler
    def_irq_handler    Reserved85_DriverIRQHandler
    def_irq_handler    Reserved86_DriverIRQHandler
    def_irq_handler    GPIO0_DriverIRQHandler
    def_irq_handler    GPIO1_DriverIRQHandler
    def_irq_handler    GPIO2_DriverIRQHandler
    def_irq_handler    GPIO3_DriverIRQHandler
    def_irq_handler    GPIO4_DriverIRQHandler
    def_irq_handler    Reserved92_DriverIRQHandler
    def_irq_handler    LPI2C2_DriverIRQHandler
    def_irq_handler    LPI2C3_DriverIRQHandler
    def_irq_handler    FLEXPWM1_RELOAD_ERROR_DriverIRQHandler
    def_irq_handler    FLEXPWM1_FAULT_DriverIRQHandler
    def_irq_handler    FLEXPWM1_SUBMODULE0_DriverIRQHandler
    def_irq_handler    FLEXPWM1_SUBMODULE1_DriverIRQHandler
    def_irq_handler    FLEXPWM1_SUBMODULE2_DriverIRQHandler
    def_irq_handler    FLEXPWM1_SUBMODULE3_DriverIRQHandler
    def_irq_handler    EQDC1_COMPARE_DriverIRQHandler
    def_irq_handler    EQDC1_HOME_DriverIRQHandler
    def_irq_handler    EQDC1_WATCHDOG_DriverIRQHandler
    def_irq_handler    EQDC1_INDEX_DriverIRQHandler
    def_irq_handler    Reserved105_DriverIRQHandler
    def_irq_handler    Reserved106_DriverIRQHandler
    def_irq_handler    Reserved107_DriverIRQHandler
    def_irq_handler    Reserved108_DriverIRQHandler
    def_irq_handler    Reserved109_DriverIRQHandler
    def_irq_handler    Reserved110_DriverIRQHandler
    def_irq_handler    LPUART5_DriverIRQHandler
    def_irq_handler    Reserved112_DriverIRQHandler
    def_irq_handler    Reserved113_DriverIRQHandler
    def_irq_handler    Reserved114_DriverIRQHandler
    def_irq_handler    Reserved115_DriverIRQHandler
    def_irq_handler    Reserved116_DriverIRQHandler
    def_irq_handler    Reserved117_DriverIRQHandler
    def_irq_handler    Reserved118_DriverIRQHandler
    def_irq_handler    Reserved119_DriverIRQHandler
    def_irq_handler    Reserved120_DriverIRQHandler
    def_irq_handler    Reserved121_DriverIRQHandler
    def_irq_handler    Reserved122_DriverIRQHandler
    def_irq_handler    MAU_DriverIRQHandler
    def_irq_handler    SMARTDMA_DriverIRQHandler
    def_irq_handler    CDOG1_DriverIRQHandler
    def_irq_handler    PKC_DriverIRQHandler
    def_irq_handler    SGI_DriverIRQHandler
    def_irq_handler    Reserved128_DriverIRQHandler
    def_irq_handler    TRNG0_DriverIRQHandler
    def_irq_handler    SECURE_ERR_DriverIRQHandler
    def_irq_handler    Reserved131_DriverIRQHandler
    def_irq_handler    ADC2_DriverIRQHandler
    def_irq_handler    ADC3_DriverIRQHandler
    def_irq_handler    Reserved134_DriverIRQHandler
    def_irq_handler    RTC_DriverIRQHandler
    def_irq_handler    RTC_1HZ_DriverIRQHandler
    def_irq_handler    SLCD_DriverIRQHandler

    .end
