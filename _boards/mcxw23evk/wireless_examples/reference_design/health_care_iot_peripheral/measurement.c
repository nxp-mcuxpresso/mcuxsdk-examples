/*
 * Copyright 2022-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_pint.h"
#include "fsl_tmp117.h"
#include "health.h"
#include "connectivity.h"
#include "datastore.h"
#include "fwk_platform.h"
#include "pin_mux.h"
#include "fsl_inputmux.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_gpio.h"
#include "measurement.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/** The interval of the measurements in milliseconds */
#define MEASUREMENT_INTERVAL_MS 1000

/** The time it takes to perform one measurement in microseconds */
#define MEASUREMENT_TIME_US 16000

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/**
 * Function that is called when the measurement is done
 */
static void MeasurementDoneCb(pint_pin_int_t pintr, pint_status_t *pmatch_status);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/** TMP117 configuration */
static tmp117_config_t s_tmp117Config;

/** Holds the callback that must be called when a measurement is done */
static measurement_done_cb_t s_doneCb;

/** Holds the function that can be used to schedule idle tasks */
static measurement_schedule_idle_task_t s_scheduleCb;

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * Start the temperature measurement.
 */
static void StartMeasurement(void *taskHandle, void *context)
{
    /* Configure the temperature sensor alert signal interrupt */
    /* Note that this needs to be done each time as this configuration is lost in power down mode */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, SENSOR_PINT_INTERRUPT, SENSOR_INPUTMUX_INTERRUPT);
    INPUTMUX_Deinit(INPUTMUX);
    PINT_Init(PINT);
    PINT_PinInterruptConfig(PINT, SENSOR_PINT_INTERRUPT, kPINT_PinIntEnableFallEdge);
    PINT_SetCallback(PINT, MeasurementDoneCb);
    PINT_EnableCallbackByIndex(PINT, SENSOR_PINT_INTERRUPT);
    /* I2C must be reinitialized each time as this configuration is lost in power down mode */
    BOARD_InitI2c();
    TMP117_StartMeasurement(&s_tmp117Config, kTMP117_Averaging_None);
}

/**
 * Function that is called when the measurement is done
 */
static void MeasurementDoneCb(pint_pin_int_t pintr, pint_status_t *pmatch_status)
{
    s_doneCb();
}

/**
 * Init the temperature sensor.
 */
void MEASUREMENT_Init(measurement_done_cb_t doneCb, measurement_schedule_idle_task_t scheduleCb)
{
    s_doneCb     = doneCb;
    s_scheduleCb = scheduleCb;

    /* Configure temperature sensor driver */
    BOARD_InitI2c();
    TMP117_GetDefaultConfig(&s_tmp117Config);
    s_tmp117Config.assertAlert  = true;
    s_tmp117Config.i2cReadFunc  = BOARD_I2cReadFunc;
    s_tmp117Config.i2cWriteFunc = BOARD_I2cWriteFunc;
    TMP117_Init(&s_tmp117Config);
}
/**
 * Deinit the temperature sensor.
 */
void MEASUREMENT_DeInit(void)
{
}

/**
 * Returns the preferred measurement interval
 */
uint32_t MEASUREMENT_GetPreferredIntervalInMs()
{
    return MEASUREMENT_INTERVAL_MS;
}

/**
 * Instructs the measurement driver to initiate a new measurement
 */
void MEASUREMENT_InitiateMeasurement()
{
    s_scheduleCb(MEASUREMENT_TIME_US, StartMeasurement, NULL);
}

/**
 * Process the data resulting from temperature measurement.
 */
status_t MEASUREMENT_Process(void)
{
    int16_t temperature;
    uint8_t tmpStatus;
    status_t status = kStatus_Success;
    PINT_DisableCallbackByIndex(PINT, SENSOR_PINT_INTERRUPT);
    /* I2C must be reinitialized each time as this configuration is lost in power down mode */
    BOARD_InitI2c();
    TMP117_GetStatus(&s_tmp117Config, &tmpStatus);
    /* Check whether the interrupt is caused by the TMP117 */
    /* Note that this is not tested inside the ISR to avoid the TMP117 being accessed */
    /* from two different contexts (concurrency issues) */
    if (tmpStatus & kTMP117_Status_DataReady)
    {
        status = TMP117_GetTemperature(&s_tmp117Config, &temperature);
        if (status == kStatus_Success)
        {
            /* Convert temperature to centigrades */
            temperature = (int16_t)(temperature * 100 / 128);

            PRINTF("Measurement done @ %lld: %d.%d\n", PLATFORM_GetTimeStamp(), (int)(temperature / 100),
                   (int)((temperature % 100) / 10));
            DATASTORE_Add(&temperature);
            CONNECTIVITY_SetTemperature(temperature);
        }
        else
        {
            PRINTF("Failed fetching temperature\n");
        }
    }
    return status;
}
