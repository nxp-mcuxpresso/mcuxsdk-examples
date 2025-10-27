/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#ifndef _NCP_PM_OS_H_
#define _NCP_PM_OS_H_

/*******************************************************************************
 * API
 ******************************************************************************/

int ncp_pm_os_init(void);
void ncp_pm_os_deinit(void);
void ncp_pm_os_activate_lp_timer(uint32_t duration_ms);

/* Toggle gpio to wakeup peer */
void ncp_pm_gpio_wakeup_peer(void);

void ncp_pm_init_wakeup_source(void *ws, uint32_t wsId, bool enable);
void ncp_pm_enable_wakeup_source(void *ws);
void ncp_pm_disable_wakeup_source(void *ws);
void ncp_pm_get_wakeup_source(void *ws);

#endif /* _NCP_PM_OS_H_ */
