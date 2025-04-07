/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////

#ifndef __COEX_CLI_H__
#define __COEX_CLI_H__

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

int coex_cli_init(void);
int pollChar(void);
void coex_menuPrint(void);
void coex_menuAction(int ch);

#endif