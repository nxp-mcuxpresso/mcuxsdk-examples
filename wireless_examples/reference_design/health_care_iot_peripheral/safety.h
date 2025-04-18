/*
 * Copyright 2022-2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_H_
#define _SAFETY_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * The different states the device can be in
 */
typedef enum
{
    kSAFETY_DeviceState_Uninitialized,
    kSAFETY_DeviceState_Shelfmode,
    kSAFETY_DeviceState_Active
} safety_device_state_t;

/**
 * The different reset causes
 */
typedef enum
{
    kSAFETY_ResetCause_Por = 0,
    kSAFETY_ResetCause_Bod = 1
} safety_reset_cause_t;

/**
 * The different radio states that can be recorded
 */
typedef enum
{
    kSAFETY_RadioState_Off         = 0,
    kSAFETY_RadioState_Advertising = 1,
    kSAFETY_RadioState_Bonded      = 2,
    kSAFETY_RadioState_Connected   = 3
} safety_radio_state_t;

/** Callback type for written callback */
typedef void (*safety_written_cb_t)(uint8_t *data, uint32_t length, void *context);

/** Watchdog handle */
typedef uint32_t safety_watchdog_handle_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * Initializes the safety process
 */
void SAFETY_Init(void);

/**
 * Writes data to flash
 *
 * @param address The address to write the data to
 * @param data Pointer to the data to be written
 * @param length The length in bytes of the data
 * @param eraseFirst The containing sectors are erased first
 * @param writtenCb Function to be called when data is written
 */
void SAFETY_WriteFlash(
    uint32_t address, uint8_t *data, uint32_t length, bool eraseFirst, safety_written_cb_t writtenCb, void *context);

/**
 * Records the radio state
 *
 * @param radioState The radio state to record
 */
void SAFETY_RecordRadioState(safety_radio_state_t radioState);

/**
 * Starts the watchdog.
 * The watchdog will reset when not all users feed the dog within timeout clocks.
 *
 * @param timeout Number of milliseconds before the watchdog resets the system.
 * @return kstatus_Success in case of success.
 */
status_t SAFETY_WatchdogStart(uint32_t timeout);

/**
 * Register a user and retrieve a handle to be used when feeding the watchdog.
 *
 * @return Handle that can be used with SAFETY_WatchdogFeed.
 */
safety_watchdog_handle_t SAFETY_WatchdogRegisterUser(void);

/**
 * Feed the watchdog. When all users fed the dog the hardware watchdog
 * is reset.
 *
 * @param handle Handle of the user that wants to feed the watchdog.
 */
void SAFETY_WatchdogFeed(safety_watchdog_handle_t handle);

#endif /* _SAFETY_H_ */
