/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifdef NCP_LWIP_SOCKET
#  include <lwip/sockets.h>
#else
#  include <stdlib.h>
#  include <unistd.h>
#  include <errno.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#endif



#include <NCP-INET-DF-Delegated-Role.h>

#include <NCP-List_of_functions_opcodes.h>

#include <NCP-DISPATCH-generic.h>



//___________________________________________________________________________
//
// Function: int NCP_socket(int family, int style, int protocol)  (see <sys/socket.h>)
//     Protocol Family:     PF_INET for IPv4 ; PF_INET6 for IPv6
//     Communication Style: SOCK_DGRAM for udp ; SOCK_STREAM for tcp
//     Specific Protocol:   zero is usually right for protocol. (i.e. IPPROTO_UDP=17 see <netinet/in.h>)
//
// Return: the file descriptor for the new socket ; or
//         (-1) in case of error ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_socket(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int family, style, protocol;
    int client_sockfd;
    
    family   = *cmd_param++;
    style    = *cmd_param++;
#ifdef NCP_LWIP_SOCKET
    style    &= ~02000000;
#endif
    protocol = *cmd_param++;
    
    client_sockfd = socket(family, style, protocol);
    if (client_sockfd < 0)
    {
        LOG_NCP_ERR("\t socket creation failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] socket Created: client_sockfd=%d", client_sockfd);
    }
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_socket);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)client_sockfd;
    if (client_sockfd < 0)
        *tlv_payload++ = (int)errno;
    
    *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    tlv_header->size = *cmd_size;
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t socket() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: int NCP_bind(int socket, const struct sockaddr *addr, socklen_t length)
//     socket: file descriptor of the socket
//     addr:   address assigned to the socket
//     length: length of the address struct (different in IPv4 and IPv6)
//
// Return:  (0) in case of success ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_bind(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int socket;
    socklen_t length;
    struct sockaddr *addr = NULL;
    int  ret_val;
    
    socket = *cmd_param++;
    length = *cmd_param++;
    
    char *var_param = (char *)cmd_param;
    int   i;
    
    addr = malloc(length);
    char *p_addr = (char *)addr;
    for (i = 0; i < length; i++)
    {
        *p_addr++ = *var_param++;
    }

    if ((ret_val = bind(socket, addr, length)) < 0)
    {
        LOG_NCP_ERR("\t Setting the Address of a Socket -- 'bind()' Operation failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] Setting the Address of a Socket -- 'bind()' Operation: ret_val=%d", ret_val);
    }
    
    if (addr)  free(addr);
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_bind);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)ret_val;
    if (ret_val < 0)
        *tlv_payload++ = (int)errno;
    
    *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    tlv_header->size = *cmd_size;
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t bind() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: int NCP_listen(int socket, int n)
//     socket: file descriptor of the socket
//     n:      specifies the length of the queue for pending connections
//
// Return:  (0) in case of success ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_listen(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int  socket;
    int  n;
    int  ret_val;
    
    socket = *cmd_param++;
    n      = *cmd_param++;
    
    if ((ret_val = listen(socket, n)) < 0)
    {
        LOG_NCP_ERR("\t TCP: listening for Connections -- 'listen()' failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] TCP: Listening for Connections -- 'listen()' operation: ret_val=%d", ret_val);
    }
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_listen);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)ret_val;
    if (ret_val < 0)
        *tlv_payload++ = (int)errno;
    
    *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    tlv_header->size = *cmd_size;
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t listen() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: int NCP_accept(int socket, struct sockaddr *addr, socklen_t *length_ptr)
//     socket:     file descriptor of the socket
//     addr:       used to return information about the name of the client socket that initiated the connection
//     length_ptr: is a value-result argument, the caller must initialize it to contain the size (in bytes) of the structure pointed to by 'addr'; on return it will contain the actual size of the peer address.
//
// Return: the file descriptor for the new socket ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_accept(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int socket;
    socklen_t length;
    struct sockaddr *addr = NULL;
    int new_socket;
    
    socket = *cmd_param++;
    length = *cmd_param++;
    
    addr = malloc(length);
    
    if ((new_socket = accept(socket, addr, &length)) < 0)
    {
        LOG_NCP_ERR("\t TCP: Accepting Connections -- 'accept()' failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] TCP: Accepting Connections -- 'accept()' Operation: length=%d ; new_socket=%d", length, new_socket);
    }
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_accept);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)new_socket;
    if (new_socket < 0)
    {
        *tlv_payload++ = (int)errno;
        
        *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    }
    else
    {
        *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
        
        char *tlv_var_payload = (char *)(&cmd_buf[*cmd_size]);
        char *p_addr;
        int   i;
        
        p_addr = (char *)&length;
        for (i = 0; i < sizeof(int); i++)
        {
            *tlv_var_payload++ = *p_addr++;
        }
        
        p_addr = (char *)addr;
        for (i = 0; i < length; i++)
        {
            *tlv_var_payload++ = *p_addr++;
        }
        
        *cmd_size += ((char *)tlv_var_payload - (char *)(&cmd_buf[*cmd_size]));
    }
    
    tlv_header->size = *cmd_size;
    
    if (addr)  free(addr);
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t accept() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: int NCP_connect(int socket, const struct sockaddr *addr, socklen_t length)
//     socket: file descriptor of the socket
//     addr:   destination address of type (struct sockaddr_in *) __OR__ (struct sockaddr_in6 *)
//     length: length of the destination address struct (different in IPv4 and IPv6)
//
// Return:  (0) in case of success ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_connect(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int socket;
    socklen_t length;
    struct sockaddr *addr = NULL;
    int  ret_val;
    
    socket = *cmd_param++;
    length = *cmd_param++;
    
    char *var_param = (char *)cmd_param;
    int   i;
    
    addr = malloc(length);
    char *p_addr = (char *)addr;
    for (i = 0; i < length; i++)
    {
        *p_addr++ = *var_param++;
    }
    
    if ((ret_val = connect(socket, addr, length)) < 0)
    {
        LOG_NCP_ERR("\t TCP: Making a Connection -- 'connect()' Operation failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] TCP: Making a Connection -- 'connect()' Operation: ret_val=%d", ret_val);
    }
    
    if (addr)  free(addr);
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_connect);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)ret_val;
    if (ret_val < 0)
        *tlv_payload++ = (int)errno;
    
    *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    tlv_header->size = *cmd_size;
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t connect() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: int NCP_getsockname(int socket, struct sockaddr *addr, socklen_t *length_ptr)
//     socket:     file descriptor of the socket
//     addr:       used to return information about the name of the client socket that initiated the connection.
//     length_ptr: is a value-result argument, the caller must initialize it to contain the size (in bytes) of the structure pointed to by 'addr'; on return it contains the actual size of the socket address.
//
// Return:  (0) in case of success ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_getsockname(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int socket;
    socklen_t length;
    struct sockaddr *addr = NULL;
    int  ret_val;
    
    socket = *cmd_param++;
    length = *cmd_param++;
    
    addr = malloc(length);
    
    if ((ret_val = getsockname(socket, addr, &length)) < 0)
    {
        LOG_NCP_ERR("\t TCP: Reading the Address of a Socket -- 'getsockname()' failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] TCP: Reading the Address of a Socket -- 'getsockname()' Operation: length=%d ; ret_val=%d", length, ret_val);
    }
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_getsockname);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)ret_val;
    if (ret_val < 0)
    {
        *tlv_payload++ = (int)errno;
        
        *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    }
    else
    {
        *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
        
        char *tlv_var_payload = (char *)(&cmd_buf[*cmd_size]);
        char *p_addr;
        int   i;
        
        p_addr = (char *)&length;
        for (i = 0; i < sizeof(int); i++)
        {
            *tlv_var_payload++ = *p_addr++;
        }
        
        p_addr = (char *)addr;
        for (i = 0; i < length; i++)
        {
            *tlv_var_payload++ = *p_addr++;
        }
        
        *cmd_size += ((char *)tlv_var_payload - (char *)(&cmd_buf[*cmd_size]));
    }
    
    tlv_header->size = *cmd_size;
    
    if (addr)  free(addr);
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t getsockname() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: int NCP_getpeername(int socket, struct sockaddr *addr, socklen_t *length_ptr)
//     socket:     file descriptor of the socket
//     addr:       used to return information about the socket that initiated the connection.
//     length_ptr: is a value-result argument, the caller must initialize it to contain the size (in bytes) of the structure pointed to by 'addr'; on return it contains the actual size of the name returned (in bytes).
//
// Return:  (0) in case of success ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_getpeername(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int socket;
    socklen_t length;
    struct sockaddr *addr = NULL;
    int  ret_val;
    
    socket = *cmd_param++;
    length = *cmd_param++;
    
    addr = malloc(length);
    
    if ((ret_val = getpeername(socket, addr, &length)) < 0)
    {
        LOG_NCP_ERR("\t TCP: Who is Connected to Me? -- 'getpeername()' failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] TCP: Who is Connected to Me? -- 'getpeername()' Operation: length=%d ; ret_val=%d", length, ret_val);
    }
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_getpeername);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)ret_val;
    if (ret_val < 0)
    {
        *tlv_payload++ = (int)errno;
        
        *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    }
    else
    {
        *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
        
        char *tlv_var_payload = (char *)(&cmd_buf[*cmd_size]);
        char *p_addr;
        int   i;
        
        p_addr = (char *)&length;
        for (i = 0; i < sizeof(int); i++)
        {
            *tlv_var_payload++ = *p_addr++;
        }
        
        p_addr = (char *)addr;
        for (i = 0; i < length; i++)
        {
            *tlv_var_payload++ = *p_addr++;
        }
        
        *cmd_size += ((char *)tlv_var_payload - (char *)(&cmd_buf[*cmd_size]));
    }
    
    tlv_header->size = *cmd_size;
    
    if (addr)  free(addr);
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t getpeername() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: int NCP_getsockopt(int socket, int level, int optname, void *optval, socklen_t *optlen_ptr)
//     socket:     file descriptor of the socket
//     level:      level at which the option resides
//     optname:    name of the socket option
//     optval:     buffer in which the value for the requested option(s) are to be returned.
//     optlen_ptr: is a value-result argument, initially containing the size of the buffer pointed to by 'optval', and modified on return to indicate the actual size of the value returned.
//
// Return:  (0) in case of success ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_getsockopt(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int socket;
    int level;
    int optname;
    socklen_t optlen;
    void *optval = NULL;
    int ret_val;
    
    socket  = *cmd_param++;
    level   = *cmd_param++;
    optname = *cmd_param++;
    optlen  = *cmd_param++;

    optval = malloc(optlen);
    
    if ((ret_val = getsockopt(socket, level, optname, optval, &optlen)) < 0)
    {
        LOG_NCP_ERR("\t TCP: GET Socket Options -- 'getsockopt()' failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] TCP: GET Socket Options -- 'getsockopt()' Operation: optlen=%d ; ret_val=%d", optlen, ret_val);
    }
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_getsockopt);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)ret_val;
    if (ret_val < 0)
    {
        *tlv_payload++ = (int)errno;
        
        *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    }
    else
    {
        *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
        
        char *tlv_var_payload = (char *)(&cmd_buf[*cmd_size]);
        char *p_val;
        int   i;
        
        p_val = (char *)&optlen;
        for (i = 0; i < sizeof(int); i++)
        {
            *tlv_var_payload++ = *p_val++;
        }
        
        p_val = (char *)optval;
        for (i = 0; i < optlen; i++)
        {
            *tlv_var_payload++ = *p_val++;
        }
        
        *cmd_size += ((char *)tlv_var_payload - (char *)(&cmd_buf[*cmd_size]));
    }
    
    tlv_header->size = *cmd_size;
    
    if (optval)  free(optval);
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t getsockopt() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: int NCP_setsockopt(int socket, int level, int optname, const void *optval, socklen_t optlen)
//     socket:     file descriptor of the socket
//     level:      level at which the option resides
//     optname:    name of the socket option
//     optval:     buffer containing the value of the option.
//     optlen:     size of the option buffer.
//
// Return:  (0) in case of success ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_setsockopt(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int socket;
    int level;
    int optname;
    socklen_t optlen;
    void *optval = NULL;
    int  ret_val;
    
    socket  = *cmd_param++;
    level   = *cmd_param++;
    optname = *cmd_param++;
    optlen  = *cmd_param++;

    char *var_param = (char *)cmd_param;
    int   i;
    
    optval = malloc(optlen);
    char *p_val = (char *)optval;
    for (i = 0; i < optlen; i++)
    {
        *p_val++ = *var_param++;
    }
    
    if ((ret_val = setsockopt(socket, level, optname, optval, optlen)) < 0)
    {
        LOG_NCP_ERR("\t SET Socket Options -- 'setsockopt()' Operation failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] SET Socket Options -- 'setsockopt()' Operation: ret_val=%d", ret_val);
    }
    
    if (optval)  free(optval);
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_setsockopt);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)ret_val;
    if (ret_val < 0)
        *tlv_payload++ = (int)errno;
    
    *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    tlv_header->size = *cmd_size;
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t setsockopt() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: ssize_t NCP_send(int socket, const void *buffer, size_t size, int flags)
//     socket: file descriptor of the socket
//     buffer: containing the data to be transmitted
//     size:   number of bytes to be transmitted
//     flags:  bit mask defining the socket data options
//
// Return: number of bytes transmitted ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_send(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int socket;
    size_t size;
    int flags;
    char *buffer = NULL;
    ssize_t nBytesTX;
    
    socket = *cmd_param++;
    size = *cmd_param++;
    flags = *cmd_param++;

    char *var_param = (char *)cmd_param;
    int   i;
    
    buffer = malloc(size);
    for (i = 0; i < size; i++)
    {
        buffer[i] = *var_param++;
    }
    
    if ((nBytesTX = send(socket, buffer, size, flags)) < 0)
    {
        LOG_NCP_ERR("\t TCP: Sending Streamed Data -- 'send()' failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] TCP: Sending Streamed Data -- 'send()': nBytesTX=%ld", nBytesTX);
    }
    
    if (buffer)  free(buffer);
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_send);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)nBytesTX;
    if (nBytesTX < 0)
        *tlv_payload++ = (int)errno;
    
    *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    tlv_header->size = *cmd_size;
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t send() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: ssize_t NCP_recv(int socket, void *buffer, size_t size, int flags)
//     socket: file descriptor of the socket
//     buffer: containing one packet read from the socket
//     size:   maximum number of bytes to be read
//     flags:  bit mask defining the socket data options
//
// Return: number of bytes received ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_recv(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int socket;
    size_t size;
    int flags;
    char *buffer = NULL;
    ssize_t nBytesRX;
    
    socket = *cmd_param++;
    size   = *cmd_param++;
    flags  = *cmd_param++;
    
    buffer = malloc(size);
    
    if ((nBytesRX = recv(socket, buffer, size, flags)) < 0)
    {
        LOG_NCP_ERR("\t TCP: Receiving Streamed Data -- 'recv()' Operation failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] TCP: Receiving Streamed Data -- 'recv()' Operation: nBytesRX=%ld", nBytesRX);
    }
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_recv);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)nBytesRX;
    if (nBytesRX < 0)
    {
        *tlv_payload++ = (int)errno;
        
        *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    }
    else
    {
        *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
        
        char *tlv_var_payload = (char *)(&cmd_buf[*cmd_size]);
        int   i;
        
        char *p_buf = (char *)buffer;
        for (i = 0; i < nBytesRX; i++)
        {
            *tlv_var_payload++ = *p_buf++;
        }
        
        *cmd_size += ((char *)tlv_var_payload - (char *)(&cmd_buf[*cmd_size]));
    }
    
    tlv_header->size = *cmd_size;
    
    if (buffer)  free(buffer);
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t recv() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: ssize_t NCP_sendto(int socket, const void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t length)
//     socket: file descriptor of the socket
//     buffer: containing the data to be transmitted
//     size:   number of bytes to be transmitted
//     flags:  bit mask defining the socket data options
//     addr:   destination address of type (struct sockaddr_in *) __OR__ (struct sockaddr_in6 *)
//     length: length of the destination address struct (different in IPv4 and IPv6)
//
// Return: number of bytes transmitted ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_sendto(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int socket;
    size_t size;
    int flags;
    socklen_t length;
    struct sockaddr *addr = NULL;
    char *buffer = NULL;
    ssize_t nBytesTX;
    
    socket = *cmd_param++;
    size   = *cmd_param++;
    flags  = *cmd_param++;
    length = *cmd_param++;
    
    char *var_param = (char *)cmd_param;
    int   i;
    
    addr = malloc(length);
    char *p_addr = (char *)addr;
    for (i = 0; i < length; i++)
    {
        *p_addr++ = *var_param++;
    }
    
    buffer = malloc(size);
    for (i = 0; i < size; i++)
    {
        buffer[i] = *var_param++;
    }
    
    if ((nBytesTX = sendto(socket, buffer, size, flags, addr, length)) < 0)
    {
        LOG_NCP_ERR("\t UDP: Sending Datagram -- 'sendto()' Operation failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] UDP: Sending Datagram -- 'sendto()' Operation: nBytesTX=%ld", nBytesTX);
    }
    
    if (addr)    free(addr);
    if (buffer)  free(buffer);
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_sendto);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)nBytesTX;
    if (nBytesTX < 0)
        *tlv_payload++ = (int)errno;
    
    *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    tlv_header->size = *cmd_size;
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t sendto() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: ssize_t NCP_recvfrom(int socket, void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t *length_ptr)
//     socket:     file descriptor of the socket
//     buffer:     containing one packet read from the socket
//     size:       maximum number of bytes to be read
//     flags:      bit mask defining the socket data options
//     addr:       address where the packet came from of type (struct sockaddr_in *) __OR__ (struct sockaddr_in6 *)
//     length_ptr: is a value-result argument. Before the call, it should be initialized to the size of the buffer associated with 'addr'. Upon return, 'length_ptr' is updated to contain the actual size of the source address.
//
// Return: number of bytes received ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_recvfrom(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int socket;
    size_t size;
    int flags;
    socklen_t length;
    struct sockaddr *addr = NULL;
    char *buffer = NULL;
    ssize_t nBytesRX;
    
    socket = *cmd_param++;
    size   = *cmd_param++;
    flags  = *cmd_param++;
    length = *cmd_param++;
    
    addr = malloc(length);
    buffer = malloc(size);
    
    if ((nBytesRX = recvfrom(socket, buffer, size, flags, addr, &length)) < 0)
    {
        LOG_NCP_ERR("\t UDP: Receiving Datagram -- 'recvfrom()' Operation failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] UDP: Receiving Datagram -- 'recvfrom()' Operation: length=%d ; nBytesRX=%ld", length, nBytesRX);
    }
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_recvfrom);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)nBytesRX;
    if (nBytesRX < 0)
    {
        *tlv_payload++ = (int)errno;
        
        *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    }
    else
    {
        *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
        
        char *tlv_var_payload = (char *)(&cmd_buf[*cmd_size]);
        char *p_addr;
        int   i;
        
        p_addr = (char *)&length;
        for (i = 0; i < sizeof(int); i++)
        {
            *tlv_var_payload++ = *p_addr++;
        }
        
        p_addr = (char *)addr;
        for (i = 0; i < length; i++)
        {
            *tlv_var_payload++ = *p_addr++;
        }
        
        p_addr = (char *)buffer;
        for (i = 0; i < nBytesRX; i++)
        {
            *tlv_var_payload++ = *p_addr++;
        }
        
        *cmd_size += ((char *)tlv_var_payload - (char *)(&cmd_buf[*cmd_size]));
    }
    
    tlv_header->size = *cmd_size;
    
    if (addr)    free(addr);
    if (buffer)  free(buffer);
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t recvfrom() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: int NCP_shutdown(int socket, int how)
//     socket:  file descriptor for the socket
//     how:     specifies what action to perform
//
// Return:  (0) in case of success ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_shutdown(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int  socket;
    int  how;
    int  ret_val;
    
    socket = *cmd_param++;
    how    = *cmd_param++;
    
    if ((ret_val = shutdown(socket, how)) < 0)
    {
        LOG_NCP_ERR("\t shutdown socket failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] shutdown socket: ret_val=%d", ret_val);
    }
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_shutdown);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)ret_val;
    if (ret_val < 0)
        *tlv_payload++ = (int)errno;
    
    *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    tlv_header->size = *cmd_size;
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t shutdown() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}

//___________________________________________________________________________
//
// Function: int NCP_close(int socket)
//     socket:  file descriptor for the socket
//
// Return:  (0) in case of success ; or
//         (-1) in case of failure ('errno' defines the error conditions)
//___________________________________________________________________________
//
void NCP_INET_DR_close(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    int *cmd_param = (int *)(&cmd_buf[sizeof(cmd_opcode)]);
    int  socket;
    int  ret_val;
    
    socket = *cmd_param++;
    
    if ((ret_val = close(socket)) < 0)
    {
        LOG_NCP_ERR("\t socket closing failed! errno=%d", errno);
    }
    else
    {
        LOG_NCP_INFO("\t [OK] socket Closed: ret_val=%d", ret_val);
    }
    
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, NCP_CMD_OPCODE_close);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)ret_val;
    if (ret_val < 0)
        *tlv_payload++ = (int)errno;
    
    *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    tlv_header->size = *cmd_size;
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t close() -- Can't send NCP TX buffer, size too big! *cmd_size=%d", *cmd_size);
}
