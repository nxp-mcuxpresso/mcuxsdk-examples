/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _T_FORMAT_H_
#define _T_FORMAT_H_

#include "fsl_flexio_t-format.h"

status_t T_Format_Readout_ABS_ABM_IRQ(encoder_T_format *enc, encoder_all_info_t *all_info);
status_t T_Format_CMD_Parse(void);

#endif
