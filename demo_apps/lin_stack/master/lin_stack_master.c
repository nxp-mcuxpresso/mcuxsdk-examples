/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "lin.h"
#include "lin_common_api.h"
#include "fsl_tpm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Values for motor1 control. */
#define MOTOR1_MAX_VALUE  100
#define MOTOR1_MIN_VALUE  50
#define MOTOR1_STOP_VALUE 150

/* Selction command. */
#define MOTOR1_SELECTION_INCREASE 0X01
#define MOTOR1_SELECTION_DECREASE 0x02
#define MOTOR1_SELECTION_STOP     0x03

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint16_t timerOverflowInterruptCount = 0U;
volatile l_u8 g_motorTickCount       = 0U;
volatile l_u8 g_motorSelctionCmd     = 0U;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* LIN master task. */
static void DEMO_MasterTaskStart(void);

/* Timer initialize for LIN cluster used. */
static void DEMO_TimerInit(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(DEMO_TIMER_TPM_ENABLE) && DEMO_TIMER_TPM_ENABLE
/*!
 * This interrupt routine checks for bus timeout and switches schedule table
 */
void DEMO_TPM_IRQHandler(void)
{
    /* Static variable, used to count if the timeout has passed to
     * provide the LIN schedule tick.
     */
    static uint32_t interruptCount = 0UL;

    /* if timer overflow flag */
    if (TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & kTPM_TimeOverflowFlag)
    {
        /* Clear interrupt flag.*/
        TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, kTPM_TimeOverflowFlag);

        /* Increment overflow count */
        timerOverflowInterruptCount++;
        /* Timer Interrupt Handler */
        lin_lld_timeout_service(LI0);

        /* If 5 ms have passed, provide the required tick */
        if (++interruptCount == 10UL)
        {
            l_sch_tick(LI0);
            interruptCount = 0UL;
        }
    }
    SDK_ISR_EXIT_BARRIER;
}

/* Timer call back for calculating the baud rate. */
void timerGetTimeIntervalCallback0(uint32_t *ns)
{
    static uint32_t previousCountValue = 0UL;
    uint32_t counterValue;

    counterValue = TPM_GetCurrentTimerCount(DEMO_TPM_BASEADDR);
    *ns = (((uint32_t)(counterValue + timerOverflowInterruptCount * DEMO_TIMER_COMPARE_VALUE - previousCountValue)) *
           1000) /
          DEMO_TIMER_TICKS_1US;
    timerOverflowInterruptCount = 0UL;
    previousCountValue          = counterValue;
}
#endif

#if (defined(DEMO_SW_USE_SEPARATE_HANDLER) && DEMO_SW_USE_SEPARATE_HANDLER)
/*!
 * This interrupt routine puts a node sends wakeup signal on button press
 */
void DEMO_SW2_IRQ_HANDLER(void)
{
    /* If button 2 was pressed, send the wake up signal to and start the normal table. */
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
    if (GPIO_GpioGetInterruptFlags(DEMO_BUTTON2_GPIO) & (1U << DEMO_BUTTON2_PIN))
#else
    if (GPIO_PortGetInterruptFlags(DEMO_BUTTON2_GPIO) & (1U << DEMO_BUTTON2_PIN))
#endif
    {
        /* Clear external interrupt flag. */
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
        GPIO_GpioClearInterruptFlags(DEMO_BUTTON2_GPIO, 1U << DEMO_BUTTON2_PIN);
#else
        GPIO_PortClearInterruptFlags(DEMO_BUTTON2_GPIO, 1U << DEMO_BUTTON2_PIN);
#endif
        /* Switch to normal table */
        l_ifc_wake_up(LI0);
        l_sch_set(LI0, LI0_NormalTable, 0u);
    }
    else
    {
        ;
    }
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * This interrupt routine puts a node into sleep mode on button press
 */
void DEMO_SW3_IRQ_HANDLER(void)
{
    /* If button 1 was pressed, send the sleep signal. */
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
    if (GPIO_GpioGetInterruptFlags(DEMO_BUTTON1_GPIO) & (1U << DEMO_BUTTON1_PIN))
#else
    if (GPIO_PortGetInterruptFlags(DEMO_BUTTON1_GPIO) & (1U << DEMO_BUTTON1_PIN))
#endif
    {
        /* Clear external interrupt flag. */
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
        GPIO_GpioClearInterruptFlags(DEMO_BUTTON1_GPIO, 1U << DEMO_BUTTON1_PIN);
#else
        GPIO_PortClearInterruptFlags(DEMO_BUTTON1_GPIO, 1U << DEMO_BUTTON1_PIN);
#endif
        l_sch_set(LI0, LI0_GOTO_SLEEP_SCHEDULE, 0u);
    }
    else
    {
        ;
    }
    SDK_ISR_EXIT_BARRIER;
}
#else
/*!
 * This interrupt routine puts a node into sleep mode or sends wakeup signal on button press
 */
void DEMO_SW_IRQ_HANDLER(void)
{
    /* If button 1 was pressed, send the sleep signal. */
    if (GPIO_PortGetInterruptFlags(DEMO_BUTTON1_GPIO) & (1U << DEMO_BUTTON1_PIN))
    {
        /* Clear external interrupt flag. */
        GPIO_PortClearInterruptFlags(DEMO_BUTTON1_GPIO, 1U << DEMO_BUTTON1_PIN);
        l_sch_set(LI0, LI0_GOTO_SLEEP_SCHEDULE, 0u);
    }
    /* If button 2 was pressed, send the wake up signal to and start the normal table. */
    else if (GPIO_PortGetInterruptFlags(DEMO_BUTTON2_GPIO) & (1U << DEMO_BUTTON2_PIN))
    {
        /* Clear external interrupt flag. */
        GPIO_PortClearInterruptFlags(DEMO_BUTTON2_GPIO, 1U << DEMO_BUTTON2_PIN);
        /* Switch to normal table */
        l_ifc_wake_up(LI0);
        l_sch_set(LI0, LI0_NormalTable, 0u);
    }
    else
    {
        ;
    }
    SDK_ISR_EXIT_BARRIER;
}
#endif

/*!
 * This interrupt routine handles LIN bus low level communication.
 */
void DEMO_LIN_IRQHandler(void)
{
    LIN_DRV_IRQHandler(DEMO_MASTER_LPUART_INSTANCE);
    SDK_ISR_EXIT_BARRIER;
}

/* @brief LIN master task.
 *        This task will emulate a master to node to receive data from slave node.
 *        And according to the temp data, send different command to slave node.
 */
static void DEMO_MasterTaskStart(void)
{
    /* Initialize LIN network interface */
    l_sys_init();
    l_ifc_init(LI0);
    /* Set Schedule table to Normal */
    l_sch_set(LI0, LI0_NormalTable, 0u);
    /* Infinite loop */
    for (;;)
    {
        /* Check if information about the Motor1 tick count has been received */
        if (l_flg_tst_LI0_Motor1Temp_flag())
        {
            /* Clear this flag... */
            l_flg_clr_LI0_Motor1Temp_flag();

            /* Store data from slave node. */
            g_motorTickCount = l_u8_rd_LI0_Motor1Temp();

            /* If the tick count value from slave node is larger than stop value, stop slave tick count. */
            if (MOTOR1_STOP_VALUE < g_motorTickCount)
            {
                if (g_motorSelctionCmd != MOTOR1_SELECTION_STOP)
                {
                    g_motorSelctionCmd = MOTOR1_SELECTION_STOP;
                    /* Update the selction command. */
                    l_u8_wr_LI0_Motor1Selection(g_motorSelctionCmd);

                    /* Set the LED 3 on. */
                    DEMO_LED1_OFF();
                    DEMO_LED2_OFF();
                    DEMO_LED3_ON();

                    PRINTF(" -> LED 3 is ON!\r\n");
                }
            }
            /* If value is larger than MAX value, set slave node to decrease the count. */
            else if (MOTOR1_MAX_VALUE < g_motorTickCount)
            {
                if (g_motorSelctionCmd != MOTOR1_SELECTION_DECREASE)
                {
                    g_motorSelctionCmd = MOTOR1_SELECTION_DECREASE;
                    /* Update the selction command. */
                    l_u8_wr_LI0_Motor1Selection(g_motorSelctionCmd);

                    /* Set the LED 1 on. */
                    DEMO_LED1_ON();
                    DEMO_LED2_OFF();
                    DEMO_LED3_OFF();

                    PRINTF(" -> LED 1 is ON!\r\n");
                }
            }
            /* If value is less than the MIN value, set slave to increase the count. */
            else if (MOTOR1_MIN_VALUE > g_motorTickCount)
            {
                if (g_motorSelctionCmd != MOTOR1_SELECTION_INCREASE)
                {
                    g_motorSelctionCmd = MOTOR1_SELECTION_INCREASE;
                    /* Update the selction command. */
                    l_u8_wr_LI0_Motor1Selection(g_motorSelctionCmd);

                    /* Set the LED 2 on. */
                    DEMO_LED1_OFF();
                    DEMO_LED2_ON();
                    DEMO_LED3_OFF();

                    PRINTF(" -> LED 2 is ON!\r\n");
                }
            }
            /* If value between MAX and MIN, keep the led state. */
            else
            {
            }
        }
    }
}

/*!
 *@brief Timer initialize for LIN cluster.
 *       Initialize a timer for LIN cluster used, the time period is 500us.
 */
static void DEMO_TimerInit(void)
{
/* Enable the TPM timer for LIN timer service. */
#if defined(DEMO_TIMER_TPM_ENABLE) && DEMO_TIMER_TPM_ENABLE
    tpm_config_t tpmInfo;

    TPM_GetDefaultConfig(&tpmInfo);
    /* Initialize TPM module */
    TPM_Init(DEMO_TPM_BASEADDR, &tpmInfo);
    /* Set module value */
    DEMO_TPM_BASEADDR->MOD = DEMO_MODULO_VALUE;
    /* Enable interrupt on overflow */
    TPM_EnableInterrupts(DEMO_TPM_BASEADDR, kTPM_TimeOverflowInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(DEMO_TPM_IRQn);
    /* Start scheduler */
    TPM_StartTimer(DEMO_TPM_BASEADDR, kTPM_SystemClock);
#endif
}
/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("LIN master demo start.\r\n");
    PRINTF("LIN master will receive data from slave node, and send control command to slave. \r\n");

    /* Initialize a timer for LIN cluster used. */
    DEMO_TimerInit();

    /* Start the LIN master task. */
    DEMO_MasterTaskStart();

    while (1)
    {
    }
}
