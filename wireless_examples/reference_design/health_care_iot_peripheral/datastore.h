/*
 * Copyright 2022,2025 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DATASTORE_H_
#define _DATASTORE_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * Initializes the datastore
 *
 * @param flashLocation The location in flash where the data is stored persistently
 * @param flashSize The reserved size in bytes for the datastore in flash
 * @param entrySize The size of a single entry in bytes
 */
void DATASTORE_Init(const uint8_t *flashLocation, uint32_t flashSize, uint32_t entrySize);

/**
 * Adds an entry to the data store.
 *
 * @param entry Pointer to the entry.
 * @post The cache might be written to flash if the cache is full
 * @return kStatus_Success in case of success
 */
status_t DATASTORE_Add(const void *entry);

#endif /* _DATASTORE_H_ */
