/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "math.h"
#include "fsl_fxls.h"
#include "fsl_ctimer.h"
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
/* FXLS device address */
const uint8_t g_accel_address = 0x18U;
volatile uint32_t g_pwmPeriod   = 0U;
volatile uint32_t g_pulsePeriod = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t CTIMER_GetPwmPeriodValue(uint32_t pwmFreqHz, uint8_t dutyCyclePercent, uint32_t timerClock_Hz)
{
    /* Calculate PWM period match value */
    g_pwmPeriod = (timerClock_Hz / pwmFreqHz) - 1U;

    /* Calculate pulse width match value */
    g_pulsePeriod = (g_pwmPeriod + 1U) * (100 - dutyCyclePercent) / 100;

    return kStatus_Success;
}

status_t CTIMER_UpdatePwmPulsePeriodValue(uint8_t dutyCyclePercent)
{
    /* Calculate pulse width match value */
    g_pulsePeriod = (g_pwmPeriod + 1U) * (100 - dutyCyclePercent) / 100;

    return kStatus_Success;
}

/* Initialize timer module */
static void Timer_Init(void)
{   
    ctimer_config_t config;
    uint32_t srcClock_Hz;
    uint32_t timerClock;
    
    srcClock_Hz = BOARD_TIMER_SOURCE_CLOCK;
    
    CTIMER_GetDefaultConfig(&config);
    timerClock = srcClock_Hz / (config.prescale + 1);
    
    CTIMER_Init(BOARD_TIMER_BASEADDR, &config);
    
    /* Get the PWM period match value and pulse width match value of 20Khz PWM signal with 50% dutycycle */
    CTIMER_GetPwmPeriodValue(20000, 50, timerClock);
    CTIMER_SetupPwmPeriod(BOARD_TIMER_BASEADDR, kCTIMER_Match_1, BOARD_FIRST_TIMER_CHANNEL, g_pwmPeriod, g_pulsePeriod, false);
    CTIMER_SetupPwmPeriod(BOARD_TIMER_BASEADDR, kCTIMER_Match_1, BOARD_SECOND_TIMER_CHANNEL, g_pwmPeriod, g_pulsePeriod, false);
    CTIMER_StartTimer(BOARD_TIMER_BASEADDR);
}

/* Update the duty cycle of an active pwm signal */
static void Board_UpdatePwm(uint16_t x, uint16_t y)
{
    /* Updated duty cycle */
    CTIMER_UpdatePwmPulsePeriodValue((uint8_t)x);
    CTIMER_UpdatePwmPulsePeriod(BOARD_TIMER_BASEADDR, BOARD_FIRST_TIMER_CHANNEL, g_pulsePeriod);
    CTIMER_UpdatePwmPulsePeriodValue((uint8_t)y);
    CTIMER_UpdatePwmPulsePeriod(BOARD_TIMER_BASEADDR, BOARD_SECOND_TIMER_CHANNEL, g_pulsePeriod);
}

int main(void)
{
    fxls_handle_t fxlsHandle    = {0};
    fxls_accel_data_t accelData = {0};
    fxls_config_t config        = {0};
    status_t result;
    uint8_t sensorRange     = 0;
    uint8_t dataScale       = 0;
    int16_t xAngle          = 0;
    int16_t yAngle          = 0;
    int16_t xDuty           = 0;
    int16_t yDuty           = 0;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* I2C initialize */
    BOARD_Accel_I2C_Init();

    /* Configure the I2C function */
    config.I2C_SendFunc    = BOARD_Accel_I2C_Send;
    config.I2C_ReceiveFunc = BOARD_Accel_I2C_Receive;
    config.slaveAddress    = g_accel_address;

    /* Initialize sensor devices */
    result = FXLS_Init(&fxlsHandle, &config);

    if (result != kStatus_Success)
    {
        PRINTF("\r\nSensor device initialize failed!\r\n");
        return -1;
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

    /* Init timer */
    Timer_Init();

    /* Print a note to terminal */
    PRINTF("\r\nWelcome to the BUBBLE example\r\n");
    PRINTF("\r\nYou will see angle data change in the console when change the angles of board\r\n");

    /* Main loop. Get sensor data and update duty cycle */
    while (1)
    {
        /* Get new accelerometer data. */
        if (FXLS_ReadAccelData(&fxlsHandle, &accelData) != kStatus_Success)
        {
            return -1;
        }

        /* Convert raw data to angle (normalize to 0-90 degrees). No negative angles. */
        xAngle = (int16_t)floor((double)accelData.accelX * (double)dataScale * 90 / 2048);
        if (xAngle < 0)
        {
            xAngle *= -1;
        }

        yAngle = (int16_t)floor((double)accelData.accelY * (double)dataScale * 90 / 2048);
        if (yAngle < 0)
        {
            yAngle *= -1;
        }
        /* Update duty cycle to turn on LEDs when angles ~ 90 */
        if (xAngle > ANGLE_UPPER_BOUND)
        {
            xDuty = 100;
        }
        if (yAngle > ANGLE_UPPER_BOUND)
        {
            yDuty = 100;
        }
        /* Update duty cycle to turn off LEDs when angles ~ 0 */
        if (xAngle < ANGLE_LOWER_BOUND)
        {
            xDuty = 0;
        }
        if (yAngle < ANGLE_LOWER_BOUND)
        {
            yDuty = 0;
        }

        Board_UpdatePwm(xDuty, yDuty);

        /* Print out the angle data. */
        PRINTF("x= %2d y = %2d\r\n", xAngle, yAngle);
    }
}