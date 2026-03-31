/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PVT_APP_CONFIG_H_
#define PVT_APP_CONFIG_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*-----------------DEMO configuration ----------------------*/
/*-------------------CPU0 demo configuration----------------*/
/* CPU0_DEMO_MAINCLK_FREQ_SP0, CPU0_DEMO_MAINCLK_FREQ_SP1, CPU0_DEMO_MAINCLK_FREQ_SP2 */
#ifndef CPU0_DEMO_MAINCLK_FREQ
#define CPU0_DEMO_MAINCLK_FREQ CPU0_DEMO_MAINCLK_FREQ_SP1
#endif

/* Define if PVT runs on CPU only (0), both on CPU & HiFi4(1), or HiFi4 only (2). */
#define CPU0_DEMO_PVT_ON_CPU_DSP 1

/* Enable temperature monitoring. */
#ifndef DEMO_TEMPERATURE_MONITORING
#define DEMO_TEMPERATURE_MONITORING 1U
#endif

/* Print the debug information in interrupt. */
#define CPU0_DEMO_PRINTF_INTERRUPT_STATUS 0U

#define TEMP_MONITORING_DELAY_MS 5000U
#define TEMPERATURE_RANGE_LIMIT  30U /* -30C to 30C is low temperature range, 30C to 85C is high temperature range */

/* How often for PVT task to check if VDD2 can go lower. */
#define CPU0_PVT_TASK_WAIT_MS 2000

/* Amount of time it takes the PMIC OUT to stabilize after changing it by 1 step.*/
#define CPU0_PMIC_SETTLING_TIME_MS 5U

/* Amount of time to wait in between Coremark or DSP workload runs.
 * Allows idle task to activate low power mode (deep sleep).
 */
#define CPU0_WORKLOAD_DELAY_MS 5000U

/*------------------------CPU0 macro------------------------*/
/* Valid Frequencies: 110000000U, 192000000U, 325000000U */
#define CPU0_DEMO_MAINCLK_FREQ_SP0 110000000U
#define CPU0_DEMO_MAINCLK_FREQ_SP1 192000000U
#define CPU0_DEMO_MAINCLK_FREQ_SP2 325000000U

#if (CPU0_DEMO_MAINCLK_FREQ == CPU0_DEMO_MAINCLK_FREQ_SP2)
#define MAX_VDD2                 1100000U /*1V1*/
#define PVT_DELAY_TEMP_VARIATION 1
#elif (CPU0_DEMO_MAINCLK_FREQ == CPU0_DEMO_MAINCLK_FREQ_SP1)
#define MAX_VDD2                 900000U /*0V9*/
#define PVT_DELAY_TEMP_VARIATION 1
#elif (CPU0_DEMO_MAINCLK_FREQ == CPU0_DEMO_MAINCLK_FREQ_SP0)
#define MAX_VDD2                 800000U /*0V8*/
#define PVT_DELAY_TEMP_VARIATION 1
#else
#error "Unsupported frequency point for PVTS."
#endif
#define MIN_VDD2 700000U /*0V7*/

/* Some typical delay value for TT samples.
 * !!!NOTE!!!
 * This is only used to allow runing the demo on some early typical samples which are not burned with delay values in
 * fuse and can't be used in production. The function may not work for some samples and temparautre.
 */
#if (CPU0_DEMO_MAINCLK_FREQ == CPU0_DEMO_MAINCLK_FREQ_SP2)
#define CPU0_DEMO_TYPICAL_DELAY (17U | (17U << 8U))
#elif (CPU0_DEMO_MAINCLK_FREQ == CPU0_DEMO_MAINCLK_FREQ_SP1)
#define CPU0_DEMO_TYPICAL_DELAY (19U | (19U << 8U))
#elif (CPU0_DEMO_MAINCLK_FREQ == CPU0_DEMO_MAINCLK_FREQ_SP0)
#define CPU0_DEMO_TYPICAL_DELAY (24U | (24U << 8U))
#endif
/*----------------------------------------------------------*/
/*-------------------CPU1 demo configuration----------------*/
#define CPU1_PVT_ENABLED 1
#ifndef CPU1_DEMO_MAINCLK_FREQ
#define CPU1_DEMO_MAINCLK_FREQ CPU1_DEMO_MAINCLK_FREQ_SP1
#endif

/* Define if PVT runs on CPU only (0) or both on CPU & HiFi1(1), or HiFi1 only (2). */
#define CPU1_DEMO_PVT_ON_CPU_DSP 1U

/* How often for PVT task to check if VDD1 can go lower. */
#define CPU1_PVT_TASK_WAIT_MS 3000

/* Amount of time it takes the PMIC OUT to stabilize after changing it by 1 step.*/
#define CPU1_PMIC_SETTLING_TIME_MS 5U

/* Amount of time to wait in between Coremark or DSP workload runs.
 * Allows idle task to activate low power mode (deep sleep).
 */
#define CPU1_WORKLOAD_DELAY_MS 5000U

#define CPU1_DEMO_PRINTF_INTERRUPT_STATUS 0U /* Print in interrupt. */

#define DEMO_HIFI1_SRAM_PT_START 27          /* The start SRAM partition used by HIFI1 */
#define DEMO_HIFI1_SRAM_PT_END   29          /* The end partition(contains this partition) used by HIFI1 */
/*----------------------------------------------------------*/
/*--------------------------CPU1 Marco----------------------*/
/* Valid Frequencies: 100000000U, 160000000U, 250000000U */
#define CPU1_DEMO_MAINCLK_FREQ_SP0 100000000U
#define CPU1_DEMO_MAINCLK_FREQ_SP1 160000000U
#define CPU1_DEMO_MAINCLK_FREQ_SP2 250000000U

/* Define voltage limits */
#if (CPU1_DEMO_MAINCLK_FREQ == CPU1_DEMO_MAINCLK_FREQ_SP2)
#define MAX_VDD1 1100000U /*1V1*/
#elif (CPU1_DEMO_MAINCLK_FREQ == CPU1_DEMO_MAINCLK_FREQ_SP1)
#define MAX_VDD1 900000U  /*0V9*/
#elif (CPU1_DEMO_MAINCLK_FREQ == CPU1_DEMO_MAINCLK_FREQ_SP0)
#define MAX_VDD1 800000U  /*0V8*/
#else
#error "Unsupported frequency point for PVTS."
#endif
#define MIN_VDD1 700000U /*0V7*/

/* Some typical delay value for TT samples.
 * !!!NOTE!!!
 * This is only used to allow runing the demo on some early typical samples which are not burned with delay values in
 * fuse and can't be used in production. The function may not work for some samples and temparautre.
 */
#if (CPU1_DEMO_MAINCLK_FREQ == CPU1_DEMO_MAINCLK_FREQ_SP2)
#define CPU1_DEMO_TYPICAL_DELAY (20U | (20U << 8U))
#elif (CPU1_DEMO_MAINCLK_FREQ == CPU1_DEMO_MAINCLK_FREQ_SP1)
#define CPU1_DEMO_TYPICAL_DELAY (20U | (20U << 8U))
#elif (CPU1_DEMO_MAINCLK_FREQ == CPU1_DEMO_MAINCLK_FREQ_SP0)
#define CPU1_DEMO_TYPICAL_DELAY (26U | (26U << 8U))
#endif
/*----------------------------------------------------------*/
/*-----------------Common macro ----------------------------*/
#define DEMO_PMIC_ADJUST_STEP (6250U) /* PCA9422_BUCK1/3_OUT_STEP in uV. */

#define APP_SWTICH_CLOCK_FOR_DS 1U    /* CPU1 switch to LPOSC when it goes to DS. */

/* Don't change this marco. Only PMIC supply is supported. */
#define DEMO_POWER_SUPPLY_PMIC   2U
#define DEMO_POWER_SUPPLY_OPTION DEMO_POWER_SUPPLY_PMIC

/* Channel transmit and receive register */
#define APP_MU_REG kMU_MsgReg0
#define BOOT_FLAG  0x1U /* Flag indicates Core1 Boot Up*/
/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif /* PVT_APP_CONFIG_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
