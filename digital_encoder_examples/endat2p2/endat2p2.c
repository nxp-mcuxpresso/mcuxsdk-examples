/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_endat2p2.h"
#include "fsl_xbar.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
endat2p2_dev_t *dev;

bool performance_enable;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void SYSTICK_StartCount()
{
    SysTick->VAL = SysTick->LOAD;
}

static uint32_t SYSTICK_GetCount()
{
    return SysTick->LOAD - SysTick->VAL;
}

static void BOARD_InitSysTick(void)
{
    /* Initialize SysTick core timer to run free */
    /* Set period to maximum value 2^24*/
    SysTick->LOAD = 0xFFFFFF;

    /*Clock source - System Clock*/
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    /*Start Sys Timer*/
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

static void ENDATDEV_PrintMenu(void)
{
    PRINTF("|--------------------------------------------------------|\r\n");
    PRINTF("|      Select the encoder command from following         |\r\n");
    PRINTF("|--------------------------------------------------------|\r\n");
    PRINTF("|  1: Encoder send position values                       |\r\n");
    PRINTF("|  2: Selection of memory area                           |\r\n");
    PRINTF("|  3: Encoder receive parameter                          |\r\n");
    PRINTF("|  4: Encoder send parameter                             |\r\n");
    PRINTF("|  5: Encoder receive reset                              |\r\n");
    PRINTF("|  6: Encoder send position with ADDINFO                 |\r\n");
    PRINTF("|  7: Encoder send position with ADDINFO select memory   |\r\n");
    PRINTF("|  8: Encoder send position with ADDINFO recv param      |\r\n");
    PRINTF("|  9: Encoder send position with ADDINFO send param      |\r\n");
    PRINTF("| 10: Encoder send position with ADDINFO recv err reset  |\r\n");
    PRINTF("| 11: Encoder receive communication command              |\r\n");
    PRINTF("|                                                        |\r\n");
    PRINTF("|100: Display encoder information                        |\r\n");
    PRINTF("|101: Display registers                                  |\r\n");
    PRINTF("|102: Configure clock                                    |\r\n");
    PRINTF("|103: Enable Delay compensation                          |\r\n");
    PRINTF("|104: Enable interrupt                                   |\r\n");
    PRINTF("|105: Disable interrupt                                  |\r\n");
    PRINTF("|                                                        |\r\n");
    PRINTF("|200: Software strobe 10 times loop                      |\r\n");
    PRINTF("|201: Internal timer strobe 10 times loop                |\r\n");
    PRINTF("|202: Hardware strobe 10 times loop                      |\r\n");
    PRINTF("|203: Test the performance                               |\r\n");
    PRINTF("|--------------------------------------------------------|\r\n");
    PRINTF("| Input command value: ");
}

const int menu_cmd_to_mode_cmd[] = {
    0,
    ENDAT2P2_CMD_SEND_POSITION_VALUE,                   /* 1 */
    ENDAT2P2_CMD_SELECT_MEM_AREA,                       /* 2 */
    ENDAT2P2_CMD_RECEIVE_PARAMETERS,                    /* 3 */
    ENDAT2P2_CMD_SEND_PARAMETERS,                       /* 4 */
    ENDAT2P2_CMD_RECEIVE_RESET,                         /* 5 */
    ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO,              /* 6 */
    ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO_SEL_MEM,      /* 7 */
    ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO_RECV_PARAM,   /* 8 */
    ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO_SEND_PARAM,   /* 9 */
    ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO_RECV_ERR_RST, /* 10 */
    ENDAT2P2_CMD_RECEIVE_COMMUNICATION_CMD,             /* 11 */
};

static void ENDATDEV_PrintEncoderInfo(endat2p2_dev_t *dev)
{
    PRINTF("EnDat Encoder: %s\r\n", ENDAT2P2_GetTypeStr(dev->type));
    PRINTF("Version: 2.%d\r\n", dev->cmd_set_2_2 ? 2 : 1);

    PRINTF("encoder ID: %u %s \tSN: %c %u %c\r\n",
            dev->id_binary,
            (char *)&dev->id_ascii,
            (char)dev->sn_ascii_msb,
            dev->sn_binary,
            (char)dev->sn_ascii_lsb);

    PRINTF("Position: %d bits ", dev->pos_res);

    if (ENDAT2P2_EncoderIsRotary(dev))
    {
        PRINTF("(singleturn: %d, multiturn: %d)\r\n",
                dev->single_turn_res,
                dev->multi_turn_res);
    }

    PRINTF("[resolution: %d %s]\r\n", dev->step,
           dev->type & (1 << ENDAT2P2_TYPE_ROTARY_SHIFT) ? "M/rev" : "nm");

    if (!dev->cmd_set_2_2)
        return;

    PRINTF("Addational Info1 status: 0x%x\r\n", dev->status_addinfo1);
    if(dev->status_addinfo1 & ENDAT2P2_ADDINFO1_STATUS_POS2_MASK)
    {
        PRINTF("Addational Info1: support Position Value 2\r\n");
    }

    if(dev->status_addinfo1 & ENDAT2P2_ADDINFO1_STATUS_TEMP1_MASK)
    {
        PRINTF("Addational Info1: support Temperature sensor 1\r\n");
    }

    if(dev->status_addinfo1 & ENDAT2P2_ADDINFO1_STATUS_TEMP2_MASK)
    {
        PRINTF("Addational Info1: support Temperature sensor 2\r\n");
    }

    if(dev->status_addinfo1 & ENDAT2P2_ADDINFO1_STATUS_SENSORS_MASK)
    {
        PRINTF("Addational Info1: support Additional sensor\r\n");
    }

    PRINTF("Addational Info2 status: 0x%x\r\n", dev->status_addinfo2);
    if(dev->status_addinfo2 & ENDAT2P2_ADDINFO2_STATUS_COMMUTATION_MASK)
    {
        PRINTF("Addational Info2: support Commutation\r\n");
    }

    if(dev->status_addinfo2 & ENDAT2P2_ADDINFO2_STATUS_ACCELERATION_MASK)
    {
        PRINTF("Addational Info2: support Accleteration\r\n");
    }

    if(dev->status_addinfo2 & ENDAT2P2_ADDINFO2_STATUS_LIMITPOS_MASK)
    {
        PRINTF("Addational Info2: support Limit position signals\r\n");
    }

    if(dev->status_addinfo2 & ENDAT2P2_ADDINFO2_STATUS_ASYNCPOS_MASK)
    {
        PRINTF("Addational Info2: support Asynchronous position value\r\n");
    }

    if(dev->status_addinfo2 & ENDAT2P2_ADDINFO2_STATUS_OPTERRSRC_MASK)
    {
        PRINTF("Addational Info2: support Operating status error sources\r\n");
    }

    if(dev->status_addinfo2 & ENDAT2P2_ADDINFO2_STATUS_ASYNCPOS_MASK)
    {
        PRINTF("Addational Info2: support Asynchronous position value\r\n");
    }

    if(dev->status_addinfo2 & ENDAT2P2_ADDINFO2_STATUS_POS2_MASK)
    {
        PRINTF("Addational Info2: support Position value 2\r\n");
    }

    if(dev->status_addinfo2 & ENDAT2P2_ADDINFO2_STATUS_TIMESTAMP_MASK)
    {
        PRINTF("Addational Info2: support Timestamp\r\n");
    }

    PRINTF("\r\n");
}

static void ENDATDEV_DumpRegs(endat2p2_dev_t *dev)
{
    PRINTF("SEND Reg : 0x%08x\r\n", dev->base->SENDREGISTER);
    PRINTF("RECVR1-1 : 0x%08x\r\n", dev->base->RECEIVEREGISTER1_1);
    PRINTF("RECVR1-2 : 0x%08x\r\n", dev->base->RECEIVEREGISTER1_2);
    PRINTF("RECVR2   : 0x%08x\r\n", dev->base->RECEIVEREGISTER2);
    PRINTF("RECVR3   : 0x%08x\r\n", dev->base->RECEIVEREGISTER3);

    PRINTF("CONF Reg1: 0x%08x\r\n", dev->base->CONFIGREGISTER1);
    PRINTF("CONF Reg2: 0x%08x\r\n", dev->base->CONFIGREGISTER2);
    PRINTF("STAT Reg : 0x%08x\r\n", dev->base->STATUSREGISTER);
    PRINTF("INT MASK : 0x%08x\r\n", dev->base->INTERRUPTMASKREGISTER);
}

static void ENDATDEV_DumpPosition(endat2p2_dev_t *dev,
                                  endat2p2_recv_data_t *data)
{
    /* float angle; */
    /* uint64_t max = 1 << dev->single_turn_res; */
    uint64_t length = 0;

    PRINTF("\r\n");

    if (ENDAT2P2_EncoderIsRotary(dev))
    {
        /*
        angle = ((float) data->position.position) /
                        (float)max * (float)360;
        */
    }
    else
    {
        length = data->position.position * dev->step;
    }

    if(dev->multi_turn_res)
    {
        PRINTF("Single Turn: %d, Multiple Turn: %d\r\n",
                (uint32_t) data->position.position,
                (uint32_t) data->position.revolution);
    }
    else if (ENDAT2P2_EncoderIsRotary(dev))
    {
        PRINTF("Single Turn: %d\r\n",
                (uint32_t) data->position.position);
    }
    else
    {
        PRINTF("position: %d\r\n", (uint32_t) length);
    }
}

static void ENDATDEV_DumpRecvData(endat2p2_dev_t *dev, int cmd,
                                  endat2p2_recv_data_t *data)
{
    uint32_t status = data->status;

    switch(cmd)
    {
        case ENDAT2P2_CMD_SEND_PARAMETERS:
            PRINTF("Addr: 0x%x Param: 0x%x\r\n",
                   data->addr_param.address,
                   data->addr_param.param);
            break;

        case ENDAT2P2_CMD_SEND_POSITION_VALUE:
            ENDATDEV_DumpPosition(dev, data);
            break;

        case ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO:
        case ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO_SEL_MEM:
        case ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO_RECV_PARAM:
        case ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO_SEND_PARAM:
        case ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO_RECV_ERR_RST:
        case ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO_RECV_TESTCMD:
            ENDATDEV_DumpPosition(dev, data);
            if (status & ENDAT2P2_STATUSREGISTER_RECEIVE_REGISTER3_MASK)
            {
                PRINTF("Additional Information 1: 0x%x\r\n",
                        data->position_addinfo.addinfo1);
            }

            if (status & ENDAT2P2_STATUSREGISTER_RECEIVE_REGISTER2_MASK)
            {
                PRINTF("Additional Information 2: 0x%x\r\n",
                        data->position_addinfo.addinfo2);
            }

            break;

        default:
            break;
    }
}

static void ENDATDEV_DumpStatus(endat2p2_dev_t *dev, uint32_t status)
{
    if (! (status & ENDAT2P2_STATUSREGISTER_RECEIVE_REGISTER1_MASK))
    {
        PRINTF("ERROR: Recv register 1 is not updated\r\n");
    }

    PRINTF("Status: ERR:%d CRC:%d F1:%d F2:%d MRS:%d\r\n",
            NXP_FLD2VAL(ENDAT2P2_STATUSREGISTER_ERROR1, status),
            NXP_FLD2VAL(ENDAT2P2_STATUSREGISTER_CRC_PW_PARITY, status),
            NXP_FLD2VAL(ENDAT2P2_STATUSREGISTER_F_TYPE_I, status),
            NXP_FLD2VAL(ENDAT2P2_STATUSREGISTER_F_TYPE_II, status),
            NXP_FLD2VAL(ENDAT2P2_STATUSREGISTER_MRS_ADR, status));

    if(status & ENDAT2P2_STATUSREGISTER_ERROR1_MASK)
    {
        PRINTF("Error: Error1 occurred\r\n");
    }

    if(status & ENDAT2P2_STATUSREGISTER_CRC_PW_PARITY_MASK)
    {
        PRINTF("Error: CRC was not correct\r\n");
    }

    if(status & ENDAT2P2_STATUSREGISTER_F_TYPE_I_MASK)
    {
        PRINTF("Error: Type 1 error occurred\r\n");
    }

    if(status & ENDAT2P2_STATUSREGISTER_F_TYPE_II_MASK)
    {
        PRINTF("Error: Type II error occurred\r\n");
    }

    if(status & ENDAT2P2_STATUSREGISTER_MRS_ADR_MASK)
    {
        PRINTF("Error: ACK of MRS error occurred\r\n");
    }

    if(status & ENDAT2P2_STATUSREGISTER_RECEIVE_REGISTER2_MASK ||
       status & ENDAT2P2_STATUSREGISTER_RECEIVE_REGISTER3_MASK)
    {
        if(status & ENDAT2P2_STATUSREGISTER_RECEIVE_REGISTER2_MASK)
        {
            PRINTF("AddINFO1 is updated\r\n");
        }
        if(status & ENDAT2P2_STATUSREGISTER_RECEIVE_REGISTER3_MASK)
        {
            PRINTF("AddINFO2 is updated\r\n");
        }
        PRINTF("AddINFO: ERR2:%d CRC1:%d CRC2:%d\r\n",
               NXP_FLD2VAL(ENDAT2P2_STATUSREGISTER_ERROR2, status),
               NXP_FLD2VAL(ENDAT2P2_STATUSREGISTER_CRC_ZI1, status),
               NXP_FLD2VAL(ENDAT2P2_STATUSREGISTER_CRC_ZI2, status));
        PRINTF("Encoder: Busy:%d RM:%d WRN:%d Spike:%d\r\n",
               NXP_FLD2VAL(ENDAT2P2_STATUSREGISTER_BUSY, status),
               NXP_FLD2VAL(ENDAT2P2_STATUSREGISTER_RM, status),
               NXP_FLD2VAL(ENDAT2P2_STATUSREGISTER_WRN, status),
               NXP_FLD2VAL(ENDAT2P2_STATUSREGISTER_SPIKE, status));

        if(status & ENDAT2P2_STATUSREGISTER_ERROR2_MASK)
        {
            PRINTF("Error: Error2 occurred\r\n");
        }

        if(status & ENDAT2P2_STATUSREGISTER_CRC_ZI1_MASK)
        {
            PRINTF("Error: CRC of additional info 1 is incorrect\r\n");
        }

        if(status & ENDAT2P2_STATUSREGISTER_CRC_ZI2_MASK)
        {
            PRINTF("Error: CRC of additional info 2 is incorrect\r\n");
        }

        if(status & ENDAT2P2_STATUSREGISTER_BUSY_MASK)
        {
            PRINTF("Encoder: is busy\r\n");
        }

        if(status & ENDAT2P2_STATUSREGISTER_RM_MASK)
        {
            PRINTF("Encoder: Reference run finished\r\n");
        }

        if(status & ENDAT2P2_STATUSREGISTER_WRN_MASK)
        {
            PRINTF("Encoder: has warning\r\n");
        }
        if(status & ENDAT2P2_STATUSREGISTER_SPIKE_MASK)
        {
            PRINTF("Encoder: spike occurred\r\n");
        }
    }
}

static void ENDATDEV_HandleRx(endat2p2_dev_t *dev, int cmd)
{
    endat2p2_recv_data_t data = {0};

    ENDAT2P2_RecvData(dev, cmd, &data);
    ENDATDEV_DumpRecvData(dev, cmd, &data);
    ENDATDEV_DumpStatus(dev, data.status);
}

static int ENDATDEV_InputMRS(int *mrs)
{
    PRINTF("input MRS code (hex value): ");

    if(SCANF("%x", mrs) < 0)
    {
        PRINTF("ERROR: MRS code should be hex value\r\n");
        return -1;
    }

    if(*mrs > 0xFF)
    {
        PRINTF("ERROR: MRS code 0x%x is incorrect \r\n", *mrs);
        return -1;
    }

    PRINTF("0x%x\r\n", *mrs);
    return 0;
}

static int ENDATDEV_InputAddr(int *addr)
{
    PRINTF("Input parameter address (hex value): ");

    if (SCANF("%x", addr) < 0)
    {
        PRINTF("ERROR: parameter address should be hex value\r\n");
        return -1;
    }

    if (*addr > 0xFF)
    {
        PRINTF("ERROR: parameter address 0x%x is incorrect\r\n", *addr);
        return -1;
    }

     PRINTF("0x%x\r\n", *addr);
    return 0;
}

static int ENDATDEV_InputParam(int *param)
{
    PRINTF("Input parameter (hex value): ");

    if (SCANF("%x\n", param) < 0)
    {
        PRINTF("ERROR: parameter should be hex value\r\n");
        return -1;
    }

    if (*param > 0xFF)
    {
        PRINTF("ERROR: parameter address 0x%x is incorrect\r\n", *param);
        return -1;
    }

    PRINTF("0x%x\r\n", *param);

    return 0;
}

static int ENDATDEV_InputFrequency(int *frequency)
{
    PRINTF("Input Frequency(Hz): ");

    if (SCANF("%u\n", &frequency) < 0)
    {
        PRINTF("ERROR: invalid value\r\n");
        return -1;
    }

    PRINTF("%d\r\n", *frequency);

    return 0;
}

static int ENDATDEV_InputLoopTime(int *loop_time)
{
    PRINTF("Input Loop Time(us): ");

    if (SCANF("%u\n", loop_time) < 0)
    {
        PRINTF("ERROR: invalid value\r\n");
        return -1;
    }

    PRINTF("%d\r\n", *loop_time);

    return 0;
}

static int ENDATDEV_ProcessModeCommand(endat2p2_dev_t *dev,
                                       endat2p2_mode_cmd_t cmd)
{
    int msr_or_addr = 0, param = 0;

    switch(cmd)
    {
        case ENDAT2P2_CMD_SELECT_MEM_AREA:
             if (ENDATDEV_InputMRS(&msr_or_addr))
                return -1;

            break;

        case ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO_SEL_MEM:
            if (ENDATDEV_InputMRS(&msr_or_addr))
                return -1;

            if(msr_or_addr == MRS_CODE_PARAM_SECTION2_MEMORY_AREA)
            {
                if (ENDATDEV_InputParam(&param))
                    return -1;
            }

            break;

        case ENDAT2P2_CMD_RECEIVE_PARAMETERS:
        case ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO_RECV_PARAM:
            if (ENDATDEV_InputAddr(&msr_or_addr))
                return -1;

            if (ENDATDEV_InputParam(&param))
                return -1;

            break;

        case ENDAT2P2_CMD_SEND_PARAMETERS:
        case ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO_SEND_PARAM:
            if (ENDATDEV_InputAddr(&msr_or_addr))
                return -1;

            break;

        case ENDAT2P2_CMD_RECEIVE_TEST_COMMAND:
        case ENDAT2P2_CMD_SEND_POSVAL_WITH_ADDINFO_RECV_TESTCMD:
            if (ENDATDEV_InputAddr(&msr_or_addr))
                return -1;

            break;

        case ENDAT2P2_CMD_RECEIVE_COMMUNICATION_CMD:
            if (ENDATDEV_InputAddr(&msr_or_addr))
                return -1;

            if (ENDATDEV_InputParam(&param))
                return -1;

            break;

        default:
            break;
    }

    ENDAT2P2_CMDProcess(dev, cmd,
                        (uint8_t) msr_or_addr, (uint16_t) param);
    return 0;
}

static int ENDATDEV_ConfigureClock(endat2p2_dev_t *dev)
{
    int frequency = 0;

    ENDATDEV_InputFrequency(&frequency);

    if (ENDAT2P2_SetFTCLOCK(dev, frequency) < 0)
    {
        PRINTF("ERROR: frequency %d is not supported\r\n", frequency);
        return -1;
    }

    return 0;
}

void ENDAT2P2_IRQHandler(void)
{
    PRINTF("Interrupt Handler:\r\n");
    ENDATDEV_HandleRx(dev, ENDAT2P2_CMD_SEND_POSITION_VALUE);

    SDK_ISR_EXIT_BARRIER;
}

static void ENDATDEV_EnableInterrupt(endat2p2_dev_t *dev)
{
    ENDAT2P2_SetInterruptMask(dev,
        ENDAT2P2_INTERRUPTMASKREGISTER_RECEIVE_REGISTER1_MASK_MASK);
    EnableIRQ(ENDAT2P2_IRQn);
}

static void ENDATDEV_DisableInterrupt(endat2p2_dev_t *dev)
{
    ENDAT2P2_SetInterruptMask(dev, 0);
    DisableIRQ(ENDAT2P2_IRQn);
}

static int ENDATDEV_PositionLoop(endat2p2_dev_t *dev)
{
    int i, loop = 10, loop_time = 0;
    /* uint32_t tick, us; */
    endat2p2_recv_data_t data = {0};
    endat2p2_mode_cmd_t cmd = ENDAT2P2_CMD_SEND_POSITION_VALUE;

    ENDATDEV_InputLoopTime(&loop_time);

    /* BOARD_SystickStart(&tick); */
    /* BOARD_SystickElapsedTime_us(&tick); */

    /* reset additional info's if present */
    ENDAT2P2_EncoderRest(dev);

    ENDAT2P2_CMDBuild(dev, cmd, 0, 0);

    ENDAT2P2_CleanStatus(dev);
    for (i = 1; i <= loop; i++)
    {
        /* BOARD_SystickStart(&tick); */
        ENDAT2P2_CMDSend(dev);
        if(loop_time)
        {
            SDK_DelayAtLeastUs(loop_time, SystemCoreClock);
        }
        ENDAT2P2_CMDWait(dev);
        ENDAT2P2_RecvData(dev, cmd, &data);
        /* us = BOARD_SystickElapsedTime_us(&tick); */

        /* PRINTF("Loop%d: interval time:%dus\r\n", i, us); */
        ENDATDEV_DumpRecvData(dev, cmd, &data);
    }

    return 0;
}

void ENDATDEV_GetPerformance(int loop)
{
    int cnt, i;
    uint32_t count, count1, count2, count3;
    uint64_t time, time1, time2, time3;
    endat2p2_mode_cmd_t cmd = ENDAT2P2_CMD_SEND_POSITION_VALUE;

    BOARD_InitSysTick();

    cnt = 10000;
    PRINTF("\r\nStart to test Master's register reading\r\n");
    SYSTICK_StartCount();
    for(i = 0; i < cnt; i++)
    {
        count1 = dev->base->RECEIVEREGISTER1_1;
    }
    count = SYSTICK_GetCount();
    count = count / cnt;
    time = (uint64_t) count * 1000000000 / SystemCoreClock; /* ns */
    PRINTF("Read a 32bits register takes clock count:%u time:%dns\r\n",
            count, (uint32_t) time);

    PRINTF("\r\nStart to test Master's register Writing\r\n");
    SYSTICK_StartCount();
    for(i = 0; i < cnt; i++)
    {
        dev->base->SENDREGISTER = 0;
    }
    count = SYSTICK_GetCount();
    count = count / cnt;
    time = (uint64_t) count * 1000000000 / SystemCoreClock; /* ns */
    PRINTF("Write a 32bits register takes clock count:%u time:%dns\r\n",
            count, (uint32_t) time);

    PRINTF("\r\nStart to test loop mode performance\r\n");

    /* reset additional info's if present */
    ENDAT2P2_EncoderRest(dev);

    ENDAT2P2_CMDBuild(dev, cmd, 0, 0);

    ENDAT2P2_CleanStatus(dev);

    for(i = 0; i < loop; i++)
    {
        SYSTICK_StartCount();
        ENDAT2P2_CMDSend(dev);
        while(!ENDAT2P2_CheckRecv(dev))
        {
            ;
        }
        count1 = SYSTICK_GetCount();
        ENDAT2P2_CMDWait(dev);
        count2 = SYSTICK_GetCount();
        ENDAT2P2_GetRecvReg1(dev);
        ENDAT2P2_CleanStatus(dev);
        count3 = SYSTICK_GetCount();
        PRINTF("count1:%d count2:%d count3:%d\r\n",
                count1, count2, count3);
        time1 = (uint64_t) count1 * 1000000000 / SystemCoreClock; /* us */
        time2 = (uint64_t) count2 * 1000000000 / SystemCoreClock; /* us */
        time3 = (uint64_t) count3 * 1000000000 / SystemCoreClock; /* us */
        PRINTF("A frame receiving takes count:%u time:%dns\r\n",
                count1, (uint32_t) time1);
        PRINTF("A frame cycle takes count:%u time:%dns\r\n",
                count2, (uint32_t) time2);
        PRINTF("A frame handle takes count:%u time:%dns\r\n",
                count3 - count2, (uint32_t) (time3 - time2));
    }
}

static int ENDATDEV_TimerLoop(endat2p2_dev_t *dev)
{
    int i, loop = 10, loop_time = 0;
    /* uint32_t tick, us; */
    endat2p2_recv_data_t data = {0};
    endat2p2_mode_cmd_t cmd = ENDAT2P2_CMD_SEND_POSITION_VALUE;

    ENDATDEV_InputLoopTime(&loop_time);

    if(loop_time <= 2 || loop_time > 25400)
    {
        PRINTF("ERROR: Loop time should be 2 - 25400 us\r\n");
        return 0;
    }

    /* reset additional info's if present */
    ENDAT2P2_EncoderRest(dev);
    SDK_DelayAtLeastUs(500U, SystemCoreClock);
    ENDAT2P2_CMDBuild(dev, cmd, 0, 0);

    ENDAT2P2_CleanStatus(dev);
    /* BOARD_SystickStart(&tick); */
    ENDAT2P2_SetSamplingRate(dev, loop_time);

    for(i = 1; i <= loop; i++)
    {
        SDK_DelayAtLeastUs(loop_time, SystemCoreClock);
        ENDAT2P2_CMDWait(dev);
        ENDAT2P2_RecvData(dev, cmd, &data);
        /* us = BOARD_SystickElapsedTime_us(&tick); */

       /*  PRINTF("Loop%d: interval time:%dus\r\n", i, us); */
        ENDATDEV_DumpRecvData(dev, cmd, &data);
    }

    ENDAT2P2_SetSamplingRate(dev, 0);
    return 0;
}

static void ENDAT2P2_EnableXbarPinTrigger(void)
{
    XBAR_Init(kXBAR_DSC1);
    XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux1Trigger0, kXBAR1_OutputEndat21StrN);
}

static int ENDATDEV_HardwareStrobeLoop(endat2p2_dev_t *dev)
{
    endat2p2_mode_cmd_t cmd = ENDAT2P2_CMD_SEND_POSITION_VALUE;
    int i, loop = 10;
    endat2p2_recv_data_t data = {0};

    /* Initialize FlexPWM to generate the trigger signalis. */
    PWM_Trigger_Init(BOARD_PWM_BASEADDR);

    ENDAT2P2_EnableXbarPinTrigger();

    /* reset additional info's if present */
    ENDAT2P2_EncoderRest(dev);
    SDK_DelayAtLeastUs(500U, SystemCoreClock);
    ENDAT2P2_CMDBuild(dev, cmd, 0, 0);

    ENDAT2P2_CleanStatus(dev);
    ENDAT2P2_SetHWStrobe(dev, true);

    /* ENDATDEV_EnableInterrupt(dev); */

    for(i = 1; i <= loop; i++)
    {
        while(!ENDAT2P2_CheckRecv(dev))
        {
            SDK_DelayAtLeastUs(100, SystemCoreClock);
        }
        ENDAT2P2_RecvData(dev, cmd, &data);
        ENDATDEV_DumpRecvData(dev, cmd, &data);
    }

    ENDAT2P2_SetHWStrobe(dev, false);

    return 0;
}

/*!
 * @brief Main function
 */
int main(void)
{
    int ret;
    int frequency, menu_cmd, mode_cmd;
    int data;

    BOARD_InitHardware();

	PRINTF("Start ENDAT2.2 Diagnostic application\r\n");

    PRINTF("ENDAT2P2_SYS_CLOCK %d\r\n", ENDAT2P2_SYS_CLOCK);
    dev = ENDAT2P2_InitMaster(ENDAT2P2_BASE, ENDAT2P2_SYS_CLOCK);
    if(!dev)
    {
        PRINTF("EnDat2.2 0x%x failed to initialize\r\n");
        return 0;
    }

    ret = ENDAT2P2_InitEncoder(dev);
    if (ret != kStatus_Success)
    {
        PRINTF("\r\n EnDat2.2 gets encoder info Error!\r\n");
        return 0;
    }

    ENDATDEV_PrintEncoderInfo(dev);

    ENDAT2P2_EnableDelayCompensation(dev);
    PRINTF("propagation time: %d fsysclk\r\n",
            ENDAT2P2_GetCablePropagationTime(dev));

    /* default frequency - 2MHz for 2.2 encoders, 1MHz for 2.1 encoders */
    if(dev->cmd_set_2_2)
    {
        frequency = ENDAT2P2_FTCLK;
        ENDAT2P2_SetFTCLOCK(dev, frequency);

        PRINTF("For EnDat2.2, change recovery time I to 3.75us\r\n");
        /* Change tm to 3.75us */
        data = ENDAT2P2_GetParamWithPos(dev, MRS_CODE_OPERATING_STATUS,
                                        ENDAT2P2_MEM_WORD_3);
        data = (data & (~3)) | 0x1;

        ENDAT2P2_SetParamWithPos(dev, MRS_CODE_OPERATING_STATUS,
                          ENDAT2P2_MEM_WORD_3, data);
        ENDAT2P2_EncoderRestWithPos(dev);

        data = ENDAT2P2_GetParamWithPos(dev, MRS_CODE_OPERATING_STATUS,
                                        ENDAT2P2_MEM_WORD_3);

        ENDAT2P2_SetRecoveryTimer(dev, 0);
    }
    else
    {
        frequency = ENDAT2P2_CLK_1M;
        ENDAT2P2_SetFTCLOCK(dev, frequency);
    }

    ret = ENDAT2P2_GetEncoderError(dev);
    PRINTF("Device Error: 0x%x\r\n", ret);

    ret = ENDAT2P2_GetEncoderWarning(dev);
    PRINTF("Device Warning: 0x%x\r\n", ret);

	ENDATDEV_DumpRegs(dev);

    if(dev->cmd_set_2_2)
    {
        ENDAT2P2_ClearEncoderErrorWithPos(dev);
        ret = ENDAT2P2_GetEncoderErrorWithPos(dev);
        PRINTF("ENDAT2P2_GetEncoderErrorWithPos: 0x%x\r\n", ret);

        ret = ENDAT2P2_GetParamWithPos(dev,
                                MRS_CODE_PARAM_ENCODER_MANUFACTURER_PAGE1,
                                ENDAT2P2_MEM_WORD_1);
        PRINTF("ENDAT2P2_GetParamWithPoS: get multiturn: %d\r\n", ret);

        /* Endat 2.2 */
        if(dev->status_addinfo1 & ENDAT2P2_ADDINFO1_STATUS_POS2_MASK)
            PRINTF("PosVal2:%d\r\n", (uint32_t)ENDAT2P2_GetPosVal2(dev));

        if(dev->status_addinfo1 & ENDAT2P2_ADDINFO1_STATUS_TEMP1_MASK)
            PRINTF("Temperature1:%d\r\n", (int)ENDAT2P2_GetTemperature1(dev));
        if(dev->status_addinfo1 & ENDAT2P2_ADDINFO1_STATUS_TEMP1_MASK)
            PRINTF("Temperature2:%d\r\n", (int)ENDAT2P2_GetTemperature2(dev));

        if(dev->status_addinfo2 & ENDAT2P2_ADDINFO2_STATUS_OPTERRSRC_MASK)
            PRINTF("OPT Err SRC:0x%x\r\n", ENDAT2P2_GetOPSERRSRC(dev));
    }

    while (1)
    {
        ENDATDEV_PrintMenu();

        menu_cmd = 0;
		SCANF("%d", &menu_cmd);
        PRINTF("%d\r\n", menu_cmd);
        if(menu_cmd >= 1 && menu_cmd <= 14)
        {
            mode_cmd = menu_cmd_to_mode_cmd[menu_cmd];
            ENDATDEV_ProcessModeCommand(dev, mode_cmd);
            ENDATDEV_HandleRx(dev, mode_cmd);
            continue;
        }

        if(menu_cmd == 100)
        {
            ENDATDEV_PrintEncoderInfo(dev);
            continue;
        }

        if(menu_cmd == 101)
        {
            ENDATDEV_DumpRegs(dev);
            continue;
        }

        if(menu_cmd == 102)
        {
            ENDATDEV_ConfigureClock(dev);
            continue;
        }

        if(menu_cmd == 103)
        {
            ENDAT2P2_EnableDelayCompensation(dev);
            continue;
        }

        if(menu_cmd == 104)
        {
            PRINTF("Enable interrupt\r\n");
            ENDATDEV_EnableInterrupt(dev);
            continue;
        }

        if(menu_cmd == 105)
        {
            PRINTF("Disable interrupt\r\n");
            ENDATDEV_DisableInterrupt(dev);
            continue;
        }

        if(menu_cmd == 200)
        {
            PRINTF("Software loop to receive position\r\n");
            ENDATDEV_PositionLoop(dev);
            continue;
        }

        if(menu_cmd == 201)
        {
            PRINTF("Timer loop to receive position\r\n");
            ENDATDEV_TimerLoop(dev);
            continue;
        }

        if(menu_cmd == 202)
        {
            PRINTF("Hardware strobe to receive position\r\n");
            ENDATDEV_HardwareStrobeLoop(dev);
            continue;
        }
        if(menu_cmd == 203)
        {
            PRINTF("Test the performance\r\n");
            ENDATDEV_GetPerformance(10);
            continue;
        }
    }

    PRINTF("Exit ENDAT2.2 Diagnostic application\r\n");

    return 0;
}
