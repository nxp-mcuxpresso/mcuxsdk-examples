/*
 * Copyright 2023-2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HCIOT_COMMON_H_
#define _HCIOT_COMMON_H_

#include <string.h>

#include "gap_types.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/** @brief Structure to copy the connection information data into.
 *
 * Usually, the GATT connection callback will allocate this structure
 * in a single contiguous memory block and copy the possibly dispersed
 * data into this structure for later handling.
 */
typedef struct connectionMsg_t
{
    deviceId_t peer;            /**< The peer identity associated with the connection event. */
    uint32_t datasize;          /**< Total size of the data portion. */
    gapConnectionEvent_t Event; /**< Received Connection Event. */
    union
    {
        gapSmpKeys_t gapSmpKeys[0]; /**< The gapSmpKeys_t struct when event type is gConnEvtKeysReceived_c. */
        uint8_t data[0];            /**< The key data when a gConnEvtKeysReceived_c type. */
        int8_t samples[0];          /**< The IQ sample data when gConnEvtIqReportReceived_c event. */
    }; /**< Anonymous union. */
} connectionMsg_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * Initializes the used supply mode based on state of SUPPLY_MODE_SELECT_PIN.
 * @li Pin high (default): System will be configured to use DC/DC in buck mode (HV mode)
 * @li Pin low: System will be configured to use external regulated single supply mode
 */
void HCIOT_InitSupplyMode(void);

/**
 * Registers the peer specific connection callback and starts the connection procedure.
 * When a connection could be established, the peer specific connection callback is called to inform the user.
 *
 * This function solves the issue of the host stack being able to only have one generic callback
 * registered by either Gap_Connect or Gap_StartAdvertising.
 *
 * @param pParameters Connection parameters
 * @param connCallback Peer specific connection callback
 * @return gBleSuccess_c in case of success.
 */
bleResult_t HCIOT_GapConnect(gapConnectionRequestParameters_t *pParameters, gapConnectionCallback_t connCallback);

/**
 * Starts advertising and registers the given connection callback as the generic callback
 *
 * @param advertisingCallback The callback used to inform the user about advertising events
 * @param connectionCallback The callback used to inform the user about connection events
 * @return gBleSuccess_c in case of success
 */
bleResult_t HCIOT_GapStartAdvertising(gapAdvertisingCallback_t advertisingCallback,
                                       gapConnectionCallback_t connectionCallback);

/**
 * @brief Calculate that size of the gapSmpKeys_t structure and the
 * key data.
 *
 * @param pKeys a reference to the gapSmpKeys_t structure of which the
 * calculate the total size requirements, including the key data.
 */
uint32_t HCIOT_GapSmpKeysSize(gapSmpKeys_t *pKeys);

/**
 * @brief Copy a gapSmpKeys_t structure and the associated key data to
 * a contiguous memory space.
 *
 * @param dst the contiguous region of space to copy the src data into. The
 * size of which can be calculated with gapSmpKeysSize().
 * @param src the reference to the gapSmpKeys_t structure of which to
 * copy the data and associated key data.
 */
void HCIOT_CopyGapSmpKeys(gapSmpKeys_t *dst, gapSmpKeys_t *src);

/**
 * @brief Copy a gapConnIqReport_t structure and the associated sample
 * data to the Event.eventData.connIqReport structure in a connectionMsg_t
 * structure. The sample data should be copied the the data space.
 *
 * @param dst the destination connectionMsg_t structure.
 * @param src the reference to the gapConnIqReport_t structure of which to
 * copy the data and associated samples.
 */
void HCIOT_CopyGapConnIqReport(connectionMsg_t *dst, gapConnIqReport_t *src);

#endif /* _HCIOT_COMMON_H_ */
