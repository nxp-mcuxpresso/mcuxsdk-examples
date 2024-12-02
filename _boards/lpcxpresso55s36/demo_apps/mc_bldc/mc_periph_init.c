/*
    * Copyright 2024 NXP
    *
    * SPDX-License-Identifier: BSD-3-Clause
*/

#include "mc_periph_init.h"
#include "peripherals.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "fsl_power.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/* Commutation table */
const uint16_t bldcCommutationTableComp[16] = {
    /*swap, mask*/
    0x440, 0x040, /* [0] - sector 0 */
    0x220, 0x400, /* [1] - sector 1 */
    0x110, 0x400, /* [2] - sector 2 */
    0x440, 0x004, /* [3] - sector 3 */
    0x220, 0x004, /* [4] - sector 4 */
    0x110, 0x040, /* [5] - sector 5 */
    0x000, 0x440, /* [6] - alignment vector (combination of sectors 0 & 1) */
    0xFF0, 0x000  /* [7] - PWM off */
};

/*******************************************************************************
 * Typedef
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void InitADC1(void);
static void InitHsCmp1(void);
static void InitPWM1(void);
static void InitCMT1(void);

static void InitSlowLoop(void);
static void InitClock(void);
static void InitInputmux(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* configuration structure for 3-phase PWM mc driver */
mcdrv_eflexpwm_t g_sM1Pwm3ph;

/* Structure for time event driver */
mcdrv_cmt_pwma_t g_sM1CmtTmr;

/* structure for current and voltage measurement*/
mcdrv_adc_t g_sM1AdcSensor;

/* Clock setup structure */
clock_setup_t g_sClockSetup;

/*******************************************************************************
 * Local functions
 ******************************************************************************/
  
   /*!
   * @brief   void InitPWM1(void)
   *           - Initialization of the eFlexPWM1 peripheral for motor M1
   *           - 3-phase center-aligned PWM
   *
   * @param   void
   *
   * @return  none
   */
  static void InitPWM1(void)
  {
      PWM_Type *PWMBase = (PWM_Type *)PWM1;

      /*eFlexPWM1 init*/
      SYSCON->PWM1SUBCTL = (SYSCON_PWM1SUBCTL_CLK0_EN_MASK | SYSCON_PWM1SUBCTL_CLK1_EN_MASK | SYSCON_PWM1SUBCTL_CLK2_EN_MASK | SYSCON_PWM1SUBCTL_CLK3_EN_MASK); //Enable Sub-module0 clock
      CLOCK_EnableClock(kCLOCK_Pwm1);
      
      /* Full cycle reload */
      PWMBase->SM[0].CTRL |= PWM_CTRL_FULL_MASK;
      PWMBase->SM[1].CTRL |= PWM_CTRL_FULL_MASK;
      PWMBase->SM[2].CTRL |= PWM_CTRL_FULL_MASK;
      
      /* value register initial values, duty cycle 50% */
      PWMBase->SM[0].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo) / 2));
      PWMBase->SM[1].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo) / 2));
      PWMBase->SM[2].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo) / 2));
      
      PWMBase->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
      PWMBase->SM[1].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
      PWMBase->SM[2].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
      
      PWMBase->SM[0].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
      PWMBase->SM[1].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
      PWMBase->SM[2].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));

      PWMBase->SM[0].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
      PWMBase->SM[1].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
      PWMBase->SM[2].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));

      PWMBase->SM[0].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
      PWMBase->SM[1].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
      PWMBase->SM[2].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
      
      PWMBase->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
      PWMBase->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
      PWMBase->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
      
      /* PWM1 module 0 trigger on VAL4 enabled for ADC synchronization */
      PWMBase->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)((-(g_sClockSetup.ui16M1PwmModulo / 2) + 10)));
      PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 4) | PWM_TCTRL_TRGFRQ(1);

      /* PWM channels set as complementary pair */
      PWMBase->SM[0].CTRL2 = (PWMBase->SM[0].CTRL2 & ~PWM_CTRL2_INDEP_MASK) | PWM_CTRL2_INDEP(0x0);
      PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_INDEP_MASK) | PWM_CTRL2_INDEP(0x0);
      PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_INDEP_MASK) | PWM_CTRL2_INDEP(0x0);
      
      /* The submodules 1 and 2 are synchronized with submodule 0 (Reload on master reload signal - submodule 0) */
      PWMBase->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
      PWMBase->SM[2].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    
      /* Modules one and two get clock from module zero */
      PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
      PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
      
      /* Master reload is generated every one opportunity */
      PWMBase->SM[0].CTRL = (PWMBase->SM[0].CTRL & ~PWM_CTRL_LDFQ_MASK) | PWM_CTRL_LDFQ(M1_FOC_FREQ_VS_PWM_FREQ - 1);
      
      /* Master sync active for modules one and two*/
      PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
      PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
      
      /* set deadtime (number of Fast Peripheral Clocks) */
      PWMBase->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
      PWMBase->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
      PWMBase->SM[2].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
      PWMBase->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
      PWMBase->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
      PWMBase->SM[2].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);

      /* PWMs are re-enabled at PWM full cycle */
      PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFULL_MASK) | PWM_FSTS_FFULL(0x1);

      /* PWM fault filter - 3 Fast periph. clocks sample rate, 5 agreeing
         samples to activate */
      PWMBase->FFILT = (PWMBase->FFILT & ~PWM_FFILT_FILT_PER_MASK) | PWM_FFILT_FILT_PER(2);

      /* All interrupts disabled, safe manual fault clearing, inversed logic (trigger level = high) */
      PWMBase->FCTRL &= ~(PWM_FCTRL_FLVL_MASK | PWM_FCTRL_FAUTO_MASK | PWM_FCTRL_FSAFE_MASK | PWM_FCTRL_FIE_MASK); /* clear FCTRL register prior further settings */
      PWMBase->FCTRL |= PWM_FCTRL_FIE(0U); /* FAULT 0 & FAULT 1 - Interrupt disable */
      PWMBase->FCTRL |= PWM_FCTRL_FLVL(0xFU);
      PWMBase->FCTRL |= PWM_FCTRL_FAUTO(0U);
      PWMBase->FCTRL |= PWM_FCTRL_FSAFE(0xFU);

      /* Clear all fault flags */
      PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);
      
      /* Prescale factor 150MHz/2 - 75MHz*/
      PWMBase->SM[0].CTRL = (PWMBase->SM[0].CTRL & ~PWM_CTRL_PRSC_MASK) | PWM_CTRL_PRSC(0x2);
          
      /* Set PWM23 as the source for the generation of the complementary PWM pair output*/
      PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_IPOL_MASK) | PWM_MCTRL_IPOL(0x0);
      
             /* Enable A&B PWM outputs for submodules zero, one and two */
      PWMBase->OUTEN = (PWMBase->OUTEN & ~PWM_OUTEN_PWMA_EN_MASK) | PWM_OUTEN_PWMA_EN(0x7);
      PWMBase->OUTEN = (PWMBase->OUTEN & ~PWM_OUTEN_PWMB_EN_MASK) | PWM_OUTEN_PWMB_EN(0x7);
      
      /* Start PWMs (set load OK flags and run - we need to trigger the ADC) */
      PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0x7);
      PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0x7);
      PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_RUN_MASK) | PWM_MCTRL_RUN(0x7);

      /* Initialize MC driver eFlexPWM base address */
      g_sM1Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWMBase;

      g_sM1Pwm3ph.ui16PhASubNum = 0U; /* PWMA phase A sub-module number */
      g_sM1Pwm3ph.ui16PhBSubNum = 1U; /* PWMA phase B sub-module number */
      g_sM1Pwm3ph.ui16PhCSubNum = 2U; /* PWMA phase C sub-module number */
      
      /* initialization of PWM modulo */
      g_sM1Pwm3ph.ui16PwmModulo = g_sClockSetup.ui16M1PwmModulo;
      
      g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
      g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */
      
      g_sM1Pwm3ph.pcBldcTable = &bldcCommutationTableComp[0]; /* Address of BLDC commutation table*/
  }

  /*!
   * @brief   void InitADC1(void)
   *           - Initialization of the ADC1 peripheral
   *           - Initialization of the A/D converter for current and voltage sensing
   *
   * @param   void
   *
   * @return  none
   */
  static void InitADC1(void)
  {
      uint32_t GCCa;
      uint32_t GCCb;
      uint32_t GCRa;
      uint32_t GCRb;

      CLOCK_SetClkDiv(kCLOCK_DivAdc1Clk, 2U, true);
      CLOCK_AttachClk(kFRO_HF_to_ADC1);

      /* Disable VREF power down */
      POWER_DisablePD(kPDRUNCFG_PD_VREF);

      /* Init ADC */
      /* Enable the clock. */
      SYSCON->AHBCLKCTRL0 |= SYSCON_AHBCLKCTRL0_ADC1_MASK;

      /* Reset the module. */
      ADC1->CTRL |= ADC_CTRL_RST_MASK;          /* ADC logic reset */
      ADC1->CTRL &= ~ADC_CTRL_RST_MASK;
      ADC1->CTRL |= ADC_CTRL_RSTFIFO0_MASK;     /* Reset FIFO 0 */
      ADC1->CTRL |= ADC_CTRL_RSTFIFO1_MASK;     /* Reset FIFO 1 */

      /* Disable the module before setting configuration. */
      ADC1->CTRL &= ~ADC_CTRL_ADCEN_MASK;

      /* Configure the module generally. */
      ADC1->CTRL &= ~ADC_CTRL_DOZEN_MASK;

      /* Set calibration average mode. */
      ADC1->CTRL = ADC_CTRL_CAL_AVGS(7U);

      /* ADC1 configuration. */
      ADC1->CFG |= ADC_CFG_PWREN_MASK  |  /* ADC analog circuits pre-enable */
                   ADC_CFG_PUDLY(0x80) |  /* Power up delay */
                   ADC_CFG_REFSEL(2U)  |  /* Reference voltage */
                   ADC_CFG_PWRSEL(3U);    /* Power configuration */

      /* Enable the module after setting configuration. */
      ADC1->CTRL |= ADC_CTRL_ADCEN_MASK;

      /* Offset calibration */
      ADC1->CTRL |= ADC_CTRL_CALOFS_MASK;
      while (ADC_STAT_CAL_RDY_MASK != (ADC1->STAT & ADC_STAT_CAL_RDY_MASK))
      {
      }

      /* AutoCalibration */
      assert((0u == ((ADC_FCTRL_FCOUNT_MASK & ADC1->FCTRL[0]) >> ADC_FCTRL_FCOUNT_SHIFT))
          && (0u == ((ADC_FCTRL_FCOUNT_MASK & ADC1->FCTRL[1]) >> ADC_FCTRL_FCOUNT_SHIFT)));

      /* Request gain calibration. */
      ADC1->CTRL |= ADC_CTRL_CAL_REQ_MASK;
      while ((ADC_GCC_RDY_MASK != (ADC1->GCC[0] & ADC_GCC_RDY_MASK)) ||
             (ADC_GCC_RDY_MASK != (ADC1->GCC[1] & ADC_GCC_RDY_MASK)))
      {
      }

      /* Calculate gain offset. */
      GCCa = (ADC1->GCC[0] & ADC_GCC_GAIN_CAL_MASK);
      GCCb = (ADC1->GCC[1] & ADC_GCC_GAIN_CAL_MASK);

      GCRa = (uint16_t)((GCCa << 16U) / (0x1FFFFU - GCCa));
      /* Gain_CalA = (131072 / (131072-(ADC_GCC_GAIN_CAL(ADC1->GCC[0])) - 1. */
      GCRb = (uint16_t)((GCCb << 16U) / (0x1FFFFU - GCCb));
      /* Gain_CalB = (131072 / (131072-(ADC_GCC_GAIN_CAL(ADC1->GCC[1])) - 1. */

      ADC1->GCR[0] = ADC_GCR_GCALR(GCRa);
      ADC1->GCR[1] = ADC_GCR_GCALR(GCRb);

      /* Indicate the values are valid. */
      ADC1->GCR[0] |= ADC_GCR_RDY_MASK;
      ADC1->GCR[1] |= ADC_GCR_RDY_MASK;
      while (ADC_STAT_CAL_RDY_MASK != (ADC1->STAT & ADC_STAT_CAL_RDY_MASK))
      {
      }

      /* Init commands */
      /* The available command number are 1-15, while the index of register group are 0-14. */
      /* Configure the CMD 1 */
      ADC1->CMD[0].CMDL = ADC_CMDL_ADCH(1U)  |  /* Channel number (ADC1IN1A (VDC_BUS) and ADC1IN1B (IDC_BUS)) */
                          ADC_CMDL_CTYPE(3U) |  /* Conversion type dual single-ended VDC_BUS, IDC_BUS */
                          ADC_CMDL_MODE(0U);    /* Resolution of conversion */
      ADC1->CMD[0].CMDH = ADC_CMDH_NEXT(2U);    /* Next command select (CMD2) */

      /* Configure the CMD 2 */
      ADC1->CMD[1].CMDL = ADC_CMDL_ADCH(3U)  |  /* Channel number (ADC1IN3A (BEMF_A) and ADC1IN3B (BEMF_C)) */
                          ADC_CMDL_CTYPE(3U) |  /* Conversion type dual single-ended BEMF_A, BEMF_C */
                          ADC_CMDL_MODE(0U);    /* Resolution of conversion */
      ADC1->CMD[1].CMDH = ADC_CMDH_NEXT(3U);    /* Next command select (CMD3) */

      /* Configure the CMD 3 */
      ADC1->CMD[2].CMDL = ADC_CMDL_ADCH(2U)  |  /* Channel number (ADC1IN2A (BEMF_B)) */
                          ADC_CMDL_CTYPE(0U) |  /* Conversion type single-ended */
                          ADC_CMDL_MODE(0U);    /* Resolution of conversion */
      ADC1->CMD[2].CMDH = ADC_CMDH_NEXT(0U);    /* Next command select (not defined) */

      /* Init triggers (use trigger 0). */
      ADC1->TCTRL[0] = ADC_TCTRL_FIFO_SEL_A(0) | /* Channels A stored to FIFO 0 */
                       ADC_TCTRL_FIFO_SEL_B(1) | /* Channels B stored to FIFO 1 - only used in dual single-ended mode (in single-ended mode, channels B stored to FIFO 0). */
                       ADC_TCTRL_TCMD(1U) |      /* Trigger command select */
                       ADC_TCTRL_HTEN_MASK;      /* Trigger enable */

      /* ADC base address */
      g_sM1AdcSensor.pToAdcBase = (ADC_Type *)ADC1;

      /* When data words stored in the FIFO is greater than 2, the STAT[RDY0] flag is asserted. */
      ADC1->FCTRL[0] = ADC_FCTRL_FWMARK(2U);

      /* Enable FIFO 0 Watermark interrupt. */
      ADC1->IE |= ADC_IE_FWMIE0(0x1);

      NVIC_SetPriority(ADC1_IRQn, 0U);
      NVIC_EnableIRQ(ADC1_IRQn);
      
      g_sM1AdcSensor.ui16Sector = 0U;
  }
  
  /*!
  * @brief   void InitCMT(void)
  *           - Initialization of the CMT_PWMA Timer
  *           - performs forced commutation control, time event
  *
  * @param   void
  *
  * @return  none
  */
  static void InitCMT1(void)
  {
    
      PWM_Type *PWMBase = (PWM_Type *)PWM1;
      
      /* INIT counter value */
      PWMBase->SM[3].INIT = PWM_INIT_INIT(0x0);
      
      /* Module three gets clock from module zero */
      PWMBase->SM[3].CTRL2 = (PWMBase->SM[3].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
      
      /* Prescale factor 75MHz/128 - 585,937.5Hz*/
      PWMBase->SM[3].CTRL = (PWMBase->SM[3].CTRL & ~PWM_CTRL_PRSC_MASK) | PWM_CTRL_PRSC(0x7);
      
      /* Changed registers of submodule three take effect immediately after LDOK being set */
      PWMBase->SM[3].CTRL = (PWMBase->SM[3].CTRL & ~PWM_CTRL_LDMOD_MASK) | PWM_CTRL_LDMOD(0x1);
      
      /* Modulo value of counter */
      PWMBase->SM[3].VAL1 = PWM_VAL1_VAL1(0xFFFF);
      
      /* Value for output compare interrupt */
      PWMBase->SM[3].VAL2 = PWM_VAL2_VAL2(0xF);
      
      /* Enables flags for compare interrupt */
      PWMBase->SM[3].INTEN = (PWMBase->SM[3].INTEN & ~PWM_INTEN_CMPIE_MASK) | PWM_INTEN_CMPIE(0x4);
      
      /* Load values of registers */
      PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0x8);
      PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0x8);
      
      /* Enable and set priority of interrupt */
      EnableIRQ(FLEXPWM1_COMPARE3_IRQn);
      NVIC_SetPriority(FLEXPWM1_COMPARE3_IRQn, 0U);
      
       /* calculate frequency of timer used for forced commutation
       * System clock divided by 2^FTM_prescaler */
      g_sClockSetup.ui32CmtTimerFreq = ((g_sClockSetup.ui32FastPeripheralClock) >> (0x7));
      
      g_sM1CmtTmr.pui32PwmBase = (PWM_Type *)PWMBase; 
      g_sM1CmtTmr.ui8PwmSubmoduleNumber = 3U;
  }

  /*!
  @brief   void InitHsCmp1(void)
            - Initialization of the comparator 1 module for dc-bus over current
              detection to generate eFlexPWM1 fault

  @param   void

  @return  none
  */
  static void InitHsCmp1(void)
  {
      /* Disable power down */
      POWER_DisablePD(kPDRUNCFG_PD_HSCMP1);
      POWER_DisablePD(kPDRUNCFG_PD_VREF);
      POWER_DisablePD(kPDRUNCFG_PD_CMPBIAS);
      POWER_DisablePD(kPDRUNCFG_PD_HSCMP1_DAC);

      CLOCK_EnableClock(kCLOCK_Hscmp1);

      /* Input minus = internal 8-bit DAC, input plus = analog mux in3 (P1_5), high power/high speed mode */
      HSCMP1->CCR2 |= (uint32_t)( HSCMP_CCR2_INMSEL(0) | HSCMP_CCR2_INPSEL(1) | HSCMP_CCR2_MSEL(7U) | HSCMP_CCR2_PSEL(3U) | HSCMP_CCR2_CMP_HPMD_MASK);

      /* Setting for external OPAMP: Set DAC_OUT to 3.15V (7.5A overcurrent through 0.02 Ohm shunt, gain 10x). */
      HSCMP1->DCR = HSCMP_DCR_DAC_DATA(243U) |   /* (3.3V / 256) * (243 + 1) = 3.145 V */
                    HSCMP_DCR_DACOE(1)      |   /* DAC output enable */
                    HSCMP_DCR_DAC_HPMD(1)   |   /* High power mode enabled */
                    HSCMP_DCR_DAC_EN(1);        /* DAC enable */

      /* HSCMP enable */
      HSCMP1->CCR0 |= HSCMP_CCR0_CMP_EN_MASK;
  }

/*!
 * @brief   void InitSlowLoop(void)
 *           - Initialization of the CTIMER0 peripheral
 *           - performs slow control loop counter
 *
 * @param   void
 *
 * @return  none
 */
static void InitSlowLoop(void)
{
    /* Use 96 MHz clock for some of the Ctimer0. */
    CLOCK_AttachClk(kFRO_HF_to_CTIMER0);

    /* Enable Timer0 clock. */
    SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL1_TIMER0_MASK;

    /* Enable Timer0 clock reset. */
    SYSCON->PRESETCTRLSET[1] = SYSCON_PRESETCTRL1_TIMER0_RST_MASK;             /* Set bit. */
    while (0u == (SYSCON->PRESETCTRL1 & SYSCON_PRESETCTRL1_TIMER0_RST_MASK))   /* Wait until it reads 0b1 */
    {
    }

    /* Clear Timer0 clock reset. */
    SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL1_TIMER0_RST_MASK;             /* Clear bit */
    while (SYSCON_PRESETCTRL1_TIMER0_RST_MASK ==                               /* Wait until it reads 0b0 */
          (SYSCON->PRESETCTRL1 & SYSCON_PRESETCTRL1_TIMER0_RST_MASK))
    {
    }

    /* Configure match control register. */
    CTIMER0->MCR |= CTIMER_MCR_MR0R(1U)  |   /* Enable reset of TC after it matches with MR0. */
                    CTIMER_MCR_MR0I(1U);     /* Enable interrupt generation after TC matches with MR0. */

    /* Configure match register. */
    CTIMER0->MR[0] = (CLOCK_GetFreq(kCLOCK_FroHf))  /* Get CTimer0 frequency for correct set Match register value. */
                     / M1_SLOW_LOOP_FREQ;           /* Set slow control loop frequency in Hz. */

    /* Configure interrupt register. */
    CTIMER0->IR = CTIMER_IR_MR0INT_MASK;     /* Set interrupt flag for match channel 0. */
    NVIC_SetPriority(CTIMER0_IRQn, 2U);      /* Set priority lower that ADC IRQ priority */
    NVIC_EnableIRQ(CTIMER0_IRQn);            /* Enable LEVEL1 interrupt and update the call back function. */

    /* Configure timer control register. */
    CTIMER0->TCR |= CTIMER_TCR_CEN_MASK;     /* Start the timer counter. */
}

/*!
* @brief   void InitClock(void)
*          - Core, bus, flash clock setup
*
* @param   void
*
* @return  none
*/
static void InitClock(void)
{
    uint32_t ui32CyclesNumber = 0U;

    /* Calculate clock dependant variables for BLDC control algorithm (Subdivided by 2 due to PWM prescaler setup)*/
    g_sClockSetup.ui32FastPeripheralClock = (CLOCK_GetFreq(kCLOCK_CoreSysClk));

    /* Parameters for motor */
    g_sClockSetup.ui16M1PwmFreq   = M1_PWM_FREQ; /* 20 kHz */
    g_sClockSetup.ui16M1PwmModulo = ((g_sClockSetup.ui32FastPeripheralClock) / 4 ) / g_sClockSetup.ui16M1PwmFreq;
    ui32CyclesNumber = ((M1_PWM_DEADTIME * (((g_sClockSetup.ui32FastPeripheralClock) / 4 ) / 1000000U)) / 1000U);
    g_sClockSetup.ui16M1PwmDeadTime   = ui32CyclesNumber;
    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SPEED_LOOP_FREQ; /* 1kHz */
}

/*!
 * @brief   void InitInputmux(void)
 *           - Initialization of the Input Multiplexing
 *
 * @param   void
 *
 * @return  none
 */
static void InitInputmux(void)
{
    CLOCK_EnableClock(kCLOCK_InputMux);

    INPUTMUX->PWM1_FAULT[0] = 22U;      /* Route HSCMP1_OUT to PWM1 fault 0*/
    INPUTMUX->ADC1_TRIG[0] = 32U;       /* ADC1 trigger 0 <- PWM1_SM0_MUX_TRIG0 */
}

/*******************************************************************************
 * Public functions
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
    InitClock();                /* Init application clock dependent variables */
    InitSlowLoop();             /* Init slow loop counter */
    InitInputmux();             /* Init signal multiplexing */
    
    InitADC1();                 /* Init ADC1 */
    InitHsCmp1();               /* Init High Speed Comparator 1 */
    InitPWM1();                 /* 6-channel PWM1 peripheral init */
    InitCMT1();                 /* Init commutation timer using eFlexPWM submodule 1 */
}
