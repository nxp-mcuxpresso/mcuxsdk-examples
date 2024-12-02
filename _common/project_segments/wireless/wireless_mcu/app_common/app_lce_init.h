/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_LCE_INIT_H_
#define _APP_LCE_INIT_H_

/* CE Buffers */
#define CE_CMD_BUF_SIZE    (256U)
#define CE_STATUS_BUF_SIZE (128U + 6U)

void APP_InitLce(void);

#endif /* _APP_LCE_INIT_H_ */
