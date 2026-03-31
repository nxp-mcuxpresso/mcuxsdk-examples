/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 #include "fsl_debug_console.h"
 #include "board.h"
 #include "math.h"
 #include "fsl_pit.h"
 #include "fsl_fxls.h"
 #include "app.h"
 
 /*******************************************************************************
  * Definitions
  ******************************************************************************/
 /* Upper bound and lower bound angle values */
 #define ANGLE_UPPER_BOUND 85U
 #define ANGLE_LOWER_BOUND 5U
 
 /*******************************************************************************
  * Prototypes
  ******************************************************************************/

 /*******************************************************************************
  * Variables
  ******************************************************************************/
 volatile int16_t g_xAngle = 0;
 volatile int16_t g_yAngle = 0;
 volatile int16_t g_xDuty  = 0;
 volatile int16_t g_yDuty  = 0;
 /* FXLS device address */
const uint8_t g_accel_address = 0x18U;
 
 /*******************************************************************************
  * Code
  ******************************************************************************/
static void Timer_Init(void)
{
    /* Structure of initialize PIT */
    pit_config_t pitConfig;
   
    /*
    * pitConfig.enableRunInDebug = false;
    */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(TIMER_BASEADDR, &pitConfig);

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(TIMER_BASEADDR, TIMER_CHANNEL, 100U);

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(TIMER_BASEADDR, TIMER_CHANNEL, kPIT_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(TIMER_IRQ_ID);

    PIT_StartTimer(TIMER_BASEADDR, TIMER_CHANNEL);
}
 
void TIMER_IRQ_HANDLER(void)
{
    static uint16_t cnt;
    uint32_t status = PIT_GetStatusFlags(TIMER_BASEADDR, TIMER_CHANNEL);

    if (status & kPIT_TimerFlag)
    {
        if (g_xDuty > cnt)
        {
            USER_LED_ON();
        }
        else
        {
            USER_LED_OFF();
        }

        cnt++;
        if (cnt > 99U)
        {
            cnt = 0U;
        }

        PIT_ClearStatusFlags(TIMER_BASEADDR, TIMER_CHANNEL, kPIT_TimerFlag);
    }

    SDK_ISR_EXIT_BARRIER;
}
 
 int main(void)
 {
     fxls_handle_t fxlsHandle    = {0};
     fxls_accel_data_t accelData = {0};
     fxls_config_t config        = {0};
     status_t result;
     uint8_t sensorRange = 0;
     uint8_t dataScale   = 0;
     uint32_t i          = 0;
     int16_t xDuty, yDuty;
 
     /* Board pin, clock, debug console init */
     BOARD_InitHardware();
     Timer_Init();

     /* Configure the I2C function */
     config.I2C_SendFunc    = BOARD_Accel_I2C_Send;
     config.I2C_ReceiveFunc = BOARD_Accel_I2C_Receive;
     config.slaveAddress    = g_accel_address;
     
     /* Initialize accelerometer sensor */
     result = FXLS_Init(&fxlsHandle, &config);
     
    if (result != kStatus_Success)
    {
        PIT_StopTimer(TIMER_BASEADDR, TIMER_CHANNEL);
        PRINTF("\r\nSensor device initialize failed!\r\n");
        PRINTF("\r\nPlease check the sensor chip U16\r\n");
        while (1)
        {
            for (i = 0; i < 30000000; i++)
            {
                __NOP();
            }
            USER_LED_TOGGLE();
        }
    }

    /* Get sensor range */
    if (FXLS_ReadReg(&fxlsHandle, SENS_CONFIG1_REG, &sensorRange, 1) != kStatus_Success)
    {
        return -1;
    }

    sensorRange = (sensorRange & 0x6) >> 1;

    if (sensorRange == 0x00)
    {
        dataScale = 2U;
    }
    else if (sensorRange == 0x01)
    {
        dataScale = 4U;
    }
    else if (sensorRange == 0x10)
    {
        dataScale = 8U;
    }
    else if (sensorRange == 0x11)
    {
        dataScale = 16U;
    }
    else
    {
    }

    /* Print a note to terminal */
    PRINTF("\r\nWelcome to the BUBBLE example\r\n");
    PRINTF("\r\nYou will see the LED brightness change when the angle of the board changes\r\n");

    /* Main loop. Get sensor data and update duty cycle */
    while (1)
    {
        /* Get new accelerometer data. */
        if (FXLS_ReadAccelData(&fxlsHandle, &accelData) != kStatus_Success)
        {
            return -1;
        }

        /* Convert raw data to angle (normalize to 0-90 degrees). No negative angles. */
        g_xAngle = (int16_t)floor((double)accelData.accelX * (double)dataScale * 90 / 2048);
        if (g_xAngle < 0)
        {
            g_xAngle *= -1;
        }
        g_yAngle = (int16_t)floor((double)accelData.accelY * (double)dataScale * 90 / 2048);
        if (g_yAngle < 0)
        {
            g_yAngle *= -1;
        }

        xDuty = g_xAngle * 100 / 90;
        yDuty = g_yAngle * 100 / 90;
        /* Update duty cycle to turn on LEDs when angles ~ 90 */
        if (g_xAngle > ANGLE_UPPER_BOUND)
        {
            xDuty = 100;
        }
        if (g_yAngle > ANGLE_UPPER_BOUND)
        {
            yDuty = 100;
        }
        /* Update duty cycle to turn off LEDs when angles ~ 0 */
        if (g_xAngle < ANGLE_LOWER_BOUND)
        {
            xDuty = 0;
        }
        if (g_yAngle < ANGLE_LOWER_BOUND)
        {
            yDuty = 0;
        }

        g_xDuty = xDuty;
        g_yDuty = yDuty;

        /* Print out the angle data. */
        PRINTF("x= %2d y = %2d\r\n", g_xAngle, g_yAngle);
    }
 }
 