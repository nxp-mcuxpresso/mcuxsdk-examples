/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CO_DRIVER_TARGET_H
#define CO_DRIVER_TARGET_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "board.h"

// We don't need Storage option, implement based on your use case and remove this line from here
#undef CO_CONFIG_STORAGE_ENABLE
#define CO_CONFIG_NMT     (CO_CONFIG_NMT_MASTER | CO_CONFIG_NMT_CALLBACK_CHANGE | \
                           CO_CONFIG_GLOBAL_FLAG_CALLBACK_PRE | \
                           CO_CONFIG_GLOBAL_FLAG_TIMERNEXT)
#define CO_CONFIG_HB_CONS (CO_CONFIG_HB_CONS_ENABLE | \
                           CO_CONFIG_HB_CONS_CALLBACK_MULTI | \
                           CO_CONFIG_GLOBAL_FLAG_CALLBACK_PRE | \
                           CO_CONFIG_GLOBAL_FLAG_TIMERNEXT | \
                           CO_CONFIG_GLOBAL_FLAG_OD_DYNAMIC)
#define CO_CONFIG_FIFO    (CO_CONFIG_FIFO_ENABLE | CO_CONFIG_FIFO_ASCII_COMMANDS | \
                           CO_CONFIG_FIFO_ASCII_DATATYPES)
#define CO_CONFIG_SDO_CLI (CO_CONFIG_SDO_CLI_ENABLE | CO_CONFIG_FLAG_CALLBACK_PRE)
#define CO_CONFIG_GTW     (CO_CONFIG_GTW_ASCII | CO_CONFIG_GTW_ASCII_SDO | \
                           CO_CONFIG_GTW_ASCII_NMT)
#define CO_CONFIG_PDO     (CO_CONFIG_RPDO_ENABLE | \
                           CO_CONFIG_TPDO_ENABLE | \
                           CO_CONFIG_RPDO_TIMERS_ENABLE | \
                           CO_CONFIG_TPDO_TIMERS_ENABLE | \
                           CO_CONFIG_PDO_SYNC_ENABLE | \
                           CO_CONFIG_PDO_OD_IO_ACCESS | \
                           CO_CONFIG_FLAG_CALLBACK_PRE | \
                           CO_CONFIG_GLOBAL_RT_FLAG_CALLBACK_PRE | \
                           CO_CONFIG_GLOBAL_FLAG_TIMERNEXT | \
                           CO_CONFIG_GLOBAL_FLAG_OD_DYNAMIC)

/* Number of loops of #CO_SDOclientDownload() in case of block download */
#define CO_CONFIG_GTW_BLOCK_DL_LOOP 1
/* Size of command buffer in ASCII gateway object. */
#define CO_CONFIG_GTWA_COMM_BUF_SIZE 200

#ifdef __cplusplus
extern "C" {
#endif

/* Stack configuration override default values.
 * For more information see file CO_config.h. */

/* Basic definitions. If big endian, CO_SWAP_xx macros must swap bytes. */
#define CO_LITTLE_ENDIAN
#define CO_SWAP_16(x) x
#define CO_SWAP_32(x) x
#define CO_SWAP_64(x) x

/* NULL is defined in stddef.h */
/* true and false are defined in stdbool.h */
/* int8_t to uint64_t are defined in stdint.h */
typedef uint_fast8_t bool_t;
typedef float float32_t;
typedef double float64_t;

/**
 * \brief CAN RX message for platform
 *
 * This is platform specific one
 */
typedef struct {
    uint32_t ident;  /*!< Standard identifier */
    uint8_t dlc;     /*!< Data length */
    uint8_t data[8]; /*!< Received data */
} CO_CANrxMsg_t;

/* Access to received CAN message */
#define CO_CANrxMsg_readIdent(msg) ((uint16_t)(((CO_CANrxMsg_t*)(msg)))->ident)
#define CO_CANrxMsg_readDLC(msg)   ((uint8_t)(((CO_CANrxMsg_t*)(msg)))->dlc)
#define CO_CANrxMsg_readData(msg)  ((uint8_t*)(((CO_CANrxMsg_t*)(msg)))->data)

/* Received message object */
typedef struct {
    uint16_t ident;
    uint16_t mask;
    void* object;
    void (*CANrx_callback)(void* object, void* message);
} CO_CANrx_t;

/* Transmit message object */
typedef struct {
    uint32_t ident;
    uint8_t DLC;
    uint8_t data[8];
    volatile bool_t bufferFull;
    volatile bool_t syncFlag;
} CO_CANtx_t;

/* CAN module object */
typedef struct {
    void* CANptr;
    CO_CANrx_t* rxArray;
    uint16_t rxSize;
    CO_CANtx_t* txArray;
    uint16_t txSize;
    uint16_t CANerrorStatus;
    volatile bool_t CANnormal;
    volatile bool_t useCANrxFilters;
    volatile bool_t bufferInhibitFlag;
    volatile bool_t firstCANtxMessage;
    volatile uint16_t CANtxCount;
    uint64_t errOld;

    /* NXP specific features */
    uint32_t primask_send; /* Primask register for interrupts for send operation */
    uint32_t primask_emcy; /* Primask register for interrupts for emergency operation */
    uint32_t primask_od;   /* Primask register for interrupts for send operation */
} CO_CANmodule_t;

/* Data storage object for one entry */
typedef struct {
    void* addr;
    size_t len;
    uint8_t subIndexOD;
    uint8_t attr;
    /* Additional variables (target specific) */
    void* addrNV;
} CO_storage_entry_t;

/* (un)lock critical section in CO_CANsend() */
// Why disabling the whole Interrupt
#define CO_LOCK_CAN_SEND(CAN_MODULE)                                       \
    do {                                                                   \
        (CAN_MODULE)->primask_send = __get_PRIMASK();                      \
        __disable_irq();                                                   \
    } while (0)
#define CO_UNLOCK_CAN_SEND(CAN_MODULE) __set_PRIMASK((CAN_MODULE)->primask_send)

/* (un)lock critical section in CO_errorReport() or CO_errorReset() */
#define CO_LOCK_EMCY(CAN_MODULE)                                           \
    do {                                                                   \
        (CAN_MODULE)->primask_emcy = __get_PRIMASK();                      \
        __disable_irq();                                                   \
    } while (0)
#define CO_UNLOCK_EMCY(CAN_MODULE) __set_PRIMASK((CAN_MODULE)->primask_emcy)

/* (un)lock critical section when accessing Object Dictionary */
#define CO_LOCK_OD(CAN_MODULE)                                             \
    do {                                                                   \
        (CAN_MODULE)->primask_od = __get_PRIMASK();                        \
        __disable_irq();                                                   \
    } while (0)
#define CO_UNLOCK_OD(CAN_MODULE) __set_PRIMASK((CAN_MODULE)->primask_od)

/* Synchronization between CAN receive and message processing threads. */
#define CO_MemoryBarrier()
#define CO_FLAG_READ(rxNew) ((rxNew) != NULL)
#define CO_FLAG_SET(rxNew)                  \
    do {                                    \
        CO_MemoryBarrier();                 \
        rxNew = (void*)1L;                  \
    } while (0)
#define CO_FLAG_CLEAR(rxNew)                \
    do {                                    \
        CO_MemoryBarrier();                 \
        rxNew = NULL;                       \
    } while (0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CO_DRIVER_TARGET_H */
