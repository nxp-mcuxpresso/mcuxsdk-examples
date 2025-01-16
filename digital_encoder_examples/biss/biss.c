/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_biss.h"
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

biss_master_t *master;

BLK_CTRL_WAKEUPMIX_Type *blk_ctrl = BLK_CTRL_WAKEUPMIX;

/* MB4 register values */
unsigned char MB4_REG[20] = {
    /*
     * RESO_ST 0x01 bits 4:6 4 -> 12bits - singleturn
     * RESO_MT 0x01 bits 0:2 3 -> 12bits - multiturn
     * EBL_MTI 0x02 bits 4:6 1 -> 1 Error bit
     * CFG_IOP 0x03 bit 7  0 0-> biss C serial mode
     * MT12 0x03 bit 2 0 ->defined by RESO_MT
     */
    0x48, 0x43, 0x00, 0x1D, /* 0x0 - 0x3 */
    /* DISBISS 0x7 - bit 5 0 ->BiSS Enable */
    0x00, 0x00, 0x00, 0x00, /* 0x4 - 0x7 */
    /* 0x0b - 0 -> allow biss reset */
    0x00, 0x80, 0x00, 0x00, /* 0x8 - 0xB */
    0x00, 0x00, 0x00, 0x00, /* 0xC - 0xF */
    0x00, 0x00, 0x00, 0x00  /* 0x10 - 0x13 */
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/* MB4 evaluation board driver */
void MB4_UpdateCRC()
{
    int iCRCPoly = 0x11D; /* CRC-Polynomial 100011101 */
    int iReg = 0;
    unsigned char ucDataStream = 0;
    unsigned char ucCRC;

    /* Calculate Config-CRC */
    ucCRC = 2; /* start value */

    for (iReg = 0 ; iReg < 12; iReg ++) {
        ucDataStream = MB4_REG[iReg];
        for (int i =0; i <= 7; i ++) {
            if ( (ucCRC & 0x80) != (ucDataStream & 0x80))
                ucCRC = (ucCRC << 1 ) ^ iCRCPoly ;
            else
                ucCRC = (ucCRC << 1 ) ;
            ucDataStream = ucDataStream << 1 ;
        }
    }

    MB4_REG[12] = ucCRC;

    /* Calculate Offset-CRC */
    ucCRC = 2; /* start value */
    for (int iReg = 13 ; iReg < 19; iReg ++) {
        ucDataStream = MB4_REG[iReg];
        for (int i =0; i <=7; i ++) {
            if ( (ucCRC & 0x80) != (ucDataStream & 0x80))
                ucCRC = (ucCRC << 1 ) ^ iCRCPoly ;
            else
                ucCRC = (ucCRC << 1 ) ;
            ucDataStream = ucDataStream << 1 ;
        }
    }

    MB4_REG[19] = ucCRC;
}

void MB4_ConfigureSlave(biss_master_t *master, int slvID)
{
    for(int i = 0; i < 20; i++) {
        BISS_SLVWriteRegister(master, slvID, i, 1, &MB4_REG[i]);
    }
}

void MB4_Init(biss_master_t *master)
{
    uint8_t data = 0;

    MB4_UpdateCRC();

    MB4_ConfigureSlave(master, 0);
    PRINTF("reset biss slave 1 \r\n");
    data = 0x1;
    BISS_SLVWriteRegister(master, 0, 0x74, 1, &data);

    MB4_ConfigureSlave(master, 1);
    PRINTF("reset biss slave 2 \r\n");
    data = 0x1;
    BISS_SLVWriteRegister(master, 1, 0x74, 1, &data);
}

void BISS_performance(int loop)
{
    uint32_t status;
    uint64_t SCData;
    uint32_t st = 0, ed = 0, err = 0, eot_err = 0;
    int slvID = 1;

    BISS_ChangeTriggerMode(master, BISS_INSTR_TRIGGER);

    int cnt = loop;

    PRINTF("Start to test performance\r\n");

    /* st = STM_GetTime();  */
    BISS_InstrSendandWait(master, BISS_INSTR_CDM_0, BISS_AGS_DISABLE);
    while(loop--)
    {
        BISS_InstrSendandWait(master, BISS_INSTR_CDM_0, BISS_AGS_DISABLE);
        status  = BISS_GetStatus(master);

        if(!(status & 0x1))
            eot_err++;
        if(!(status & BISS_STATUS1_SVALID_MASK(slvID)))
            err++;

        status = BISS_SLVGetSCD(master, slvID, &SCData);
        if (status != kStatus_Success)
            err++;
    }
    /* ed = STM_GetTime(); */

    PRINTF("Read %d frames take %dus\r\n", cnt, ed - st);
    PRINTF("each frame takes %dus\r\n", (ed - st)/cnt);
    PRINTF("There are err:%d EOT_err:%d\r\n", err, eot_err);
}

static void BISS_DumpRegs(biss_master_t *master)
{
    PRINTF("\r\nSCD: \t\t0x%08x 0x%08x\r\n",
           master->base->SCDATA1_LOW, master->base->SCDATA1_HIGH);
    PRINTF("RDATA1: \t0x%08x\r\n", master->base->RDATA1);
    PRINTF("CONFIGSL1: \t0x%08x\r\n", master->base->CONFIGSL1);
    PRINTF("CTRLCOMM1: \t0x%08x\r\n", master->base->CTRLCOMM1);
    PRINTF("CTRLCOMM2: \t0x%08x\r\n", master->base->CTRLCOMM2);
    PRINTF("MACONFIG: \t0x%08x\r\n", master->base->MACONFIG);
    PRINTF("CHCONFIG2: \t0x%08x\r\n", master->base->BISSINTDATACHCONFIG2);
    PRINTF("STATUS1: \t0x%08x\r\n", master->base->STATUS1);
    PRINTF("DACQ: \t\t0x%08x\r\n", master->base->DACQ);
    PRINTF("STATUS2: \t0x%08x\r\n", master->base->STATUS2);
}

static void BISS_SLVDumpInfo(biss_master_t *master, int slvID)
{
    biss_slave_info_t *slv;

    slv = BISS_SLVGet(master, slvID);

    PRINTF("device ID: %d %d %d %d %d %d\r\n",
           slv->did[0], slv->did[1], slv->did[2],
           slv->did[3], slv->did[4],slv->did[5]);
    PRINTF("Manufacure ID: %d\r\n", slv->mid);
    PRINTF("SN: %d\r\n", slv->sn);
    PRINTF("Common EDS: %d\r\n", slv->commEDS);
    PRINTF("Profile: 0x%x\r\n", slv->profile);
    PRINTF("DataType: %d\r\n", slv->dataType);
    PRINTF("Data length: %d\r\n", slv->dataLen);
    PRINTF("Multiple turn length: %d\r\n", slv->mtLen);
    PRINTF("Single turn resolution: %d\r\n", slv->stLen);
    PRINTF("CRC length: %d\r\n", slv->crcLen);
}

static uint64_t BISSSLV_GetMtVal(uint64_t position)
{
    return (position >> BISS_DEVICE_ST_LEN) & (((uint64_t) 1 << BISS_DEVICE_MT_LEN) - 1);
}

static uint64_t BISSSLV_GetStVal(uint64_t position)
{
    return (position) & (((uint64_t) 1 << BISS_DEVICE_ST_LEN) - 1);
}

static void BISS_SLVDumpPosition(biss_master_t *master, uint8_t slvID)
{
    uint64_t position;

    position = BISS_SLVGetSCDRawData(master, slvID);

    position = (position & (((uint64_t) 1 << (BISS_DEVICE_DATA_LEN)) - 1)) >> 2;

    PRINTF("Multiturn value: %u, Singleturn value: %u\r\n",
            (uint32_t)BISSSLV_GetMtVal(position),
            (uint32_t)BISSSLV_GetStVal(position));
}

static void BISS_EnableXbarPinTrigger(void)
{
    XBAR_Init(kXBAR_DSC1);
    XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux1Trigger0, kXBAR1_OutputBissGetsens);
}

void BISS_SOT_IRQHandler(void)
{
    /* clear SOT interrupt */
    blk_ctrl->BISS1_EOT_CTL =
        BLK_CTRL_WAKEUPMIX_BISS1_EOT_CTL_biss_eot_fall_clr_int_b(1) | 3;

    PRINTF("BISS_SOT_IRQHandler %x\r\n", blk_ctrl->BISS1_EOT_STATUS);

    SDK_ISR_EXIT_BARRIER;
}

void BISS_EOT_IRQHandler(void)
{
    /* clear EOT interrupt */
    blk_ctrl->BISS1_EOT_CTL =
        BLK_CTRL_WAKEUPMIX_BISS1_EOT_CTL_biss_eot_rise_clr_int_b(1) | 3;

    PRINTF("BISS_EOT_IRQHandler status 0x%08x\r\n", BISS_GetStatus(master));

    BISS_SLVDumpPosition(master, 0);

    blk_ctrl->BISS1_EOT_CTL = 3;

    SDK_ISR_EXIT_BARRIER;
}

static void BISS_DisableInterrupt(void)
{
    DisableIRQ(BISS_EOT_IRQn);
    DisableIRQ(BISS_SOT_IRQn);
    blk_ctrl->BISS1_EOT_CTL = 0;
}

static void BISS_EnableInterrupt(void)
{
    EnableIRQ(BISS_EOT_IRQn);

    blk_ctrl->BISS1_EOT_CTL = 0x3;
}

/*!
 * @brief Main function
 */
int main(void)
{
    status_t status;
    uint8_t slvID;
    char inputChar;
    biss_slave_info_t *slv;

    BLK_CTRL_WAKEUPMIX_Type *blk_ctrl = BLK_CTRL_WAKEUPMIX;

    BOARD_InitHardware();

    PRINTF("\r\nThis example use one board as BiSS master and connect to the BiSS encoder.\r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("   MA       --   BISS Clock Line Output\r\n");
    PRINTF("   MO       --   BISS Data Line Output\r\n");
    PRINTF("   SL       --   BISS Data Line Input\r\n");

    BISS_DisableInterrupt();

    master = BISS_MasterInit(BISS_BASE, BISS_SYS_CLK_FREQ,
                             BISS_MA_CLK_FREQ, BISS_AGS_CLK_FREQ);
    if (master == NULL)
    {
        PRINTF("\r\n BISS_Master Init Error!\r\n");
        return -1;
    }
    SDK_DelayAtLeastUs(100U, SystemCoreClock);

    BISS_InitBissSequence(master);
    SDK_DelayAtLeastUs(400U, SystemCoreClock);

    /* Broadcast Active all slaves */
    BISS_CMDProcess(master, BISS_CMD_IDS_BOARDCAST,
                    BISS_CMD_BOARDCAST_CTRL_ACTIVATED);
    SDK_DelayAtLeastUs(400U, SystemCoreClock);

    /* If use MB4 Evaluation board, please all this function to init */
    /* MB4_Init(master); */

    status = BISS_SLVScan(master);
    if (status != kStatus_Success)
    {
        PRINTF("\r\n BISS_ScanSlave Error!\r\n");
    }

    PRINTF("Find %d BiSS Slave devices\r\n", master->slvCnt);
    for (slvID = 0; slvID < master->slvCnt; slvID++)
    {
        slv = BISS_SLVGet(master, slvID);
        BISS_SLVDumpInfo(master, slvID);
        if (slv->dataLen == 0)
            slv->dataLen = BISS_DEVICE_DATA_LEN;
        if (slv->crcLen == 0)
            slv->crcLen = BISS_DEVICE_CRC_LEN;

        /* Disable automatically initialize the slaves */
        /* BISS_SLVSetup(master, slvID); */
    }

    /* Manually initialize the slaves */
    BISS_SLVSetSCD(master, 0, BISS_DEVICE_DATA_LEN, BISS_DEVICE_CRC_LEN);

    BISS_DumpRegs(master);

    while (1)
    {
        PRINTF("\r\nSelect the encoder command from following:\r\n");
        PRINTF("1: Dump slave position\r\n");
        PRINTF("2: Dump registers\r\n");
        PRINTF("3: Enable instruction trigger\r\n");
        PRINTF("4: Enable AGS repetition trigger\r\n");
        PRINTF("5: Enable timeout trigger\r\n");
        PRINTF("6: GETSENS pin trigger\r\n");
        PRINTF("7: Reset BiSS-C\r\n");
        PRINTF("8: Re-scan BiSS bus\r\n");
        PRINTF("9: Dump slave information\r\n");
        PRINTF("Input command values: ");

        inputChar = GETCHAR();

        PRINTF("%c\r\n", inputChar);

        if ('1' == inputChar)
        {
           BISS_SLVDumpPosition(master, 0);
        }
        else if ('2' == inputChar)
        {
            BISS_DumpRegs(master);
        }
        else if ('3' == inputChar)
        {
            BISS_ChangeTriggerMode(master, BISS_INSTR_TRIGGER);
            BISS_InstrSend(master, BISS_INSTR_CDM_0, BISS_AGS_DISABLE);
            BISS_SLVDumpPosition(master, 0);
        }
        else if ('4' == inputChar)
        {
            BISS_ChangeTriggerMode(master, BISS_AGS_TRIGGER);
            BISS_InstrSend(master, BISS_INSTR_CDM_0, BISS_AGS_ENABLE);
            BISS_SLVDumpPosition(master, 0);
        }
        else if ('5' == inputChar)
        {
            BISS_ChangeTriggerMode(master, BISS_TIMEOUT_TRIGGER);
            BISS_InstrSend(master, BISS_INSTR_CDM_0, BISS_AGS_ENABLE);
            BISS_SLVDumpPosition(master, 0);
        }
        else if ('6' == inputChar)
        {
            BISS_ChangeTriggerMode(master, BISS_PIN_TRIGGER);

            /* Initialize FlexPWM to generate the trigger signalis. */
            PWM_Trigger_Init(BOARD_PWM_BASEADDR);

            BISS_EnableXbarPinTrigger();

            BISS_EnableInterrupt();
        }
        else if ('7' == inputChar)
        {
            BISS_DisableInterrupt();
            BISS_InitBissSequence(master);
        }
        else if ('8' == inputChar)
        {
           BISS_SLVScan(master);
            PRINTF("Find %d BiSS Slave devices\r\n", master->slvCnt);
        }
        else if ('9' == inputChar)
        {
            for (slvID = 0; slvID < master->slvCnt; slvID++)
            {
                slv = BISS_SLVGet(master, slvID);
                BISS_SLVDumpInfo(master, slvID);
            }
        }
    }

    return 0;
}
