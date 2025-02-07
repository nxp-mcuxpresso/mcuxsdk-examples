/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "zephyr/debug/coredump.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define COREDUMP_HEADER_SIZE (sizeof(struct coredump_hdr_t))
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern uint32_t fault_capture(uint32_t msp, uint32_t psp, uint32_t exc_return, struct arch_esf *ptrEsf);
void DEMO_TriggetUseFault(void);

struct arch_esf esf;

union {
  uint8_t u8CoredumpHeaderBuf[COREDUMP_HEADER_SIZE];
  uint32_t u32CoredumpHeaderBuf[COREDUMP_HEADER_SIZE/4];
} CoredumpHeader;
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void UsageFault_Handler(void)
{
    uint32_t reason;
    uint32_t msp = __get_MSP();
    uint32_t psp = __get_PSP();
    uint32_t exc_return = 0UL;
#if defined(__GNUC__)
    __asm volatile ("mov %0, lr" : "=r" (exc_return));
#elif defined(__ICCARM__)
    __asm("mov %0, lr" : "=r" (exc_return));
#elif defined(__CC_ARM)
    __asm volatile("mov %0, lr" : "=r"(exc_return));
#endif
    reason = fault_capture(msp, psp, exc_return, &esf);
    coredump(reason, &esf, NULL);
    /* Trigger software reset. */
    NVIC_SystemReset();
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();
    
    if(BOARD_SoftwareResetAsserted())
    {
        PRINTF("Coredump Fault Example Rebooted!!!\r\n");
        PRINTF("Press any key to check coredump reason...\r\n");
        GETCHAR();
        if (coredump_query(COREDUMP_QUERY_HAS_STORED_DUMP, NULL) != 0)
        {
            if (coredump_query(COREDUMP_QUERY_GET_ERROR, NULL) != 0)
            {
                PRINTF("[Error] Stored dump have error!\r\n");
            }
            int32_t storedDumpSize = coredump_query(COREDUMP_QUERY_GET_STORED_DUMP_SIZE, NULL);
            if (storedDumpSize > 0)
            {
                PRINTF("[Info] Size: %d.\r\n", storedDumpSize);
                struct coredump_cmd_copy_arg tmpArgs = 
                {
                    .offset = 0U,
                    .length = COREDUMP_HEADER_SIZE,
                    .buffer = CoredumpHeader.u8CoredumpHeaderBuf,
                };
                if (coredump_cmd(COREDUMP_CMD_COPY_STORED_DUMP, &tmpArgs) == COREDUMP_HEADER_SIZE)
                {
                    if (CoredumpHeader.u32CoredumpHeaderBuf[2] == K_ERR_ARM_USAGE_DIV_0)
                    {
                       PRINTF("[Info] Reason: Usagefault caused by an integer divsion by zero!\r\n");
                    }
                }
                else
                {
                  PRINTF("[Error] Fail copy stored dump!\r\n");
                }
            }
            else
            {
                PRINTF("[Error] Stored dump size query fail!\r\n");
            }
        }
        else
        {
            PRINTF("[Error] Stored dump not found!\r\n");
        }
    }
    else
    {
        PRINTF("Coredump Fault Example Start!\r\n");
        PRINTF("Press any key to trigger usagefault...\r\n");
        GETCHAR();
        DEMO_TriggetUseFault();
    }
    
    PRINTF("Coredump Fault Example End!\r\n");
    while (1)
    {
    }
}


void DEMO_TriggetUseFault(void)
{
    volatile uint32_t x = 1UL;
    volatile uint32_t y = 0UL;
    
    /* Enable division by zero cause a usagefault. */
    SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;
    SCB->AIRCR |= SCB_AIRCR_BFHFNMINS_Msk;
    
    /* Trigger usefault by divide 0. */
    x = x / y;
}
