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

static void BISS_SLVDumpInfo(biss_master_t *master, uint8_t slvID)
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

void BISS_IRQHandler(void)
{
    uint32_t count;
    uint64_t time;

    if (performance_enable)
    {
        count = SYSTICK_GetCount();
        time = (uint64_t) count * 1000000 / SystemCoreClock;
        PRINTF("A frame take count:%u time:%dus\r\n", count, (uint32_t) time);
    }
    PRINTF("BISS_IRQHandler status 0x%08x\r\n", BISS_GetStatus(master));

    BISS_SLVDumpPosition(master, 0);

    BISS_IRQ_Clear();

    SDK_ISR_EXIT_BARRIER;
}

static void BISS_DisableInterrupt(void)
{
    DisableIRQ(BISS_IRQn);
    BISS_IRQ_Clear();
}

static void BISS_EnableInterrupt(void)
{
    EnableIRQ(BISS_IRQn);
}

void BISS_performance(int loop)
{
    uint32_t status;
    uint64_t SCData, time;
    uint32_t count, cnt, i;
    int slvID = 0;

    BOARD_InitSysTick();

    BISS_ChangeTriggerMode(master, BISS_INSTR_TRIGGER);

    cnt = 10000;

    PRINTF("\r\nStart to test Master's register reading\r\n");
    SYSTICK_StartCount();
    for(i = 0; i < cnt; i++)
    {
        status = master->base->SCDATA1_LOW;
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
        master->base->RDATA1 = 0x12345678;
    }
    count = SYSTICK_GetCount();
    count = count / cnt;
    time = (uint64_t) count * 1000000000 / SystemCoreClock; /* ns */
    PRINTF("Write a 32bits register takes clock count:%u time:%dns\r\n",
            count, (uint32_t) time);

    PRINTF("\r\nStart to test loop mode performance\r\n");

    BISS_InstrSendandWait(master, BISS_INSTR_CDM_0, BISS_AGS_DISABLE);

    for(i = 0; i < loop; i++)
    {
        SYSTICK_StartCount();
        BISS_InstrSendandWait(master, BISS_INSTR_CDM_0, BISS_AGS_DISABLE);

        status  = BISS_GetStatus(master);
        SCData = BISS_SLVGetSCDRawData(master, slvID);

        count = SYSTICK_GetCount();
        time = (uint64_t) count * 1000000 / SystemCoreClock; /* us */

        PRINTF("A frame takes count:%u time:%dus status 0x%x SCDData 0x%x\r\n",
               count, (uint32_t) time, status, (uint32_t) SCData);
    }

    PRINTF("Start to test Interrupt mode performance\r\n");
    BISS_EnableInterrupt();
    performance_enable = true;

    for(i = 0; i < loop; i++)
    {
        SYSTICK_StartCount();
        BISS_InstrSendandWait(master, BISS_INSTR_CDM_0, BISS_AGS_DISABLE);
        SDK_DelayAtLeastUs(100U, SystemCoreClock);
    }
    BISS_DisableInterrupt();
    performance_enable = false;
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
        PRINTF("6: Enable GETSENS pin trigger. Press \"7\" to cancel\r\n");
        PRINTF("7: Reset BiSS-C\r\n");
        PRINTF("8: Re-scan BiSS bus\r\n");
        PRINTF("9: Dump slave information\r\n");
        PRINTF("0: Test Performance\r\n");
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
            /* Wait for the response to be received */
            SDK_DelayAtLeastUs(1000U, SystemCoreClock);
            BISS_SLVDumpPosition(master, 0);
        }
        else if ('4' == inputChar)
        {
            BISS_ChangeTriggerMode(master, BISS_AGS_TRIGGER);
            BISS_InstrSend(master, BISS_INSTR_CDM_0, BISS_AGS_ENABLE);
            /* Wait for the response to be received */
            SDK_DelayAtLeastUs(1000U, SystemCoreClock);
            BISS_SLVDumpPosition(master, 0);
        }
        else if ('5' == inputChar)
        {
            BISS_ChangeTriggerMode(master, BISS_TIMEOUT_TRIGGER);
            BISS_InstrSend(master, BISS_INSTR_CDM_0, BISS_AGS_ENABLE);
            /* Wait for the response to be received */
            SDK_DelayAtLeastUs(1000U, SystemCoreClock);
            BISS_SLVDumpPosition(master, 0);
        }
        else if ('6' == inputChar)
        {
            BISS_ChangeTriggerMode(master, BISS_PIN_TRIGGER);

            /* Initialize FlexPWM to generate the trigger signalis. */
            PWM_Trigger_Init(BOARD_PWM_BASEADDR);

            BISS_EnableInterrupt();
        }
        else if ('7' == inputChar)
        {
            BISS_DisableInterrupt();
            BISS_InitBissSequence(master);
        }
        else if ('8' == inputChar)
        {
            BISS_ChangeTriggerMode(master, BISS_TIMEOUT_TRIGGER);
            BISS_SLVScan(master);
            PRINTF("Find %d BiSS Slave devices\r\n", master->slvCnt);
            BISS_ChangeTriggerMode(master, BISS_INSTR_TRIGGER);
        }
        else if ('9' == inputChar)
        {
            for (slvID = 0; slvID < master->slvCnt; slvID++)
            {
                slv = BISS_SLVGet(master, slvID);
                BISS_SLVDumpInfo(master, slvID);
            }
        }
        else if ('0' == inputChar)
        {
           BISS_performance(10);
        }
    }
}
