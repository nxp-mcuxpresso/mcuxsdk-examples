/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef _NCP_DISPATCH_FUNCTIONS_H_
#define _NCP_DISPATCH_FUNCTIONS_H_


#include <NCP-INET-DF-Delegated-Role.h>



/* -------------------------------------------------------------------------- */
/*                        NCP Command Opcodes                                 */
/* -------------------------------------------------------------------------- */

#include <NCP-List_of_functions_opcodes.h>

const int nbrOfAPIs     = (OPCODE__API__END    - OPCODE__API__START    - 1);


/*______________________________________________________________________
**
**               _   _  ____ ____        _    ____ ___
**              | \ | |/ ___|  _ \      / \  |  _ \_ _|
**              |  \| | |   | |_) |    / _ \ | |_) | |
**              | |\  | |___|  __/    / ___ \|  __/| |
**              |_| \_|\____|_|      /_/   \_\_|  |___|
**
**______________________________________________________________________
*/

#include <NCP-DISPATCH-generic.h>


/* -------------------------------------------------------------------------- */
/*                      NCP Functions Dispatch Table                          */
/* -------------------------------------------------------------------------- */

const dispatch_table    NCP_Dispatch_API[] = {
    {OPCODE__API__START, NULL},    //...WARNING: ...Do not use opcode=0 => reserved...

    //________________________________________________________________________________
    //
    // INET Functions:  Internet API based on POSIX and BSD Socket programming model...
    //________________________________________________________________________________
    //
    {_OPC(socket),      _INET(socket)},      // Opening Sockets
    {_OPC(bind),        _INET(bind)},        // TCP:Server: Setting the Address of a Socket
                                             // UDP:Server: Setting the Address of a Socket
    {_OPC(listen),      _INET(listen)},      // TCP:Server: Listening for Connections
    {_OPC(accept),      _INET(accept)},      // TCP:Server: Accepting Connections
    {_OPC(connect),     _INET(connect)},     // TCP:Client: Making a Connection
    {_OPC(getsockname), _INET(getsockname)}, // TCP:Client,Server: Reading the Address of a Socket
    {_OPC(getpeername), _INET(getpeername)}, // TCP:Server: Who is Connected to Me?
    {_OPC(getsockopt),  _INET(getsockopt)},  // GET Socket Options
    {_OPC(setsockopt),  _INET(setsockopt)},  // SET Socket Options
    {_OPC(send),        _INET(send)},        // TCP: Sending Streamed Data
    {_OPC(recv),        _INET(recv)},        // TCP: Receiving Streamed Data
    {_OPC(sendto),      _INET(sendto)},      // UDP: Datagram Socket 'send' Operations
    {_OPC(recvfrom),    _INET(recvfrom)},    // UDP: Datagram Socket 'receive' Operations
    {_OPC(shutdown),    _INET(shutdown)},    // Shutdown Sockets
    {_OPC(close),       _INET(close)},       // Closing Sockets

    //________________________________________________________________________________
    //

    {OPCODE__API__END,   NULL},    //...WARNING: nbrOfAPIs=(OPCODE__API__END-1)
};

const int nbrOfAPICommands = (sizeof(NCP_Dispatch_API) / sizeof(NCP_Dispatch_API[0]));


#endif // _NCP_DISPATCH_FUNCTIONS_H_
