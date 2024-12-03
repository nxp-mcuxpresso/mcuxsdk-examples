/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Configuration structure for 3-phase PWM mc driver */
mcdrv_sctimer_pwm_t g_sM1Pwm3ph;

/* Structure for current and voltage measurement*/
mcdrv_adc_t g_sM1AdcSensor;

uint32_t GCCa;
uint32_t GCCb;
uint32_t GCRa;
uint32_t GCRb;

uint32_t period;
uint32_t pulsePeriod;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
* @brief   void MCDRV_Init_M1(void)
*           - Motor control driver main initialization
*           - Calls initialization functions of peripherals required for motor
*             control functionality
*
* @param   void
*
* @return  none
*/
void MCDRV_Init_M1(void)
{
    /* Init ADC */
    M1_MCDRV_ADC_PERIPH_INIT();

    /* Init CTMR2 (slow loop counter) */
    M1_MCDRV_TMR_SLOWLOOP_INIT();

    /* Init CMP */
    M1_MCDRV_CMP_INIT();

    /* 6-channel PWM peripheral init for M1 */
    M1_MCDRV_PWM_PERIPH_INIT();

}

/*!
* @brief   void InitADC(void)
*           - Initialization of the ADC peripheral
*           - 3-phase center-aligned PWM
*
* @param   void
*
* @return  none
*/
void InitADC(void)
{
  /* ***************************************************************************************************************************
   * SW/HW trigger -> TCTRL - set which CMD is called -> CMDLx - choose ADC channel               -> ADC conversion            *
   *                        - set which FIFO is used           - choose single or dual conversion                              *
   *                        - set HW trigger enabled             (executed simultaneously)                                     *
   *                                                     CMDHx - chose next ADC channel                                        *
   *                                                           - set wait trigger                                              *
   *                                                                                                                           *
   * If more ADC conversion are executed in a row - set watermark interrupt on number of conversion FCTRL[x] -> FWMARK.        *
   * Then will be interrupt generated after end of all ADC conversion. FIFO1Watermark is a programmable threshold setting.     *
   * When the number of datawords stored in the ADC Result FIFO1 is greater than the value in this field, the ready flag would *
   * be asserted to indicate stored data has reached the programmable threshold.                                               *
   *****************************************************************************************************************************/

    /* Set clock source and divider for ADC0 before calibration */
    // Clocked from fro_hf
    CLOCK_SetClkDiv(kCLOCK_DivAdcAsyncClk, 1U, true); //96MHz/2 = 48MHz
    CLOCK_AttachClk(kFRO_HF_to_ADC_CLK);
    
    /*
    // Clocked from main_clk
    CLOCK_SetClkDiv(kCLOCK_DivAdcAsyncClk, 5U, true); //150MHz/6 = 25MHz
    CLOCK_AttachClk(kMAIN_CLK_to_ADC_CLK);
    */

    /* Disable LDOGPADC power down */
    PMC->PDRUNCFGCLR0 = (uint32_t)(1UL << 19);

    
    /* Init ADC0 */
    /* Enable the clock for the ADC. */
    SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL0_ADC_MASK;
 
    /* Reset the module. */
    /* Reset all ADC internal logic and registers, except the CTRL register. */
    ADC0->CTRL |= ADC_CTRL_RST_MASK;
    ADC0->CTRL &= ~ADC_CTRL_RST_MASK;
    ADC0->CTRL |= ADC_CTRL_RSTFIFO0_MASK;       /* Reset the conversion FIFO0 */
    ADC0->CTRL |= ADC_CTRL_RSTFIFO1_MASK;       /* Reset the conversion FIFO1 */
    ADC0->CTRL &= ~ADC_CTRL_ADCEN_MASK;         /* Disable the module before configuration */
    
    /* Configure the module generally. */
    ADC0->CTRL &= ~ADC_CTRL_DOZEN_MASK;         /* Enable ADC in Doze mode */
    ADC0->CFG = ADC_CFG_PWREN_MASK    |         /* Pre-enable ADC analog circuits */
                ADC_CFG_PUDLY(0U)     |         /* Reset power-up delay */
                ADC_CFG_REFSEL(1U)    |         /* Reference voltage selection */
                ADC_CFG_PWRSEL(3U);             /* Power configuration */
    
    ADC0->PAUSE = 0U;                           /* Disable the ADC delay */
    ADC0->FCTRL[0] = ADC_FCTRL_FWMARK(2U);      /* Watermark level selection */
    
    /* Enable the module after configuration. */
    ADC0->CTRL |= ADC_CTRL_ADCEN_MASK;
    
    
    /* Request gain calibration. */
    ADC0->CTRL |= ADC_CTRL_CAL_REQ_MASK;
    while ((ADC_GCC_RDY_MASK != (ADC0->GCC[0] & ADC_GCC_RDY_MASK)) ||
           (ADC_GCC_RDY_MASK != (ADC0->GCC[1] & ADC_GCC_RDY_MASK)))
    {
    }

    /* Calculate gain offset. */
    GCCa = (ADC0->GCC[0] & ADC_GCC_GAIN_CAL_MASK);
    GCCb = (ADC0->GCC[1] & ADC_GCC_GAIN_CAL_MASK);
    
    /* Gain_CalA = (131072 / (131072-(ADC_GCC_GAIN_CAL(ADC0->GCC[0])) - 1. */
    GCRa = (uint16_t)((GCCa << 16U) / (0x1FFFFU - GCCa));
    
    /* Gain_CalB = (131072 / (131072-(ADC_GCC_GAIN_CAL(ADC0->GCC[1])) - 1. */
    GCRb = (uint16_t)((GCCb << 16U) / (0x1FFFFU - GCCb));
    
    ADC0->GCR[0] = ADC_GCR_GCALR(GCRa);
    ADC0->GCR[1] = ADC_GCR_GCALR(GCRb);
    
    /* Indicate the values are valid. */
    ADC0->GCR[0] |= ADC_GCR_RDY_MASK;
    ADC0->GCR[1] |= ADC_GCR_RDY_MASK;
    while (ADC_STAT_CAL_RDY_MASK != (ADC0->STAT & ADC_STAT_CAL_RDY_MASK))
    {
    }

    
    /* Request offset calibration */
    ADC0->CTRL |= ADC_CTRL_CALOFS_MASK;            /* Enable the offset calibration function */
    while (ADC_STAT_CAL_RDY_MASK != (ADC0->STAT & ADC_STAT_CAL_RDY_MASK))
    {
    }

    
    /* ************************************************************************
     * FIFO0                            FIFO1                                 *
     * I_A      - ADC_00    P19-4       I_B     - ADC_08 P19-2                *
     * I_C      - ADC_02    P18-11                                            *
     * UDCBus   - ADC_04    P17-19                                            *
     **************************************************************************/
    
    /* The available command number are 1-15, while the index of register group are 0-14. */
    
    /* Set conversion CMD1 configuration. */
    assert(1U < (ADC_CMDL_COUNT + 1U));            /* Check if the command 1 is available */
    ADC0->CMD[0].CMDL = ADC_CMDL_ADCH(0U)      |   /* Channel number selection */
                        ADC_CMDL_CTYPE(3U);        /* Conversion type selection  */
    ADC0->CMD[0].CMDH = ADC_CMDH_NEXT(2U);         /* Next execuited CMD will be CMD2 */

    
    /* Set conversion CMD2 configuration. */
    assert(2U < (ADC_CMDL_COUNT + 1U));            /* Check if the command 2 is available */
    ADC0->CMD[1].CMDL = ADC_CMDL_ADCH(2U)      |   /* Channel number selection */
                        ADC_CMDL_CTYPE(0U);        /* Conversion type selection  */
    ADC0->CMD[1].CMDH = ADC_CMDH_NEXT(3U);         /* Next execuited CMD will be CMD2 */
    

    /* Set conversion CMD3 configuration. */
    assert(3U < (ADC_CMDL_COUNT + 1U));            /* Check if the command 3 is available */
    ADC0->CMD[2].CMDL = ADC_CMDL_ADCH(4U)      |   /* Channel number selection */
                        ADC_CMDL_CTYPE(0U);        /* Conversion type selection  */
    ADC0->CMD[2].CMDH = ADC_CMDH_NEXT(0U);         /* This was last execuited command */

    /* Set trigger configuration. */
    assert(4U < ADC_TCTRL_COUNT);                  /* Check if the trigger 4 is available */
    ADC0->TCTRL[4] = ADC_TCTRL_TCMD(1U)        |   /* CMD1 is selected to execute.*/
                     ADC_TCTRL_FIFO_SEL_B(1U)  |   /* Channels B store to FIFO1 */
                     ADC_TCTRL_HTEN_MASK;          /* Hardware trigger source enabled */

    /* Offset filter window */
    g_sM1AdcSensor.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;

    /* Enable the watermark interrupt. */
    ADC0->IE |= ADC_IE_FWMIE0_MASK;
    
    /* Enable & setup interrupt from ADC0 */
    NVIC_EnableIRQ(ADC0_IRQn);
    NVIC_SetPriority(ADC0_IRQn, 1U);
}

/*!
* @brief   void InitCTMR2(void)
*           - Initialization of the CTMR2 peripheral
*           - Performs slow control loop counter
*
* @param   void
*
* @return  none
*/
void InitCTMR2(void)
{
    /* Use 96 MHz clock for some of the Ctimers */
    CLOCK_AttachClk(kFRO_HF_to_CTIMER2);

    /* Enable the timer clock */
    SYSCON->AHBCLKCTRLSET[1] = SYSCON_PRESETCTRL1_TIMER2_RST_MASK;

    /* Reset the module. */
    /* Assert reset to peripheral. */    
    SYSCON->PRESETCTRLSET[1] = SYSCON_PRESETCTRL1_TIMER2_RST_MASK;               /* set bit */
    while (0u == (SYSCON->PRESETCTRLX[1] & SYSCON_PRESETCTRL1_TIMER2_RST_MASK))  /* wait until it reads 0b1 */
    {
    }
    
    /* Clear reset to peripheral. */
    SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL1_TIMER2_RST_MASK;               /* clear bit */
    while (SYSCON_PRESETCTRL1_TIMER2_RST_MASK ==                                 /* wait until it reads 0b0 */
          (SYSCON->PRESETCTRLX[1] & SYSCON_PRESETCTRL1_TIMER2_RST_MASK))
    {
    }

    /* Set the counter operation when a match on this channel occurs. */
    CTIMER2->MCR = CTIMER_MCR_MR1I_MASK  |  /* Enable interrupt after MR1 matches with TC */
                   CTIMER_MCR_MR1R_MASK;    /* Enable reset of TC after it matches with MR1 */
    
    /* Set the match value. */
    CTIMER2->MR[1] = CTIMER_CLK_FREQ / M1_SPEED_LOOP_FREQ;
    
    /* Clear status flags. */
    CTIMER2->IR = CTIMER_IR_MR1INT_MASK;
    
    /* Enable & setup interrupt. */
    NVIC_EnableIRQ(CTIMER2_IRQn);
    NVIC_SetPriority(CTIMER2_IRQn, 2U);
    
    /* Starts the Timer counter. */
    CTIMER2->TCR |= CTIMER_TCR_CEN_MASK;
}

/*!
* @brief   void InitPWM_M1(void)
*           - Initialization of the eFlexPWMA peripheral for motor M1
*           - 3-phase center-aligned PWM
*
* @param   void
*
* @return  none
*/
void M1_InitPWM(void)
{
    /* Initialize MC driver. */
    g_sM1Pwm3ph.pui32PwmBaseAddress = (SCT_Type *)SCT0;  /* Set SCTimer base address */
    g_sM1Pwm3ph.updatedDutycycle = 50U;                  /* Set dutycycle */
    g_sM1Pwm3ph.ui16DeadTimePWM = 37;                    /* Set deadtime: 1 puls is 13.33ns (1/(150MHz/2)), 37*13.33ns = 493.21ns */
    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM;          /* PWMA fixed-value over-current fault number */
    g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM;          /* PWMA adjustable over-current fault number */
    
    /* Calculate PWM period match value. */
    period = 150000000 / (M1_PWM_FREQ * 2U); /* Frequency of core system / (PWM frequency * 2U) */

    /* Set clock source for SCTimer. */
    CLOCK_SetClkDiv(kCLOCK_DivSctClk, 0U, true);             /* main_clk divided by 1 */
    CLOCK_AttachClk(kMAIN_CLK_to_SCT_CLK);                   /* Clocked from main_clk */
    SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL1_SCT_MASK;  /* Enable the SCTimer clock */

    /* Initialize SCTimer module. */
    /* Assert reset to peripheral. */
    SYSCON->PRESETCTRLSET[1] = SYSCON_PRESETCTRL1_SCT_RST_MASK;               /* Set bit */
    while (0u == (SYSCON->PRESETCTRLX[1] & SYSCON_PRESETCTRL1_SCT_RST_MASK))  /* Wait until it reads 0b1 */
    {
    }
    
    /* Clear reset to peripheral. */
    SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL1_SCT_RST_MASK;                 /* Clear bit */
    while (SYSCON_PRESETCTRL1_SCT_RST_MASK ==                                   /* Wait until it reads 0b0 */
          (SYSCON->PRESETCTRLX[1] & SYSCON_PRESETCTRL1_SCT_RST_MASK))
    {
    }
    
    /* Setup the counter operation. */
    SCT0->CONFIG = SCT_CONFIG_UNIFY(1U)      /* SCT bit operation selection */
                 | SCT_CONFIG_INSYNC(0x0F);  /* Synchronization for input N */
    
    /* Use bi-directional mode for center-aligned PWM. */
    SCT0->CTRL |= SCT_CTRL_BIDIR_L_MASK;
    
    
    
    /* Configure PWM params with frequency 10kHZ from output. */ 
    // AT P18-5
    /* Schedule an event when we reach the PWM period. */
    SCT0->MATCH[0]    = period;                         /* Set match value */
    SCT0->MATCHREL[0] = period;                         /* Set match value */
    SCT0->EV[0].CTRL  = SCT_EV_CTRL_COMBMODE(1U)        /* SCTimer use only match event */
                      | SCT_EV_CTRL_MATCHSEL(0U);       /* Selects the match register associated with this event */
    SCT0->EV[0].STATE = SCT_EV_STATE_STATEMSKn(1U);     /* Enable the event in the current state */

    /* Schedule an event when we reach the pulse width. */
    /* For 100% dutycycle, make pulse period greater than period so the event will never occur. */
    pulsePeriod = (period * ((g_sM1Pwm3ph.updatedDutycycle + 1U)) / 100U);
    
    SCT0->MATCH[1]    = pulsePeriod;                    /* Set match value */
    SCT0->MATCHREL[1] = pulsePeriod;                    /* Set match value */
    SCT0->EV[1].CTRL  = SCT_EV_CTRL_COMBMODE(1U)        /* SCTimer use only match event */
                      | SCT_EV_CTRL_MATCHSEL(1U);       /* Selects the match register associated with this event */
    SCT0->EV[1].STATE = SCT_EV_STATE_STATEMSKn(1U);     /* Enable the event in the current state */

    /* Reset the counter when we reach the PWM period. */
    /* Use Counter_L bits when counter is operating in 32-bit mode (unify counter). */
    SCT0->LIMIT |= SCT_LIMIT_LIMMSK_L(1U << 0);

    /* Return the event number. */
    g_sM1Pwm3ph.eventNumberOutput0 = 0U;

    /* For low-true level */
    SCT0->OUTPUT        &= ~SCT_OUTPUT_OUT(1U << 0);         /* Set the initial output level to low which is the inactive state */
    SCT0->OUT[0].SET    |= SCT_OUT_SET_SET(1U << 1);         /* Set the output when we reach the PWM pulse event */
    SCT0->OUTPUTDIRCTRL &= ~SCT_OUTPUTDIRCTRL_SETCLR0_MASK;  /* Reverse output when down counting */
    SCT0->OUTPUTDIRCTRL |= (1U << 0);

    
    
    // AB P18-20
    /* Schedule an event when we reach the PWM period. */
    SCT0->MATCH[2]    = period;                         /* Set match value */
    SCT0->MATCHREL[2] = period;                         /* Set match value */
    SCT0->EV[2].CTRL  = SCT_EV_CTRL_COMBMODE(1U)        /* SCTimer use only match event */
                      | SCT_EV_CTRL_MATCHSEL(2U);       /* Selects the match register associated with this event */ 
    SCT0->EV[2].STATE = SCT_EV_STATE_STATEMSKn(1U);     /* Enable the event in the current state */

    /* Schedule an event when we reach the pulse width. */
    /* For 100% dutycycle, make pulse period greater than period so the event will never occur. */
    pulsePeriod = (period * ((g_sM1Pwm3ph.updatedDutycycle - 1U)) / 100U);

    SCT0->MATCH[3]    = pulsePeriod;                    /* Set match value */
    SCT0->MATCHREL[3] = pulsePeriod;                    /* Set match value */
    SCT0->EV[3].CTRL  = SCT_EV_CTRL_COMBMODE(1U)        /* SCTimer use only match event */
                      | SCT_EV_CTRL_MATCHSEL(3U);       /* Selects the match register associated with this event */
    SCT0->EV[3].STATE = SCT_EV_STATE_STATEMSKn(1U);     /* Enable the event in the current state */

    /* Reset the counter when we reach the PWM period. */
    /* Use Counter_L bits when counter is operating in 32-bit mode (unify counter). */
    SCT0->LIMIT |= SCT_LIMIT_LIMMSK_L(1U << 2);

    /* Return the event number. */
    g_sM1Pwm3ph.eventNumberOutput1 = 2U;

    /* For high-true level */
    SCT0->OUTPUT        |= SCT_OUTPUT_OUT(1U << 1);          /* Set the initial output level to high which is the inactive state */
    SCT0->OUT[1].CLR    |= SCT_OUT_CLR_CLR(1U << 3);         /* Clear the output when we reach the PWM pulse event */
    SCT0->OUTPUTDIRCTRL &= ~SCT_OUTPUTDIRCTRL_SETCLR1_MASK;  /* Reverse output when down counting */
    SCT0->OUTPUTDIRCTRL |= (1U << 2);


    
    // BT P18-6
    /* Schedule an event when we reach the PWM period. */
    SCT0->MATCH[4]    = period;                         /* Set match value */
    SCT0->MATCHREL[4] = period;                         /* Set match value */
    SCT0->EV[4].CTRL  = SCT_EV_CTRL_COMBMODE(1U)        /* SCTimer use only match event */
                      | SCT_EV_CTRL_MATCHSEL(4U);       /* Selects the match register associated with this event */
    SCT0->EV[4].STATE = SCT_EV_STATE_STATEMSKn(1U);     /* Enable the event in the current state */

    /* Schedule an event when we reach the pulse width. */
    /* For 100% dutycycle, make pulse period greater than period so the event will never occur. */
    pulsePeriod = (period * ((g_sM1Pwm3ph.updatedDutycycle + 1U)) / 100U);
    
    SCT0->MATCH[5]    = pulsePeriod;                    /* Set match value */
    SCT0->MATCHREL[5] = pulsePeriod;                    /* Set match value */
    SCT0->EV[5].CTRL  = SCT_EV_CTRL_COMBMODE(1U)        /* SCTimer use only match event */
                      | SCT_EV_CTRL_MATCHSEL(5U);       /* Selects the match register associated with this event */
    SCT0->EV[5].STATE = SCT_EV_STATE_STATEMSKn(1U);     /* Enable the event in the current state */

    /* Reset the counter when we reach the PWM period. */
    /* Use Counter_L bits when counter is operating in 32-bit mode (unify counter). */
    SCT0->LIMIT |= SCT_LIMIT_LIMMSK_L(1U << 4);

    /* Return the event number. */
    g_sM1Pwm3ph.eventNumberOutput2 = 4U;

    /* For low-true level */
    SCT0->OUTPUT        &= ~SCT_OUTPUT_OUT(1U << 2);         /* Set the initial output level to low which is the inactive state */
    SCT0->OUT[2].SET    |= SCT_OUT_SET_SET(1U << 5);         /* Set the output when we reach the PWM pulse event */
    SCT0->OUTPUTDIRCTRL &= ~SCT_OUTPUTDIRCTRL_SETCLR2_MASK;  /* Reverse output when down counting */
    SCT0->OUTPUTDIRCTRL |= (1U << 4);

    

    // BB P18-3
    /* Schedule an event when we reach the PWM period. */
    SCT0->MATCH[6]    = period;                         /* Set match value */
    SCT0->MATCHREL[6] = period;                         /* Set match value */
    SCT0->EV[6].CTRL  = SCT_EV_CTRL_COMBMODE(1U)        /* SCTimer use only match event */
                      | SCT_EV_CTRL_MATCHSEL(6U);       /* Selects the match register associated with this event */ 
    SCT0->EV[6].STATE = SCT_EV_STATE_STATEMSKn(1U);     /* Enable the event in the current state */

    /* Schedule an event when we reach the pulse width. */
    /* For 100% dutycycle, make pulse period greater than period so the event will never occur. */
    pulsePeriod = (period * ((g_sM1Pwm3ph.updatedDutycycle - 1U)) / 100U);

    SCT0->MATCH[7]    = pulsePeriod;                    /* Set match value */
    SCT0->MATCHREL[7] = pulsePeriod;                    /* Set match value */
    SCT0->EV[7].CTRL  = SCT_EV_CTRL_COMBMODE(1U)        /* SCTimer use only match event */
                      | SCT_EV_CTRL_MATCHSEL(7U);       /* Selects the match register associated with this event */
    SCT0->EV[7].STATE = SCT_EV_STATE_STATEMSKn(1U);     /* Enable the event in the current state */

    /* Reset the counter when we reach the PWM period. */
    /* Use Counter_L bits when counter is operating in 32-bit mode (unify counter). */
    SCT0->LIMIT |= SCT_LIMIT_LIMMSK_L(1U << 6);

    /* Return the event number. */
    g_sM1Pwm3ph.eventNumberOutput3 = 6U;

    /* For high-true level */
    SCT0->OUTPUT        |= SCT_OUTPUT_OUT(1U << 3);          /* Set the initial output level to high which is the inactive state */
    SCT0->OUT[3].CLR    |= SCT_OUT_CLR_CLR(1U << 7);         /* Clear the output when we reach the PWM pulse event */
    SCT0->OUTPUTDIRCTRL &= ~SCT_OUTPUTDIRCTRL_SETCLR3_MASK;  /* Reverse output when down counting */
    SCT0->OUTPUTDIRCTRL |= (1U << 6);


    
    // CT P18-19
    /* Schedule an event when we reach the PWM period. */
    SCT0->MATCH[8]    = period;                         /* Set match value */
    SCT0->MATCHREL[8] = period;                         /* Set match value */
    SCT0->EV[8].CTRL  = SCT_EV_CTRL_COMBMODE(1U)        /* SCTimer use only match event */
                      | SCT_EV_CTRL_MATCHSEL(8U);       /* Selects the match register associated with this event */
    SCT0->EV[8].STATE = SCT_EV_STATE_STATEMSKn(1U);     /* Enable the event in the current state */

    /* Schedule an event when we reach the pulse width. */
    /* For 100% dutycycle, make pulse period greater than period so the event will never occur. */
    pulsePeriod = (period * ((g_sM1Pwm3ph.updatedDutycycle + 1U)) / 100U);
    
    SCT0->MATCH[9]    = pulsePeriod;                    /* Set match value */
    SCT0->MATCHREL[9] = pulsePeriod;                    /* Set match value */
    SCT0->EV[9].CTRL  = SCT_EV_CTRL_COMBMODE(1U)        /* SCTimer use only match event */
                      | SCT_EV_CTRL_MATCHSEL(9U);       /* Selects the match register associated with this event */
    SCT0->EV[9].STATE = SCT_EV_STATE_STATEMSKn(1U);     /* Enable the event in the current state */

    /* Reset the counter when we reach the PWM period. */
    /* Use Counter_L bits when counter is operating in 32-bit mode (unify counter). */
    SCT0->LIMIT |= SCT_LIMIT_LIMMSK_L(1U << 8);

    /* Return the event number. */
    g_sM1Pwm3ph.eventNumberOutput4 = 8U;

    /* For low-true level */
    SCT0->OUTPUT        &= ~SCT_OUTPUT_OUT(1U << 4);         /* Set the initial output level to low which is the inactive state */
    SCT0->OUT[4].SET    |= SCT_OUT_SET_SET(1U << 9);         /* Set the output when we reach the PWM pulse event */
    SCT0->OUTPUTDIRCTRL &= ~SCT_OUTPUTDIRCTRL_SETCLR4_MASK;  /* Reverse output when down counting */
    SCT0->OUTPUTDIRCTRL |= (1U << 8);


    
    // CB P17-13
    /* Schedule an event when we reach the PWM period. */
    SCT0->MATCH[10]    = period;                        /* Set match value */
    SCT0->MATCHREL[10] = period;                        /* Set match value */
    SCT0->EV[10].CTRL  = SCT_EV_CTRL_COMBMODE(1U)       /* SCTimer use only match event */
                       | SCT_EV_CTRL_MATCHSEL(10U);     /* Selects the match register associated with this event */ 
    SCT0->EV[10].STATE = SCT_EV_STATE_STATEMSKn(1U);    /* Enable the event in the current state */

    /* Schedule an event when we reach the pulse width. */
    /* For 100% dutycycle, make pulse period greater than period so the event will never occur. */
    pulsePeriod = (period * ((g_sM1Pwm3ph.updatedDutycycle - 1U)) / 100U);

    SCT0->MATCH[11]    = pulsePeriod;                   /* Set match value */
    SCT0->MATCHREL[11] = pulsePeriod;                   /* Set match value */
    SCT0->EV[11].CTRL  = SCT_EV_CTRL_COMBMODE(1U)       /* SCTimer use only match event */
                       | SCT_EV_CTRL_MATCHSEL(11U);     /* Selects the match register associated with this event */
    SCT0->EV[11].STATE = SCT_EV_STATE_STATEMSKn(1U);    /* Enable the event in the current state */

    /* Reset the counter when we reach the PWM period. */
    /* Use Counter_L bits when counter is operating in 32-bit mode (unify counter). */
    SCT0->LIMIT |= SCT_LIMIT_LIMMSK_L(1U << 10);

    /* Return the event number. */
    g_sM1Pwm3ph.eventNumberOutput5 = 10U;

    /* For high-true level */
    SCT0->OUTPUT        |= SCT_OUTPUT_OUT(1U << 5);          /* Set the initial output level to high which is the inactive state */
    SCT0->OUT[5].CLR    |= SCT_OUT_CLR_CLR(1U << 11);        /* Clear the output when we reach the PWM pulse event */
    SCT0->OUTPUTDIRCTRL &= ~SCT_OUTPUTDIRCTRL_SETCLR5_MASK;  /* Reverse output when down counting */
    SCT0->OUTPUTDIRCTRL |= (1U << 10);

    

    // TRIGGER SETUP (sct0_output[9])
    /* Schedule an event when we reach the PWM period. */
    SCT0->MATCH[12]    = period;                        /* Set match value */
    SCT0->MATCHREL[12] = period;                        /* Set match value */
    SCT0->EV[12].CTRL  = SCT_EV_CTRL_COMBMODE(1U)       /* SCTimer use only match event */
                       | SCT_EV_CTRL_MATCHSEL(12U);     /* Selects the match register associated with this event */ 
    SCT0->EV[12].STATE = SCT_EV_STATE_STATEMSKn(1U);    /* Enable the event in the current state */

    SCT0->MATCH[13]    = SCT_MATCH_MATCHn_L(1U);        /* Set match value */        
    SCT0->MATCHREL[13] = SCT_MATCHREL_RELOADn_L(25U);   /* Set match value */
    SCT0->EV[13].CTRL  = SCT_EV_CTRL_COMBMODE(1U)       /* SCTimer use only match event */
                       | SCT_EV_CTRL_MATCHSEL(13U);     /* Selects the match register associated with this event */
    SCT0->EV[13].STATE = SCT_EV_STATE_STATEMSKn(1U);    /* Enable the event in the current state */

    /* Reset the counter when we reach the PWM period. */
    /* Use Counter_L bits when counter is operating in 32-bit mode (unify counter). */
    SCT0->LIMIT |= SCT_LIMIT_LIMMSK_L(1U << 12);

    /* For high-true level */
    SCT0->OUTPUT        |= SCT_OUTPUT_OUT(1U << 9);          /* Set the initial output level to high which is the inactive state */
    SCT0->OUT[9].CLR    |= SCT_OUT_CLR_CLR(1U << 13);        /* Clear the output when we reach the PWM pulse event */
    SCT0->OUTPUTDIRCTRL &= ~SCT_OUTPUTDIRCTRL_SETCLR9_MASK;  /* Reverse output when down counting */
    SCT0->OUTPUTDIRCTRL |= (1U << 18);


    
    /* PWM Faults */
    SCT0->EV[14].STATE = 0x01;                          /* Enable event 14 */
    SCT0->EV[14].CTRL |= SCT_EV_CTRL_DIRECTION(0x0)     /* This event is triggered regardless of the count direction */
                       | SCT_EV_CTRL_IOCOND(0x1)        /* Input condition - high //0x1 */
                       | SCT_EV_CTRL_OUTSEL(0x0)        /* Selects the inputs selected by IOSEL */
                       | SCT_EV_CTRL_IOSEL(0x0)         /* input 0 (mapped from CMP) */
                       | SCT_EV_CTRL_COMBMODE(0x2);     /* Uses the specified I/O condition only */
    SCT0->EVEN = SCT_EVEN_IEN(1U << 14);                /* IRQ initiated from CMP */
    
    MCDRV_scTmrPwm3PhOutInit(&g_sM1Pwm3ph);           /* Initialize PWM structure */
    MCDRV_scTmrPwm3PhOutDis(&g_sM1Pwm3ph);            /* Disable PWM output */

    /* Enable & setup interrupt. */
    NVIC_EnableIRQ(SCT0_IRQn);
    NVIC_SetPriority(SCT0_IRQn, 0U);
}

/*!
@brief   void InitCMP(void)
          - Initialization of the comparator module for dc-bus over current
            detection

@param   void

@return  none
*/
void InitCMP(void)
{
    /* Enable the clock */
    SYSCON->AHBCLKCTRLSET[2] = SYSCON_AHBCLKCTRL2_COMP_MASK;
    
    /* Enable the power to the analog comparator */
    PMC->PDRUNCFG0 &= ~PMC_PDRUNCFG0_PDEN_COMP_MASK;
      
    /* Asserts reset to peripheral. */
    SYSCON->PRESETCTRLSET[2] = SYSCON_PRESETCTRL2_COMP_RST_MASK;                 /* Set bit */
    while (0u == (SYSCON->PRESETCTRLX[2] & SYSCON_PRESETCTRL2_COMP_RST_MASK))    /* Wait until it reads 0b1 */
    {
    }
    
    /* Clear reset to peripheral. */
    SYSCON->PRESETCTRLCLR[2] = SYSCON_PRESETCTRL2_COMP_RST_MASK;                 /* Clear bit */
    while (SYSCON_PRESETCTRL2_COMP_RST_MASK ==                                   /* Wait until it reads 0b0 */
          (SYSCON->PRESETCTRLX[2] & SYSCON_PRESETCTRL2_COMP_RST_MASK))
    {
    }

    PMC->COMP = PMC_COMP_FILTERCGF_CLKDIV(4U)      |    /* Filter clock divider selection */
                PMC_COMP_FILTERCGF_SAMPLEMODE(2U);      /* Control the filtering of CMP output*/
    
    /* Set the input channels for the comparator. PositiveChannel 1, NegativeChannel 0 */
    PMC->COMP |= PMC_COMP_PMUX(1U) | PMC_COMP_NMUX(0U);

    /* Configure the VREFINPUT. */
    PMC->COMP |= PMC_COMP_VREFINPUT(1U) |  /* Select VDDA as VREF */
                 PMC_COMP_VREF(31U);       /* Reference voltage step, per steps of (VREFINPUT/31) */

    /* Configure analog comparator interrupt. */
    SYSCON->COMP_INT_CTRL |= SYSCON_COMP_INT_CTRL_INT_CTRL(3U)     |   /* Set interrupt on high level */
                             SYSCON_COMP_INT_CTRL_INT_ENABLE_MASK  |   /* Enable interrupt */
                             SYSCON_COMP_INT_CTRL_INT_CLEAR_MASK;      /* Clear interrupt */
}