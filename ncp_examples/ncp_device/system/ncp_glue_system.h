/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef __NCP_GLUE_SYSTEM_H__
#define __NCP_GLUE_SYSTEM_H__

#include "ncp_cmd_system.h"

#define NCP_RESET_FLAG_MAGIC 0x9A

NCPCmd_DS_SYS_COMMAND *ncp_sys_get_resp_buf(void);
uint8_t *ncp_sys_evt_status(uint32_t evt_id, void *msg);
void ncp_get_sys_resp_buf_lock();
void ncp_put_sys_resp_buf_lock();
int ncp_sys_get_host_type(void);
int ncp_sys_dev_reset_send_rsp(void);
#endif /* __NCP_GLUE_SYSTEM_H__ */
