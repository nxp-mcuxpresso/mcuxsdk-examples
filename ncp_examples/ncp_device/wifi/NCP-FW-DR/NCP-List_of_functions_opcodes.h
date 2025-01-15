/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef _NCP_OPCODES_OF_FUNCTIONS_H_
#define _NCP_OPCODES_OF_FUNCTIONS_H_



/* -------------------------------------------------------------------------- */
/*                        NCP Command Opcodes                                 */
/* -------------------------------------------------------------------------- */

// clang-format off

typedef enum _NCP_CMD_OPCODE_t {
    OPCODE__CMD__START = 0,   //...WARNING: ...Do not use opcode=0 => Reserved...

    /*.........................{ API }.........................*/

    OPCODE__API__START = OPCODE__CMD__START,

    // INET Functions:  Internet API based on POSIX and BSD Socket programming model...
    NCP_CMD_OPCODE_socket,           // Opening Sockets
    NCP_CMD_OPCODE_bind,             // TCP:Server: Setting the Address of a Socket
                                     // UDP:Server: Setting the Address of a Socket
    NCP_CMD_OPCODE_listen,           // TCP:Server: Listening for Connections
    NCP_CMD_OPCODE_accept,           // TCP:Server: Accepting Connections
    NCP_CMD_OPCODE_connect,          // TCP:Client: Making a Connection
    NCP_CMD_OPCODE_getsockname,      // TCP:Client,Server: Reading the Address of a Socket
    NCP_CMD_OPCODE_getpeername,      // TCP:Server: Who is Connected to Me?
    NCP_CMD_OPCODE_getsockopt,       // GET Socket Options
    NCP_CMD_OPCODE_setsockopt,       // SET Socket Options
    NCP_CMD_OPCODE_send,             // TCP: Sending Streamed Data
    NCP_CMD_OPCODE_recv,             // TCP: Receiving Streamed Data
    NCP_CMD_OPCODE_sendto,           // UDP: Datagram Socket 'send' Operations
    NCP_CMD_OPCODE_recvfrom,         // UDP: Datagram Socket 'receive' Operations
    NCP_CMD_OPCODE_shutdown,         // Shutdown Sockets
    NCP_CMD_OPCODE_close,            // Closing Sockets

    OPCODE__API__END,         //...WARNING: nbrOfAPIs=(OPCODE__API__END - OPCODE__API__START - 1)

    /*.........................................................*/

    OPCODE__CMD__END,         //...WARNING: nbrOfCommands=(OPCODE__CMD__END-1)
} NCP_CMD_OPCODE_t;

// clang-format on


#endif // _NCP_OPCODES_OF_FUNCTIONS_H_
