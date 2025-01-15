/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#include <errno.h>


#include <NCP-UTILS-Delegated-Role.h>

#include <NCP-DISPATCH-generic.h>


void NCP_UTILS_DR_UNKNOWN_CMD(int  cmd_opcode, char  *cmd_buf, int  *cmd_size, int  cmd_dump)
{
    NCP_TLV_COMMAND *tlv_header = (NCP_TLV_COMMAND *)cmd_buf;
    
    M_NCP_PREPARE_WLAN_TLV_HEADER(tlv_header, 0);
    
    *cmd_size = NCP_TLV_HDR_LENGTH;
    
    int *tlv_payload = (int *)(&cmd_buf[*cmd_size]);
    
    *tlv_payload++ = (int)(-1);
    *tlv_payload++ = (int)EPERM;
    
    *cmd_size += ((char *)tlv_payload - (char *)(&cmd_buf[*cmd_size]));
    tlv_header->size = *cmd_size;
    
    if (*cmd_size <= NCP_BUF_SIZE)
        ncp_tlv_send(cmd_buf, *cmd_size);
    else
        LOG_NCP_ERR("\t %s() -- Can't send NCP TX buffer, size too big! ncp_cmd_size=%d", __func__, *cmd_size);
}
