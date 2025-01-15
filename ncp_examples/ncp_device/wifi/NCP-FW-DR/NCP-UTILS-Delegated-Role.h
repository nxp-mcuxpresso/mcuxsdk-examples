/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef _NCP_UTILS_DR_H_
#define _NCP_UTILS_DR_H_


#include <NCP-PROTOCOL-functions.h>



/* -------------------------------------------------------------------------- */
/*                                 Types                                      */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                              Constants                                     */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                              Public Functions                              */
/* -------------------------------------------------------------------------- */

/*______________________________________________________________________
**
**             _   _  ____ ____     _   _ _   _ _
**            | \ | |/ ___|  _ \   | | | | |_(_) |___
**            |  \| | |   | |_) |  | | | | __| | / __|
**            | |\  | |___|  __/   | |_| | |_| | \__ \
**            |_| \_|\____|_|       \___/ \__|_|_|___/
**
**______________________________________________________________________
*/

/**
 * \fn int NCP_UTILS_DR_UNKNOWN_CMD
 *
 * \brief  Report to HOST processor Unknown NCP Command.
 *
 * \param 
 *
 * \return    (-1) failure (Unknown cmd_opcode) + ('errno'=EPERM="Operation not permitted")
 */
extern void NCP_UTILS_DR_UNKNOWN_CMD(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);



#endif // _NCP_UTILS_DR_H_
