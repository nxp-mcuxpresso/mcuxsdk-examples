/*
 * Copyright 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef EMBEDDING_DB_H
#define EMBEDDING_DB_H

#include <mobilefacenet_database_infos.h>

#define DATABASE_MAX_SIZE 10

static face_t g_embedding_db[DATABASE_MAX_SIZE] = {0};

#endif /* EMBEDDING_DB_H */
