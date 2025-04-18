/*
 * Copyright 2022-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_common.h"
#include "fsl_p3t1755.h"
#include "fsl_component_timer_manager.h"
#include "connectivity.h"
#include "fwk_platform.h"
#include "datastore.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "measurement.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/** The interval of the measurements in milliseconds */
#define MEASUREMENT_INTERVAL_MS 1000

/** The time it takes to perform one measurement in milliseconds.
 *  Maximum one-shot conversion time per specification is 12ms.
 */
#define MEASUREMENT_TIME_MS 12
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/**
 * Function that is called when the measurement is done
 */
static void MeasurementDoneCb(void *pParam);

/** Measurement timer handle */
static TIMER_MANAGER_HANDLE_DEFINE(s_conversionTimerHandle);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static p3t1755_config_t s_p3t1755Config;
static p3t1755_handle_t s_p3t1755Handle;

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
    timer_status_t timerStatus;
    status_t status;
    (void)timerStatus;
    (void)status;

    /* I2C must be reinitialized each time as this configuration is lost in power down mode */
    BOARD_InitI2c();
    /* Initiate one-shot measurement */
    status = P3T1755_OneShotMeasurement(&s_p3t1755Handle);
    assert_equal(status, kStatus_Success);
    timerStatus = TM_Start((timer_handle_t)s_conversionTimerHandle, kTimerModeLowPowerTimer | kTimerModeSingleShot,
                           MEASUREMENT_TIME_MS);
    assert_equal(timerStatus, kStatus_TimerSuccess);
}

/**
 * Function that is called when the measurement is done
 */
static void MeasurementDoneCb(void *pParam)
{
    s_doneCb();
}

/**
 * Init the temperature sensor.
 */
void MEASUREMENT_Init(measurement_done_cb_t doneCb, measurement_schedule_idle_task_t scheduleCb)
{
    timer_status_t timerStatus;
    status_t status;
    (void)timerStatus;
    (void)status;

    s_doneCb     = doneCb;
    s_scheduleCb = scheduleCb;

    /* Configure temperature sensor driver */
    BOARD_InitI2c();
    s_p3t1755Config.sensorAddress = 0x40U;
    s_p3t1755Config.readTransfer  = BOARD_I2cReadFunc;
    s_p3t1755Config.writeTransfer = BOARD_I2cWriteFunc;
    s_p3t1755Config.oneshotMode  = true;
    status                        = P3T1755_Init(&s_p3t1755Handle, &s_p3t1755Config);
    assert_equal(status, kStatus_Success);

    /* Create a timer for the measurements */
    timerStatus = TM_Open(s_conversionTimerHandle);
    assert_equal(timerStatus, kStatus_TimerSuccess);
    timerStatus = TM_InstallCallback((timer_handle_t)s_conversionTimerHandle, MeasurementDoneCb, NULL);
    assert_equal(timerStatus, kStatus_TimerSuccess);
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
    s_scheduleCb(MEASUREMENT_TIME_MS * 1000, StartMeasurement, NULL);
}

/**
 * Process the data resulting from temperature measurement.
 */
status_t MEASUREMENT_Process(void)
{
    status_t status = kStatus_Success;
    double temperature;
    /* I2C must be reinitialized each time as this configuration is lost in power down mode */
    BOARD_InitI2c();
    status = P3T1755_ReadTemperature(&s_p3t1755Handle, &temperature);
    if (status == kStatus_Success)
    {
        /* Convert temperature to centigrades */
        int temp_int = temperature * 100;
        PRINTF("Measurement done @ %lld: %d.%d\n", PLATFORM_GetTimeStamp(), (int)(temp_int / 100),
                       (int)((temp_int % 100) / 10));
        DATASTORE_Add(&temp_int);
        CONNECTIVITY_SetTemperature(temp_int);
    }
    else
    {
        PRINTF("Failed fetching temperature\n");
    }
    return status;
}
