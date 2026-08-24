/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_ENCODER_PROTOCOL "EnDat2"

#define DEMO_ENCODER_EQN_1337 0U
#define DEMO_ENCODER_EQI_1331 1U
#define DEMO_ENCODER_SENSOR_EMU_250K 2U
#define DEMO_ENCODER          DEMO_ENCODER_EQI_1331

#if (DEMO_ENCODER == DEMO_ENCODER_EQN_1337)
#define DEMO_ENCODER_NAME     "EQN1337"
#define DEMO_ENCODER_BIT_RATE 15000000U
#define DEMO_ENCODER_MT_LEN   12U
#define DEMO_ENCODER_ST_LEN   25U
#elif (DEMO_ENCODER == DEMO_ENCODER_EQI_1331)
#define DEMO_ENCODER_NAME     "EQI1331_810662-03"
#define DEMO_ENCODER_BIT_RATE 15000000U
#define DEMO_ENCODER_MT_LEN   12U
#define DEMO_ENCODER_ST_LEN   19U
#elif (DEMO_ENCODER == DEMO_ENCODER_SENSOR_EMU_250K)
#define DEMO_ENCODER_NAME     "SENSOR_EMU_250K"
#define DEMO_ENCODER_BIT_RATE 250000U
#define DEMO_ENCODER_MT_LEN   0U
#define DEMO_ENCODER_ST_LEN   25U
#else
#error "Unsupported EnDat2 encoder"
#endif

#define DEMO_WAIT_RESULTS_POLLING   0U
#define DEMO_WAIT_RESULTS_INTERRUPT 1U
#define DEMO_WAIT_RESULTS           DEMO_WAIT_RESULTS_POLLING

#define DEMO_FLEXIO_TX_TRIG_SW 1U
#define DEMO_FLEXIO_TX_TRIG    DEMO_FLEXIO_TX_TRIG_SW

#define DEMO_FLEXIO_INSTANCE      (FLEXIO0)
#define DEMO_FLEXIO_IRQ_NUMBER    FLEXIO_IRQn
#define DEMO_FLEXIO_CLOCK_SOURCE  kPll1ClkDiv_to_FLEXIO0
#define DEMO_FLEXIO_CLOCK_DIV     1U
#define DEMO_FLEXIO_CLOCK_FREQ    CLOCK_GetFlexioClkFreq()
#define DEMO_FLEXIO_TXD_CHANNEL   16U
#define DEMO_FLEXIO_RXD_CHANNEL   17U
#define DEMO_FLEXIO_CLK_CHANNEL   18U
#define DEMO_FLEXIO_DIR_CHANNEL   19U
#define DEMO_SAMPLE_PERIOD_US     5000U
#define DEMO_PRINT_DECIMATION     100U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
