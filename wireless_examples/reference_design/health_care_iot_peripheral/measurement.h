/*
 * Copyright 2022-2023,2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MEASUREMENT_H_
#define _MEASUREMENT_H_
#include "fsl_common.h"
#include "connectivity.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef void (*measurement_done_cb_t)(void);
typedef void (*measurement_schedule_idle_task_t)(uint32_t idleTime, connectivity_idle_task_t task, void *context);

/*******************************************************************************
 * API
 ******************************************************************************/
/**
 * Initializes the measurement driver.
 *
 * @param doneCb The callback that will be called each time the measurement is done
 * @param scheduleCb The callback used by the measurement driver to schedule idle tasks
 */
void MEASUREMENT_Init(measurement_done_cb_t doneCb, measurement_schedule_idle_task_t scheduleCb);

/**
 * Deinit the temperature sensor.
 */
void MEASUREMENT_DeInit(void);

/**
 * Returns the preferred measurement interval
 */
uint32_t MEASUREMENT_GetPreferredIntervalInMs();

/**
 * Instructs the measurement driver to initiate a new measurement
 */
void MEASUREMENT_InitiateMeasurement(void);

status_t MEASUREMENT_Process(void);
#endif /* _MEASUREMENT_H_ */
