/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef EMBEDDING_DATABASE_H
#define EMBEDDING_DATABASE_H

#include "models/mobilefacenet/mobilefacenet_output_postproc_quantized.h"

#define DATABASE_MAX_PEOPLE 10
#define DATABASE_NUM_PEOPLE 0

static person Embedding_database[DATABASE_MAX_PEOPLE] = {0};

#endif /* EMBEDDING_DATABASE_H */
