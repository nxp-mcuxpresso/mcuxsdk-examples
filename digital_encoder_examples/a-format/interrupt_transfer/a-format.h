/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _A_FORMAT_H_
#define _A_FORMAT_H_

#include "fsl_flexio_a-format.h"

status_t A_Format_CMD_Parse(void);
/* Getting the single-turn data and the multi-turn data */
status_t A_Format_ABS_Readout_Multi_Single_IRQ(encoder_A_format *enc, uint8_t enc_addr,
                                               encoder_abs_multi_single_t *abs_data);
/* Getting the single-turn data */
status_t A_Format_ABS_Readout_Single_IRQ(encoder_A_format *enc, uint8_t enc_addr,
                                         encoder_abs_single_t *single_data);
/* Getting the multi-turn data */
status_t A_Format_ABS_Readout_Multi_IRQ(encoder_A_format *enc, uint8_t enc_addr,
                                        encoder_abs_multi_t *multiData);
/* Reading the status of the encoder */
status_t A_Format_Readout_Encoder_status_IRQ(encoder_A_format *enc, uint8_t enc_addr,
                                             encoder_status_t *statusData);
/* Getting the temperature */
status_t A_Format_Get_Temperature_IRQ(encoder_A_format *enc, uint8_t enc_addr, float *temp);
/* The encoder ID is get by matching the encoder address */
status_t A_Format_Get_ID_IRQ(encoder_A_format *enc, uint8_t enc_addr, uint32_t *id);

#endif
