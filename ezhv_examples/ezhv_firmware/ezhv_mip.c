/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "ezhv_api.h"

#if (EZHV_MIP_EN)
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MIP_VST_GSP kEZHV_GPIO_Pin8
#define MIP_XRST_INTB kEZHV_GPIO_Pin9
#define MIP_VCK_GCK kEZHV_GPIO_Pin10
#define MIP_HCK_BCK kEZHV_GPIO_Pin11

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*! @brief Modify heartbeat timer
 *
 * @param cnt, heartbeat count number
 */
static inline void ModifyHeatBeat(uint32_t cnt)
{
    EZHV_WriteMiscReg(cnt);
}

/*! @brief Modify heartbeat timer, then sleep
 *
 * @param cnt, heartbeat count number
 */
static inline void HeatBeatDelay(uint32_t cnt)
{
    ModifyHeatBeat(cnt);
    wfi();
}

/*! @brief toggle gpio pin and then sleep for a given delay
 *
 * @param cnt, heartbeat count number
 * @param pin, which pin to be toggled
 */
static inline void ToggleWithDelay(uint16_t cnt, ezhv_gpio_pin_t pin)
{
    EZHV_PinToggle(pin);
    HeatBeatDelay(cnt);
}

/*! @brief sleep for a given delay and then toggle gpio pin
 *
 * @param cnt, heartbeat count number
 * @param pin, which pin to be toggled
 */
static inline void DelayWithToggle(uint16_t cnt, ezhv_gpio_pin_t pin)
{
    HeatBeatDelay(cnt);
    EZHV_PinToggle(pin);
}

static void EZHV_MiPInit(void)
{
    EZHV_PortDirSet(0);
    /* EZHV event manager */
    EZHV_DeInitEmCfg();
    /* normal event manager */
    EZHV_EnableEmNormalEvent(kEM_EventHeartBeat);
    EZHV_EnableEmWakeExitEn();
    EZHV_EnableEmEventAutoClear();
}
/*! @brief memory-in-pixel parallel interface
 */
void EZHV_MiP(uint32_t paraAddr)
{
    assert(0 != paraAddr);
    
    ezhv_para_mip_t *pstPara = (ezhv_para_mip_t*)paraAddr;
    float    cycle_ns; 
    uint32_t vstHold = pstPara->engine.vstHoldTime;
    uint32_t vckHigh = pstPara->engine.vckHighLevelTime;
    uint32_t s1Delay1 = (vckHigh > vstHold) ? (vstHold) : (vckHigh);
    uint32_t s1Delay2 = (vckHigh>vstHold) ? (vckHigh-vstHold) : (vstHold-vckHigh);
    uint32_t s1flag   = (vckHigh > vstHold) ? (0) : (1);

    if (0 != pstPara->freqHz)
    {
        cycle_ns = 1000000000.0 / (float)pstPara->freqHz;
    }
    else
    {
        XZMSG_IndIntChan(1);
        return;
    }

    uint32_t mclkCntPerUs = (uint32_t)(1000.0/cycle_ns);
    uint32_t delayPer10Cycles = (uint32_t)(10.0*cycle_ns); /* window for 10 mclks */

    uint32_t startLine = pstPara->startline;
    uint32_t endLine = pstPara->endline;
    uint32_t dataWidth = pstPara->stride;
    uint32_t panelHight = pstPara->panelHeight;
    uint8_t *pbuf = (uint8_t *)pstPara->pdmaStart;
    uint32_t hckCycle = pstPara->engine.hckHighLevelTime*10/delayPer10Cycles/2;
    uint32_t u32tdHst = pstPara->engine.tdHst*10/delayPer10Cycles;
    uint32_t s3HckNum = pstPara->engine.hckEdgeNum;
    startLine <<= 1;
    endLine <<= 1;
    dataWidth <<= 1;
    panelHight -= 1;
    panelHight <<= 1;

    EZHV_MiPInit();
    /* heartbeat preheat */
    EZHV_EnableEmHeartBeat();
    HeatBeatDelay(mclkCntPerUs*20);

    /* S1 */
    ToggleWithDelay(mclkCntPerUs*pstPara->engine.xrstSetupTime, MIP_XRST_INTB);
    ToggleWithDelay(mclkCntPerUs*pstPara->engine.vstSetupTime, MIP_VST_GSP);
    ToggleWithDelay(mclkCntPerUs*s1Delay1, MIP_VCK_GCK);
    if (0 == s1flag)
    {
        ToggleWithDelay(mclkCntPerUs*(s1Delay2), MIP_VST_GSP);
        EZHV_PinToggle(MIP_VCK_GCK); /* vck hight to low */
    }
    else
    {
        if (0 == startLine)
        {
            EZHV_PinToggle(MIP_VCK_GCK);
        }
    }

    /*S2*/
    if (0 != startLine)
    {
        uint32_t i;
        if (0 == s1flag)
        {
            DelayWithToggle(mclkCntPerUs*vckHigh, MIP_VCK_GCK);
            startLine -= 1;
            i = 0;
        }
        else
        {
            ToggleWithDelay(mclkCntPerUs*(s1Delay2), MIP_VCK_GCK);
            EZHV_PinToggle(MIP_VST_GSP);
            DelayWithToggle(mclkCntPerUs*(2*vckHigh-vstHold), MIP_VCK_GCK);
            startLine -= 2;
            i = 1;
        }
        ModifyHeatBeat(mclkCntPerUs * pstPara->engine.vckFastHighLevelTime);
        for (; i < startLine; i++)
        {
            asm volatile ("wfi");
            EZHV_PinToggle(MIP_VCK_GCK);
        }
    }

    /* S3 */
    startLine = pstPara->startline;
    startLine <<= 1;
    if (0 != startLine)
    {
        uint32_t k = 0;
        for (; k < endLine - startLine + 3; k++)
        {
            HeatBeatDelay(u32tdHst);
            uint32_t hckCnt = 0;
            for (uint32_t i = 0; i < dataWidth >> 1; i++)
            {
                uint32_t value = EZHV_PortRead();
                value &= ~0xFF;
                value |= *(pbuf++);
                EZHV_PortWrite(value);
                HeatBeatDelay(hckCycle);
                if (hckCnt++ < s3HckNum)
                {
                    EZHV_PinToggle(MIP_HCK_BCK);
                }
                HeatBeatDelay(hckCycle);
            }
            for (uint32_t i = hckCnt; i < s3HckNum; i++)
            {
                HeatBeatDelay(hckCycle);
                EZHV_PinToggle(MIP_HCK_BCK);
                HeatBeatDelay(hckCycle);
            }
            EZHV_PinToggle(MIP_VCK_GCK);
        }
    }
    else
    {
        uint32_t k = 0;
        uint32_t hckCnt = 0;
        if (1 == s1flag)
        {
            HeatBeatDelay(u32tdHst);
            for (uint32_t i = 0; i < dataWidth >> 1; i++)
            {
                uint32_t value = EZHV_PortRead();
                value &= ~0xFF;
                value |= *(pbuf++);
                EZHV_PortWrite(value);
                HeatBeatDelay(hckCycle);
                if (hckCnt++ < s3HckNum)
                {
                    EZHV_PinToggle(MIP_HCK_BCK);
                }
                HeatBeatDelay(hckCycle);
                if (i == dataWidth >> 2)
                {
                    EZHV_PinToggle(MIP_VST_GSP);
                }
            }
            for (uint32_t i = hckCnt; i < s3HckNum; i++)
            {
                HeatBeatDelay(hckCycle);
                EZHV_PinToggle(MIP_HCK_BCK);
                HeatBeatDelay(hckCycle);
            }
            k = 1;
            EZHV_PinToggle(MIP_VCK_GCK);
        }
        for (; k < endLine - startLine + 3; k++)
        {
            hckCnt = 0;
            HeatBeatDelay(u32tdHst);
            for (uint32_t i = 0; i < dataWidth >> 1; i++)
            {
                uint32_t value = EZHV_PortRead();
                value &= ~0xFF;
                value |= *(pbuf++);
                EZHV_PortWrite(value);
                HeatBeatDelay(hckCycle);
                if (hckCnt++ < s3HckNum)
                {
                    EZHV_PinToggle(MIP_HCK_BCK);
                }
                HeatBeatDelay(hckCycle);
            }
            for (uint32_t i = hckCnt; i < s3HckNum; i++)
            {
                HeatBeatDelay(hckCycle);
                EZHV_PinToggle(MIP_HCK_BCK);
                HeatBeatDelay(hckCycle);
            }
            EZHV_PinToggle(MIP_VCK_GCK);
        }
    }

    /*S4*/
    uint32_t loop = panelHight - endLine;
    if ((1 == s1flag) && (0 != startLine))
    {
        loop += 2;
    }
    ModifyHeatBeat(mclkCntPerUs * pstPara->engine.vckFastHighLevelTime);
    for (uint32_t i = 0; i < loop; i++)
    {
        asm volatile ("wfi");
        EZHV_PinToggle(MIP_VCK_GCK);
    }

    /*S5*/
    ModifyHeatBeat(mclkCntPerUs * pstPara->engine.vckHighLevelTime/2);
    for (uint32_t i = 1; i < pstPara->engine.extVckEdgeNum1; i++)
    {
        asm volatile ("wfi");
        asm volatile ("wfi");
        EZHV_PinToggle(MIP_VCK_GCK);
    }
    asm volatile ("wfi");
    EZHV_PinToggle(MIP_XRST_INTB);
    asm volatile ("wfi");
    EZHV_PinToggle(MIP_VCK_GCK);
    for (uint32_t i = 1; i < pstPara->engine.extVckEdgeNum2; i++)
    {
        asm volatile ("wfi");
        asm volatile ("wfi");
        EZHV_PinToggle(MIP_VCK_GCK);
    }

    EZHV_DisableEmHeartBeat();
    XZMSG_IndIntChan(1);
    return;
}
#endif
