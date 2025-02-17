/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _T_FORMAT_H_
#define _T_FORMAT_H_

#include "fsl_flexio_t-format.h"

char *T_Format_GetStatusFlag(status_t status);
status_t T_Format_Readout_ABS_ABM(encoder_T_format *enc, encoder_all_info_t *all_info);
status_t T_Format_Get_Encoder_ID(encoder_T_format *enc, uint8_t *encID);
status_t T_Format_Get_Temperature(encoder_T_format *enc, int8_t *temperature);

#endif
