/*
 * Copyright 2022-2023,2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HEALTH_H_
#define _HEALTH_H_

/**
 * Initializes the health process.
 */
void HEALTH_Init(void);

/**
 * Activates the health task/process.
 */
void HEALTH_Activate(void);

/**
 * Deactivates the health process and stops the task
 */
void HEALTH_Deactivate(void);

#endif /* _HEALTH_H_ */
