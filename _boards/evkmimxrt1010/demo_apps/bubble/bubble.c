/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "math.h"
#include "fsl_gpt.h"
#include "fsl_fxos.h"
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
static void Board_UpdatePwm(uint16_t x);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile int16_t g_xAngle = 0;
volatile int16_t g_yAngle = 0;
volatile int16_t g_xDuty  = 0;
volatile int16_t g_yDuty  = 0;
/* FXOS device address */
const uint8_t g_accel_address[] = {0x1CU, 0x1DU, 0x1EU, 0x1FU};

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Initialize timer module */
static void Timer_Init(void)
{
    gpt_config_t gptConfig;

    GPT_GetDefaultConfig(&gptConfig);
    /* config to restart mode */
    gptConfig.enableFreeRun = false;
    /* Initialize GPT module */
    GPT_Init(TIMER_PERIPHERAL, &gptConfig);
    /* Set GPT output compare channel1 to freq 50kHz */
    GPT_SetOutputCompareValue(TIMER_PERIPHERAL, kGPT_OutputCompare_Channel1, TIMER_CLOCK_SOURCE / 50000);
    GPT_SetOutputCompareValue(TIMER_PERIPHERAL, kGPT_OutputCompare_Channel2, TIMER_CLOCK_SOURCE / 100000);
    /* Enable GPT Output Compare1 & Compare2 interrupt */
    GPT_EnableInterrupts(TIMER_PERIPHERAL, kGPT_OutputCompare1InterruptEnable | kGPT_OutputCompare2InterruptEnable);
    /* Enable at the Interrupt */
    EnableIRQ(TIMER_IRQ_ID);
    /* Start Timer */
    GPT_StartTimer(TIMER_PERIPHERAL);
}

void TIMER_IRQ_HANDLER(void)
{
    uint32_t status = GPT_GetStatusFlags(TIMER_PERIPHERAL, kGPT_OutputCompare1Flag);

    if (status & kGPT_OutputCompare1Flag)
    {
        USER_LED_ON();
        GPT_ClearStatusFlags(TIMER_PERIPHERAL, kGPT_OutputCompare1Flag);
    }

    status = GPT_GetStatusFlags(TIMER_PERIPHERAL, kGPT_OutputCompare2Flag);
    if (status & kGPT_OutputCompare2Flag)
    {
        USER_LED_OFF();
        Board_UpdatePwm(g_xDuty);
        GPT_ClearStatusFlags(TIMER_PERIPHERAL, kGPT_OutputCompare2Flag);
    }
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
    __DSB();
#endif
}

/* Update the duty cycle of an active pwm signal */
static void Board_UpdatePwm(uint16_t x)
{
    GPT_SetOutputCompareValue(TIMER_PERIPHERAL, TIMER_COMPARE_CHANNEL, TIMER_CLOCK_SOURCE / 50000 * x / 101);
}

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < EXAMPLE_DELAY_COUNT; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

int main(void)
{
    fxos_handle_t fxosHandle = {0};
    fxos_data_t sensorData   = {0};
    fxos_config_t config     = {0};
    status_t result;
    gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    uint8_t sensorRange          = 0;
    uint8_t dataScale            = 0;
    int16_t xData                = 0;
    int16_t yData                = 0;
    uint8_t i                    = 0;
    uint8_t array_addr_size      = 0;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Init output LED GPIO. */
    GPIO_PinInit(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, &led_config);
    /* Initialize LED pins below */
    USER_LED_INIT(LOGIC_LED_OFF);

    /* I2C initialize */
    BOARD_Accel_I2C_Init();
    /* Configure the I2C function */
    config.I2C_SendFunc    = BOARD_Accel_I2C_Send;
    config.I2C_ReceiveFunc = BOARD_Accel_I2C_Receive;

    /* Initialize sensor devices */
    array_addr_size = sizeof(g_accel_address) / sizeof(g_accel_address[0]);
    for (i = 0; i < array_addr_size; i++)
    {
        config.slaveAddress = g_accel_address[i];
        /* Initialize accelerometer sensor */
        result = FXOS_Init(&fxosHandle, &config);
        if (result == kStatus_Success)
        {
            break;
        }
    }

    if (result != kStatus_Success)
    {
        PRINTF("\r\nSensor device initialize failed!\r\n");
        PRINTF("\r\nPlease check the sensor chip U26\r\n");
        while (1)
        {
            delay();
            GPIO_PortToggle(EXAMPLE_LED_GPIO, 1u << EXAMPLE_LED_GPIO_PIN);
        }
    }

    /* Get sensor range */
    if (FXOS_ReadReg(&fxosHandle, XYZ_DATA_CFG_REG, &sensorRange, 1) != kStatus_Success)
    {
        return -1;
    }

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
    else
    {
    }

    USER_LED_INIT(0);

    /* Init timer */
    Timer_Init();

    /* Print a note to terminal */
    PRINTF("\r\nWelcome to the BUBBLE example\r\n");
    PRINTF("\r\nYou will see the LED brightness change when the angle of the board changes\r\n");

    /* Main loop. Get sensor data and update duty cycle */
    while (1)
    {
        /* Get new accelerometer data. */
        if (FXOS_ReadSensorData(&fxosHandle, &sensorData) != kStatus_Success)
        {
            return -1;
        }

        /* Get the X and Y data from the sensor data structure in 14 bit left format data*/
        xData = (int16_t)((uint16_t)((uint16_t)sensorData.accelXMSB << 8) | (uint16_t)sensorData.accelXLSB) / 4U;
        yData = (int16_t)((uint16_t)((uint16_t)sensorData.accelYMSB << 8) | (uint16_t)sensorData.accelYLSB) / 4U;

        /* Convert raw data to angle (normalize to 0-90 degrees). No negative angles. */
        g_xAngle = (int16_t)floor((double)xData * (double)dataScale * 90 / 8192);
        if (g_xAngle < 0)
        {
            g_xAngle *= -1;
        }
        g_yAngle = (int16_t)floor((double)yData * (double)dataScale * 90 / 8192);
        if (g_yAngle < 0)
        {
            g_yAngle *= -1;
        }
        g_xDuty = g_xAngle * 100 / 90;
        g_yDuty = g_yAngle * 100 / 90;
        /* Update duty cycle to turn on LEDs when angles ~ 90 */
        if (g_xAngle > ANGLE_UPPER_BOUND)
        {
            g_xDuty = 100;
        }
        if (g_yAngle > ANGLE_UPPER_BOUND)
        {
            g_yDuty = 100;
        }
        /* Update duty cycle to turn off LEDs when angles ~ 0 */
        if (g_xAngle < ANGLE_LOWER_BOUND)
        {
            g_xDuty = 0;
        }
        if (g_yAngle < ANGLE_LOWER_BOUND)
        {
            g_yDuty = 0;
        }
        /* Print out the angle data. */
        PRINTF("x= %2d y = %2d\r\n", g_xAngle, g_yAngle);
    }
}
