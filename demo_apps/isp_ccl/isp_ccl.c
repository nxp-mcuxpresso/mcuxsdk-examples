/*
 * Copyright 2023-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"



#include "fsl_dpu.h"
#include "display_support.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "display_task.h"
#include "camera_task.h"

#ifdef USB_STDOUT
    #include "stdout_usb.h"
    #include "virtual_com.h"
#endif

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*==============================================================================
*                              LOCAL FUNCTIONS
==============================================================================*/

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    char *pcTaskName )
{
    PRINTF("\r\nStack overflow: %s\r\n", pcTaskName);

    /* coverity[infinite_loop:SUPPRESS] */
    while (1)
    {
    }

}

#pragma GCC push_options
#pragma GCC optimize ("O0")
void HardFault_Handler(void)
{

    __asm volatile
    (
        "TST lr, #4 \n"
        "ITE EQ \n"
        "MRSEQ r0, MSP \n"
        "MRSNE r0, PSP \n"
        "B HardFault_HandlerC \n"
    );
}

void HardFault_HandlerC(uint32_t *stack_address)
{
    uint32_t r0  = stack_address[0];
    uint32_t r1  = stack_address[1];
    uint32_t r2  = stack_address[2];
    uint32_t r3  = stack_address[3];
    uint32_t r12 = stack_address[4];
    uint32_t lr  = stack_address[5];
    uint32_t pc  = stack_address[6];  // Faulting instruction address
    uint32_t psr = stack_address[7];

    PRINTF("\r\n========== HardFault ==========\r\n");
    PRINTF("R0  = 0x%08lx\r\n", r0);
    PRINTF("R1  = 0x%08lx\r\n", r1);
    PRINTF("R2  = 0x%08lx\r\n", r2);
    PRINTF("R3  = 0x%08lx\r\n", r3);
    PRINTF("R12 = 0x%08lx\r\n", r12);
    PRINTF("LR  = 0x%08lx\r\n", lr);
    PRINTF("PC  = 0x%08lx\r\n", pc);
    PRINTF("PSR = 0x%08lx\r\n", psr);
    PRINTF("================================\r\n");

    while (1)
    {
    }
}


void DefaultISR(void)
{
    __asm volatile
    (
        "TST lr, #4 \n"
        "ITE EQ \n"
        "MRSEQ r0, MSP \n"
        "MRSNE r0, PSP \n"
        "B DefaultISR_c \n"
    );
}

void DefaultISR_c(uint32_t *stack_address)
{
    volatile uint32_t ipsr;
    __asm volatile ("MRS %0, ipsr" : "=r" (ipsr));

    volatile uint32_t exc = ipsr & 0x1FF;

    uint32_t r0  = stack_address[0];
    uint32_t r1  = stack_address[1];
    uint32_t r2  = stack_address[2];
    uint32_t r3  = stack_address[3];
    uint32_t r12 = stack_address[4];
    uint32_t lr  = stack_address[5];
    uint32_t pc  = stack_address[6];  // Faulting instruction address
    uint32_t psr = stack_address[7];

    PRINTF("\r\n========== DefaultISR ==========\r\n");
    PRINTF("ISR  = 0x%08ld\r\n", exc);
    switch (exc)
    {
        case 1:
            PRINTF("Reset\n\r");
            break;
        case 2:
            PRINTF("NMI\n\r");
            break;
        case 3:
            PRINTF("HardFault\n\r");
            break;
        case 4:
            PRINTF("MemManage\n\r");
            break;
        case 5:
            PRINTF("BusFault\n\r");
            break;
        case 6:
            PRINTF("UsageFault\n\r");
            break;
    }
    PRINTF("R0  = 0x%08lx\r\n", r0);
    PRINTF("R1  = 0x%08lx\r\n", r1);
    PRINTF("R2  = 0x%08lx\r\n", r2);
    PRINTF("R3  = 0x%08lx\r\n", r3);
    PRINTF("R12 = 0x%08lx\r\n", r12);
    PRINTF("LR  = 0x%08lx\r\n", lr);
    PRINTF("PC  = 0x%08lx\r\n", pc);
    PRINTF("PSR = 0x%08lx\r\n", psr);

    PRINTF("================================\r\n");

    while (1); // breakpoint here
}


#pragma GCC pop_options

int mainC(void);
int main(void)
{
    // clear bss variables in ddr to 0
    __asm volatile (
        "ldr r1, =__bss_ddr_start__\n"
        "ldr r2, =__bss_ddr_end__\n"

        "movs    r0, 0\n"
    ".bss_clear:\n"
        "cmp     r1, r2\n"
        "itt    lt\n"
        "strlt   r0, [r1], #4\n"
        "blt    .bss_clear\n"
    );
    mainC();
}

/*!
 * @brief Main function
 */
int mainC(void)
{
#ifdef USB_STDOUT
    UsbStdOutInit();
#endif
    BOARD_InitHardware();
    PRINTF("\r\n ISP Example.\r\n");

#ifdef USB_STDOUT
    StartUsb();
#endif

    DPU_Init(APP_DPU);

    DPU_PreparePathConfig(APP_DPU);

    PRINTF("\r\nInit display.\r\n");
    /* Display frame buffer ready, start to show. */
    StartDisplayTask();

    StartCameraTask();

    PRINTF("\r\nStarting scheduler.\r\n");
    vTaskStartScheduler();

    // coverity[infinite_loop:SUPPRESS]
    while (1) {  }
}
