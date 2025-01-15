/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef _NCP_DISPATCH_GENERIC_H_
#define _NCP_DISPATCH_GENERIC_H_



/* -------------------------------------------------------------------------- */
/*                                 Types                                      */
/* -------------------------------------------------------------------------- */

typedef void processing_function(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

typedef struct dispatch_table_t { int cmd_opcode; processing_function *process_func; } dispatch_table;


/* -------------------------------------------------------------------------- */
/*                              Constants                                     */
/* -------------------------------------------------------------------------- */

#define NCP_BUF_SIZE    (2048 * 2)


#define _OPC(x)     NCP_CMD_OPCODE_##x

#define _INET(x)    NCP_INET_DR_##x



#endif // _NCP_DISPATCH_GENERIC_H_
