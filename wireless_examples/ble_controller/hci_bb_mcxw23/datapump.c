/**
 * Copyright 2018-2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>
#include "fsl_ctimer.h"
#include "hci_uart.h"
#include "hci_transport.h"
#include "hci.h"
#include "event_manager.h"
#include "fsl_debug_console.h"
#include "ll_mem.h"
#include "hci_custom_cmds.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* List of custom HCI commands needed by datapump */
typedef enum
{
    HCI_OCF_DATAPUMP_ENABLE      = 0x1E,
    HCI_OCF_DATAPUMP_DISABLE     = 0x1F,
    HCI_OCF_DATAPUMP_TRANSMIT    = 0x20,
    HCI_OCF_DATAPUMP_RECEIVE     = 0x21,
    HCI_OCF_DATAPUMP_READ_REPORT = 0x22,
    HCI_OCF_DATAPUMP_END         = 0x23
} hci_custom_commands_data_pump_t;

#define MAX_ACL_DATA_BUFFER (8)
#define MAX_CONN            (4)
#define NUMBER_OF_COMPLETED_PACKETS_EVENT (0x13)
#define DISCONNECT_COMPLETE_EVENT         (0x5)
#define PAYLOAD_TYPE_RANDOM_LENGTH (0x1)
#define PAYLOAD_PATTERN_RANDOM_DATA (0x1)
#define PAYLOAD_FREQUENCY_PERIODIC (0x2)
#define MIN_REPORT_INTERVAL (0x1f4 * 1000)   // 500 ms
#define MAX_REPORT_INTERVAL (0x493E0 * 1000) // 5 minutes
#define MAX_TIMER_CHANNELS (4)
#define NO_TIMER_EVENT     (0)
#define HCI_ACL   (2)
#define HCI_EVENT (4)
#define HCI_HEADER_LENGTH (4)

#define DIFF(x, y) (x > y ? x - y : y - x)

/* Support of the little endian format */
#define READ_2_BYTES(pckt, pos) (((uint16_t)(pckt)[pos]) | (((uint16_t)(pckt)[(pos) + 1]) << 8))

#define READ_3_BYTES(pckt, pos) \
    (((uint32_t)(pckt)[pos]) | (((uint32_t)(pckt)[pos + 1]) << 8) | (((uint32_t)(pckt)[pos + 2]) << 16))

#define READ_4_BYTES(pckt, pos)                                                                           \
    (((uint32_t)(pckt)[pos]) | (((uint32_t)(pckt)[pos + 1]) << 8) | (((uint32_t)(pckt)[pos + 2]) << 16) | \
     (((uint32_t)(pckt)[pos + 3]) << 24))

/* Write bytes to memory macro like function */
#define WRITE_2_BYTES(pckt, pos, var)                                  \
    do                                                                 \
    {                                                                  \
        ((uint8_t *)pckt)[pos]     = (uint8_t)(var);                   \
        ((uint8_t *)pckt)[pos + 1] = (uint8_t)(((var) & 0xff00) >> 8); \
    } while (0)

#define WRITE_3_BYTES(pckt, pos, var)                        \
    do                                                       \
    {                                                        \
        ((uint8_t *)pckt)[pos]     = (uint8_t)(var);         \
        ((uint8_t *)pckt)[pos + 1] = (uint8_t)((var) >> 8);  \
        ((uint8_t *)pckt)[pos + 2] = (uint8_t)((var) >> 16); \
    } while (0)

#define WRITE_4_BYTES(pckt, pos, var)                        \
    do                                                       \
    {                                                        \
        ((uint8_t *)pckt)[pos]     = (uint8_t)(var);         \
        ((uint8_t *)pckt)[pos + 1] = (uint8_t)((var) >> 8);  \
        ((uint8_t *)pckt)[pos + 2] = (uint8_t)((var) >> 16); \
        ((uint8_t *)pckt)[pos + 3] = (uint8_t)((var) >> 24); \
    } while (0)

typedef struct /*!< Internal info we pack into the data range of the ACL packet. */
{
    uint32_t sequenceNumber; /*!< Sequence number; always incrementing. */
    uint32_t sentTimestamp;  /*!< Timestamp of submission (sender). */
} packet_info_t;

typedef struct
{
    uint8_t lc;                     /*!< Logical channel byte preceding the command */
    uint16_t connectionHandle : 12; /*!< Connection handle of a ACL packet */
    uint16_t PB_Flag : 2;           /*!< Packet boundary flag */
    uint16_t BC_Flag : 2;           /*!< Broadcast flag of a ACL packet */
    uint16_t dataTotalLength;       /*!< The total length of a ACL packet */
    uint8_t pad[2];                 /* To ensure proper alignment of packetInfo. */
    packet_info_t packetInfo;
    uint8_t data[251 - 2 - sizeof(packet_info_t)]; /*!< Array containing the data for an ACL packet */
} __attribute__((packed, aligned(4))) acl_packet_t;

typedef struct
{
    uint8_t lc;
    uint8_t eventCode;
    uint8_t length;
    uint8_t numHandles;
    uint16_t connHandle;
    uint16_t numCompletedPackets;
} completed_packets_t;

typedef struct
{
    uint8_t lc;
    uint8_t eventCode;
    uint8_t length;
    uint8_t status;
    uint16_t connHandle;
    uint8_t reason;
} disconnect_complete_t;

typedef enum
{
    kNotInUse       = 0, /* The entry is not being used. */
    kReceiveContext  = 1, /* The entry is being used as a receive_params_t type. */
    kTransmitContext = 2, /* The entry is being used as a transmit_params_t type. */
} context_type_t;

typedef struct
{
    uint8_t inUse; /* One of the context_type_t values. */
    uint16_t connHandle;
    uint8_t type;
    uint8_t pattern;
    uint8_t frequency;
    uint16_t length;
    uint32_t numPackets;
    uint32_t numPacketsSent;
    uint32_t lastTxTimeUs;
    uint16_t interval;
    uint16_t delay;
    uint8_t credits;
    uint8_t timerId;
    uint8_t stopTransmit;
} transmit_params_t;

typedef struct
{
    uint8_t inUse; /* One of the context_type_t values. */
    uint16_t connHandle;
    uint8_t timerId;
    uint32_t reportInterval; /* in milliseconds */
    uint32_t numPackets;
    uint32_t numberOfBytes; /* accumulated number of bytes received since first ACL packet or last report; used for
                               calculating throughput */
    uint32_t timestamp;     /* The timestamp is used to calculate the throughput, it is set when the first ACL packet is
                               received or a status report is generated; in microseconds */
    uint32_t expSequenceNumber; /* Expected sequence number. */
} receive_params_t;

/*
 * The available transmit and receive contexts are kept in a generic
 * array s_contexts below. As long as the contexts are not used, the inUse flag
 * is clear; as soon as they are allocated, the inUse flag resembles their function
 * and is determined by the context_type_t value.
 */
typedef union
{
    struct /* Header is similar to transmit_params_t and receive_params_t */
    {
      uint8_t inUse; /* One of the context_type_t values. */
      uint16_t connHandle;
    };
    transmit_params_t transmit;
    receive_params_t receive;
} datapump_context_t;

typedef enum
{
    kDATAPUMP_CTIMER_0 = 0,
    kDATAPUMP_CTIMER_1,
    kDATAPUMP_CTIMER_2,
    kDATAPUMP_CTIMER_MAX,
} datapump_ctimer_id_t;

typedef struct __attribute__((packed)) _datapump_transmit_parameters
{
    uint16_t connHandle;
    uint8_t type;
    uint8_t pattern;
    uint16_t length;
    uint8_t frequency;
    uint16_t interval;
    uint32_t numPackets;
    uint8_t credits;
} datapump_transmit_parameters_t;

typedef struct __attribute__((packed)) _datapump_receive_parameters
{
    uint16_t connHandle;
    uint32_t reportInterval;
} datapump_receive_parameters_t;

typedef struct __attribute__((packed)) _datapump_end_parameters
{
    uint16_t connHandle;
} datapump_end_parameters_t;

typedef struct __attribute__((packed)) _datapump_read_report_parameters
{
    uint16_t connHandle;
} datapump_read_report_parameters_t;

enum
{
    kDATAPUMP_Status_Success           = 0,
    kDATAPUMP_Status_Invalid_Param     = 1,
    kDATAPUMP_Status_Disallowed        = 2,
    kDATAPUMP_Status_Capacity_Exceeded = 3,
    kDATAPUMP_Status_Unknown_Conn_Id   = 4,
};

typedef struct __attribute__((packed))
{
    uint8_t status;
    uint16_t connHandle;
} general_status_datapump_t;

typedef struct __attribute__((packed))
{
    uint8_t status;
    uint16_t connHandle;
    uint32_t numPacketsTx;
    uint32_t numPacketsRx;
    uint32_t throughput;
} report_status_datapump_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Function called before main is entered */
int DatapumpPreMainInit(void) __attribute__((constructor));

static void DATAPUMP_TimerCb(uint32_t flags);

/* To please the timer callback interface with an extra indirection. */
static ctimer_callback_t DATAPUMP_TimerCbIndir[] = {DATAPUMP_TimerCb};

static ctimer_callback_t NoCallback[] = {NULL};

static ble_stat_t DatapumpCallEnable(uint8_t nbOfInParams,
                                     uint8_t *in_params,
                                     uint8_t *evnt_pckt_p,
                                     uint8_t *out_params_length,
                                     hci_return_command_type *return_command_type);
static ble_stat_t DatapumpCallDisable(uint8_t nbOfInParams,
                                      uint8_t *in_params,
                                      uint8_t *evnt_pckt_p,
                                      uint8_t *out_params_length,
                                      hci_return_command_type *return_command_type);
static ble_stat_t DatapumpCallTransmit(uint8_t nbOfInParams,
                                       uint8_t *in_params,
                                       uint8_t *evnt_pckt_p,
                                       uint8_t *out_params_length,
                                       hci_return_command_type *return_command_type);
static ble_stat_t DatapumpCallReceive(uint8_t nbOfInParams,
                                      uint8_t *in_params,
                                      uint8_t *evnt_pckt_p,
                                      uint8_t *out_params_length,
                                      hci_return_command_type *return_command_type);
static ble_stat_t DatapumpCallReadReport(uint8_t nbOfInParams,
                                         uint8_t *in_params,
                                         uint8_t *evnt_pckt_p,
                                         uint8_t *out_params_length,
                                         hci_return_command_type *return_command_type);
static ble_stat_t DatapumpCallEnd(uint8_t nbOfInParams,
                                  uint8_t *in_params,
                                  uint8_t *evnt_pckt_p,
                                  uint8_t *out_params_length,
                                  hci_return_command_type *return_command_type);
static void DatapumpInit(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static bool s_isInitialized;
static datapump_context_t s_contexts[MAX_CONN];
static acl_packet_t s_acl[MAX_ACL_DATA_BUFFER];
static int s_timerIds[MAX_TIMER_CHANNELS] = {
    0}; /* When a timer is in use, the associated s_timerIds[] slot is non zero. */
static uint32_t s_txTimerReferences;
static uint8_t s_dataPattern[MAX_ACL_DATA_BUFFER];
static uint32_t s_aclDataQueue;
static uint8_t s_txTimerId = 255;

static const datapump_ctimer_id_t s_cClockId   = 2; /* Used as a microsecond clock. */
static const datapump_ctimer_id_t s_ctimerIdTx = 1;

static CTIMER_Type *s_ctimers[kDATAPUMP_CTIMER_MAX] = {CTIMER0, CTIMER1,
                                                       CTIMER2}; /* Note that s_ctimers[0] is nowhere used. */

/* Custom commands related to Radio Test Suite */
static const CustomHciCmd customHCIOperations_Datapump[] = {
    {.opcode = GET_OP_CODE(HCI_OCF_DATAPUMP_ENABLE, HCI_OGF_EXT), .nbOfInParams = 0, .func = DatapumpCallEnable},
    {.opcode = GET_OP_CODE(HCI_OCF_DATAPUMP_DISABLE, HCI_OGF_EXT), .nbOfInParams = 0, .func = DatapumpCallDisable},
    {.opcode          = GET_OP_CODE(HCI_OCF_DATAPUMP_TRANSMIT, HCI_OGF_EXT),
     .nbOfInParams = sizeof(datapump_transmit_parameters_t),
     .func            = DatapumpCallTransmit},
    {.opcode          = GET_OP_CODE(HCI_OCF_DATAPUMP_RECEIVE, HCI_OGF_EXT),
     .nbOfInParams = sizeof(datapump_receive_parameters_t),
     .func            = DatapumpCallReceive},
    {.opcode          = GET_OP_CODE(HCI_OCF_DATAPUMP_READ_REPORT, HCI_OGF_EXT),
     .nbOfInParams = sizeof(datapump_read_report_parameters_t),
     .func            = DatapumpCallReadReport},
    {.opcode          = GET_OP_CODE(HCI_OCF_DATAPUMP_END, HCI_OGF_EXT),
     .nbOfInParams = sizeof(datapump_end_parameters_t),
     .func            = DatapumpCallEnd}};

/*******************************************************************************
 * Code
 ******************************************************************************/

static void ProgramTimer(datapump_ctimer_id_t ctimerId, ctimer_match_t matchId, uint32_t usec)
{
    CTIMER_Type *base     = s_ctimers[ctimerId];
    uint32_t clockFreqHz  = CLOCK_GetCTimerClkFreq(ctimerId);
    uint32_t count        = USEC_TO_COUNT(usec, clockFreqHz);
    uint32_t currentCount = CTIMER_GetTimerCountValue(base);
    ctimer_match_config_t timerMatchConfig = {.matchValue         = currentCount + count,
                                              .enableCounterReset = false,
                                              .enableCounterStop  = false,
                                              .outControl         = kCTIMER_Output_NoAction,
                                              .outPinInitState    = false,
                                              .enableInterrupt    = true};
    CTIMER_SetupMatch(base, matchId, &timerMatchConfig);
}

static void DATAPUMP_ProgramTxTimer(void)
{
    /*
     * 2 * min connection interval.
     * Todo timer stops firing when using 1250
     * 2500 offers good granularity for all the current tests
     */
    static const uint32_t txTimerUs = 2500;
    assert(s_txTimerId <= MAX_TIMER_CHANNELS);
    ProgramTimer(s_ctimerIdTx, s_txTimerId, txTimerUs);
}

static transmit_params_t *GetTransmitContext(uint16_t connHandle)
{
    for (int i = 0; i < MAX_CONN; i++)
    {
        if (s_contexts[i].inUse == kTransmitContext && (s_contexts[i].transmit.connHandle == connHandle))
        {
            return &s_contexts[i].transmit;
        }
    }
    return NULL;
}

static receive_params_t *GetReceiveContext(uint16_t connHandle)
{
    for (int i = 0; i < MAX_CONN; i++)
    {
        if (s_contexts[i].inUse == kReceiveContext && (s_contexts[i].receive.connHandle == connHandle))
        {
            return &s_contexts[i].receive;
        }
    }
    return NULL;
}

static receive_params_t *GetReceiveContextFromTimerId(uint8_t timerId)
{
    for (int i = 0; i < MAX_CONN; i++)
    {
        if (s_contexts[i].inUse == kReceiveContext && (s_contexts[i].receive.timerId == timerId))
        {
            return &s_contexts[i].receive;
        }
    }
    return NULL;
}

static status_t DATAPUMP_AllocateTimerId(uint8_t *timerId)
{
    status_t status = kStatus_Fail;
    for (int i = 0; i < MAX_TIMER_CHANNELS; i++)
    {
        if (!s_timerIds[i]) /* Zero value for s_timerIds[x] means it is not in use. */
        {
            *timerId      = i;
            s_timerIds[i] = 1; /* Mark it in use. */
            status        = kStatus_Success;
            break;
        }
    }
    return status;
}

static uint32_t DATAPUMP_GetTimeUs(void)
{
    CTIMER_Type *base    = s_ctimers[s_cClockId];
    uint32_t clockFreqHz = CLOCK_GetCTimerClkFreq(s_cClockId);
    return COUNT_TO_USEC(CTIMER_GetTimerCountValue(base), clockFreqHz);
}

static void DATAPUMP_FreeTimer(uint8_t timerId)
{
    s_timerIds[timerId]                    = 0; /* Mark no longer in use. */
    ctimer_match_config_t timerMatchConfig = {NO_TIMER_EVENT, false, false, kCTIMER_Output_NoAction, false, false};
    CTIMER_Type *timer                     = s_ctimers[s_cClockId];

    CTIMER_RegisterCallBack(timer, &DATAPUMP_TimerCbIndir[0], kCTIMER_SingleCallback);
    CTIMER_SetupMatch(timer, timerId, &timerMatchConfig);
}

static void DATAPUMP_TimerInit(datapump_ctimer_id_t ctimerId)
{
    ctimer_config_t timerConfig;
    ctimer_match_config_t timerMatchConfig = {NO_TIMER_EVENT, false, false, kCTIMER_Output_NoAction, false, false};
    CTIMER_Type *base                      = s_ctimers[ctimerId];

    /* Set up timer */
    CTIMER_GetDefaultConfig(&timerConfig);

    CTIMER_Init(base, &timerConfig);

    CTIMER_RegisterCallBack(base, &NoCallback[0], kCTIMER_SingleCallback);

    CTIMER_SetupMatch(base, kCTIMER_Match_0, &timerMatchConfig);

    /* Start timer */
    CTIMER_StartTimer(base);
}

static void DATAPUMP_TimerDeInit(datapump_ctimer_id_t ctimerId)
{
    CTIMER_Deinit(s_ctimers[ctimerId]);
}

static void DATAPUMP_End(uint16_t connHandle)
{
    transmit_params_t *transmitDatapump = GetTransmitContext(connHandle);
    receive_params_t *receiveDatapump   = GetReceiveContext(connHandle);

    if (receiveDatapump != NULL)
    {
        if (receiveDatapump->reportInterval != 0)
        {
            DATAPUMP_FreeTimer(receiveDatapump->timerId);
        }
        memset(receiveDatapump, 0, sizeof(receive_params_t));
    }

    if (transmitDatapump != NULL)
    {
        if (s_txTimerReferences > 0)
        {
            if (--s_txTimerReferences == 0)
            {
                DATAPUMP_FreeTimer(s_txTimerId);
                s_txTimerId = 255;
            }
        }

        memset(transmitDatapump, 0, sizeof(transmit_params_t));
    }
}

static void DATAPUMP_UnRegisterTapHciPacketCallback()
{
    ll_uart_unregister_write_prefilter();
}

static void DATAPUMP_DisableEvent()
{
    DATAPUMP_UnRegisterTapHciPacketCallback();
    DATAPUMP_TimerDeInit(s_ctimerIdTx);
    DATAPUMP_TimerDeInit(s_cClockId);
}

static void Mem_Copy_Per_Byte(uint8_t *dst, uint8_t *src, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        dst[i] = src[i];
    }
}

static void DATAPUMP_Transmit(transmit_params_t *transmit)
{
    if (transmit->stopTransmit == false)
    {
        while (transmit->credits > 0 && (0 == transmit->numPackets /* Some tests don't set numPackets. */
                                        ||
                                        transmit->numPacketsSent < transmit->numPackets))
        {
            /* The following is a hack to work around the compiler warning of taking the address out of an unaligned
             * structure. */
            packet_info_t *info                    = (packet_info_t *)(void *)&s_acl[s_aclDataQueue].pad[2];
            s_acl[s_aclDataQueue].connectionHandle = transmit->connHandle;
            s_acl[s_aclDataQueue].dataTotalLength  = transmit->length;
            info->sentTimestamp                    = DATAPUMP_GetTimeUs();
            info->sequenceNumber                   = transmit->numPacketsSent;
            ble_buff_hdr_t *ble_buff_hdr           = (ble_buff_hdr_t *)hci_alloc_msg();
            assert(ble_buff_hdr != NULL);
            ble_buff_hdr->data_size  = sizeof(s_acl[0]) - sizeof(s_acl[0].lc);
            ble_buff_hdr->buff_start = ll_intf_alloc_ll_pkt();
            if (ble_buff_hdr->buff_start == NULL)
            {
                PRINTF("Could not allocate Cmd Buffer\n");
                assert(false);
            }

            Mem_Copy_Per_Byte(ble_buff_hdr->buff_start, (uint8_t *)&s_acl[s_aclDataQueue] + sizeof(s_acl[0].lc),
                              transmit->length + HCI_HEADER_LENGTH);

            ble_buff_hdr->ble_hdr_flags = BLE_BUFF_HDR_ACL_DATA_PCKT;

            int ret = emngr_post_event(HCI_HANDLER, ble_buff_hdr);
#ifdef DEBUG_DATAPUMP
            GPIO->NOT[0] = 1U << 20;
#endif
            if (ret != 0)
            {
                if (ret == HANDLE_IS_FULL)
                {
                    PRINTF("Handle is full\n");
                    assert(false);
                }
                else
                {
                    PRINTF("Unknown error\n");
                    assert(false);
                }
            }
            else
            {
                transmit->credits -= 1; /* Only decrement when successful. */
                transmit->numPacketsSent += 1;
#ifdef DEBUG_DATAPUMP
                uint32_t now = DATAPUMP_GetTimeUs();
                uint32_t usPerTx = transmit->lastTxTimeUs ? now - transmit->lastTxTimeUs : 0;
                PRINTF("%6u %8u Sent packet for handle %u: %u of %u.\n", usPerTx, now, transmit->connHandle, transmit->numPacketsSent, transmit->numPackets);
                transmit->lastTxTimeUs = now;
#endif
            }

            s_aclDataQueue = (s_aclDataQueue + 1) % MAX_ACL_DATA_BUFFER;
        }
    }
}

static void DATAPUMP_GenerateReport(uint16_t connHandle, report_status_datapump_t *status)
{
    status->status                      = kDATAPUMP_Status_Unknown_Conn_Id;
    status->numPacketsRx                = 0;
    status->numPacketsTx                = 0;
    status->throughput                  = 0;
    status->connHandle                  = connHandle;
    receive_params_t *receiveDatapump   = GetReceiveContext(connHandle);
    transmit_params_t *transmitDatapump = GetTransmitContext(connHandle);
    if (receiveDatapump != NULL)
    {
        status->status       = kDATAPUMP_Status_Success;
        status->numPacketsRx = receiveDatapump->numPackets;
        uint32_t currentTime = DATAPUMP_GetTimeUs();
        uint32_t diff        = DIFF(currentTime, receiveDatapump->timestamp);

        // Throughput is expressed in bits/second; notice that diff is expressed in microseconds.
        // formula:         (bytes*8)/ (diff/1e6)
        // using integers:  (bytes*8) *1e6 / diff
        // As this gives an overflow of a 32-bit number, we divide both numerator and denominator with 512:
        // bytes * (8 * 1e6 / 512) / (diff/512)

        status->throughput = (receiveDatapump->numberOfBytes * 15625) / (diff >> 9);

        receiveDatapump->timestamp     = DATAPUMP_GetTimeUs();
        receiveDatapump->numberOfBytes = 0;
    }
    if (transmitDatapump != NULL)
    {
        status->status       = kDATAPUMP_Status_Success;
        status->numPacketsTx = transmitDatapump->numPacketsSent;
    }
}

static void DATAPUMP_SendAutonomousReport(uint16_t connHandle)
{
    report_status_datapump_t report;

    DATAPUMP_GenerateReport(connHandle, &report);

    uint8_t *evnt_pckt_p = os_mem_pool_alloc(os_Pool(hci_buffer_t));
    assert(evnt_pckt_p != NULL);

    ble_buff_hdr_t *ptr_buff_hdr = (ble_buff_hdr_t *)hci_alloc_msg();
    assert(ptr_buff_hdr != NULL);

    ptr_buff_hdr->buff_start    = evnt_pckt_p;
    ptr_buff_hdr->data_size     = sizeof(report) + 2; // content size + evnt_code + total len
    ptr_buff_hdr->data_offset   = 0;
    ptr_buff_hdr->ble_hdr_flags = BLE_BUFF_HDR_EVNT_CMD_PCKT;

    evnt_pckt_p[0] = 0xFE;           // custom evnt_code
    evnt_pckt_p[1] = sizeof(report); // Parameter total length for the parameters contained in the HCI event packet
    evnt_pckt_p[2] = report.status;
    WRITE_2_BYTES(evnt_pckt_p, 3, report.connHandle);
    WRITE_4_BYTES(evnt_pckt_p, 5, report.numPacketsTx);
    WRITE_4_BYTES(evnt_pckt_p, 9, report.numPacketsRx);
    WRITE_4_BYTES(evnt_pckt_p, 13, report.throughput);

    hci_transport_send_pckt(ptr_buff_hdr);
}

static bool DATAPUMP_ProcessAclPacket(const acl_packet_t *aclPacket)
{
    bool ret              = true;
    receive_params_t *rxp = GetReceiveContext(aclPacket->connectionHandle);

    if (rxp == NULL)
    {
        ret = false;
    }
    else
    {
        rxp->numPackets += 1;

        if (0 == rxp->timestamp)
        {
            rxp->timestamp = DATAPUMP_GetTimeUs();
        }

        rxp->numberOfBytes += aclPacket->dataTotalLength;

        packet_info_t Info;

        memcpy(&Info, &aclPacket->pad[2],
               sizeof(packet_info_t)); /* To overcome alignment issues, bytecopy into local struct. */

        if (Info.sequenceNumber != rxp->expSequenceNumber)
        {
            PRINTF("ERROR handle %u expected %u but got %u\n", rxp->connHandle, rxp->expSequenceNumber, Info.sequenceNumber);
            /* TODO message this to the Bluekit2 framework? */
            assert(false);
        }

        rxp->expSequenceNumber++;
    }
    return ret;
}

static bool DATAPUMP_ProcessCompletedPackets(const completed_packets_t *completedPackets)
{
    bool ret                            = true;
    transmit_params_t *transmitDatapump = GetTransmitContext(completedPackets->connHandle);

    if (transmitDatapump == NULL)
    {
        ret = false;
    }
    else
    {
#ifdef DEBUG_DATAPUMP
        for (int i = 0; i < completedPackets->numCompletedPackets; i++)
        {
            volatile uint8_t j;
            GPIO->NOT[0] = 1U << 22;
            for (j = 0; j < 2; j++)
            {
            }
        }
#endif
        transmitDatapump->credits += completedPackets->numCompletedPackets;

        if (transmitDatapump->frequency != PAYLOAD_FREQUENCY_PERIODIC)
        {
            DATAPUMP_Transmit(transmitDatapump);
        }
    }

    return ret;
}

static bool DATAPUMP_ProcessDisconnectComplete(uint16_t connHandle)
{
    transmit_params_t *transmitDatapump = GetTransmitContext(connHandle);
    if (transmitDatapump != NULL)
    {
        transmitDatapump->stopTransmit = true;
    }

    /* Never filter out disconnect complete events. They should reach the host */
    return false;
}

static void DATAPUMP_CheckTransmitJobs(void)
{
    for (int i = 0; i < MAX_CONN; i++)
    {
        transmit_params_t *transmit = &s_contexts[i].transmit;
        if (s_contexts[i].inUse == kTransmitContext && (transmit->frequency == PAYLOAD_FREQUENCY_PERIODIC))
        {
            transmit->delay++;
            if (transmit->delay >= transmit->interval)
            {
                transmit->delay = 0;
                DATAPUMP_Transmit(transmit);
            }
        }
    }
}

static void DATAPUMP_TransmitTimeoutCb(void)
{
    DATAPUMP_CheckTransmitJobs();
    DATAPUMP_ProgramTxTimer(); /* Rearm the transmit timer. */
}

/*
   The CTIMER implementation will call this function when a timer match occurs; the
   passed flags argument has a 1 bit for the corresponding timer that fired.
*/
static void DATAPUMP_ProcessEventTimer(uint32_t flags)
{
    for (int timerId = 0; timerId < MAX_TIMER_CHANNELS; timerId++)
    {
        if ((flags >> timerId) & 0x1)
        {
            if (timerId == s_txTimerId)
            {
#ifdef DEBUG_DATAPUMP
                PRINTF("TX\n");
#endif
                DATAPUMP_TransmitTimeoutCb();
            }
            else
            {
                receive_params_t *rxp = GetReceiveContextFromTimerId(timerId);
                if (rxp)
                {
                    ProgramTimer(s_cClockId, rxp->timerId, rxp->reportInterval);
                    DATAPUMP_SendAutonomousReport(rxp->connHandle);
                }
#ifdef DEBUG_DATAPUMP
                PRINTF("RX\n");
#endif
            }
        }
    }
}

static bool DATAPUMP_HciPacketReceived(uint8_t *hciPacket, uint16_t size)
{
    bool returnVal = true;
    assert(size > 0);
    acl_packet_t *aclPacket                   = ((acl_packet_t *)(hciPacket));
    completed_packets_t *completedPacket      = ((completed_packets_t *)(hciPacket));
    disconnect_complete_t *disconnectComplete = ((disconnect_complete_t *)(hciPacket));
#ifdef DEBUG_DATAPUMP
    PRINTF("PR-");
#endif
    assert((hciPacket[0] == HCI_EVENT) || (hciPacket[0] == HCI_ACL));

    if (aclPacket->lc == HCI_ACL)
    {
#ifdef DEBUG_DATAPUMP
        PRINTF("ACL\n");
#endif
        returnVal = DATAPUMP_ProcessAclPacket(aclPacket);
    }
    else if ((completedPacket->lc == HCI_EVENT))
    {
#ifdef DEBUG_DATAPUMP
        PRINTF("EVT:");
#endif
        if (completedPacket->eventCode == NUMBER_OF_COMPLETED_PACKETS_EVENT)
        {
#ifdef DEBUG_DATAPUMP
            PRINTF("CP\n");
#endif
            returnVal = DATAPUMP_ProcessCompletedPackets(completedPacket);
        }
        else if (disconnectComplete->eventCode == DISCONNECT_COMPLETE_EVENT)
        {
#ifdef DEBUG_DATAPUMP
            PRINTF("DC\n");
#endif
            returnVal = DATAPUMP_ProcessDisconnectComplete(disconnectComplete->connHandle);
        }
        else
        {
#ifdef DEBUG_DATAPUMP
            PRINTF("UE\n");
#endif
            returnVal = false;
        }
    }
    else
    {
#ifdef DEBUG_DATAPUMP
        PRINTF("UP\n");
#endif
        returnVal = false;
    }

    return returnVal;
}

hci_uart_prefilter_t DATAPUMP_SerialOutPrefilter(uint8_t *buffer, uint16_t size)
{
    bool processed = DATAPUMP_HciPacketReceived(buffer, size);
    return processed ? kPREFILTER_BLOCK_SENDING : kPREFILTER_ALLOW_SENDING;
}

static void DATAPUMP_RegisterTapHciPacketCallback()
{
    ll_uart_register_write_prefilter(DATAPUMP_SerialOutPrefilter);
}

static void DATAPUMP_TimerCb(uint32_t flags)
{
#ifdef DEBUG_DATAPUMP
    PRINTF("T:\n");
#endif
    DATAPUMP_ProcessEventTimer(flags);
}

static void DatapumpInit(void)
{
    for (int i = 0; i < MAX_ACL_DATA_BUFFER; i++)
    {
        s_acl[i].lc      = HCI_ACL;
        s_acl[i].PB_Flag = 0;
        s_acl[i].BC_Flag = 0;
    }
    s_dataPattern[0] = 0x11;
    s_dataPattern[1] = 0x22;
    s_dataPattern[2] = 0x33;
    s_dataPattern[3] = 0x44;
    s_dataPattern[4] = 0x55;
    s_dataPattern[5] = 0x66;
    s_dataPattern[6] = 0x77;
    s_dataPattern[7] = 0x88;
    s_aclDataQueue   = 0;

    for (uint32_t i = 0; i < MAX_ACL_DATA_BUFFER; i++)
    {
        memset(s_acl[i].data, s_dataPattern[i], sizeof(s_acl[i].data));
    }
}

static void DatapumpEnable(void)
{
#ifdef DEBUG_DATAPUMP
    PRINTF("%s\n", __FUNCTION__);
#endif

    if (!s_isInitialized)
    {
        DatapumpInit();
        s_isInitialized = 1;
    }

    DATAPUMP_RegisterTapHciPacketCallback();

    DATAPUMP_TimerInit(s_cClockId);
    DATAPUMP_TimerInit(s_ctimerIdTx);
}

static void DatapumpDisable(void)
{
#ifdef DEBUG_DATAPUMP
    PRINTF("%s\n", __FUNCTION__);
#endif
    DATAPUMP_DisableEvent();
}

static void DatapumpTransmit(datapump_transmit_parameters_t *p)
{
#ifdef DEBUG_DATAPUMP
    PRINTF("%s\n", __FUNCTION__);
#endif
    assert(p != NULL);

    general_status_datapump_t status;
    status.status     = kDATAPUMP_Status_Success;
    status.connHandle = p->connHandle;

    // check if all the parameters are right
    if ((p->type > PAYLOAD_TYPE_RANDOM_LENGTH) || (p->pattern > PAYLOAD_PATTERN_RANDOM_DATA) ||
        (p->frequency > PAYLOAD_FREQUENCY_PERIODIC))
    {
        status.status = kDATAPUMP_Status_Invalid_Param;
        PRINTF("Invalid parameter! Handle %u\n", p->connHandle);
    }
    if (GetTransmitContext(p->connHandle) != NULL)
    {
        status.status = kDATAPUMP_Status_Disallowed;
        PRINTF("No context! Handle %u\n", p->connHandle);
    }

    if (status.status == kDATAPUMP_Status_Success)
    {
        int i;

        /* If there is still a transmit datapump available, configure the parameters of that datapump. */
        for (i = 0; i < MAX_CONN; i++)
        {
            if (!s_contexts[i].inUse)
            {
                s_contexts[i].inUse = kTransmitContext;
                transmit_params_t *txp = &s_contexts[i].transmit;
                txp->connHandle     = p->connHandle;
                txp->type           = p->type;
                txp->pattern        = p->pattern;
                txp->length         = p->length;
                txp->frequency      = p->frequency;
                txp->interval       = p->interval;
                txp->numPackets     = p->numPackets;
#ifdef DEBUG_DATAPUMP
                PRINTF("handle %u interval %d num packets %d\n", txp->connHandle, txp->interval, txp->numPackets);
#endif
                txp->credits        = p->credits;
                txp->numPacketsSent = 0;
                txp->timerId        = 0;
                txp->delay          = 0;
                txp->lastTxTimeUs   = 0;

                /*
                  Notes about the s_txTimerId;
                  - there is at most 1 transmit timer active; indicated by the s_txTimerReferences count.
                  - it is assigned one of the available, if any, timer ids in the s_timerIds[] array.
                */

                if (p->frequency == PAYLOAD_FREQUENCY_PERIODIC)
                {
                    if (s_txTimerReferences == 0)
                    {
                        if (DATAPUMP_AllocateTimerId(&s_txTimerId) != kDATAPUMP_Status_Success)
                        {
                            status.status = kDATAPUMP_Status_Capacity_Exceeded;
                        }
                        else
                        {
                            CTIMER_RegisterCallBack(s_ctimers[s_ctimerIdTx], &DATAPUMP_TimerCbIndir[0], kCTIMER_SingleCallback);
                            DATAPUMP_ProgramTxTimer();
                        }
                    }
                    s_txTimerReferences++;
                }

                DATAPUMP_Transmit(txp);

                break;
            }
        }
        if (i == MAX_CONN)
        {
            status.status = kDATAPUMP_Status_Capacity_Exceeded;
            PRINTF("No transmit context available.\n");
        }
    }
}

static void DatapumpReceive(datapump_receive_parameters_t *params)
{
#ifdef DEBUG_DATAPUMP
    PRINTF("%s\n", __FUNCTION__);
#endif
    assert(params != NULL);

    uint16_t connHandle     = ((datapump_receive_parameters_t *)params)->connHandle;
    uint32_t reportInterval = ((datapump_receive_parameters_t *)params)->reportInterval * 1000;
    uint8_t timerId         = 0;

    general_status_datapump_t status;
    status.status     = kDATAPUMP_Status_Success;
    status.connHandle = connHandle;

    if (GetReceiveContext(connHandle) != NULL)
    {
        status.status = kDATAPUMP_Status_Disallowed;
    }

    // before reserving a datapump check if the datapump needs a timer for autonomous reporting
    if ((reportInterval >= MIN_REPORT_INTERVAL) && (reportInterval <= MAX_REPORT_INTERVAL))
    {
        if (DATAPUMP_AllocateTimerId(&timerId) != kDATAPUMP_Status_Success)
        {
            status.status = kDATAPUMP_Status_Capacity_Exceeded;
        }
    }
    else if (reportInterval != 0)
    {
        status.status = kDATAPUMP_Status_Invalid_Param;
    }

    if (status.status == kDATAPUMP_Status_Success)
    {
        int i;
        /* If there is still a receive datapump available, configure the parameters of that datapump. */
        for (i = 0; i < MAX_CONN; i++)
        {
            if (!s_contexts[i].inUse)
            {
                s_contexts[i].inUse = kReceiveContext;
                receive_params_t *rxp = &s_contexts[i].receive;
                rxp->connHandle     = connHandle;
                rxp->reportInterval = reportInterval;
                rxp->numPackets     = 0;
                rxp->numberOfBytes  = 0;
                rxp->timestamp      = 0;
                rxp->timerId        = timerId;

                if (rxp->reportInterval != 0)
                {
                    CTIMER_RegisterCallBack(s_ctimers[s_cClockId], &DATAPUMP_TimerCbIndir[0], kCTIMER_SingleCallback);
                    ProgramTimer(s_cClockId, timerId, reportInterval);
                }

                break;
            }
        }
        if (i == MAX_CONN)
        {
            status.status = kDATAPUMP_Status_Capacity_Exceeded;
        }
    }
}

static void DatapumpEnd(datapump_end_parameters_t *params)
{
#ifdef DEBUG_DATAPUMP
    PRINTF("%s\n", __FUNCTION__);
#endif
    assert(params != NULL);

    DATAPUMP_End(params->connHandle);
}

static void DatapumpReadReport(datapump_read_report_parameters_t *params, report_status_datapump_t *status)
{
#ifdef DEBUG_DATAPUMP
    PRINTF("%s\n", __FUNCTION__);
#endif
    assert(params != NULL);
    DATAPUMP_GenerateReport(params->connHandle, status);
}

static ble_stat_t DatapumpCallEnable(uint8_t nbOfInParams,
                                     uint8_t *in_params,
                                     uint8_t *evnt_pckt_p,
                                     uint8_t *out_params_length,
                                     hci_return_command_type *return_command_type)
{
    DatapumpEnable();
    return SUCCESS;
}

static ble_stat_t DatapumpCallDisable(uint8_t nbOfInParams,
                                      uint8_t *in_params,
                                      uint8_t *evnt_pckt_p,
                                      uint8_t *out_params_length,
                                      hci_return_command_type *return_command_type)
{
    DatapumpDisable();
    return SUCCESS;
}

static ble_stat_t DatapumpCallTransmit(uint8_t nbOfInParams,
                                       uint8_t *in_params,
                                       uint8_t *evnt_pckt_p,
                                       uint8_t *out_params_length,
                                       hci_return_command_type *return_command_type)
{
    datapump_transmit_parameters_t transmit_params;
    transmit_params.connHandle = READ_2_BYTES(in_params, 0);
    transmit_params.type       = in_params[2];
    transmit_params.pattern    = in_params[3];
    transmit_params.length     = READ_2_BYTES(in_params, 4);
    transmit_params.frequency  = in_params[6];
    transmit_params.interval   = READ_2_BYTES(in_params, 7);
    transmit_params.numPackets = READ_4_BYTES(in_params, 9);
    transmit_params.credits    = in_params[13];
    DatapumpTransmit(&transmit_params);
    return SUCCESS;
}

static ble_stat_t DatapumpCallReceive(uint8_t nbOfInParams,
                                      uint8_t *in_params,
                                      uint8_t *evnt_pckt_p,
                                      uint8_t *out_params_length,
                                      hci_return_command_type *return_command_type)
{
    datapump_receive_parameters_t receive_params;
    receive_params.connHandle     = READ_2_BYTES(in_params, 0);
    receive_params.reportInterval = READ_4_BYTES(in_params, 2);
    DatapumpReceive(&receive_params);
    return SUCCESS;
}

static ble_stat_t DatapumpCallReadReport(uint8_t nbOfInParams,
                                         uint8_t *in_params,
                                         uint8_t *evnt_pckt_p,
                                         uint8_t *out_params_length,
                                         hci_return_command_type *return_command_type)
{
    datapump_read_report_parameters_t read_report_params;
    read_report_params.connHandle = READ_2_BYTES(in_params, 0);

    report_status_datapump_t status;
    DatapumpReadReport(&read_report_params, &status);

    *out_params_length   = sizeof(status);
    *return_command_type = HCI_RETURN_COMMAND_TYPE_COMPLETE;

    evnt_pckt_p[0] = status.status;
    WRITE_2_BYTES(evnt_pckt_p, 1, status.connHandle);
    WRITE_4_BYTES(evnt_pckt_p, 3, status.numPacketsTx);
    WRITE_4_BYTES(evnt_pckt_p, 7, status.numPacketsRx);
    WRITE_4_BYTES(evnt_pckt_p, 11, status.throughput);

    return SUCCESS;
}

static ble_stat_t DatapumpCallEnd(uint8_t nbOfInParams,
                                  uint8_t *in_params,
                                  uint8_t *evnt_pckt_p,
                                  uint8_t *out_params_length,
                                  hci_return_command_type *return_command_type)
{
    datapump_end_parameters_t end_params;
    end_params.connHandle = READ_2_BYTES(in_params, 0);
    DatapumpEnd(&end_params);
    return SUCCESS;
}

int DatapumpPreMainInit(void)
{
    int res;
    (void)res;
    /* Extend custom commannd array */
    res = HCICUSTOMCMD_Register(customHCIOperations_Datapump,
                                sizeof(customHCIOperations_Datapump) / sizeof(customHCIOperations_Datapump[0]));
    assert(res == 0);

    return 0;
}
