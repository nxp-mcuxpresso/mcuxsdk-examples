/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

/*
** NCP Dumping functions
*/

#include <NCP-DUMP-functions.h>


// clang-format off
#ifndef NCP_LWIP_SOCKET
#  include <string.h>
#endif


/* This method returns true if a byte is a printable ascii character */
static char is_character_printable(char c) { return ((c < 0x7F) && (c >= 0x20)); }

/* This method returns the passed byte if it is printable otherwise return '.' */
static char validate_byte_as_printable(char byte) { return is_character_printable(byte) ? byte : '.'; }

#define DUMP_COLORING C_BoldWhite

void NCP_dump_buf(int start_pos, int end_pos, unsigned char *buf, int len)
{
    int i;
    int mem_addr = 0;
    char ascii_string[16 + 1] = "";
    char *pas = ascii_string;

    NCP_PRINTF("    "
               " -Offset- 00112233445566778899AABBCCDDEEFF  0123456789ABCDEF\r\n");
    NCP_PRINTF("    "
               " -------- --------------------------------  ----------------");

    for (i = 0; i < len; mem_addr++, i++)
    {
        *pas++ = validate_byte_as_printable(buf[i]);
        if (mem_addr % 16 == 0)
        {
            NCP_PRINTF(C_Reset); fflush(stdout);

            /* start a new raw */
            NCP_PRINTF("\r\n"
                       "    "
                       "  0x%04x ",
                       mem_addr);

            NCP_PRINTF(" "); fflush(stdout);

            if ((i >= start_pos) && (i <= end_pos))
            {
                NCP_PRINTF(DUMP_COLORING); fflush(stdout);
            }
            NCP_PRINTF("%02x",
                       buf[i]);
        }
        else if (mem_addr % 16 == 15)
        {
            /* print last hex dump value, and print also the ASCII string */
            NCP_PRINTF("%02x", buf[i]);

            NCP_PRINTF(C_Reset); fflush(stdout);

            NCP_PRINTF("  "
                       "%s",
                       ascii_string);
            memset(ascii_string, 0, 16 + 1);
            pas = ascii_string;
        }
        else
        {
            if ((i >= start_pos) && (i <= end_pos))
            {
                NCP_PRINTF(DUMP_COLORING); fflush(stdout);
            }
            else
            {
                NCP_PRINTF(C_Reset); fflush(stdout);
            }
            /* print hex dump value */
            NCP_PRINTF("%02x", buf[i]);
        }
    }

    if (mem_addr % 16)
    {
        int remain = 16 - (mem_addr % 16);

        NCP_PRINTF(C_Reset); fflush(stdout);

        while (remain--)
            NCP_PRINTF("  ");
        NCP_PRINTF("  "
                   "%s",
                   ascii_string);
    }

    NCP_PRINTF(C_Reset "\r\n"); fflush(stdout);
}
// clang-format on
