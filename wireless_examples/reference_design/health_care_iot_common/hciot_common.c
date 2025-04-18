/*
 * Copyright 2023-2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "fsl_iocon.h"
#include "gap_interface.h"
#include "hciot_common.h"
#include "fsl_common.h"
#include "fsl_power.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/** Structure holding peer specific information */
typedef struct
{
    bool used;
    bool connected;
    bleAddressType_t addressType;
    bleDeviceAddress_t address;
    deviceId_t deviceId;
    gapConnectionCallback_t connCallback;
} PeerCallbacks_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/** Array of peer-specific callbacks */
static PeerCallbacks_t s_peerCallbacks[gAppMaxConnections_c];
/** Generic connection callback */
static gapConnectionCallback_t s_advConnectionCallback = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * Searches the peer by address.
 *
 * @param addressType Address type
 * @param address Address
 * @return Pointer to the peer callbacks or NULL when not found
 */
static PeerCallbacks_t *GetCallbackByAddress(bleAddressType_t addressType, bleDeviceAddress_t address)
{
    for (uint32_t i = 0; i < ARRAY_SIZE(s_peerCallbacks); i++)
    {
        if ((s_peerCallbacks[i].used) && (s_peerCallbacks[i].addressType == addressType) &&
            (memcmp(s_peerCallbacks[i].address, address, sizeof(bleDeviceAddress_t)) == 0))
        {
            return s_peerCallbacks + i;
        }
    }
    return NULL;
}

/**
 * Searches the peer by device id.
 *
 * @param deviceId Device ID
 * @return Pointer to the peer callbacks or NULL when not found
 */
static PeerCallbacks_t *GetCallbackById(deviceId_t deviceId)
{
    for (uint32_t i = 0; i < ARRAY_SIZE(s_peerCallbacks); i++)
    {
        /* deviceId is only valid when the entry is in use and the device is connected */
        if ((s_peerCallbacks[i].used) && (s_peerCallbacks[i].connected) && (s_peerCallbacks[i].deviceId == deviceId))
        {
            return s_peerCallbacks + i;
        }
    }
    return NULL;
}

/**
 * Registers the peer.
 *
 * @param addressType Address type
 * @param address Address
 * @return Pointer to entry or NULL when no free entry could be found.
 */
static PeerCallbacks_t *RegisterPeer(bleAddressType_t addressType, bleDeviceAddress_t address)
{
    for (uint32_t i = 0; i < ARRAY_SIZE(s_peerCallbacks); i++)
    {
        if (!s_peerCallbacks[i].used)
        {
            s_peerCallbacks[i].connected   = false;
            s_peerCallbacks[i].used        = true;
            s_peerCallbacks[i].addressType = addressType;
            memcpy(s_peerCallbacks[i].address, address, sizeof(bleDeviceAddress_t));
            return s_peerCallbacks + i;
        }
    }
    return NULL;
}

/**
 * Generic connection callback.
 *
 * This function checks the peer specific connection callback, if known. Otherwise,
 * the generic connection callback is called.
 *
 * @param deviceId The ID assigned to the newly connected peer
 * @param pConnectionEvent Additional information about the connection
 */
static void ConnectionCallback(deviceId_t deviceId, gapConnectionEvent_t *pConnectionEvent)
{
    PeerCallbacks_t *callbacks = NULL;
    /* Has a new connection been established? */
    if (pConnectionEvent->eventType == gConnEvtConnected_c)
    {
        /* Yes, the device ID is not known yet, search peer by address */
        callbacks = GetCallbackByAddress(pConnectionEvent->eventData.connectedEvent.peerAddressType,
                                         pConnectionEvent->eventData.connectedEvent.peerAddress);
        if (callbacks != NULL)
        {
            callbacks->connected = true;
        }
    }
    else
    {
        /* No, if registered before, the device is known by ID */
        callbacks = GetCallbackById(deviceId);
    }

    /* Is the peer known? */
    if (callbacks != NULL)
    {
        /* Has the connection been lost? */
        if (pConnectionEvent->eventType == gConnEvtDisconnected_c)
        {
            /* Yes, unregister the peer */
            callbacks->used      = false;
            callbacks->connected = false;
        }
        callbacks->deviceId = deviceId;
        callbacks->connCallback(deviceId, pConnectionEvent);
    }
    /* No, is a generic connection callback registered? (should be at this point) */
    else if (s_advConnectionCallback)
    {
        /* Yes, call the generic callback */
        s_advConnectionCallback(deviceId, pConnectionEvent);
    }
}

/**
 * Initializes the used supply mode based on state of SUPPLY_MODE_SELECT_PIN.
 * @li Pin high (default): System will be configured to use DC/DC in buck mode (HV mode)
 * @li Pin low: System will be configured to use external regulated single supply mode
 */
void HCIOT_InitSupplyMode(void)
{
    status_t res;
    (void)res;
    bool supplyModeIsExternallyRegulated = false;
#ifdef SUPPLY_MODE_SELECT_PIN
    /* Read state of pin */
    supplyModeIsExternallyRegulated = (GPIO_PinRead(GPIO, 0, SUPPLY_MODE_SELECT_PIN) == 0);
    /* Disable pull again from pin to avoid power loss */
    const uint32_t pinConfig =
        (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_SLEW_STANDARD | IOCON_PIO_INV_DI | IOCON_DIGITAL_EN | IOCON_PIO_OPENDRAIN_DI);
    IOCON_PinMuxSet(IOCON, 0U, SUPPLY_MODE_SELECT_PIN, pinConfig);
#endif /*SUPPLY_MODE_SELECT_PIN*/

    /* Configure supply mode */
    if (supplyModeIsExternallyRegulated)
    {
        /* Configure the system for externally regulated supply mode */
        PRINTF("Supply mode set to: external regulated single supply\n");
        res = POWER_DCDC_SetSupplyMode(kDCDC_MODE_XR_SM_SS);
        assert_equal(res, kStatus_Success);
    }
    else
    {
        if (POWER_DCDC_GetSupplyMode() == kDCDC_MODE_HV_SM)
        {
            res = POWER_DCDC_SetSupplyMode(kDCDC_MODE_HV_SM);
            assert_equal(res, kStatus_Success);
            PRINTF("Supply mode set to: DC/DC HV supplied (buck)\n");
        }
        else if (POWER_DCDC_GetSupplyMode() == kDCDC_MODE_LV_SM)
        {
            res = POWER_DCDC_SetSupplyMode(kDCDC_MODE_LV_SM);
            assert_equal(res, kStatus_Success);
            PRINTF("Supply mode set to: DC/DC LV supplied (boost)\n");
        }
        else
        {
            assert(false);
        }
    }
}

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
bleResult_t HCIOT_GapConnect(gapConnectionRequestParameters_t *pParameters, gapConnectionCallback_t connCallback)
{
    PeerCallbacks_t *callbacks = GetCallbackByAddress(pParameters->peerAddressType, pParameters->peerAddress);

    /* Is the peer already known? */
    if (callbacks == NULL)
    {
        /* No, register the peer */
        callbacks = RegisterPeer(pParameters->peerAddressType, pParameters->peerAddress);
    }

    /* Could the callback be registered? */
    if (callbacks == NULL)
    {
        /* No, report out of memory */
        return gBleOutOfMemory_c;
    }

    callbacks->connCallback = connCallback;
    return Gap_Connect(pParameters, ConnectionCallback);
}

/**
 * Starts advertising and registers the given connection callback as the generic callback
 *
 * @param advertisingCallback The callback used to inform the user about advertising events
 * @param connectionCallback The callback used to inform the user about connection events
 * @return gBleSuccess_c in case of success
 */
bleResult_t HCIOT_GapStartAdvertising(gapAdvertisingCallback_t advertisingCallback,
                                       gapConnectionCallback_t connectionCallback)
{
    s_advConnectionCallback = connectionCallback;
    return Gap_StartAdvertising(advertisingCallback, ConnectionCallback);
}

/* Calculate the total contiguous size of a keys received message. */
uint32_t HCIOT_GapSmpKeysSize(gapSmpKeys_t *pKeys)
{
    uint32_t size = sizeof(gapSmpKeys_t);

    /* add room for LTK and Rand data */
    size += (pKeys->aLtk != NULL) ? (pKeys->cLtkSize + pKeys->cRandSize) : 0;

    /* add room for IRK data */
    size += (pKeys->aIrk != NULL) ? (gcSmpIrkSize_c + gcBleDeviceAddressSize_c) : 0;

    /*  add room for CSRK data */
    size += (pKeys->aCsrk != NULL) ? gcSmpCsrkSize_c : 0;

    /* add room for device address data */
    size += (pKeys->aAddress != NULL) ? gcBleDeviceAddressSize_c : 0;

    return size;
}

/* Copy from src to contiguous block in destination. */
void HCIOT_CopyGapSmpKeys(gapSmpKeys_t *dst, gapSmpKeys_t *src)
{
    typedef struct
    {
        gapSmpKeys_t SmpKeys;
        uint8_t data[0];
    } DstStruct_t;

    uint8_t *cursor = &((DstStruct_t *)dst)->data[0];

    memset(dst, 0x00, sizeof(gapSmpKeys_t));
    dst->cLtkSize    = src->cLtkSize;
    dst->cRandSize   = src->cRandSize;
    dst->ediv        = src->ediv;
    dst->addressType = src->addressType;

    if (src->aLtk)
    {
        dst->aLtk = cursor;
        memcpy(dst->aLtk, src->aLtk, src->cLtkSize);
        cursor += src->cLtkSize;

        dst->aRand = cursor;
        memcpy(dst->aRand, src->aRand, src->cRandSize);
        cursor += src->cRandSize;
    }

    if (src->aIrk)
    {
        dst->aIrk = cursor;
        memcpy(dst->aIrk, src->aIrk, gcSmpIrkSize_c);
        cursor += gcSmpIrkSize_c;

        dst->aAddress = cursor;
        memcpy(dst->aAddress, src->aAddress, gcBleDeviceAddressSize_c);
        cursor += gcBleDeviceAddressSize_c;
    }

    if (src->aCsrk)
    {
        dst->aCsrk = cursor;
        memcpy(dst->aCsrk, src->aCsrk, gcSmpCsrkSize_c);
    }
}

void HCIOT_CopyGapConnIqReport(connectionMsg_t *msg, gapConnIqReport_t *src)
{
    gapConnIqReport_t *dst = &msg->Event.eventData.connIqReport;
    int8_t *samples        = &msg->samples[0];

    memcpy(dst, src, sizeof(gapConnIqReport_t));

    dst->aI_samples = samples;
    memcpy(dst->aI_samples, src->aI_samples, src->sampleCount);
    samples += src->sampleCount;

    dst->aQ_samples = samples;
    memcpy(dst->aQ_samples, src->aQ_samples, src->sampleCount);
}
