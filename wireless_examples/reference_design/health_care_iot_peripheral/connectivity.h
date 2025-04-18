/*
 * Copyright 2022,2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CONNECTIVITY_H_
#define _CONNECTIVITY_H_

/**
 * # Idle tasks
 *
 * To reduce peak currents, this component offers features to schedule tasks
 * in between radio activities. This way the peak currents are spread in time and
 * do not coincide resulting in even higher peak currents.
 * CONNECTIVITY_ScheduleIdleTask can be used to register a function that is
 * called when there is no radio activity scheduled for the requested amount of
 * time. When there are multiple idle task scheduled, one task is executed at
 * a time.
 * Due to task scheduling or interrupts, the introduced delay may cause the
 * actual execution to overlap again with radio activity. The function
 * CONNECTIVITY_GetRadioIdleTime can be used to see whether the gap is still
 * sufficient for the task to execute.  If not, CONNECTIVITY_RescheduleIdleTask
 * will trigger the connectivity task to find another gap and call the
 * registered function again.
 * If the task has executed, the caller must call CONNECTIVITY_FinishIdleTask to
 * inform the connectivity task that it can delete the scheduled task and move
 * to the next queued scheduled task.
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * Idle task callback function definition
 */
typedef void (*connectivity_idle_task_t)(void *taskHandle, void *context);

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * Initializes the connectivity process.
 */
void CONNECTIVITY_Init(void);

bool CONNECTIVITY_IsActive(void);

/**
 * Sets the temperature
 *
 * @param temperature The new temperature expressed in hundredths of a degree
 */
void CONNECTIVITY_SetTemperature(int16_t temperature);

/**
 * Sets the battery level
 *
 * @param level The new battery level
 */
void CONNECTIVITY_SetBatteryLevel(uint8_t level);

/**
 * Returns the time till next radio activity in us or 0 when busy
 *
 * @return the radio idle time
 */
uint32_t CONNECTIVITY_GetRadioIdleTime();

/**
 * Schedules an idle task. The registered function will be called when the radio
 * is idle for at least idleTime.
 *
 * @param idleTime The minimum idle gap length in us
 * @param task The function to be called when the radio is idle for at least * idleTime
 * @param context Optional context pointer
 * @return Handle to the idle task
 */
void *CONNECTIVITY_ScheduleIdleTask(uint32_t idleTime, connectivity_idle_task_t task, void *context);

/**
 * Function to request the idle task to be scheduled again. This function must
 * be used when the idle gap is no longer large enough at the time the task was
 * about to execute. This can happen due to OS overhead and interrupts.
 *
 * @param Handle to the idle task that needs to be rescheduled
 */
void CONNECTIVITY_RescheduleIdleTask(void *idleTaskHandle);

/**
 * Called when the idle task is finished. This function will delete the idle
 * task and schedule the next queued idle task
 *
 * @param Handle to the idle task that has finished
 */
void CONNECTIVITY_FinishIdleTask(void *idleTaskHandle);

/**
 * Starts the BLE connectivity stack
 */
void CONNECTIVITY_StartBle(void);

#endif /* _CONNECTIVITY_H_ */
