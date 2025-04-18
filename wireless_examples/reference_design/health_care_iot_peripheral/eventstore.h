/*
 * Copyright 2022,2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EVENTSTORE_H_
#define _EVENTSTORE_H_

#include "fsl_common.h"
#include "safety.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * Initializes the event store
 * The flashLocation must be sector aligned
 * The flashSize must be exactly 2 * FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES
 *
 * @param flashLocation The location in flash
 * @param flashSize The reserved space in flash
 * @return kStatus_Success on success
 */
status_t EVENTSTORE_Init(const uint8_t *flashLocation, uint32_t flashSize);

/**
 * Returns the last recorded device state
 *
 * @return The last recorded device state
 */
safety_device_state_t EVENTSTORE_GetLatestDeviceState(void);

/**
 * Records the given device state to the event store
 *
 * @param state The device state to record
 * @param cb Function called when new state is stored in flash
 * @return kStatus_Success upon success
 */
status_t EVENTSTORE_RecordDeviceState(safety_device_state_t state, safety_written_cb_t cb, void *context);

/**
 * Records the given radio state to the event store
 *
 * @param radioState The radio state
 */
status_t EVENTSTORE_RecordRadioState(safety_radio_state_t radioState);

/**
 * Records the reset cause
 *
 * @param radioState The radio state
 */
status_t EVENTSTORE_RecordResetCause(safety_reset_cause_t resetCause);

/**
 * Returns the offset of current time after synchronization with the client
 *
 * @return  offset of current time after synchronization
 */
uint32_t EVENTSTORE_GetCurrentTimeOffset(void);

/**
 * Records the user current time
 *
 * @param currentTime User current time to record
 */
status_t EVENTSTORE_RecordCurrentTime(uint32_t currentTime);

#endif /* _EVENTSTORE_H_ */
