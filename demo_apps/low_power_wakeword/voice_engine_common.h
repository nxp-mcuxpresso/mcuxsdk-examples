/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _VOICE_ENGINE_H_
#define _VOICE_ENGINE_H_

/* Initiates voice engine. */
void voice_engine_create(void);

/* Functions used by voice engine core
 * implementation specific to the underlying algorithm.
 */
void voice_engine_algo_init(void);
uint8_t voice_engine_algo_process(void *data);
void voice_engine_algo_reset(void);


#ifdef VOICE_ENGINE_VERBOSE
#define PRINTF_VERBOSE(...)  PRINTF(__VA_ARGS__)
#else
#define PRINTF_VERBOSE(...)
#endif

#endif
