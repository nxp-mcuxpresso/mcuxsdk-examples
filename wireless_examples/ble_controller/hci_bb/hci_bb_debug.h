/*! *********************************************************************************
* Copyright 2021-2024 NXP
* All rights reserved.
*
* This file provides additional features for the hci black box application
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#define HCI_BB_DEBUG
#ifdef HCI_BB_DEBUG

#include <stdarg.h>
#include "fsl_component_timer_manager.h"

enum
{
    PKT_ERROR_SUCCESS,
    PKT_ERROR_LENGTH,
    PKT_ERROR_HDR,
    PKT_ERROR_SEQ_PLUS,
    PKT_ERROR_SEQ_MINUS,
    PKT_ERROR_PAYLOAD,
    PKT_ERROR_TX,
};

/* KW45:
   01 02 20 00 (7.8.2 LE Read Buffer Size Command [LE_Read_Buffer_Size])
   04 0E 07 01 02 20 00   00 01   08 (7.7.14 Command Complete Event)
*/
#define MAX_PKT_LENGTH                         256 // payload only, handle length not included
#define NB_MAX_CONNECTIONS                     8
#define CONN_IDX_MASK                          (NB_MAX_CONNECTIONS-1)  // connection index in the connection handle
#if (NB_MAX_CONNECTIONS&CONN_IDX_MASK) != 0
#error Number of connections must be power of 2
#endif

#define PERIODIC_REPORT_DELAY_US               1000000ULL

#define _GET32(pData) ((uint32_t)(pData)[0] | ((uint32_t)(pData)[1] << 8U) | ((uint32_t)(pData)[2] << 16U) | ((uint32_t)(pData)[3] << 24U))
#define _GET24(pData) ((uint32_t)(pData)[0] | ((uint32_t)(pData)[1] << 8U) | ((uint32_t)(pData)[2] << 16U))
#define _GET16(pData) ((uint16_t)(pData)[0] | ((uint16_t)(pData)[1] << 8U))

typedef struct
{
    uint8_t  config_valid;                  // valid config received or not
    uint8_t  tx_enabled;                    // tx test enabled or not
    uint8_t  rx_enabled;                    // rx test enabled or not

    uint8_t  payload_type;                  // packet type (incremental, pseudo etc.), not used
    uint16_t payload_len;                   // payload length

    // Tx config
    uint32_t nb_pkt_to_tx;                  // number of packets asked for tx from host
    uint8_t  nb_pkt_ll_buffer_max;          // maximum number of pending packets in LL
    uint32_t burst_interval_ms;             // interval between two Tx packet bursts
    uint16_t nb_pkt_burst;                  // number of packets per Tx burst

    // Tx
    uint16_t handle;                        // tx connection handle
    uint8_t  nb_pkt_ll_pending;             // current number of pending packets in LL
    uint32_t nb_pkt_sent_to_ll;             // number of packets actually sent to LL
    uint32_t nb_pkt_completed;              // number of packets actually acked by LL
    uint32_t burst_pkt_queued;              // counter for burst packets
    TIMER_MANAGER_HANDLE_DEFINE(timerHandle);  // periodic Tx

    // tx errors
    uint32_t tx_error_total;                // number of errors during call to Hci_SendPacketToController()
    uint32_t tx_error_x4;                   // record 4 last errors, one error per byte

    // Rx
    uint32_t nb_rxed_ok_pkts_total;            // total number of received packets
    uint32_t nb_rxed_ok_pkts[NB_MAX_CONNECTIONS];               // number of received packets per handle

    uint32_t next_expected_pkt_seq_no[NB_MAX_CONNECTIONS];       // expected sequence number per handle

    // rx errors
    uint32_t rx_error_hdr_total;            // number of header errors
    uint32_t rx_error_hdr[NB_MAX_CONNECTIONS];          // cumul

    uint32_t rx_error_len_total;            // number of length errors
    uint64_t rx_error_len_x4;               // record 4 last incorrect lengths
    uint32_t rx_error_len[NB_MAX_CONNECTIONS];          // cumul length

    uint32_t rx_error_payload_total;        // number total of payload error (seq ok)
    uint8_t  rx_error_payload[NB_MAX_CONNECTIONS];          // number error per handle

    uint32_t rx_error_seq_total;        // number total of payload error (seq ok)
    uint16_t rx_error_seq_plus_total;       // sequence number error: increment > 1
    int32_t  rx_error_seq_plus_plus_max;   // maximum value of the seq error > 1
    int32_t  rx_error_seq_plus_last[NB_MAX_CONNECTIONS];    // latest sequence errors
    uint32_t rx_error_seq_plus[NB_MAX_CONNECTIONS];          // sequence number error per handle

    uint16_t rx_error_seq_minus_total;      // sequence number error: increment < 1
    int32_t  rx_error_seq_minus_max;  // minimum value of the seq error < 1
    int32_t  rx_error_seq_minus_last[NB_MAX_CONNECTIONS];    // latest sequence errors
    uint32_t rx_error_seq_minus[NB_MAX_CONNECTIONS];         // sequence number error per handle

    uint64_t last_report_time;              // last report timestamp, avoid serial interface overload

    /* store segmented packets */
    uint8_t  rx_buffer_seg[NB_MAX_CONNECTIONS][4+MAX_PKT_LENGTH]; // handle + length + payload

} test_tx_rx_t;
static test_tx_rx_t trx;

static void Test_SendAclPacketToLinkLayer(int max_nb_pkt_to_send);

static uint64_t Test_GetTimestamp(void)
{
    // read the current time in us
    volatile uint32_t l = TSTMR0->L;
    volatile uint32_t h = TSTMR0->H;

    return (uint64_t)h*0x100000000ULL+l;
}

static void hcibb_TimerHandler(void *param)
{
    if (trx.tx_enabled && trx.nb_pkt_to_tx>trx.nb_pkt_sent_to_ll) {
        Test_SendAclPacketToLinkLayer(trx.nb_pkt_burst);
    }
}

static void Hcit_SerialFreePacket(void *pPacket,
                                  serial_manager_callback_message_t *message,
                                  serial_manager_status_t status);

/* send data to host/UART */
static void Test_SerialWrite(uint8_t *data, uint32_t len)
{
    uint8_t* pSerialPacket = NULL;
    uint8_t* pPacketBuffer;
    uint8_t* hciWriteHandle;

    /* Increase by 1 the lenght to insert the packet type */
    pPacketBuffer = MEM_BufferAlloc(SERIAL_MANAGER_WRITE_HANDLE_SIZE + len);
    if (pPacketBuffer == NULL)
    {
        assert(0);
        return;
    }
    hciWriteHandle   = pPacketBuffer;
    pSerialPacket    = pPacketBuffer + SERIAL_MANAGER_WRITE_HANDLE_SIZE;

    FLib_MemCpy(&pSerialPacket[0], (uint8_t*)data, len);
    (void)SerialManager_OpenWriteHandle((serial_handle_t)gSerMgrIf, (serial_write_handle_t)hciWriteHandle);
    (void)SerialManager_InstallTxCallback((serial_write_handle_t)hciWriteHandle, Hcit_SerialFreePacket, pPacketBuffer);
    if ( kStatus_SerialManager_Success != SerialManager_WriteNonBlocking((serial_write_handle_t)hciWriteHandle, pSerialPacket, len) )
    {
        SerialManager_CloseWriteHandle((serial_write_handle_t)pPacketBuffer);
        (void)MEM_BufferFree(pPacketBuffer);
    }
    else
    {
        uint32_t regPrimask = DisableGlobalIRQ();
        nb_pkt_in_uart_tx_queue++;
        EnableGlobalIRQ(regPrimask);
    }
}

/* send event to host */
static void Test_SendEventToHost(uint8_t eventCode, ...)
{
    uint8_t *pPacketBuffer = MEM_BufferAlloc(1+1+255);
    if (pPacketBuffer == NULL)
    {
        assert(0);
        return;
    }

    uint32_t len;

    va_list args;
    va_start(args, eventCode);

    pPacketBuffer[0] = 0x04;
    pPacketBuffer[1] = eventCode;
    pPacketBuffer[2] = 0;  // place holder for length

    len = 3;
    while(1)
    {
        uint32_t sz = va_arg(args, uint32_t);
        if(sz==0)
            break;

        uint32_t val = va_arg(args, uint32_t);
        switch(sz)
        {
            case 1:
                pPacketBuffer[len++] = (uint8_t) val;
                break;
            case 2:
                pPacketBuffer[len++] = (uint8_t) val;
                pPacketBuffer[len++] = (uint8_t) (val>>8);
                break;
            case 4:
                pPacketBuffer[len++] = (uint8_t) val;
                pPacketBuffer[len++] = (uint8_t) (val>>8);
                pPacketBuffer[len++] = (uint8_t) (val>>16);
                pPacketBuffer[len++] = (uint8_t) (val>>24);
                break;
            default:
                assert(0);
        }
    }

    pPacketBuffer[2] = len-3;

    Test_SerialWrite(pPacketBuffer, len);
    (void)MEM_BufferFree(pPacketBuffer);
}

/* send ACL packet to host */
static void Test_SendAclPktToHost(uint8_t *pPacket, uint16_t packetSize)
{
    uint8_t *pPacketBuffer = MEM_BufferAlloc(1+packetSize);
    if (pPacketBuffer == NULL)
    {
        assert(0);
        return;
    }

    assert(packetSize==4+_GET16(&pPacket[2]));

    pPacketBuffer[0] = 0x02;
    memcpy(pPacketBuffer+1, pPacket, packetSize);

    Test_SerialWrite(pPacketBuffer, 1+packetSize);
    (void)MEM_BufferFree(pPacketBuffer);
}

/* send data to LL/controller */
static int8_t Test_SendMessageToLinkLayer(uint8_t pktType, uint8_t *data, uint32_t len)
{
    uint8_t*        pSerialPacket = NULL;
    uint32_t        packet_Size = len + 1;

    pSerialPacket = MEM_BufferAlloc(packet_Size);

    if (pSerialPacket == NULL)
    {
        assert(0);
        return 1;
    }

    pSerialPacket[0] = pktType;
    FLib_MemCpy(&pSerialPacket[1],data,  len);

    PLATFORM_SendHciMessage(pSerialPacket, packet_Size);

    MEM_BufferFree(pSerialPacket);
    return 0;
}

// send the test report to HCI
// param:
//     force: true:  send the report
//            false: send the report only if the prvious report was sent long time ago
static void Test_SendReportToHost(int force, uint8_t status)
{
    uint64_t time = Test_GetTimestamp(); // llh_get_current_instant_clk();
    if (!force || status == PKT_ERROR_SUCCESS)
    {
        uint64_t delay = time - trx.last_report_time;

        // send report after some delay
        if( delay < PERIODIC_REPORT_DELAY_US )
            return;
    }

    // check if the report content has changed using a checksum
    static uint32_t last_report_chk = -1;
    uint32_t chksum = (status +
                       trx.nb_pkt_completed +
                       trx.nb_rxed_ok_pkts_total +
                       trx.rx_error_len_total +
                       trx.rx_error_seq_total +
                       trx.rx_error_payload_total);
    if( last_report_chk == chksum )
    {
        // nothing changed
        return;
    }
    last_report_chk = chksum;

    trx.last_report_time   = time;

    Test_SendEventToHost(0xFF,
        1, 0xF0, // subevent
        1, status,
        4, trx.nb_pkt_completed,
        4, trx.nb_rxed_ok_pkts_total,
        // 4, trx.tx_error_total,   // always success on KW45
        4, trx.rx_error_len_total,
        4, trx.rx_error_seq_total,
        4, trx.rx_error_payload_total,

        0
    );
}

/* BlueLite commander data packet format:
    02
    20 00
    FB 00
    797A7B7C7D7E7F808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9FA0A1A2A3A4A5A6A7A8A9AA
    ABACADAEAFB0B1B2B3B4B5B6B7B8B9BABBBCBDBEBFC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDC
    DDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF000102030405060708090A0B0C0D0E
    0F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F40
    4142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E5F606162636465666768696A6B6C6D6E6F707172
    73
*/

// send packets to LL HCI interface
// packet format: 02 handle length seq seq+1 ...
static void Test_SendAclPacketToLinkLayer(int max_nb_pkt_to_send)
{
    // Temporary buffer to store tx packet to be sent to LL
    // 4 bytes for:
    //      2-byte Handle
    //      2-byte Length
    // MAX_PKT_LENGTH:
    //      payload data: 4-byte seq number, remaining payload
    static uint8_t tx_buffer[4+MAX_PKT_LENGTH];

    // IRQ must be enabled as serial manager calls freeRTOS function OSA_SemaphoreWait
    // OSA_InterruptDisable();
    tx_buffer[0]=trx.handle&0xff;
    tx_buffer[1]=trx.handle>>8;

    tx_buffer[2]=trx.payload_len&0xff;
    tx_buffer[3]=trx.payload_len>>8;

    while( trx.nb_pkt_sent_to_ll < trx.nb_pkt_to_tx && trx.nb_pkt_ll_pending < trx.nb_pkt_ll_buffer_max)
    {
        /* Build hci acl packet */
        tx_buffer[4] = trx.nb_pkt_sent_to_ll;
        tx_buffer[5] = trx.nb_pkt_sent_to_ll>>8;
        tx_buffer[6] = trx.nb_pkt_sent_to_ll>>16;
        tx_buffer[7] = trx.nb_pkt_sent_to_ll>>24;

        uint8_t byte = tx_buffer[4];
        for(int i=0; i<trx.payload_len-4; i++)
            tx_buffer[i+8]= byte++;

        uint8_t tx_status = Test_SendMessageToLinkLayer(gHciDataPacket_c, tx_buffer, 4+trx.payload_len);
        if( tx_status != 0 )
        {
            trx.tx_error_total++;
            trx.tx_error_x4 =(trx.tx_error_x4<<8)|tx_status;
            Test_SendReportToHost(1, PKT_ERROR_TX);

            // stop sending packets in case of error
            break;
        }

        trx.nb_pkt_ll_pending++;
        trx.nb_pkt_sent_to_ll++;

        if (--max_nb_pkt_to_send == 0)
          break;
    }
}

/* send HCI response after receiving the HCI command */
static void Test_SendCmdCompleteToHost(uint8_t *pPacket, uint8_t status)
{
    Test_SendEventToHost(0x0E,
        1, 1,           // number of cmds
        1, pPacket[0],  // opcode
        1, pPacket[1],
        1, status,      // status
        0
    );
}

// tx block of type 0x01:
//
//      xx xx        opcode
//      10           length
//
//      xx xx        handle
//      xx           pkt type
//      xx xx        pkt length
//      xx xx xx xx  number of pkts
//      xx           pending max
//      xx           anchor move enable / disable
static void Test_CmdTRxConfig(uint8_t *pPacket, uint16_t packetSize)
{
    memset(&trx, 0, sizeof(trx));

    if( pPacket[2] != 12 )
    {
        // wrong parameters
        Test_SendCmdCompleteToHost(pPacket, 0x12);
    }
    else
    {
        trx.payload_type         = pPacket[3];
        trx.payload_len          = _GET16(&pPacket[4]);
        trx.nb_pkt_to_tx         = _GET32(&pPacket[6]);
        trx.nb_pkt_ll_buffer_max = pPacket[10];
        trx.burst_interval_ms    = _GET16(&pPacket[11]);
        trx.nb_pkt_burst         = _GET16(&pPacket[13]);

        if(trx.payload_len<4 || trx.payload_len>MAX_PKT_LENGTH)
        {
            // wrong parameters
            Test_SendCmdCompleteToHost(pPacket, 0x12);
        }
        else
        {
            Test_SendCmdCompleteToHost(pPacket, 0x00);
            if (trx.burst_interval_ms != 0)
            {
                (void)TM_Open(trx.timerHandle);
                (void)TM_InstallCallback(trx.timerHandle, hcibb_TimerHandler, (void*)NULL);
                (void)TM_Start(trx.timerHandle, kTimerModeIntervalTimer | kTimerModeLowPowerTimer, trx.burst_interval_ms);
            }
            trx.config_valid=1;
        }

        trx.last_report_time = 0-PERIODIC_REPORT_DELAY_US; // set to an elapsed time to trigger 1st report asap
    }
}

// tx block of type 0x01:
//
//      xx xx        opcode
//      01           length
//      xx           enable
static void Test_CmdTxEnable(uint8_t *pPacket, uint16_t packetSize)
{
    if( pPacket[2] != 0x03 )
        Test_SendCmdCompleteToHost(pPacket, 0x12);

    if( pPacket[3] > 1 || _GET16(&pPacket[4]) > 0x0EFF)
        Test_SendCmdCompleteToHost(pPacket, 0x12);

    trx.tx_enabled = pPacket[3];
    trx.handle     = _GET16(&pPacket[4]);

    Test_SendCmdCompleteToHost(pPacket, 0x00);

    if( trx.tx_enabled )
    {
        // Send a single packet here.
        // Remaining packets will be sent when LL reports nb completed packets event.
        // Otherwise LL could call Test_SendAclPacketToLinkLayer when this call is still ongoing
        // in case LL had sent out a frame before the return of the below call.
        Test_SendAclPacketToLinkLayer(1);
    }
}

// tx block of type 0x01:
//
//      xx xx        opcode
//      00           length
static void Test_CmdReportRequest(uint8_t *pPacket, uint16_t packetSize)
{
    if( pPacket[2] != 0 )
        Test_SendCmdCompleteToHost(pPacket, 0x12);

    Test_SendCmdCompleteToHost(pPacket, 0x00);

    // send short report to HCI
    Test_SendReportToHost(1, PKT_ERROR_SUCCESS);

}

// reset the board, HCI Reset may not work well
static void Test_CmdHardwareReset(uint8_t *pPacket, uint16_t packetSize)
{
    __NVIC_SystemReset();
}

// from UART
// Exemple for Reset command:
//  packetTypeMarker   01
//  Raw                01 09 10 00
//  bytesReceived      04
// For ACL Pkt: pPacket: HH HH LL LL PP PP PP ... PP
//             packetSize = payload length + 4
// return 0 if not processed
int Test_MsgFromHost(uint8_t packetType, uint8_t *pPacket, uint16_t packetSize)
{
    if( packetType != 0x01 )
        return 0;

    uint16_t op = pPacket[0] + pPacket[1]*256;
    switch(op)
    {
        case 0x0C03:    // intercept reset cmd
            memset(&trx, 0, sizeof(trx));
            return 0;   // continue processing of the command

        case 0xFDA8:    // test config
            Test_CmdTRxConfig(pPacket, packetSize);
            return 1;   // stop further processing of the command

        case 0xFDA9:    // start or stop test
            Test_CmdTxEnable(pPacket, packetSize);
            return 1;   // stop further processing of the command

        case 0xFDAA:    // read report
            Test_CmdReportRequest(pPacket, packetSize);
            return 1;   // stop further processing of the command

        case 0xFDAF:    // reset the board
            Test_CmdHardwareReset(pPacket, packetSize);
            return 1;   // should not run after as board is reset

        default:
            break;
    }

    // command not processed in this function
    return 0;
}

// check if the rxed ACK packet is correct
static void Test_CheckTotalAclPacket(uint8_t *pPacket, uint16_t packetSize)
{
    uint8_t status;
    uint32_t seq_no;

    do
    {
        uint16_t handle = _GET16(&pPacket[0]) & 0xFFF;
        int conn_no = handle & CONN_IDX_MASK;

        // payload length
        uint16_t len = _GET16(&pPacket[2]);

        assert(packetSize == len+4);

        // check packet length
        if( len != trx.payload_len )
        {
            trx.rx_error_len_total++;
            trx.rx_error_len_x4 =(trx.rx_error_len_x4<<16)+len;
            status = PKT_ERROR_LENGTH;
            break;
        }

        // check sequence number
        seq_no = _GET32(&pPacket[4]);
        int delta = seq_no - trx.next_expected_pkt_seq_no[conn_no];
        trx.next_expected_pkt_seq_no[conn_no] = seq_no+1;

        if( delta != 0)
        {
            trx.rx_error_seq_total++;
            if( delta > 0 )
            {
                trx.rx_error_seq_plus_total++;
                trx.rx_error_seq_plus_last[conn_no] = delta;
                trx.rx_error_seq_plus[conn_no]++;
                if( delta > trx.rx_error_seq_plus_plus_max )
                    trx.rx_error_seq_plus_plus_max = delta;
                status = PKT_ERROR_SEQ_PLUS;
            }
            else
            {
                trx.rx_error_seq_minus_total++;
                trx.rx_error_seq_minus_last[conn_no] = delta;
                trx.rx_error_seq_minus[conn_no]++;
                if( delta < trx.rx_error_seq_minus_max )
                    trx.rx_error_seq_minus_max = delta;
                status = PKT_ERROR_SEQ_MINUS;
            }
            break;
        }

        // check payload
        // A4 20  FB 00  02 00 00 00  02 03 04 05 06 07...
        // Handle Length PL: Seq_no   PL: length-4 bytes
        int32_t index = 8; // start aftersequence number
        len-=4; // remove sequence number
        uint8_t expected = (uint8_t)seq_no;
        while (len>0)
        {
            if (pPacket[index] != expected)
            {
                // payload error
                trx.rx_error_payload_total++;
                trx.rx_error_payload[conn_no]++;
                status = PKT_ERROR_PAYLOAD;
                break;
            }
            len--;
            index++;
            expected++;
        }

        // all check ok
        status = PKT_ERROR_SUCCESS;
        trx.nb_rxed_ok_pkts_total++;
        trx.nb_rxed_ok_pkts[conn_no]++;
    } while (0);

    if (status != PKT_ERROR_SUCCESS)
    {
        // trace the current packet
        Test_SendReportToHost(1, status);
        // send the ACL packet
        Test_SendAclPktToHost(pPacket, packetSize);
    }
}

static void Test_HandleAckPkt(uint8_t *pPacket, uint16_t packetSize)
{
    uint16_t handle  = _GET16(&pPacket[0]) & 0xFFF;
    uint16_t conn_no = handle & CONN_IDX_MASK;
    uint16_t len     = _GET16(&pPacket[2]);
    uint8_t pb_flag  = (pPacket[1]>>4)&3;

    assert(len!=0);
    assert(packetSize == len+4);

    uint8_t *pPacket_prev = &trx.rx_buffer_seg[conn_no][0];
    uint16_t len_prev     = _GET16(&pPacket_prev[2]);
    if(len_prev != 0)
    {
      assert( (_GET16(&pPacket_prev[0]) & CONN_IDX_MASK) == conn_no );
    }

    int drop_prev;
    int drop_curr;

    if( pb_flag == 2 )
    {
        // new packet
        drop_prev = 1;
        drop_curr = len>=trx.payload_len ? 1 : 0;
    }
    else if( pb_flag == 1 )
    {
        // continuous packet
        drop_prev = drop_curr = (len_prev == 0 || len_prev + len > trx.payload_len) ? 1 : 0;
    }
    else
    {
        drop_prev = drop_curr = 1;
    }

    if(len_prev != 0 && drop_prev)
    {
        Test_CheckTotalAclPacket(pPacket_prev, 4+len_prev);
        memset(pPacket_prev, 0, sizeof(trx.rx_buffer_seg[0]));
        len_prev = 0;
    }

    if(drop_curr)
    {
        Test_CheckTotalAclPacket(pPacket, 4 + len);
    }
    else
    {
        // append
        if(len_prev==0)
        {
            memcpy(pPacket_prev, pPacket, packetSize);
        }
        else
        {
            memcpy(pPacket_prev+4+len_prev, pPacket+4, len);
            len_prev += len;
            pPacket_prev[2] = len_prev&0xff;
            pPacket_prev[3] = (len_prev>>8)&0xff;
        }
        if( len_prev == trx.payload_len )
        {
            Test_CheckTotalAclPacket(pPacket_prev, 4+len_prev);
            memset(pPacket_prev, 0, sizeof(trx.rx_buffer_seg[0]));
        }
    }
}

// to UART
// Response for Read BD_Address
// packetType: 04
// pPacket:    0e 0a 01 09 10 00 15 00 00 9f 04 00
// packetSize: 0c
// return
//      0 message not processed
//      1 message processed
int Test_MsgFromLinkLayer(uint8_t packetType, uint8_t *pPacket, uint16_t packetSize)
{
    int processed = 0;    // processing status by default

    if(!trx.config_valid)
    {
        return processed;
    }

    // can receive data packet when trx.enabed=0 as the sending
    // device can be enabled before the receiving device

    if( packetType==0x04 && trx.tx_enabled && pPacket[0] == 0x13 && pPacket[1] == 0x05 )
    {
       // intercept Number_of_Completed_Packets
        int completed = pPacket[5] + (pPacket[6]<<8);
        trx.nb_pkt_completed += completed;
        trx.nb_pkt_ll_pending -= completed;

        if (trx.tx_enabled && trx.nb_pkt_to_tx>trx.nb_pkt_sent_to_ll && trx.burst_interval_ms == 0)
            Test_SendAclPacketToLinkLayer(trx.nb_pkt_ll_buffer_max);
        processed = 1;
    }
    else if( packetType==0x02 && trx.config_valid)
    {
        Test_HandleAckPkt(pPacket, packetSize);
        processed = 1;
    }

    // periodic report
    Test_SendReportToHost(0, PKT_ERROR_SUCCESS);

    return processed;
}

static void HCI_AppControllerRxCallback (uint8_t packetType, uint8_t *data, uint16_t len);
static void HCI_AppControllerRxCallback2(uint8_t packetType, uint8_t *data, uint16_t len)
{
    if( !Test_MsgFromLinkLayer(packetType, data, len) )
    {
        HCI_AppControllerRxCallback(packetType, data, len);
    }
}

#define PLATFORM_SetHciRxCallback(p)  PLATFORM_SetHciRxCallback(HCI_AppControllerRxCallback2)

#define PLATFORM_SendHciMessage(pSerialPacket, packet_Size) do {\
    if( !Test_MsgFromHost(pSerialPacket[0], &pSerialPacket[1], mHcitData.bytesReceived)) \
    { PLATFORM_SendHciMessage(pSerialPacket, packet_Size); }} while(0)

#else
#endif // HCI_BB_DEBUG
