/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "tzm_config.h"

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
   
/* SAU region boundaries */
#define SAU_REGION_0_BASE 0x00810000U
#define SAU_REGION_0_END  0x008FFFFFU
#define SAU_REGION_1_BASE 0x20004000U
#define SAU_REGION_1_END  0x2001BFFFU
#define SAU_REGION_2_BASE 0x1080FE00U
#define SAU_REGION_2_END  0x1080FFFFU
#define SAU_REGION_3_BASE 0x40000000U
#define SAU_REGION_3_END  0x4FFFFFFFU

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
functional_group:
- called_from_default_init: 'true'
- id_prefix: ''
- prefix_user_defined: 'true'
- name: 'BOARD_InitTrustZone'
- description: ''
- trdc:
  - interrupts:
    - security: {Secure: {id: [adc0_irq, can0_irq, cmc0_irq, cti_irq, dma0_ch0_irq, dma0_ch10_irq, dma0_ch11_irq, dma0_ch12_irq, dma0_ch13_irq, dma0_ch14_irq, dma0_ch15_irq,
          dma0_ch1_irq, dma0_ch2_irq, dma0_ch3_irq, dma0_ch4_irq, dma0_ch5_irq, dma0_ch6_irq, dma0_ch7_irq, dma0_ch8_irq, dma0_ch9_irq, dsb_irq, ele_cmd_irq, ele_nonsecure_irq,
          ele_secure_irq, ewm0_irq, flexio0_irq, fmu0_irq, gpioa_int0_irq, gpioa_int1_irq, gpiob_int0_irq, gpiob_int1_irq, gpioc_int0_irq, gpioc_int1_irq, gpiod_int0_irq,
          gpiod_int1_irq, i3c0_irq, lpcmp0_irq, lpcmp1_irq, lpi2c0_irq, lpi2c1_irq, lpit0_irq, lpspi0_irq, lpspi1_irq, lptmr0_irq, lptmr1_irq, lpuart0_irq, lpuart1_irq,
          mcm0_irq, mscm0_irq, porta_irq, portb_irq, portc_irq, portd_irq, rf_bric_irq, rf_fmu_irq, rf_generic_irq, rf_imu0_irq, rf_imu1_irq, rf_lant_sw_irq, rf_nbu_irq,
          rf_wor_irq, rfmc_irq, rtc_alarm_irq, rtc_seconds_irq, scg0_irq, sfa0_irq, spc0_irq, tpm0_irq, tpm1_irq, trdc0_irq, vbat_irq, wdog0_irq, wdog1_irq, wuu0_irq]}}
  - ports:
    - GPIOA: {pin_security: {id: '0', security: s_priv}, pin_security: {id: '1', security: s_priv}, pin_security: {id: '16', security: s_priv}, pin_security: {
        id: '17', security: s_priv}, pin_security: {id: '18', security: s_priv}, pin_security: {id: '19', security: s_priv}, pin_security: {id: '20', security: s_priv},
      pin_security: {id: '21', security: s_priv}, pin_security: {id: '4', security: s_priv}, pin_interrupt_security: {id: '0', security: s_priv}, pin_interrupt_security: {
        id: '1', security: s_priv}}
    - GPIOB: {pin_security: {id: '0', security: s_priv}, pin_security: {id: '1', security: s_priv}, pin_security: {id: '2', security: s_priv}, pin_security: {id: '3',
        security: s_priv}, pin_security: {id: '4', security: s_priv}, pin_security: {id: '5', security: s_priv}, pin_interrupt_security: {id: '0', security: s_priv},
      pin_interrupt_security: {id: '1', security: s_priv}}
    - GPIOC: {pin_security: {id: '0', security: s_priv}, pin_security: {id: '1', security: s_priv}, pin_security: {id: '2', security: s_priv}, pin_security: {id: '3',
        security: s_priv}, pin_security: {id: '4', security: s_priv}, pin_security: {id: '5', security: s_priv}, pin_security: {id: '6', security: s_priv}, pin_security: {
        id: '7', security: s_priv}, pin_interrupt_security: {id: '0', security: s_priv}, pin_interrupt_security: {id: '1', security: s_priv}}
    - GPIOD: {pin_security: {id: '0', security: s_priv}, pin_security: {id: '1', security: s_priv}, pin_security: {id: '2', security: s_priv}, pin_security: {id: '3',
        security: s_priv}, pin_security: {id: '4', security: s_priv}, pin_security: {id: '5', security: s_priv}, pin_interrupt_security: {id: '0', security: s_priv},
      pin_interrupt_security: {id: '1', security: s_priv}}
  - masters: [{id: cm33, domain_assignments: [{enabled: 'true', locked: 'true', domain_id: '0', domain_input: mda}]}, {id: dma3, domain_assignments: [{enabled: 'false',
          locked: 'false', domain_id: '0', secure: bypass, privileged: bypass, domain_bypass: 'false'}]}, {id: dsb, domain_assignments: [{enabled: 'false', locked: 'false',
          domain_id: '0', secure: bypass, privileged: bypass, domain_bypass: 'false'}]}, {id: nbu, domain_assignments: [{enabled: 'false', locked: 'false', domain_id: '0',
          secure: bypass, privileged: bypass, domain_bypass: 'false'}]}]
  - mbcs:
    - mbc:
      - checker_id: 'MBC0'
      - using_global_templates: 'true'
      - slaves: [{id: FLASH, base_region: {start: '0x00000000', size: '0x00010000'}, domains: [{security: s, template: FLASH}, {security: s, template: NO_ACCESS},
            {security: s, template: NO_ACCESS}]}, {id: FLASH, base_region: {start: '0x00010000', size: '0x000F0000'}, domains: [{security: ns, template: FLASH}, {
              security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: IFR0, base_region: {start: '0x00000000', size: '0x00008000'}, domains: [
            {security: s, template: FLASH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: IFR1, base_region: {start: '0x00000000',
            size: '0x00002000'}, domains: [{security: s, template: FLASH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: ROM_BOOT,
          base_region: {start: '0x00000000', size: '0x00018000'}, domains: [{security: s, template: FLASH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}]
    - mbc:
      - checker_id: 'MBC1'
      - using_global_templates: 'true'
      - slaves: [{id: TCM_CODE, base_region: {start: '0x00000000', size: '0x00004000'}, domains: [{security: s, template: RAM}, {security: s, template: NO_ACCESS},
            {security: s, template: NO_ACCESS}]}, {id: TCM_SYS, base_region: {start: '0x00000000', size: '0x00004000'}, domains: [{security: s, template: RAM}, {
              security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: TCM_SYS, base_region: {start: '0x00004000', size: '0x00018000'}, domains: [
            {security: ns, template: RAM}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}]
    - mbc:
      - checker_id: 'MBC2'
      - using_global_templates: 'true'
      - slaves: [{id: AXBS0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: CMC0, domains: [
            {security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: DMA0, domains: [{security: s, template: PERIPH},
            {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: EWM0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS},
            {security: s, template: NO_ACCESS}]}, {id: MSCM, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]},
        {id: SMSCM, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: SPC0, domains: [{security: s,
              template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: SYSPM, domains: [{security: s, template: PERIPH},
            {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: TRGMUX0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS},
            {security: s, template: NO_ACCESS}]}, {id: WUU0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]},
        {id: WDOG0, domains: [{security: ns, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: WDOG1, domains: [{
              security: ns, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: MRCC, domains: [{security: s, template: PERIPH},
            {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: SFA0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS},
            {security: s, template: NO_ACCESS}]}, {id: SCG0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]},
        {id: CCM32K, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: FMU0, domains: [{security: s,
              template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: REGFILE0, domains: [{security: s, template: PERIPH},
            {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: REGFILE1, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS},
            {security: s, template: NO_ACCESS}]}, {id: CRC0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]},
        {id: ELEMUA, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: NPX, domains: [{security: s,
              template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: TRDC, domains: [{security: s, template: PERIPH}, {
              security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: VBAT0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS},
            {security: s, template: NO_ACCESS}]}, {id: RTC, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]},
        {id: LPTMR0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: LPTMR1, domains: [
            {security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: LPIT0, domains: [{security: s, template: PERIPH},
            {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: TSTMR0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS},
            {security: s, template: NO_ACCESS}]}, {id: TPM0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]},
        {id: TPM1, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: LPI2C0, domains: [{security: s,
              template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: LPI2C1, domains: [{security: s, template: PERIPH},
            {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: I3C, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS},
            {security: s, template: NO_ACCESS}]}, {id: LPSPI0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]},
        {id: LPSPI1, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: LPUART0, domains: [
            {security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: LPUART1, domains: [{security: s, template: PERIPH},
            {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: FLEXIO0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS},
            {security: s, template: NO_ACCESS}]}, {id: CAN0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]},
        {id: SEMA42, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: RFMC, domains: [{security: s,
              template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: DSB0, domains: [{security: s, template: PERIPH}, {
              security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: PORTA, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS},
            {security: s, template: NO_ACCESS}]}, {id: PORTB, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]},
        {id: PORTC, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: PORTD, domains: [{security: s,
              template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: GPIOD, domains: [{security: s, template: PERIPH},
            {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: ADC0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS},
            {security: s, template: NO_ACCESS}]}, {id: LPCMP0, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]},
        {id: LPCMP1, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: VREF0, domains: [{
              security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: GPIOA, domains: [{security: s, template: PERIPH},
            {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: GPIOB, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS},
            {security: s, template: NO_ACCESS}]}, {id: GPIOC, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]},
        {id: GENFSK, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: RADIO_CTRL, domains: [
            {security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: XCVR_RX_DIG, domains: [{security: s, template: PERIPH},
            {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: TX_PACKET_RAM, domains: [{security: s, template: PERIPH}, {security: s,
              template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: eDMA-TCD-CHx, base_region: {start: '0x00000000', size: '0x00010000'}, domains: [{security: s,
              template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: TRDC-MBC0, base_region: {start: '0x00000000', size: '0x00001000'},
          domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: TRDC-MBC1, base_region: {start: '0x00000000',
            size: '0x00001000'}, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: TRDC-MBC2,
          base_region: {start: '0x00000000', size: '0x00001000'}, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]},
        {id: TRDC-MRC0, base_region: {start: '0x00000000', size: '0x00001000'}, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s,
              template: NO_ACCESS}]}, {id: ISP-AP0, base_region: {start: '0x00000000', size: '0x00010000'}, domains: [{security: s, template: PERIPH}, {security: s,
              template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: RF_GENLL_REMAP0, base_region: {start: '0x00000000', size: '0x00001000'}, domains: [
            {security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: RF_GENLL_REMAP1, base_region: {start: '0x00000000',
            size: '0x00001000'}, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}, {id: RF_GENLL_REMAP2,
          base_region: {start: '0x00000000', size: '0x00001000'}, domains: [{security: s, template: PERIPH}, {security: s, template: NO_ACCESS}, {security: s, template: NO_ACCESS}]}]
  - mrcs:
    - mrc:
      - checker_id: 'MRC0'
      - using_global_templates: 'true'
      - domains: [[{index: '0', enabled: 'true', security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '1', enabled: 'false', security: s,
            template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '2', enabled: 'false', security: s, template: Template_1, start: '0x48800000',
            size: '0x00200000'}, {index: '3', enabled: 'false', security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '4', enabled: 'false',
            security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '5', enabled: 'false', security: s, template: Template_1, start: '0x48800000',
            size: '0x00200000'}, {index: '6', enabled: 'false', security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '7', enabled: 'false',
            security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}], [{index: '0', enabled: 'false', security: s, template: Template_1, start: '0x48800000',
            size: '0x00200000'}, {index: '1', enabled: 'false', security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '2', enabled: 'false',
            security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '3', enabled: 'false', security: s, template: Template_1, start: '0x48800000',
            size: '0x00200000'}, {index: '4', enabled: 'false', security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '5', enabled: 'false',
            security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '6', enabled: 'false', security: s, template: Template_1, start: '0x48800000',
            size: '0x00200000'}, {index: '7', enabled: 'false', security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}], [{index: '0', enabled: 'false',
            security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '1', enabled: 'false', security: s, template: Template_1, start: '0x48800000',
            size: '0x00200000'}, {index: '2', enabled: 'false', security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '3', enabled: 'false',
            security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '4', enabled: 'false', security: s, template: Template_1, start: '0x48800000',
            size: '0x00200000'}, {index: '5', enabled: 'false', security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '6', enabled: 'false',
            security: s, template: Template_1, start: '0x48800000', size: '0x00200000'}, {index: '7', enabled: 'false', security: s, template: Template_1, start: '0x48800000',
            size: '0x00200000'}]]
- sau:
  - enabled: 'true'
  - all_non_secure: 'false'
  - generate_code_for_disabled_regions: 'false'
  - regions: [{index: '0', enabled: 'true', security: ns, start: '0x00010000', size: '0x000F0000'}, {index: '1', enabled: 'true', security: ns, start: '0x20004000',
      size: '0x00018000'}, {index: '2', enabled: 'true', security: nsc, start: '0x1000FE00', size: '0x00000200'}, {index: '3', enabled: 'true', security: ns, start: '0x40000000',
      size: '0x10000000'}, {index: '4', enabled: 'false', security: ns, start: '0x00000000', size: '0x00000020'}, {index: '5', enabled: 'false', security: ns, start: '0x00000000',
      size: '0x00000020'}, {index: '6', enabled: 'false', security: ns, start: '0x00000000', size: '0x00000020'}, {index: '7', enabled: 'false', security: ns, start: '0x00000000',
      size: '0x00000020'}]
- global_options:
  - yes:
    - id: [TRDC_CR_GVLDM, TRDC_CR_GVLDB, TRDC_IDAU_CR_VLD, NSACR_CP0, NSACR_CP1, NSACR_CP10, NSACR_CP11]
  - no:
    - id: [TRDC_CR_GVLDR, TRDC_IDAU_CR_LKSAU, TRDC_IDAU_CR_LKNSMPU, TRDC_IDAU_CR_LKSMPU, TRDC_IDAU_CR_LKNSVTOR, TRDC_IDAU_CR_LKSVTAIRCR, AIRCR_PRIS, AIRCR_BFHFNMINS,
      AIRCR_SYSRESETREQS, SCR_SLEEPDEEPS, SHCSR_SECUREFAULTENA, NSACR_CP2, NSACR_CP3, NSACR_CP4, NSACR_CP5, NSACR_CP6, NSACR_CP7, CPPWR_SU0, CPPWR_SUS0, CPPWR_SU1,
      CPPWR_SUS1, CPPWR_SU2, CPPWR_SUS2, CPPWR_SU3, CPPWR_SUS3, CPPWR_SU4, CPPWR_SUS4, CPPWR_SU5, CPPWR_SUS5, CPPWR_SU6, CPPWR_SUS6, CPPWR_SU7, CPPWR_SUS7, CPPWR_SU10,
      CPPWR_SUS10, CPPWR_SU11, CPPWR_SUS11, GPIOA_PCNS, GPIOA_ICNS, GPIOA_PCNP, GPIOA_ICNP, GPIOB_PCNS, GPIOB_ICNS, GPIOB_PCNP, GPIOB_ICNP, GPIOC_PCNS, GPIOC_ICNS,
      GPIOC_PCNP, GPIOC_ICNP, GPIOD_PCNS, GPIOD_ICNS, GPIOD_PCNP, GPIOD_ICNP]
  - 0:
    - id: [CPACR_CP0, CPACR_CP1, CPACR_CP2, CPACR_CP3, CPACR_CP4, CPACR_CP5, CPACR_CP6, CPACR_CP7]
  - 3:
    - id: [CPACR_CP10, CPACR_CP11]
- user_memory_regions: []
- mpus:
  - mpu:
    - enabled: 'false'
    - id: 's'
    - priv_default_map: 'false'
    - handler_enabled: 'false'
    - generate_code_for_disabled_regions: 'false'
    - attributes: [{index: '0', id: '0', memory_type: device, device: nGnRE, regions_properties: [{id: inner, cacheable: 'false', write_back: 'false', read_allocation: 'false',
            write_allocation: 'false', transientness: 'false'}, {id: outer, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false',
            transientness: 'false'}]}, {index: '1', id: '1', memory_type: device, device: nGnRE, regions_properties: [{id: inner, cacheable: 'false', write_back: 'false',
            read_allocation: 'false', write_allocation: 'false', transientness: 'false'}, {id: outer, cacheable: 'false', write_back: 'false', read_allocation: 'false',
            write_allocation: 'false', transientness: 'false'}]}, {index: '2', id: '2', memory_type: device, device: nGnRE, regions_properties: [{id: inner, cacheable: 'false',
            write_back: 'false', read_allocation: 'false', write_allocation: 'false', transientness: 'false'}, {id: outer, cacheable: 'false', write_back: 'false',
            read_allocation: 'false', write_allocation: 'false', transientness: 'false'}]}, {index: '3', id: '3', memory_type: device, device: nGnRE, regions_properties: [
          {id: inner, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false', transientness: 'false'}, {id: outer, cacheable: 'false',
            write_back: 'false', read_allocation: 'false', write_allocation: 'false', transientness: 'false'}]}, {index: '4', id: '4', memory_type: device, device: nGnRE,
        regions_properties: [{id: inner, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false', transientness: 'false'}, {
            id: outer, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false', transientness: 'false'}]}, {index: '5', id: '5',
        memory_type: device, device: nGnRE, regions_properties: [{id: inner, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false',
            transientness: 'false'}, {id: outer, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false', transientness: 'false'}]},
      {index: '6', id: '6', memory_type: device, device: nGnRE, regions_properties: [{id: inner, cacheable: 'false', write_back: 'false', read_allocation: 'false',
            write_allocation: 'false', transientness: 'false'}, {id: outer, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false',
            transientness: 'false'}]}, {index: '7', id: '7', memory_type: device, device: nGnRE, regions_properties: [{id: inner, cacheable: 'false', write_back: 'false',
            read_allocation: 'false', write_allocation: 'false', transientness: 'false'}, {id: outer, cacheable: 'false', write_back: 'false', read_allocation: 'false',
            write_allocation: 'false', transientness: 'false'}]}]
    - regions: [{executable: 'false', read_only: 'false', shareability: non_shareable, attributes_index: '0', index: '0', enabled: 'false', security: priv, start: '0x00000000',
        size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable, attributes_index: '1', index: '1', enabled: 'false', security: priv,
        start: '0x00000000', size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable, attributes_index: '2', index: '2', enabled: 'false',
        security: priv, start: '0x00000000', size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable, attributes_index: '3', index: '3',
        enabled: 'false', security: priv, start: '0x00000000', size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable, attributes_index: '4',
        index: '4', enabled: 'false', security: priv, start: '0x00000000', size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable,
        attributes_index: '5', index: '5', enabled: 'false', security: priv, start: '0x00000000', size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable,
        attributes_index: '6', index: '6', enabled: 'false', security: priv, start: '0x00000000', size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable,
        attributes_index: '7', index: '7', enabled: 'false', security: priv, start: '0x00000000', size: '0x00000020'}]
  - mpu:
    - enabled: 'false'
    - id: 'ns'
    - priv_default_map: 'false'
    - handler_enabled: 'false'
    - generate_code_for_disabled_regions: 'false'
    - attributes: [{index: '0', id: '0', memory_type: device, device: nGnRE, regions_properties: [{id: inner, cacheable: 'false', write_back: 'false', read_allocation: 'false',
            write_allocation: 'false', transientness: 'false'}, {id: outer, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false',
            transientness: 'false'}]}, {index: '1', id: '1', memory_type: device, device: nGnRE, regions_properties: [{id: inner, cacheable: 'false', write_back: 'false',
            read_allocation: 'false', write_allocation: 'false', transientness: 'false'}, {id: outer, cacheable: 'false', write_back: 'false', read_allocation: 'false',
            write_allocation: 'false', transientness: 'false'}]}, {index: '2', id: '2', memory_type: device, device: nGnRE, regions_properties: [{id: inner, cacheable: 'false',
            write_back: 'false', read_allocation: 'false', write_allocation: 'false', transientness: 'false'}, {id: outer, cacheable: 'false', write_back: 'false',
            read_allocation: 'false', write_allocation: 'false', transientness: 'false'}]}, {index: '3', id: '3', memory_type: device, device: nGnRE, regions_properties: [
          {id: inner, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false', transientness: 'false'}, {id: outer, cacheable: 'false',
            write_back: 'false', read_allocation: 'false', write_allocation: 'false', transientness: 'false'}]}, {index: '4', id: '4', memory_type: device, device: nGnRE,
        regions_properties: [{id: inner, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false', transientness: 'false'}, {
            id: outer, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false', transientness: 'false'}]}, {index: '5', id: '5',
        memory_type: device, device: nGnRE, regions_properties: [{id: inner, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false',
            transientness: 'false'}, {id: outer, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false', transientness: 'false'}]},
      {index: '6', id: '6', memory_type: device, device: nGnRE, regions_properties: [{id: inner, cacheable: 'false', write_back: 'false', read_allocation: 'false',
            write_allocation: 'false', transientness: 'false'}, {id: outer, cacheable: 'false', write_back: 'false', read_allocation: 'false', write_allocation: 'false',
            transientness: 'false'}]}, {index: '7', id: '7', memory_type: device, device: nGnRE, regions_properties: [{id: inner, cacheable: 'false', write_back: 'false',
            read_allocation: 'false', write_allocation: 'false', transientness: 'false'}, {id: outer, cacheable: 'false', write_back: 'false', read_allocation: 'false',
            write_allocation: 'false', transientness: 'false'}]}]
    - regions: [{executable: 'false', read_only: 'false', shareability: non_shareable, attributes_index: '0', index: '0', enabled: 'false', security: priv, start: '0x00000000',
        size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable, attributes_index: '1', index: '1', enabled: 'false', security: priv,
        start: '0x00000000', size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable, attributes_index: '2', index: '2', enabled: 'false',
        security: priv, start: '0x00000000', size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable, attributes_index: '3', index: '3',
        enabled: 'false', security: priv, start: '0x00000000', size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable, attributes_index: '4',
        index: '4', enabled: 'false', security: priv, start: '0x00000000', size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable,
        attributes_index: '5', index: '5', enabled: 'false', security: priv, start: '0x00000000', size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable,
        attributes_index: '6', index: '6', enabled: 'false', security: priv, start: '0x00000000', size: '0x00000020'}, {executable: 'false', read_only: 'false', shareability: non_shareable,
        attributes_index: '7', index: '7', enabled: 'false', security: priv, start: '0x00000000', size: '0x00000020'}]
- access_templates: {template: {id: NO_ACCESS, name: No access, locked: 'true', accesses: {ns_user: '000', ns_priv: '000', s_user: '000', s_priv: '000'}}, template: {
    id: FLASH, name: ROM/Flash memory, locked: 'true', accesses: {ns_user: '101', ns_priv: '101', s_user: '101', s_priv: '101'}}, template: {id: RAM, name: RAM memory,
    locked: 'true', accesses: {ns_user: '111', ns_priv: '111', s_user: '111', s_priv: '111'}}, template: {id: PERIPH, locked: 'false', accesses: {ns_user: '110',
      ns_priv: '110', s_user: '110', s_priv: '110'}}}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitTrustZone
 * Description   :
 *
 * END ****************************************************************************************************************/

/***********************************************************************************************************************
 * BOARD_InitTrustZone function
 **********************************************************************************************************************/
void BOARD_InitTrustZone()
{
    /* SAU / IDAU configuration */

    /* Set SAU Control register: Disable SAU and All Secure */
    SAU->CTRL = 0;

    /* Set SAU region number */
    SAU->RNR = 0;
    /* Region base address */
    SAU->RBAR = SAU_REGION_0_BASE & SAU_RBAR_BADDR_Msk;
    /* Region end address */
    SAU->RLAR = (SAU_REGION_0_END & SAU_RLAR_LADDR_Msk) | ((0U << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk) |
                ((1U << SAU_RLAR_ENABLE_Pos) & SAU_RLAR_ENABLE_Msk);

    /* Set SAU region number */
    SAU->RNR = 1;
    /* Region base address */
    SAU->RBAR = SAU_REGION_1_BASE & SAU_RBAR_BADDR_Msk;
    /* Region end address */
    SAU->RLAR = (SAU_REGION_1_END & SAU_RLAR_LADDR_Msk) | ((0U << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk) |
                ((1U << SAU_RLAR_ENABLE_Pos) & SAU_RLAR_ENABLE_Msk);

    /* Set SAU region number */
    SAU->RNR = 2;
    /* Region base address */
    SAU->RBAR = SAU_REGION_2_BASE & SAU_RBAR_BADDR_Msk;
    /* Region end address */
    SAU->RLAR = (SAU_REGION_2_END & SAU_RLAR_LADDR_Msk) | ((1U << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk) |
                ((1U << SAU_RLAR_ENABLE_Pos) & SAU_RLAR_ENABLE_Msk);

    /* Set SAU region number */
    SAU->RNR = 3;
    /* Region base address */
    SAU->RBAR = SAU_REGION_3_BASE & SAU_RBAR_BADDR_Msk;
    /* Region end address */
    SAU->RLAR = (SAU_REGION_3_END & SAU_RLAR_LADDR_Msk) | ((0U << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk) |
                ((1U << SAU_RLAR_ENABLE_Pos) & SAU_RLAR_ENABLE_Msk);

    /* Force memory writes before continuing */
    __DSB();
    /* Flush and refill pipeline with updated permissions */
    __ISB();

    /* Set IDAU Control register: enable IDAU */
    TRDC->TRDC_IDAU_CR = (TRDC->TRDC_IDAU_CR & 0xFFFFFFFEU) | 0x00000001U;

    /* Set SAU Control register: Enable SAU and All Secure (applied only if disabled) */
    SAU->CTRL = ((0U << SAU_CTRL_ALLNS_Pos) & SAU_CTRL_ALLNS_Msk) | ((1U << SAU_CTRL_ENABLE_Pos) & SAU_CTRL_ENABLE_Msk);

    /* =========================== TRDC configuration =====================*/    

    
    /*=====  RAM =====*/
    /* Security level configuration of MBC 1, responsible for RAM configs
        32 KB secure memory configured, each nible controls 4KB block size of SRAM, Only configuring SRAM0 and SRAM1, used by application core */
    TRDC->MBC_INDEX[1].MBC_DOM0_MEM0_BLK_CFG_W[0] = 0xBBBB4444U;  
    TRDC->MBC_INDEX[1].MBC_DOM0_MEM0_BLK_CFG_W[1] = 0xBBBBBBBBU;
    
    TRDC->MBC_INDEX[1].MBC_DOM0_MEM1_BLK_CFG_W[0] = 0xBBBBBBBBU;
    TRDC->MBC_INDEX[1].MBC_DOM0_MEM1_BLK_CFG_W[1] = 0xBBBBBBBBU;

    /*=====  FLASH =====*/
    /* Security level configuration of MBC 3, responsible for Flash configs
       size of flash is 32KB x 48 blocks = 1.5MB,
       353 KB for NBU firmware, thats 11 blocks
       64 KB for TZ secure app, thats 2 blocks
       rest around 960 KB for non-secure app, 30 blocks */
    TRDC->MBC_INDEX[3].MBC_DOM0_MEM0_BLK_CFG_W[0] = 0x44444444U;         //P0-FLASH (32K , 48 blocks) 8
    TRDC->MBC_INDEX[3].MBC_DOM0_MEM0_BLK_CFG_W[1] = 0x88844444U;         //P0-FLASH (32K , 48 blocks) 16
    TRDC->MBC_INDEX[3].MBC_DOM0_MEM0_BLK_CFG_W[2] = 0x88888888U;         //P0-FLASH (32K , 48 blocks) 24
    TRDC->MBC_INDEX[3].MBC_DOM0_MEM0_BLK_CFG_W[3] = 0x88888888U;         //P0-FLASH (32K , 48 blocks) 32
    TRDC->MBC_INDEX[3].MBC_DOM0_MEM0_BLK_CFG_W[4] = 0x88888888U;         //P0-FLASH (32K , 48 blocks) 40
    TRDC->MBC_INDEX[3].MBC_DOM0_MEM0_BLK_CFG_W[5] = 0x88888888U;         //P0-FLASH (32K , 48 blocks) 48

    /* no configs required  IFR0. IFR1 and ROM regions */

    /*=====  Peripherals =====*/
    /* Peripherals: MBC 0*/
        
    /* LSB to MSB: WDT1, TPM2, RF_CMC0, DSB0, INTM0 */
    TRDC->MBC_INDEX[0].MBC_DOM0_MEM0_BLK_CFG_W[0] = 0x00099999U;
    
    /* Slave 1 peripheral entries mem blocks*/
    //TRDC->MBC_INDEX[0].MBC_DOM0_MEM1_BLK_CFG_W[6] =
    
    /* LSB to MSB: SPC0, CMC0, SCG0, LPSPI0, LPUART0, LPTMR0, LPTMR1, TPM1 */
    TRDC->MBC_INDEX[0].MBC_DOM0_MEM2_BLK_CFG_W[0] = 0x99999999U;
    
    /* LSB to MSB: SEMA0,SRTC0, ANAREG0, REGFILE0, TRDC0-MGR, TRDC0-MBC00, TRDC0-MBC1, TRDC0-MBC2 */
    TRDC->MBC_INDEX[0].MBC_DOM0_MEM2_BLK_CFG_W[1] = 0x55559999U;
    
    /* LSB to MSB: TRDC0-MBC3, TRDC0-MBC01, MU0-A, MU0-B, WUU0, PORTD, GPIOD, PORTA */
    TRDC->MBC_INDEX[0].MBC_DOM0_MEM2_BLK_CFG_W[2] = 0x99999955U;
    
    /* LSB to MSB: SFA0, TSTMR0, TSTMR1_0, TSTMR1_1, EWM0, WDT0, SYSCON0, SECCON0 */
    TRDC->MBC_INDEX[0].MBC_DOM0_MEM2_BLK_CFG_W[3] = 0x99999999U;
    
    /* LSB to MSB: LPI2C0, MRCC0, TDET0, RFMC */
    TRDC->MBC_INDEX[0].MBC_DOM0_MEM2_BLK_CFG_W[4] = 0x55559999U;
    
    /* no entries yet for these mem blocks*/
    //TRDC->MBC_INDEX[0].MBC_DOM0_MEM2_BLK_CFG_W[5] = 
    //TRDC->MBC_INDEX[0].MBC_DOM0_MEM2_BLK_CFG_W[6] = 
    
    /* GPIOA */
    TRDC->MBC_INDEX[0].MBC_DOM0_MEM3_BLK_CFG_W[0] = 0x00000009U;
    
    /* Peripheral MBC 5 */
        
    /* LSB to MSB: AXBS0, DMA0-MP, DMA0-CH0, DMA0-CH1, DMA0-CH2, DMA0-CH3, DMA0-CH5, DMA0-CH5 */
    TRDC->MBC_INDEX[4].MBC_DOM0_MEM0_BLK_CFG_W[0] = 0x99999995U;
    
    /* LSB to MSB: DMA0-CH6, DMA0-CH7, DMA0-CH9, DMA0-CH9, DMA0-CH10, DMA0-CH11, DMA0-CH12, DMA0-CH13 */
    TRDC->MBC_INDEX[4].MBC_DOM0_MEM0_BLK_CFG_W[1] = 0x99999999U;
    
    /* LSB to MSB: DMA0-CH15, DMA0-CH15, MSCM0, SMSCM0, SYSPM0, TRGMUX0, ITRC0, FMU0 */
    TRDC->MBC_INDEX[4].MBC_DOM0_MEM0_BLK_CFG_W[2] = 0x55555599U;
    
    /* LSB to MSB: CRC0, CRC1, NPX0, NPX1, FARB, LPIT0, TPM0, TPM3 */
    TRDC->MBC_INDEX[4].MBC_DOM0_MEM0_BLK_CFG_W[3] = 0x99999999U;
    
    /* LSB to MSB: TPM5,FlexPWM0,-,PKCWRAP0,-,LPSPI1,LPSPI2,LPUART1 */
    TRDC->MBC_INDEX[4].MBC_DOM0_MEM0_BLK_CFG_W[4] = 0x99995999U;
    
    /* LSB to MSB: CAN0, CAN0, CAN0, CAN0 , LPI2C1,  PORTB, PORTC, ADC-GP0 */
    TRDC->MBC_INDEX[4].MBC_DOM0_MEM0_BLK_CFG_W[5] = 0x99999999U;
    
    /* LSB to MSB: CMP-GP0, ATX0, FMU0-TST, MTR0, TCU0, ROMCP0, AOI0, MU1-A */
    TRDC->MBC_INDEX[4].MBC_DOM0_MEM0_BLK_CFG_W[6] = 0x99999999U;
    
     /* LSB to MSB: MU1-B, DGDET0, SGI0, UDF0, TRNG0, AXBS_MAIN0 */
    TRDC->MBC_INDEX[4].MBC_DOM0_MEM0_BLK_CFG_W[7] = 0x99555559U;
    
    /* LSB to MSB: ISP-AP0, GPIOB, GPIOC, PKC RAM, CWDT0 */
    TRDC->MBC_INDEX[4].MBC_DOM0_MEM1_BLK_CFG_W[0] = 0x95999999U;    
     
    /*--------------------------------------------------------------------
     - Pins: Access protection on IO pins and GPIO port IRQ/DMA requests
     -------------------------------------------------------------------*/
    /* Possible values for security control:
     * *-----------=----------=---------------------=---------------------*
     * | Bit-field |  Config  |   Security access   |  Privilege access   |
     * |   value   |  state   | control (PCNS/ICNS) | control (PCNP/ICNP) |
     * *-----------+----------+---------------------+---------------------*
     * |    0b00   |  S-Priv  |        Secure       |      Privilege      |
     * |    0b01   |  S-User  |        Secure       |    Non-Privilege    |
     * |    0b10   |  NS-Priv |      Non-Secure     |      Privilege      |
     * |    0b11   |  NS-User |      Non-Secure     |    Non-Privilege    |
     * *-----------=----------=---------------------=---------------------*
     * NOTE:
     * Pin Control Non-Secure (PCNS) & Non-Privilege (PCNP) configurable for every IO pin,
     * Interrupt Control Non-Secure (ICNS) & Non-Privilege (ICNP) for requests 0 a 1 only. */

//    GPIOA->PCNS = 0;
//    GPIOB->PCNS = 0;
//    GPIOC->PCNS = 0;
//    GPIOD->PCNS = 0;
//    GPIOA->PCNP = 0;
//    GPIOB->PCNP = 0;
//    GPIOC->PCNP = 0;
//    GPIOD->PCNP = 0;
//    GPIOA->ICNS = 0;
//    GPIOB->ICNS = 0;
//    GPIOC->ICNS = 0;
//    GPIOD->ICNS = 0;
//    GPIOA->ICNP = 0;
//    GPIOB->ICNP = 0;
//    GPIOC->ICNP = 0;
//    GPIOD->ICNP = 0;

    /*--------------------------------------------------------------------
     - Interrupts: Interrupt security configuration
     -------------------------------------------------------------------*/
    /* Possible values for every interrupt:
     *  0b0    Secure
     *  0b1    Non-secure */
    NVIC->ITNS[0] = 0;
    NVIC->ITNS[1] = 0;
    NVIC->ITNS[2] = 0;

    /* Global Options */
    TRDC->TRDC_CR      = (TRDC->TRDC_CR & 0xFFFF3FFEU) | 0x00004001U;
    TRDC->TRDC_IDAU_CR = (TRDC->TRDC_IDAU_CR & 0xFFFFE0FEU) | 0x00000001U;
    SCB->AIRCR         = (SCB->AIRCR & 0x00009FF7U) | 0x05FA0000U;
    SCB->SCR &= 0xFFFFFFF7U;
    SCB->SHCSR &= 0xFFF7FFFFU;
    SCB->CPACR    = 0x00F00000U;
    SCB->NSACR    = 0x00000C03U;
    SCnSCB->CPPWR = 0;
    
//    GPIOA->LOCK   = 0;
//    GPIOB->LOCK   = 0;
//    GPIOC->LOCK   = 0;
//    GPIOD->LOCK   = 0;
}

/***********************************************************************************************************************
 * BOARD_InitBootTEE function
 **********************************************************************************************************************/
void BOARD_InitBootTEE()
{
    BOARD_InitTrustZone();
}
