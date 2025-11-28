/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "301/CO_driver.h"
#include "CO_app_NXP.h"

flexcan_handle_t flexcanHandle;
volatile bool txComplete = false;
volatile bool rxComplete = false;
volatile bool wakenUp    = false;
flexcan_mb_transfer_t txXfer[TX_MB_NUMBER], rxXfer[RX_MB_NUMBER];
flexcan_frame_t txFrame[TX_MB_NUMBER], rxFrame[RX_MB_NUMBER];

/* Local CAN module object */
static CO_CANmodule_t* CANModule_local = NULL; /* Local instance of global CAN module */

/* CAN masks for identifiers */
#define CANID_MASK 0x07FF /*!< CAN standard ID mask */
#define FLAG_RTR   0x8000 /*!< RTR flag, part of identifier */

static FLEXCAN_CALLBACK(flexcan_callback);

void CO_CANsetConfigurationMode(void* CANptr)
{
    /* Put CAN module in configuration mode */
    if (CANptr != NULL)
    {
        FLEXCAN_Enable(((CANopenNodeNXP *)CANptr)->CANHandle, false);
    }
}

void CO_CANsetNormalMode(CO_CANmodule_t* CANmodule)
{
    /* Put CAN module in normal mode */
    if (CANmodule->CANptr != NULL)
    {
        FLEXCAN_Enable(((CANopenNodeNXP *)CANmodule->CANptr)->CANHandle, true);
        CANmodule->CANnormal = true;
    }
}

/* Called by the CO_CANinit function in CANopen.c */
CO_ReturnError_t CO_CANmodule_init(CO_CANmodule_t *CANmodule, void *CANptr,
                                   CO_CANrx_t rxArray[], uint16_t rxSize,
                                   CO_CANtx_t txArray[], uint16_t txSize,
                                   uint16_t CANbitRate)
{
    uint8_t idx, mbIdx;

    /* verify arguments */
    if (CANmodule == NULL || rxArray == NULL || txArray == NULL)
    {
        return CO_ERROR_ILLEGAL_ARGUMENT;
    }

    /* Hold CANModule variable */
    CANmodule->CANptr = CANptr;

    /* Keep a local copy of CANModule */
    CANModule_local = CANmodule;

    /* Configure object variables */
    CANmodule->rxArray           = rxArray;
    CANmodule->rxSize            = rxSize;
    CANmodule->txArray           = txArray;
    CANmodule->txSize            = txSize;
    CANmodule->CANerrorStatus    = 0;
    CANmodule->CANnormal         = false;
    CANmodule->useCANrxFilters   = false; /* Do not use HW filters */
    CANmodule->bufferInhibitFlag = false;
    CANmodule->firstCANtxMessage = true;
    CANmodule->CANtxCount        = 0U;
    CANmodule->errOld            = 0U;

    /* Reset all variables */
    for (uint16_t i = 0U; i < rxSize; i++)
    {
        rxArray[i].ident          = 0U;
        rxArray[i].mask           = 0xFFFFU;
        rxArray[i].object         = NULL;
        rxArray[i].CANrx_callback = NULL;
    }
    for (uint16_t i = 0U; i < txSize; i++)
    {
        txArray[i].bufferFull = false;
    }

    /***************************************/
    /* NXP related configuration */
    /***************************************/
    ((CANopenNodeNXP *)CANptr)->HWInitFunction();

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(((CANopenNodeNXP *)CANptr)->CANHandle, &flexcanHandle,
                                 flexcan_callback, NULL);

    /* Enable notifications */
    /* Activate the CAN notification interrupts */
    for (idx = 0, mbIdx = RX_MB_FIRST_INDEX; idx < RX_MB_NUMBER; idx++, mbIdx++)
    {
        rxXfer[idx].mbIdx = mbIdx;
        rxXfer[idx].frame = &rxFrame[idx];
        (void)FLEXCAN_TransferReceiveNonBlocking(((CANopenNodeNXP *)CANptr)->CANHandle,
                                                 &flexcanHandle, &rxXfer[idx]);
    }

    return CO_ERROR_NO;
}

void CO_CANmodule_disable(CO_CANmodule_t* CANmodule)
{
    if (CANmodule != NULL && CANmodule->CANptr != NULL)
    {
        FLEXCAN_Enable(((CANopenNodeNXP *)CANmodule->CANptr)->CANHandle, false);
    }
}

CO_ReturnError_t CO_CANrxBufferInit(CO_CANmodule_t *CANmodule, uint16_t index,
                                    uint16_t ident, uint16_t mask, bool_t rtr,
                                    void *object,
                                    void (*CANrx_callback)(void *object, void *message))
{
    CO_ReturnError_t ret = CO_ERROR_NO;

    if (CANmodule != NULL && object != NULL && CANrx_callback != NULL && index < CANmodule->rxSize)
    {
        CO_CANrx_t *buffer = &CANmodule->rxArray[index];

        /* Configure object variables */
        buffer->object         = object;
        buffer->CANrx_callback = CANrx_callback;

        /*
         * Configure global identifier, including RTR bit
         *
         * This is later used for RX operation match case
         */
        buffer->ident = (ident & CANID_MASK) | (rtr ? FLAG_RTR : 0x00);
        buffer->mask  = (mask & CANID_MASK) | FLAG_RTR;

        /* Set CAN hardware module filter and mask. */
        if (CANmodule->useCANrxFilters)
        {
            __NOP();
        }
    }
    else
    {
        ret = CO_ERROR_ILLEGAL_ARGUMENT;
    }

    return ret;
}

CO_CANtx_t *CO_CANtxBufferInit(CO_CANmodule_t *CANmodule, uint16_t index,
                               uint16_t ident, bool_t rtr, uint8_t noOfBytes,
                               bool_t syncFlag)
{
    CO_CANtx_t *buffer = NULL;

    if (CANmodule != NULL && index < CANmodule->txSize)
    {
        buffer = &CANmodule->txArray[index];

        /* CAN identifier, DLC and rtr, bit aligned with CAN module transmit buffer */
        buffer->ident      = ((uint32_t)ident & CANID_MASK) | ((uint32_t)(rtr ? FLAG_RTR : 0x00));
        buffer->DLC        = noOfBytes;
        buffer->bufferFull = false;
        buffer->syncFlag   = syncFlag;
    }
    return buffer;
}

/**
 * \brief           Send CAN message to network
 * This function must be called with atomic access.
 *
 * \param[in]       CANmodule: CAN module instance
 * \param[in]       buffer: Pointer to buffer to transmit
 */
static status_t prv_send_can_message(CO_CANmodule_t* CANmodule, CO_CANtx_t* buffer)
{
    status_t status = kStatus_Success;
    flexcan_mb_transfer_t txXfer;
    flexcan_frame_t frame;

    frame.id     = FLEXCAN_ID_STD(buffer->ident & CANID_MASK);
    frame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
    frame.type   = (buffer->ident & FLAG_RTR) ? (uint8_t)kFLEXCAN_FrameTypeRemote:
                                                (uint8_t)kFLEXCAN_FrameTypeData;
    frame.length = buffer->DLC;

    switch (buffer->DLC)
    {
    case 8:
        frame.dataByte7 = buffer->data[7];
    case 7:
        frame.dataByte6 = buffer->data[6];
    case 6:
        frame.dataByte5 = buffer->data[5];
    case 5:
        frame.dataByte4 = buffer->data[4];
    case 4:
        frame.dataByte3 = buffer->data[3];
    case 3:
        frame.dataByte2 = buffer->data[2];
    case 2:
        frame.dataByte1 = buffer->data[1];
    case 1:
        frame.dataByte0 = buffer->data[0];
    default:
        break;
    }

    txXfer.frame = &frame;
    for (int i = 0; i < TX_MB_NUMBER; i++)
    {
        txXfer.mbIdx = i;
        status = FLEXCAN_TransferSendNonBlocking(((CANopenNodeNXP *)CANmodule->CANptr)->CANHandle,
                                                 &flexcanHandle, &txXfer);
        if (status == kStatus_Success)
        {
            break;
        }
    }

    return status;
}

CO_ReturnError_t CO_CANsend(CO_CANmodule_t *CANmodule, CO_CANtx_t *buffer)
{
    CO_ReturnError_t err = CO_ERROR_NO;

    /* Verify overflow */
    if (buffer->bufferFull)
    {
        if (!CANmodule->firstCANtxMessage)
        {
            /* don't set error, if bootup message is still on buffers */
            CANmodule->CANerrorStatus |= CO_CAN_ERRTX_OVERFLOW;
        }
        err = CO_ERROR_TX_OVERFLOW;
    }

    /*
     * Send message to CAN network
     *
     * Lock interrupts for atomic operation
     */
    CO_LOCK_CAN_SEND(CANmodule);
    if (prv_send_can_message(CANmodule, buffer) == kStatus_Success)
    {
        CANmodule->bufferInhibitFlag = buffer->syncFlag;
    }
    else
    {
        buffer->bufferFull = true;
        CANmodule->CANtxCount++;
    }
    CO_UNLOCK_CAN_SEND(CANmodule);

    return err;
}

void CO_CANclearPendingSyncPDOs(CO_CANmodule_t *CANmodule)
{
    uint32_t tpdoDeleted = 0U;

    CO_LOCK_CAN_SEND(CANmodule);
    /* Abort message from CAN module, if there is synchronous TPDO.
     * Take special care with this functionality. */
    if (CANmodule->bufferInhibitFlag)
    {
        /* clear TXREQ */
        CANmodule->bufferInhibitFlag = false;
        tpdoDeleted = 1U;
    }
    /* delete also pending synchronous TPDOs in TX buffers */
    if (CANmodule->CANtxCount > 0)
    {
        for (uint16_t i = CANmodule->txSize; i > 0U; --i)
        {
            if (CANmodule->txArray[i].bufferFull)
            {
                if (CANmodule->txArray[i].syncFlag)
                {
                    CANmodule->txArray[i].bufferFull = false;
                    CANmodule->CANtxCount--;
                    tpdoDeleted = 2U;
                }
            }
        }
    }
    CO_UNLOCK_CAN_SEND(CANmodule);
    if (tpdoDeleted)
    {
        CANmodule->CANerrorStatus |= CO_CAN_ERRTX_PDO_LATE;
    }
}

/* Get error counters from the module. If necessary, function may use
 * different way to determine errors.
 */
void CO_CANmodule_process(CO_CANmodule_t *CANmodule)
{
    uint64_t err = FLEXCAN_GetStatusFlags(((CANopenNodeNXP *)CANmodule->CANptr)->CANHandle);

    /* CANOpen just care about Bus_off, Warning, Passive and Overflow */
    if (CANmodule->errOld != err)
    {
        uint16_t status = CANmodule->CANerrorStatus;

        CANmodule->errOld = err;

        if (((err & (uint64_t)CAN_ESR1_FLTCONF_MASK) >> CAN_ESR1_FLTCONF_SHIFT) > 1)
        {
            status |= CO_CAN_ERRTX_BUS_OFF;
            /* This error will eventually handled automatically.
             * After counting the 128th occurrences of 11 consecutive recessive bits on the bus,
             * TXERRCNT is reset to zero.
             */
        }
        else
        {
            uint16_t mask = CO_CAN_ERRRX_OVERFLOW | CO_CAN_ERRRX_PASSIVE | CO_CAN_ERRRX_WARNING |
                            CO_CAN_ERRTX_BUS_OFF | CO_CAN_ERRTX_WARNING | CO_CAN_ERRTX_PASSIVE;
            /* recalculate CANerrorStatus, first clear some flags */
            status &= (uint16_t)(~mask);

            if (FLEXCAN_EFIFO_STATUS_UNMASK(err) & CAN_ERFSR_ERFOVF_MASK)
            {
                status |= CO_CAN_ERRRX_OVERFLOW;
            }

            if (((err & (uint64_t)CAN_ESR1_FLTCONF_MASK) >> CAN_ESR1_FLTCONF_SHIFT) == 1)
            {
                status |= CO_CAN_ERRRX_PASSIVE | CO_CAN_ERRTX_PASSIVE;
            }
            if (err & (uint64_t)CAN_ESR1_RXWRN_MASK)
            {
                status |= CO_CAN_ERRRX_WARNING;
            }

            if (err & (uint64_t)CAN_ESR1_TXWRN_MASK)
            {
                status |= CO_CAN_ERRTX_WARNING;
            }
        }

        CANmodule->CANerrorStatus = status;
    }
}

/**
 * \brief           Read message from RX FIFO
 * \param           base: pointer to an CAN_Type structure that contains
 *                        the configuration information for the specified CAN.
 * \param[in]       mbIdx: Message buffer number to use for read
 * \param[in]       fifo_isrs: List of interrupts for respected FIFO
 */
static void prv_read_can_received_msg(CAN_Type *base, uint8_t mbIdx, uint32_t fifo_isrs)
{

    CO_CANrxMsg_t rcvMsg;
    CO_CANrx_t* buffer = NULL; /* receive message buffer from CO_CANmodule_t object. */
    uint16_t index;            /* index of received message */
    uint32_t rcvMsgIdent;      /* identifier of the received message */
    uint8_t messageFound = 0;
    flexcan_frame_t *pRxFrame;

    if (mbIdx >= RX_MB_FIRST_INDEX)
    {
        pRxFrame = rxXfer[mbIdx - RX_MB_FIRST_INDEX].frame;
    }
    else
    {
        return;
    }
    /* Setup identifier (with RTR) and length */
    rcvMsg.ident = (pRxFrame->id >> CAN_ID_STD_SHIFT) | (pRxFrame->type == kFLEXCAN_FrameTypeRemote ? FLAG_RTR : 0x00);
    rcvMsg.dlc   = pRxFrame->length;
    rcvMsgIdent  = rcvMsg.ident;

    /* Read received message from FIFO */
    switch (rcvMsg.dlc)
    {
    case 8:
        rcvMsg.data[7] = pRxFrame->dataByte7;
    case 7:
        rcvMsg.data[6] = pRxFrame->dataByte6;
    case 6:
        rcvMsg.data[5] = pRxFrame->dataByte5;
    case 5:
        rcvMsg.data[4] = pRxFrame->dataByte4;
    case 4:
        rcvMsg.data[3] = pRxFrame->dataByte3;
    case 3:
        rcvMsg.data[2] = pRxFrame->dataByte2;
    case 2:
        rcvMsg.data[1] = pRxFrame->dataByte1;
    case 1:
        rcvMsg.data[0] = pRxFrame->dataByte0;
    default:
        break;
    }

    /*
     * Hardware filters are not used for the moment
     * \todo: Implement hardware filters...
     */
    if (CANModule_local->useCANrxFilters)
    {
        __BKPT(0);
    }
    else
    {
        /*
         * We are not using hardware filters, hence it is necessary
         * to manually match received message ID with all buffers
         */
        buffer = CANModule_local->rxArray;
        for (index = CANModule_local->rxSize; index > 0U; --index, ++buffer)
        {
            if (((rcvMsgIdent ^ buffer->ident) & buffer->mask) == 0U)
            {
                messageFound = 1;
                break;
            }
        }
    }

    /* Call specific function, which will process the message */
    if (messageFound && buffer != NULL && buffer->CANrx_callback != NULL)
    {
        buffer->CANrx_callback(buffer->object, (void*)&rcvMsg);
    }

    (void)FLEXCAN_TransferReceiveNonBlocking(base, &flexcanHandle, &rxXfer[mbIdx - RX_MB_FIRST_INDEX]);
}

/**
 * \brief           TX buffer has been well transmitted callback
 * \param[in]       hcan: pointer to an CAN_HandleTypeDef structure that contains
 *                      the configuration information for the specified CAN.
 * \param[in]       MailboxNumber: the mailbox number that has been transmitted
 */
void CO_CANinterrupt_TX(CO_CANmodule_t* CANmodule, uint32_t MailboxNumber)
{

    CANmodule->firstCANtxMessage = false;            /* First CAN message (bootup) was sent successfully */
    CANmodule->bufferInhibitFlag = false;            /* Clear flag from previous message */
    if (CANmodule->CANtxCount > 0U)                  /* Are there any new messages waiting to be send */
    {
        CO_CANtx_t* buffer = &CANmodule->txArray[0]; /* Start with first buffer handle */
        uint16_t i;

        /*
         * Try to send more buffers, process all empty ones
         *
         * This function is always called from interrupt,
         * however to make sure no preemption can happen, interrupts are anyway locked
         * (unless you can guarantee no higher priority interrupt will try to access to CAN instance and send data,
         *  then no need to lock interrupts..)
         */
        CO_LOCK_CAN_SEND(CANmodule);
        for (i = CANmodule->txSize; i > 0U; --i, ++buffer)
        {
            /* Try to send message */
            if (buffer->bufferFull)
            {
                if (prv_send_can_message(CANmodule, buffer))
                {
                    buffer->bufferFull = false;
                    CANmodule->CANtxCount--;
                    CANmodule->bufferInhibitFlag = buffer->syncFlag;
                }
            }
        }
        /* Clear counter if no more messages */
        if (i == 0U)
        {
            CANmodule->CANtxCount = 0U;
        }
        CO_UNLOCK_CAN_SEND(CANmodule);
    }
}

/*!
 * @brief FlexCAN Call Back function
 */
static FLEXCAN_CALLBACK(flexcan_callback)
{
    switch (status)
    {
        case kStatus_FLEXCAN_RxIdle:
            if ((RX_MB_FIRST_INDEX <= result) && (result <= RX_MB_LAST_INDEX))
            {
                rxComplete = true;
                prv_read_can_received_msg(base, (uint8_t)result, 0);
            }
            break;

        case kStatus_FLEXCAN_TxIdle:
#if TX_MB_FIRST_INDEX
            if ((TX_MB_FIRST_INDEX <= result) && (result <= TX_MB_LAST_INDEX))
#else
            if (result <= TX_MB_LAST_INDEX)
#endif
            {
                txComplete = true;
                CO_CANinterrupt_TX(CANModule_local, 0);
            }
            break;

        case kStatus_FLEXCAN_WakeUp:
            wakenUp = true;
            break;

        default:
            break;
    }
}

