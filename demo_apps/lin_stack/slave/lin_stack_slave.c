/*
 * Copyright 2019,2026 NXP
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
#include "lin_commontl_api.h"
#include "lin_diagnostic_service.h"
#include "fsl_tpm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Values for motor1 control. */
#define MOTOR1_STOP_VALUE_DEFAULT 150U

/* Selection command. */
#define MOTOR1_SELECTION_INCREASE 0X01
#define MOTOR1_SELECTION_DECREASE 0x02
#define MOTOR1_SELECTION_STOP     0x03

/* Button 1 press count threshold to trigger a system error event via event-triggered frame. */
#define SLAVE_BTN1_EVENT_THRESHOLD   5U
#define MOTOR1_ERROR_CODE_SYS_EVENT  0x01U

/* Bit 2 of the Motor1Control frame byte used to signal a sporadic command to increase the motor stop
   value. Bits [7:2] are unused by any defined signal so they do not need the signal write macros. */
#define MOTOR1_SELECTION_INC_STOP_BIT (1U << 2U)

/* Interrupt priority. */
#define DEMO_LIN_PRIO   0
#define DEMO_TIMER_PRIO (DEMO_LIN_PRIO + 1)

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint16_t timerOverflowInterruptCount = 0U;
volatile l_u8 g_motorSelectionCmd    = 0U;
volatile l_u8 g_motorTickCount       = 10U;
volatile uint32_t capturedValue      = 0U;
uint16_t timerCounterValue[2]        = {0u};
volatile l_u8 g_motor1StopValue      = MOTOR1_STOP_VALUE_DEFAULT;
static volatile l_u8 g_btn1EventCount  = 0U;
static volatile bool g_eventDataPending = false;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* LIN slave task. */
static void DEMO_SlaveTaskStart(void);
/* LIN timer initialize. */
static void DEMO_TimerInit(void);
/* Button 2 press handling. */
static void DEMO_HandleButton1Press(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(DEMO_TIMER_TPM_ENABLE) && DEMO_TIMER_TPM_ENABLE
/*!
 * This interrupt routine checks for bus timeout and switches schedule table
 */
void DEMO_TPM_IRQHandler(void)
{
    uint32_t counterVal;

    /* If auto baud is enabled. */
    if (TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & DEMO_TPM_CH_IN_FLG)
    {
        TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, DEMO_TPM_CH_IN_FLG);
        capturedValue = TPM_GetCurrentTimerCount(DEMO_TPM_BASEADDR);
        if (LIN_DRV_AutoBaudCapture(DEMO_SLAVE_LPUART_INSTANCE) == LIN_SUCCESS)
        {
            TPM_DisableInterrupts(DEMO_TPM_BASEADDR, DEMO_TPM_CH_IN_FLG);
        }
    }

    /* Timer service for time out function. */
    if (TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & DEMO_TPM_CH_OUT_FLG)
    {
        static uint32_t interruptCount = 0UL;

        TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, DEMO_TPM_CH_OUT_FLG);
        counterVal                                             = (uint16_t)(DEMO_TPM_BASEADDR->CNT);
        (DEMO_TPM_BASEADDR->CONTROLS[DEMO_TPM_CH_OUT_NUM].CnV) = (uint16_t)(counterVal + DEMO_MODULO_VALUE);

        /* Timer Interrupt Handler. */
        lin_lld_timeout_service(LI0);
        /* Increase the interrupt count. */
        interruptCount++;

        /* Update the tick count every 50ms. */
        if (interruptCount >= 100)
        {
            /* Increase tick count */
            if (g_motorSelectionCmd == MOTOR1_SELECTION_INCREASE)
            {
                g_motorTickCount++;
            }
            /* Decrease tick count */
            else if (g_motorSelectionCmd == MOTOR1_SELECTION_DECREASE)
            {
                g_motorTickCount--;
            }
            interruptCount = 0U;
        }
    }

    /* if timer overflow flag */
    if (TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & kTPM_TimeOverflowFlag)
    {
        /* Clear interrupt flag.*/
        TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, kTPM_TimeOverflowFlag);
        /* Increment overflow count */
        timerOverflowInterruptCount++;
    }
    SDK_ISR_EXIT_BARRIER;
}

void timerGetTimeIntervalCallback0(uint32_t *ns)
{
    uint32_t currentOverflowCount = timerOverflowInterruptCount;

    /* check current CNT value */
    timerCounterValue[1] = (uint16_t)(DEMO_TPM_BASEADDR->CNT);
    /* calculate number of ns from current and previous count value */
    if (timerCounterValue[1] >= timerCounterValue[0])
    {
        /* Correction: Timer overflow interrupt may be delayed by other processes
         * if TOF is set, timer overflow occurred so increase the number of interrupt and clear a flag
         * */
        if (TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & kTPM_TimeOverflowFlag)
        {
            TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, kTPM_TimeOverflowFlag);
            currentOverflowCount++;
        }
        *ns = (uint32_t)(((uint32_t)((timerCounterValue[1] - timerCounterValue[0]) * DEMO_1TICK_DURATION_PS) / 1000U) +
                         (currentOverflowCount * DEMO_TIMER_COMPARE_VALUE));
    }
    else /* (timerCounterValue[1]<timerCounterValue[0]) */
    {
        *ns = ((uint32_t)(((DEMO_TIMER_COMPARE_VALUE - timerCounterValue[0] + timerCounterValue[1])) *
                          DEMO_1TICK_DURATION_PS) /
               1000U);
        if (TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & kTPM_TimeOverflowFlag)
        {
            TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, kTPM_TimeOverflowFlag);
            currentOverflowCount++;
        }
        if (currentOverflowCount > 0U)
        {
            *ns += (uint32_t)((currentOverflowCount - 1U) * DEMO_TIMER_COMPARE_VALUE);
        }
    }

    /* set current count value to previous count value */
    timerCounterValue[0] = timerCounterValue[1];
    /* clear timerOverflowInterruptCount mark */
    timerOverflowInterruptCount = 0u;
}
#endif

/* If button 1 was pressed, force the tick count above MOTOR1_STOP_VALUE causing the master to
 * send the STOP command. On every 5th press signal a system error event via event-triggered frame */
static void DEMO_HandleButton1Press(void)
{
    /* Set the tick count larger than stop value. */
    g_motorTickCount = g_motor1StopValue + 20U;

    g_btn1EventCount++;
    if (g_btn1EventCount >= SLAVE_BTN1_EVENT_THRESHOLD)
    {
        l_u8_wr_LI0_Motor1ErrorCode(MOTOR1_ERROR_CODE_SYS_EVENT);
        l_u8_wr_LI0_Motor1ErrorValue(g_btn1EventCount);
        g_btn1EventCount  = 0U;
        g_eventDataPending = true;
    }
}

#if (defined(DEMO_SW_USE_SEPARATE_HANDLER) && DEMO_SW_USE_SEPARATE_HANDLER)
void DEMO_SW2_IRQ_HANDLER(void)
{
    /* If button 2 was pressed, reset the tick count to a low value
     * and restart the increase cycle. */
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
        g_motorTickCount = 20U;
    }
    else
    {
        ;
    }
    SDK_ISR_EXIT_BARRIER;
}

void DEMO_SW3_IRQ_HANDLER(void)
{
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
        DEMO_HandleButton1Press();
    }
    else
    {
        ;
    }
    SDK_ISR_EXIT_BARRIER;
}
#else
void DEMO_SW_IRQ_HANDLER(void)
{
    if (GPIO_PortGetInterruptFlags(DEMO_BUTTON1_GPIO) & (1U << DEMO_BUTTON1_PIN))
    {
        /* Clear external interrupt flag. */
        GPIO_PortClearInterruptFlags(DEMO_BUTTON1_GPIO, 1U << DEMO_BUTTON1_PIN);

        DEMO_HandleButton1Press();
    }
    /* If button 2 was pressed, reset the tick count to a low value
     * and restart the increase cycle. */
    else if (GPIO_PortGetInterruptFlags(DEMO_BUTTON2_GPIO) & (1U << DEMO_BUTTON2_PIN))
    {
        /* Clear external interrupt flag. */
        GPIO_PortClearInterruptFlags(DEMO_BUTTON2_GPIO, 1U << DEMO_BUTTON2_PIN);
        g_motorTickCount = 20U;
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
    LIN_DRV_IRQHandler(DEMO_SLAVE_LPUART_INSTANCE);
    SDK_ISR_EXIT_BARRIER;
}

/* @brief LIN slave task.
*        This task will emulate a slave node to send temp data to the master node.
*        And according to the command from master, control the LED state.
 */
static void DEMO_SlaveTaskStart(void)
{
    /* Initialize LIN network interface */
    l_sys_init();
    l_ifc_init(LI0);
    /* Initialize the transport layer so the slave can handle diagnostic frames
     * (0x3C/0x3D). Without this the TL rx/tx queue data pointers remain NULL,
     * causing a fault when the master sends the first master-request frame. */
    ld_init(LI0);

    /* Set IRQ priority for LIN stack. */
    NVIC_SetPriority(DEMO_LIN_IRQn, DEMO_LIN_PRIO);

    /* Infinite loop */
    for (;;)
    {
        /* Check if information about the Motor1 Temp has been received */
        if (l_flg_tst_LI0_Motor1Selection_flag())
        {
            /* Clear this flag... */
            l_flg_clr_LI0_Motor1Selection_flag();

            /* Check for sporadic INC_STOP command encoded in bit 2 of the Motor1Control byte.
             * This bit is set by the master when Button 2 is pressed while the bus is awake. */
            if (g_lin_frame_data_buffer[LIN_LI0_Motor1Selection_BYTE_OFFSET] &
                (l_u8)MOTOR1_SELECTION_INC_STOP_BIT)
            {
                g_lin_frame_data_buffer[LIN_LI0_Motor1Selection_BYTE_OFFSET] &=
                    (l_u8)(~MOTOR1_SELECTION_INC_STOP_BIT);

                g_motor1StopValue += 10U;

                PRINTF("Sporadic frame received: MOTOR1_STOP_VALUE raised to %d\r\n",
                       (int)g_motor1StopValue);
            }

            /* Update the temp data. */
            l_u8_wr_LI0_Motor1Temp(g_motorTickCount);

            /* If the command from master is change. */
            if (g_motorSelectionCmd != l_u8_rd_LI0_Motor1Selection())
            {
                /* Store selection data */
                g_motorSelectionCmd = l_u8_rd_LI0_Motor1Selection();

                /* If the temp value from slave node is larger than stop value, stop slave temp count. */
                if (MOTOR1_SELECTION_STOP == g_motorSelectionCmd)
                {
                    /* Set the LED 3 on. */
                    DEMO_LED1_OFF();
                    DEMO_LED2_OFF();
                    DEMO_LED3_ON();

                    PRINTF(" -> LED 3 is ON!\r\n");
                }

                /* If value is larger than MAX value, set slave node to decrease the count. */
                else if (MOTOR1_SELECTION_DECREASE == g_motorSelectionCmd)
                {
                    /* Set the LED 1 on. */
                    DEMO_LED1_ON();
                    DEMO_LED2_OFF();
                    DEMO_LED3_OFF();

                    PRINTF(" -> LED 1 is ON!\r\n");
                }
                /* If temp value is less than the MIN value, set slave to increase the count. */
                else if (MOTOR1_SELECTION_INCREASE == g_motorSelectionCmd)
                {
                    /* Set the LED 2 on. */
                    DEMO_LED1_OFF();
                    DEMO_LED2_ON();
                    DEMO_LED3_OFF();

                    PRINTF(" -> LED 2 is ON!\r\n");
                }
                /* If value between MAX and MIN, keep the state. */
                else
                {
                }
            }
        }

        /* Check if master sent a READ_BY_IDENTIFIER diagnostic request.
         * The middleware processes the request and sends the response
         * automatically; this block gives the application visibility. */
        if (diag_get_flag(LI0, LI0_DIAGSRV_READ_BY_IDENTIFIER_ORDER) == 1U)
        {
            diag_clear_flag(LI0, LI0_DIAGSRV_READ_BY_IDENTIFIER_ORDER);
            PRINTF("Diagnostic: READ_BY_IDENTIFIER request handled.\r\n");
        }

        /* Check whether the ETF response for Motor1State_Event was just transmitted.
         * The stack sets the ETF frame flag after lin_update_tx_flags() completes. */
        if (l_flg_tst_LI0_EventTriggeredFrame_flag())
        {
            l_flg_clr_LI0_EventTriggeredFrame_flag();
            if (g_eventDataPending)
            {
                g_eventDataPending = false;
                PRINTF("Event-triggered frame sent: system error reported to master.\r\n");
                /* Zero the event bytes directly (not via the write macro) so the pending flag is
                 * not re-armed.  The collision resolver polls Motor1State_Event as an unconditional
                 * frame and the slave always answers; clearing here ensures that subsequent polls
                 * return 0x00/0x00, which the master ignores via its errCode != 0 guard. */
                g_lin_frame_data_buffer[LIN_LI0_Motor1ErrorCode_BYTE_OFFSET]  = 0x00U;
                g_lin_frame_data_buffer[LIN_LI0_Motor1ErrorValue_BYTE_OFFSET] = 0x00U;
            }
        }

        /* Reset the sporadic stop value when the bus goes to sleep so that it returns
         * to the default after a sleep/wake cycle. */
        if (l_ifc_read_status(LI0) & GO_TO_SLEEP_SET)
        {
            g_motor1StopValue = MOTOR1_STOP_VALUE_DEFAULT;
        }
    }
}

/*!
 * @brief Initialize a timer for LIN slave.
 * This function will initialize a timer for LIN stack network, the time period is 500us.
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
    DEMO_TPM_BASEADDR->MOD = 0xFFFF;
    /* Setup TPM output compare mode */
    TPM_SetupOutputCompare(DEMO_TPM_BASEADDR, DEMO_TPM_CH_OUT, kTPM_NoOutputSignal, DEMO_MODULO_VALUE);
    /* Setup TPM input capture mode - capture LPUART-RX falling edge */
    TPM_SetupInputCapture(DEMO_TPM_BASEADDR, DEMO_TPM_CH_IN, kTPM_RiseAndFallEdge);
    /* Enable interrupt on overflow */
    TPM_EnableInterrupts(DEMO_TPM_BASEADDR,
                         kTPM_TimeOverflowInterruptEnable | DEMO_TPM_CH_OUT_IRQ | DEMO_TPM_CH_IN_IRQ);
    /* Set the second highest priority */
    NVIC_SetPriority(DEMO_TPM_IRQn, DEMO_TIMER_PRIO);
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

    PRINTF("LIN slave demo start!\r\n");
    PRINTF("Slave will send data to master and receive command from master.\r\n");
    PRINTF("Slave will control the LED according to the command from master.\r\n");

    /* Initialize a timer for LIN cluster. */
    DEMO_TimerInit();

    /* Start LIN slave task. */
    DEMO_SlaveTaskStart();

    while (1)
    {
    }
}
