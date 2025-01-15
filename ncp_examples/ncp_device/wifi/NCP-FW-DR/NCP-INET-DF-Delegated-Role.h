/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef _NCP_INET_DF_DR_H_
#define _NCP_INET_DF_DR_H_


#include <NCP-PROTOCOL-functions.h>


/**
 * \fn int NCP_socket(int family, int style, int protocol)
 *
 * \brief  This function creates a socket and specifies a communication style.
 *
 * \param family    specifies the namespace also called Protocol Family (PF) (PF_INET for IPv4 ; PF_INET6 for IPv6)
 * \param style     specifies communication style of the socket (SOCK_DGRAM for udp ; SOCK_STREAM for tcp)
 * \param protocol  designates the specific protocol (i.e. IPPROTO_UDP=17); zero is usually right for protocol.
 *
 * \return          the file descriptor for the new socket ; or (-1) in case of error ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_socket(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn int NCP_bind(int socket, const struct sockaddr *addr, socklen_t length)
 *
 * \brief  This function assigns an address to the socket.
 *
 * \param socket  file descriptor of the socket
 * \param addr    address assigned to the socket
 * \param length  length of the address (different in IPv4 and IPv6)
 * 
 * \return        (0) in case of success ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_bind(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn int NCP_listen(int socket, int n)
 *
 * \brief  This function enables the socket socket to accept connections, thus making it a server socket.
 *         It is not allowed for sockets using connectionless communication styles.
 *
 * \param socket  file descriptor of the socket
 * \param n       specifies the length of the queue for pending connections
 * 
 * \return         (0) in case of success ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_listen(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn int NCP_accept(int socket, struct sockaddr *addr, socklen_t *length_ptr)
 *
 * \brief  This function is used to accept a connection request on the server socket.
 *         This function waits if there are no connections pending, unless the socket has nonblocking mode set.
 *         The addr and length-ptr arguments are used to return information about the name of the client socket that initiated the connection.
 *         Accepting a connection does not make socket part of the connection. Instead, it creates a new socket which becomes connected. The normal return value of accept is the file descriptor for the new socket.
 *         After accept, the original socket remains open and unconnected, and continues listening until you close it. You can accept further connections with socket by calling accept again.
 *
 * \param socket      file descriptor of the socket
 * \param addr        used to return information about the name of the client socket that initiated the connection.
 * \param length_ptr  is a value-result argument, the caller must initialize it to contain the size (in bytes) of the structure pointed to by 'addr'; on return it will contain the actual size of the peer address.
 * 
 * \return the file descriptor for the new socket ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_accept(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn int NCP_connect(int socket, const struct sockaddr *addr, socklen_t length)
 *
 * \brief This function initiates a connection from the socket with file descriptor 'socket' to the socket whose address is specified by the 'addr' and 'length' arguments. 
 *        (This socket is typically on another machine, and it must be already set up as a server.)
 *
 * \param socket      file descriptor of the socket
 * \param addr        destination address of type (struct sockaddr_in *) __OR__ (struct sockaddr_in6 *)
 * \param length      length of the destination address struct (different in IPv4 and IPv6)
 *
 * \return  (0) in case of success ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_connect(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn int NCP_getsockname(int socket, struct sockaddr *addr, socklen_t *length_ptr)
 *
 * \brief Reading the Address of a Socket
 *        This function is used to examine the address of an Internet socket. It returns information about the address of the 'socket' in the locations specified by the 'addr' and 'length_ptr' arguments.
 *
 * \param socket      file descriptor of the socket
 * \param addr        used to return information about the name of the client socket that initiated the connection.
 * \param length_ptr  is a value-result argument, the caller must initialize it to contain the size (in bytes) of the structure pointed to by 'addr'; on return it contains the actual size of the socket address.
 *
 * \return  (0) in case of success ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_getsockname(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn int NCP_getpeername(int socket, struct sockaddr *addr, socklen_t *length_ptr)
 *
 * \brief Who is Connected to Me?
 *        This function returns the address of the socket that 'socket' is connected to.
 *        It stores the address in the memory space specified by 'addr' and 'length_ptr'.
 *
 * \param socket      file descriptor of the socket
 * \param addr        used to return information about the socket that initiated the connection.
 * \param length_ptr  is a value-result argument, the caller must initialize it to contain the size (in bytes) of the structure pointed to by 'addr'; on return it contains the actual size of the name returned (in bytes).
 *
 * \return  (0) in case of success ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_getpeername(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn int NCP_getsockopt(int socket, int level, int optname, void *optval, socklen_t *optlen_ptr)
 *
 * \brief GET Socket Options
 *        This function gets information about the value of option 'optname' at level 'level' for socket 'socket'. The option value is stored in the buffer that 'optval' points to. Before the call, you should supply in '*optlen_ptr' the size of this buffer; on return, it contains the number of bytes of information actually stored in the buffer. Most options interpret the 'optval' buffer as a single int value.
 *
 * \param socket      file descriptor of the socket
 * \param level       level at which the option resides (specified as SOL_SOCKET at the sockets  API  level)
 * \param optname     name of the socket option
 * \param optval      buffer in which the value for the requested option(s) are to be returned.
 * \param optlen_ptr  is a value-result argument, initially containing the size of the buffer pointed to by 'optval', and modified on return to indicate the actual size of the value returned.
 *
 * \return  (0) in case of success ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_getsockopt(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn int NCP_setsockopt(int socket, int level, int optname, const void *optval, socklen_t optlen)
 *
 * \brief SET Socket Options
 *        This function is used to set the socket option 'optname' at level 'level' for socket 'socket'. The value of the option is passed in the buffer 'optval' of size 'optlen'.
 *
 * \param socket      file descriptor of the socket
 * \param level       level at which the option resides (specified as SOL_SOCKET at the sockets  API  level)
 * \param optname     name of the socket option
 * \param optval      buffer containing the value of the option.
 * \param optlen      size of the option buffer.
 *
 * \return  (0) in case of success ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_setsockopt(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn ssize_t NCP_send(int socket, const void *buffer, size_t size, int flags)
 *
 * \brief This function returns the number of bytes transmitted, or -1 on failure. 
 *        If the socket is nonblocking, then send (like write) can return after sending 
 *            just part of the data.
 *        Note, however, that a successful return value merely indicates that the message 
 *            has been sent without error, not necessarily that it has been received without error. 
 *
 * \param socket  file descriptor of the socket
 * \param buffer  containing the data to be transmitted
 * \param size    number of bytes to be transmitted
 * \param flags   bit mask defining the socket data options
 *
 * \return number of bytes transmitted ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_send(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn ssize_t NCP_recv(int socket, void *buffer, size_t size, int flags)
 *
 * \brief This function returns the number of bytes received, or -1 on failure.
 *        If nonblocking mode is set for socket, and no data are available to be read, 
 *            recv fails immediately rather than waiting.
 *
 * \param socket  file descriptor of the socket
 * \param buffer  containing one packet read from the socket
 * \param size    maximum number of bytes to be read
 * \param flags   bit mask defining the socket data options
 * 
 * \return number of bytes received ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_recv(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn ssize_t NCP_sendto(int socket, const void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t length)
 *
 * \brief This function sends data on a datagram socket to the specified destination address.
 *
 * \param socket  file descriptor of the socket
 * \param buffer  containing the data to be transmitted
 * \param size    number of bytes to be transmitted
 * \param flags   bit mask defining the socket data options
 * \param addr    destination address of type (struct sockaddr_in *) __OR__ (struct sockaddr_in6 *)
 * \param length  length of the destination address struct (different in IPv4 and IPv6)
 * 
 * \return number of bytes transmitted ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_sendto(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn ssize_t NCP_recvfrom(int socket, void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t *length_ptr)
 *
 * \brief This function reads a packet from a datagram socket and also tells you where it was sent from
 *
 * \param socket      file descriptor of the socket
 * \param buffer      containing one packet read from the socket
 * \param size        maximum number of bytes to be read
 * \param flags       bit mask defining the socket data options
 * \param addr        address where the packet came from of type (struct sockaddr_in *) __OR__ (struct sockaddr_in6 *)
 * \param length_ptr  is a value-result argument. Before the call, it should be initialized to the size of the buffer associated with 'addr'. Upon return, 'length_ptr' is updated to contain the actual size of the source address.
 * 
 * \return number of bytes received ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_recvfrom(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn int NCP_shutdown(int socket, int how)
 *
 * \brief  This function shuts down only reception or transmission on a connection.
 *         
 *
 * \param socket   file descriptor of the socket
 * \param how      specifies what action to perform:
 *                   (0) Stop receiving data for this socket. If further data arrives, reject it.
 *                   (1) Stop trying to transmit data from this socket. Discard any data waiting to be sent. Stop looking for acknowledgement of data already sent; don’t retransmit it if it is lost.
 *                   (2) Stop both reception and transmission.
 *
 * \return    (0) in case of success ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_shutdown(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);

/**
 * \fn int NCP_close(int socket)
 *
 * \brief  When you have finished using a socket, you can simply close its file descriptor with this function. If there is still data waiting to be transmitted over the connection, normally close tries to complete this transmission.
 *
 * \param socket   file descriptor of the socket
 *
 * \return    (0) in case of success ; or (-1) in case of failure ('errno' defines the error conditions)
 */
extern void NCP_INET_DR_close(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump);



#endif // _NCP_INET_DF_DR_H_
