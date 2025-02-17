/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _A_FORMAT_H_
#define _A_FORMAT_H_

#include "fsl_flexio_a-format.h"

typedef int (*logFunc)(const char *fmt_s, ...);

void A_Format_PrintfES(logFunc logES, uint8_t es);
/* Getting the single-turn data and the multi-turn data */
status_t A_Format_ABS_Readout_Multi_Single(encoder_A_format *enc, uint8_t enc_addr,
                                           encoder_abs_multi_single_t *abs_data);
/* Getting the single-turn data */
status_t A_Format_ABS_Readout_Single(encoder_A_format *enc, uint8_t enc_addr,
                                     encoder_abs_single_t *singleData);
/* Getting the multi-turn data */
status_t A_Format_ABS_Readout_Multi(encoder_A_format *enc, uint8_t enc_addr,
                                    encoder_abs_multi_t *multiData);
/* Reading the status of the encoder */
status_t A_Format_Readout_Encoder_status(encoder_A_format *enc, uint8_t enc_addr,
                                         encoder_status_t *statusData);
/* Setting the encoder address */
status_t A_Format_Set_Encoder_Address_1to1(encoder_A_format *enc, uint8_t enc_addr);
status_t A_Format_Memory_Read(encoder_A_format *enc, uint8_t enc_addr, encoder_eeprom_t *eeprom);
status_t A_Format_Memory_Write(encoder_A_format *enc, uint8_t enc_addr, encoder_eeprom_t *eeprom);
/* Getting the temperature */
status_t A_Format_Get_Temperature(encoder_A_format *enc, uint8_t enc_addr, float *temp);
/* The encoder ID is get by matching the encoder address */
status_t A_Format_Get_ID(encoder_A_format *enc, uint8_t enc_addr, uint32_t *id);
status_t A_Format_Get_ID_1to1(encoder_A_format *enc, uint32_t *id);
status_t A_Format_Set_ID(encoder_A_format *enc, uint8_t enc_addr, uint32_t id);
status_t A_Format_Set_ID_1to1(encoder_A_format *enc, uint32_t id);
status_t A_Format_Set_Encoder_Address_MATCH_ID(encoder_A_format *enc, uint32_t id, uint8_t enc_addr);
status_t A_Format_ABS_Readout_Single_with_status(encoder_A_format *enc, uint8_t enc_addr, encoder_single_stat_t *singleStat);
status_t A_Format_ABS_Readout_Single_with_temperature(encoder_A_format *enc, uint8_t enc_addr, encoder_single_temp_t *singleTemp);

#endif
