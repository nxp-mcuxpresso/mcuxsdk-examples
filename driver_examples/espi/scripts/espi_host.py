#!/usr/bin/env python3
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

#==========================================================================
# (c) 2014-2019 Total Phase, Inc.
#--------------------------------------------------------------------------
# Project : Promira Sample Code
# File    : espi_generator.py
#--------------------------------------------------------------------------
# Generate eSPI transactions using the SPI active application.
#--------------------------------------------------------------------------
# Redistribution and use of this file in source and binary forms, with
# or without modification, are permitted.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#==========================================================================

#==========================================================================
# IMPORTS
#==========================================================================
import sys
import os
import time

_script_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, _script_dir)
if hasattr(os, 'add_dll_directory'):
    os.add_dll_directory(_script_dir)

from promira_py import *
from promact_is_py import *
from espi_simulator import *


class NxpEspiSimulator(EspiSimulator):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        ps_phy_level_shift(self.channel, 3.3)

    def espi_transact(self, pkts, debug=False):
        io_mode = self.io[self.slave_id]
        if io_mode != 4:
            super().espi_transact(pkts, debug)
            return
        word_size = 4
        ps_queue_clear(self.queue)
        for pkt in pkts:
            cmd = pkt[0]
            resp = pkt[1]
            if debug:
                n = 64
                print("[>]", ['%02x' % x for x in (cmd + resp)[:n]],
                      end=' ')
                print("..." if len(cmd + resp) > n else "")
            raw = self._espi_pack_data(*pkt, single_on_dual=False)
            raw_len = len(raw) * 2
            ps_queue_spi_ss(self.queue, self.ss_mask)
            if self.sim_mode == ESPI_SIMULATOR_MODE_TRANS:
                ps_queue_spi_write(self.queue, io_mode, word_size,
                                   raw_len, raw)
            else:
                cmd_words = (len(cmd) + 1) * 2
                ps_queue_spi_write(self.queue, io_mode, word_size,
                                   cmd_words, raw)
                ps_queue_spi_read(self.queue, io_mode, word_size,
                                  raw_len - cmd_words)
            ps_queue_spi_ss(self.queue, 0)
        self._spi_submit()


# Flash channel config (SET_CONFIGURATION offset 0x40):
#   Byte 0 = 0x05: ch_enable=1, erase_block_size=1 (4K)
#   Byte 1 = 0x2A: max_payload + max_read_request encoding
FLASH_CH_CONFIG = [0x05, 0x2A, 0x00, 0x00]

#==========================================================================
# eSPI FUNCTIONS
#==========================================================================
def espi_reset(simulator):
    """ Send a RESET """
    simulator.espi_toggle_reset()

def espi_alert(simulator):
    """ Send an ALERT """
    simulator.espi_toggle_alert()

def espi_inband_reset(simulator):
    """ Send an INBAND RESET """
    simulator.espi_inband_reset()

def espi_setup_channels(simulator):
    """Enable all channels in current IO mode (call before switching to quad)."""
    simulator.espi_set_config_10h(7, 3, 1, 1)        # Peripheral
    simulator.espi_set_config_20h(7, 1)              # VWire (0-based: 7 = 8 groups, NXP HW max)
    simulator.espi_set_config_30h(2, 1)              # OOB
    simulator.espi_set_config(0x40, FLASH_CH_CONFIG) # Flash

def espi_raw_get_config(simulator, offset):
    """GET_CONFIGURATION at given offset (hex). E.g. raw_get_config 0x10"""
    data = [0, 0, 0, 0]
    simulator.espi_get_config(int(offset, 0), data)
    time.sleep(0.05)

def espi_set_bus_config(simulator, io_mode_name, freq_mhz):
    """SET_CONFIGURATION(0x08) with both frequency and IO mode.
    io_mode_name: 'single'/'dual'/'quad'
    freq_mhz: '20'/'25'/'33'/'50'/'66'
    """
    io_map = {'single': 0, 'dual': 1, 'quad': 2}
    freq_map = {'20': 0, '25': 1, '33': 2, '50': 3, '66': 4}
    simulator.espi_set_config_08h(freq_map[freq_mhz], io_map[io_mode_name])

def espi_set_config_10h(simulator):
    """ Send a SET_CONFIGURATION command to configure the peripheral channel \
operating mode. Maximum Read Request Size: 4096 bytes, Maximum Payload Size: 256 bytes """
    # max_req_size : 4096, max_payload_size : 256, bus_enable, ch_enable
    simulator.espi_set_config_10h(7, 3, 1, 1)

def espi_put_msg(simulator):
    """ Send multiple down stream messages on Slave 0 and Slave 1 """
    msg_data = [ 1, 2, 3, 4, 5 ]

    # put msg on slave 0

    for _ in range(1):
        tag = random.randrange(0, 15)
        data = random.sample(range(0, 0xff), random.randrange(0, 32))

        simulator.espi_perif_put_msg(
            True, tag, msg_data, data,
            status=STAT_PC_FREE|STAT_PC_AVAIL)

    # put msg on slave 1
    msg_data = [ 5, 4, 3, 2, 1 ]

    for _ in range(2):
        tag = random.randrange(0, 15)
        data = random.sample(range(0, 0xff), random.randrange(0, 32))

        simulator.espi_perif_put_msg(
            True, tag, msg_data, data,
            status=STAT_PC_FREE|STAT_PC_AVAIL)

def espi_get_status(simulator):
    """ Send a GET_STATUS command """
    resp_data  = []

    # Send a get_status command indicating that the
    # slave has a posted/completion rx queue free
    simulator.espi_get_status(resp_data, RESP_ACCEPTED, STAT_NORMAL)

def espi_get_status_pc_free(simulator):
    """ Send a GET_STATUS command indicating a free posted/completion RX queue"""
    resp_data  = []

    # Send a get_status command indicating that the
    # slave has a posted/completion rx queue free
    simulator.espi_get_status(resp_data, RESP_ACCEPTED, STAT_PC_FREE)

def espi_get_status_np_free(simulator):
    """ Send a GET_STATUS command indicating a free non-posted RX queue"""
    resp_data  = []

    # Send a get_status command indicating that the
    # slave has a non-posted rx queue free
    simulator.espi_get_status(resp_data, RESP_ACCEPTED, STAT_NP_FREE)

def espi_perif_downstream_wr32(simulator):
    """ Send a downstream memory write to a 32-bit address. Address: 0xFF008000, Tag: 0xA, Data: 8 bytes """
    write_data = [10, 11, 12, 13, 14, 15, 16 , 17]

    # Send downstream posted write command with 8 bytes of data
    # to address 0xFF008000, Tag value: 0xA
    # Command   : PUT_PC
    # Cycle type: MEMORY WRITE 32
    # Tag value : 0xA
    # Address   : 0xFF008000
    simulator.espi_perif_write_mem(True, 10, 0xFF008000, True, write_data,
                                   RESP_ACCEPTED, STAT_NORMAL)

def espi_perif_downstream_rd32(simulator):
    """ Send a downstream memory read request to a 32-bit address with a connected completion (successful with data, only). Address: 0x80800000, Tag: 0xA, Data: 16 bytes """
    cmpl_data = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]

    # Send a downstream non posted read request with an ACCEPT
    # response to a 32 bit address 0x80800000, Tag value: 0xA
    # Command   : PUT_NP
    # Cycle type: MEMORY READ 32
    # Tag value : 0xA
    # Address   : 0x80800000
    simulator.espi_perif_read_mem(True, 10, 0x80800000, True,
                                  16, CYCLE_SC_DATA_11, cmpl_data,
                                  RESP_ACCEPTED, STAT_NORMAL)

def espi_perif_downstream_rd64(simulator):
    """ Send a downstream memory read request to a 64-bit address with a connected completion (successful with data, only). Address: 0xFFFFC00080800000, Tag: 0xA, Data: 16 bytes """
    cmpl_data = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]

    # Send a downstream non posted read request with an ACCEPT response
    # to a 64 bit address 0xFFFFC00080800000, Tag value: 0xA
    # Command   : PUT_NP
    # Cycle type: MEMORY READ 64
    # Tag value : 0xA
    # Address   : 0xFFFFC00080800000
    simulator.espi_perif_read_mem(True, 10, 0xFFFFC00080800000, False,
                                  16, CYCLE_SC_DATA_11, cmpl_data,
                                  RESP_ACCEPTED, STAT_NORMAL)

"""
eSPI Slave Status Register Definition (16-bit)
================================================================================
Bit  | Field Name      | R/W | Description
-----|-----------------|-----|----------------------------------------------
[0]  | PC_FREE         | R   | Peripheral Posted/Completion Rx Queue Free
     |                 |     | When '1', indicates the slave is free to accept
     |                 |     | at least one channel 0 peripheral posted or
     |                 |     | completion header and data up to maximum
     |                 |     | payload size.
-----|-----------------|-----|----------------------------------------------
[1]  | NP_FREE         | R   | Peripheral Non-Posted Rx Queue Free
     |                 |     | When '1', indicates the slave is free to accept
     |                 |     | at least one channel 0 peripheral non-posted
     |                 |     | header and 1 DW of Data (if applicable).
-----|-----------------|-----|----------------------------------------------
[2]  | VWIRE_FREE      | R   | Virtual Wire Rx Queue Free (Always '1')
     |                 |     | This bit must be always a '1'. Tunneling of
     |                 |     | channel 1 virtual wires is not flow controlled.
-----|-----------------|-----|----------------------------------------------
[3]  | OOB_FREE        | R   | OOB Posted Rx Queue Free
     |                 |     | When '1', indicates the slave is free to accept
     |                 |     | at least one channel 2 OOB (tunneled SMBus)
     |                 |     | message with data up to maximum payload size.
-----|-----------------|-----|----------------------------------------------
[4]  | PC_AVAIL        | R   | Peripheral Posted/Completion Tx Queue Avail
     |                 |     | When '1', indicates the slave has a channel 0
     |                 |     | peripheral posted or completion header and
     |                 |     | optional data up to maximum payload size
     |                 |     | available to send.
-----|-----------------|-----|----------------------------------------------
[5]  | NP_AVAIL        | R   | Peripheral Non-Posted Tx Queue Avail
     |                 |     | When '1', indicates the slave has a channel 0
     |                 |     | peripheral non-posted header available to send.
-----|-----------------|-----|----------------------------------------------
[6]  | VWIRE_AVAIL     | R   | Virtual Wire Tx Queue Avail
     |                 |     | When '1', indicates the slave has a channel 1
     |                 |     | tunneled virtual wire available to send.
-----|-----------------|-----|----------------------------------------------
[7]  | OOB_AVAIL       | R   | OOB Posted Tx Queue Avail
     |                 |     | When '1', indicates the slave has a channel 2
     |                 |     | OOB (tunneled SMBus) message with data up to
     |                 |     | maximum payload size available to send.
-----|-----------------|-----|----------------------------------------------
[8]  | FLASH_C_FREE    | R   | Flash Completion Rx Queue Free (Always '1')
     |                 |     | When '1', indicates the slave is free to accept
     |                 |     | at least one channel 3 Flash Access completion
     |                 |     | header and data up to maximum payload size.
     |                 |     | This bit must be always a '1'. The slave must
     |                 |     | be able to accept the completion for the non-
     |                 |     | posted request it sends.
     |                 |     | Only applicable when master attached flash
     |                 |     | sharing is supported and in operation.
-----|-----------------|-----|----------------------------------------------
[9]  | FLASH_NP_FREE   | R   | Flash Non-Posted Rx Queue Free
     |                 |     | When '1', indicates the slave is free to accept
     |                 |     | at least one channel 3 Flash Access non-posted
     |                 |     | header and data up to maximum payload size.
     |                 |     | Only applicable when slave attached flash
     |                 |     | sharing is supported and in operation.
-----|-----------------|-----|----------------------------------------------
[11:10] Reserved       | R   | Reserved
-----|-----------------|-----|----------------------------------------------
[12] | FLASH_C_AVAIL   | R   | Flash Completion Tx Queue Avail
     |                 |     | When '1', indicates the slave has a channel 3
     |                 |     | Flash Access completion header and data up to
     |                 |     | maximum payload size available to send.
     |                 |     | Only applicable when slave attached flash
     |                 |     | sharing is supported and in operation.
-----|-----------------|-----|----------------------------------------------
[13] | FLASH_NP_AVAIL  | R   | Flash Non-Posted Tx Queue Avail
     |                 |     | When '1', indicates the slave has a channel 3
     |                 |     | Flash Access non-posted header and data up to
     |                 |     | maximum payload size available to send.
     |                 |     | Only applicable when master attached flash
     |                 |     | sharing is supported and in operation.
-----|-----------------|-----|----------------------------------------------
[15:14] Reserved       | R   | Reserved
================================================================================

Channel Mapping:
  - Channel 0: Peripheral (Memory/IO)
  - Channel 1: Virtual Wire
  - Channel 2: OOB (Out-of-Band / SMBus)
  - Channel 3: Flash Access
"""

def espi_mcux_sts_get(simulator):
    simulator.espi_get_status(
        resp_data=[],
        resp_code=RESP_ACCEPTED,
        status=STAT_OOB_AVAIL | STAT_OOB_FREE | STAT_NORMAL
    )

def espi_mcux_vw_put(simulator):
    """Send all 8 VWire groups in a single PUT_VW (used by S01 serial verification)."""
    vw_cmds = [
        0x02, 0x77,  # SLP_S3#, SLP_S4#, SLP_S5#
        0x03, 0x77,  # SUS_STAT, PLTRST#, OOB_RST_WARN
        0x07, 0x11,  # HOST_RST_WARN
        0x41, 0x77,  # SUS_WARN, SUS_PWRDN_ACKN, SLP_AN
        0x42, 0x33,  # SLP_LAN, SLP_WLAN
        0x43, 0x55,  # P2E lower nibble
        0x44, 0x55,  # P2E upper nibble
        0x47, 0x11,  # HOST_C10N
    ]
    vw_count = (len(vw_cmds) // 2) - 1
    simulator.espi_vw_put(vw_count, vw_cmds, resp_code=RESP_ACCEPTED, status=STAT_NORMAL)

def espi_mcux_vw_put_p1(simulator):
    """PUT_VW batch 1/3: groups 0x02, 0x03, 0x07 (3 groups)."""
    vw = [0x02, 0x77, 0x03, 0x77, 0x07, 0x11]
    simulator.espi_vw_put(len(vw) // 2 - 1, vw, resp_code=RESP_ACCEPTED, status=STAT_NORMAL)

def espi_mcux_vw_put_p2(simulator):
    """PUT_VW batch 2/3: groups 0x41, 0x42, 0x43 (3 groups)."""
    vw = [0x41, 0x77, 0x42, 0x33, 0x43, 0x55]
    simulator.espi_vw_put(len(vw) // 2 - 1, vw, resp_code=RESP_ACCEPTED, status=STAT_NORMAL)

def espi_mcux_vw_put_p3(simulator):
    """PUT_VW batch 3/3: groups 0x44, 0x47 (2 groups)."""
    vw = [0x44, 0x55, 0x47, 0x11]
    simulator.espi_vw_put(len(vw) // 2 - 1, vw, resp_code=RESP_ACCEPTED, status=STAT_NORMAL)

def espi_raw_vw_config(simulator):
    simulator.espi_set_config_20h(6, 1)

def espi_raw_vw_get(simulator):
    vw_data = [0,0] * 4
    simulator.espi_vw_get(2, vw_data)

def espi_mcux_vw_get(simulator):
    espi_raw_vw_config(simulator)
    espi_raw_vw_get(simulator)

def espi_raw_get_config_20h(simulator):
    # GET_CONFIGURATION for VWire channel (offset 0x20)
    # CMD: 21 00 20 crc
    # RSP: rsp_code config[4B] status[2B] crc
    data = [0, 0, 0, 0]
    simulator.espi_get_config(0x20, data)

def espi_raw_oob_config(simulator):
    simulator.espi_set_config_30h(max_payload_size=3, ch_enable=1)

def espi_raw_oob_get(simulator):
    oob_data = [0] * 6
    simulator.espi_oob_get(tag=9, data=oob_data)

def espi_mcux_oob_put(simulator):
    # PUT_OOB(06) CYCLE_OOB(0x21) tag length_all slave_addr cmd_code byte_count data... crc

    """ Send an OOB PUT message. Typical case: 8-byte SMBus-like message """
    ### max_payload_size = 1..3
    simulator.espi_set_config_30h(3, 1)

    write_data = list(range(128))
    oob_data = [
        (0x50 << 1) | 0,  # Slave Address with Write bit (0xA0)
        0xA0,             # Command Code
        len(write_data)   # Byte Count
    ] + write_data

    tag = 0
    simulator.espi_oob_put(tag, oob_data)

def espi_mcux_oob_get(simulator):
    espi_raw_oob_config(simulator)
    espi_raw_oob_get(simulator)

def espi_mcux_saf_erase(simulator):
    simulator.espi_set_config(0x40, FLASH_CH_CONFIG)

    write_data = list(range(128))
    simulator.espi_flash_erase(True, 5, 0x500, len(write_data), CYCLE_FLASH_ER)
    simulator.espi_flash_erase_cmpl(True, 5, CYCLE_FLASH_ER)

def espi_mcux_saf_write(simulator):
    # PUT_FLASH_NP(0x0A) CYCLE_TYPE(1) Tag(4bits)+Len(4bits) Len Addr(32bits 32-0) Data
    ### max_req_size     = 1..7 = 64,128,256,..4096 bytes
    ### max_payload_size = 1..3 = 64,128,256
    ### block_erase_size = 1..5 = 4K, 64K, 4K/64K, 128K, 256K
    ### ch_enable        = 0..1 = disable,enable
    simulator.espi_set_config(0x40, FLASH_CH_CONFIG)

    write_data = list(range(128))
    simulator.espi_flash_write(True, 5, 0x500, write_data, CYCLE_FLASH_WR)
    simulator.espi_flash_write_cmpl(True, 5, CYCLE_FLASH_WR)
    data = list(range(127, -1, -1))
    simulator.espi_flash_write(True, 5, 0x580, data, CYCLE_FLASH_WR)
    simulator.espi_flash_write_cmpl(True, 5, CYCLE_FLASH_WR)

def espi_mcux_saf_read(simulator):
    simulator.espi_set_config(0x40, FLASH_CH_CONFIG)

    simulator.espi_flash_read(True, 5, 0x500, 256, CYCLE_FLASH_RD, resp_data=[])
    time.sleep(0.1)
    # WAIT_STATE needs one more cycle to take last byte
    read_data = [0] * 257
    simulator.espi_flash_read_cmpl(True, 5, CYCLE_FLASH_RD, read_data)

def espi_mcux_saf_read_compl(simulator):
    # WAIT_STATE needs one more cycle to take last byte
    read_data = [0] * 257
    simulator.espi_flash_read_cmpl(True, 5, CYCLE_FLASH_RD, read_data)

def espi_mcux_saf_wrd(simulator):
    simulator.espi_set_config(0x40, FLASH_CH_CONFIG)

    data = list(range(128))
    simulator.espi_flash_erase(True, 5, 0x500, len(data), CYCLE_FLASH_ER)
    simulator.espi_flash_erase_cmpl(True, 5, CYCLE_FLASH_ER)
    simulator.espi_flash_write(True, 5, 0x500, data, CYCLE_FLASH_WR)
    simulator.espi_flash_write_cmpl(True, 5, CYCLE_FLASH_WR)
    data2 = list(range(127, -1, -1))
    simulator.espi_flash_write(True, 5, 0x580, data2, CYCLE_FLASH_WR)
    simulator.espi_flash_write_cmpl(True, 5, CYCLE_FLASH_WR)
    simulator.espi_flash_read(True, 5, 0x500, 256, CYCLE_FLASH_RD, resp_data=[])
    time.sleep(0.1)
    read_data = [0] * 257
    simulator.espi_flash_read_cmpl(True, 5, CYCLE_FLASH_RD, read_data)

# Endpoint. Write base address 0 data, address 4 command
def espi_mcux_ep_write_p80(simulator):
    data = [0x88]
    simulator.espi_perif_write_io(0x80, data)

def espi_mcux_ep_write_data(simulator):
    data = [0x56]
    simulator.espi_perif_write_io(0x0100, data)

def espi_mcux_ep_write_cmd(simulator):
    cmd = [0x57]
    simulator.espi_perif_write_io(0x0104, cmd)

def espi_mcux_ep_read_data(simulator):
    data = [0]
    simulator.espi_perif_read_io(0x0100, data)

# Index-data.
def espi_mcux_idx_write_data(simulator):
    data = [0x58]
    simulator.espi_perif_write_io(0x0200, data)

def espi_mcux_idx_write_cmd(simulator):
    cmd = [0x59]
    simulator.espi_perif_write_io(0x0201, cmd)

# Mailbox.
def espi_mcux_mb_write(simulator):
    # PUT_PC(0x00) CYCLE_TYPE(1) Tag(4b)+Len(4b) Len(8b) Addr(32b 32-0) Data CRC
    write_data = [1, 2, 3, 4, 8, 7, 6, 5]
    simulator.espi_perif_write_mem(True, 0xA, 0x0300, True, write_data)
    # Typical Cmd  00 01 A0 08 00 00 04 00 01 02 03 04 08 07 06 05 1E
    # Typical Resp 08 0F 03 9B

def espi_mcux_mb_read(simulator):
    # PUT_NP(0x02) CYCLE_TYPE(CYCLE_MEMRD32 = 0) Tag(4bits)+Len(4bits) Len Addr(32bits 32-0) Data CRC
    read_data = [0] * 9
    # Send a downstream non posted read request with an ACCEPT
    # Command   : PUT_NP
    # Cycle type: MEMORY READ 32
    # Tag value : 0xA
    # CYCLE_SC_DATA_11 Successful Completion With Data
    simulator.espi_perif_read_mem(True, 0xA, 0x0300, True, len(read_data), CYCLE_SC_DATA_11, read_data)
    # Typical Cmd  02 00 A0 08 00 00 04 00 BC
    # Typical Resp 08 cycle_type(0F) A0 08 01 02 03 04 08 07 06 05 STAT(0F 03) 85

def espi_mcux_mb_wrd(simulator):
    simulator.espi_set_config_10h(7, 3, 1, 1)

    data = list(range(256))
    simulator.espi_perif_write_mem(True, 0xA, 0x0300, True, data)
    simulator.espi_perif_read_mem(True, 0xA, 0x0300, True, len(data), CYCLE_SC_DATA_11, data)

#==========================================================================
# MAIN PROGRAM
#==========================================================================
if __name__ == '__main__':
    if (len(sys.argv) < 3):
        print("usage: espi_generator IP sim_mode command1 command2 ... command")
        print("- sim_mode 0 : transaction mode that generates eSPI transaction")
        print("  sim_mode 1 : master mode that generates eSPI command phase " +
              "and clock only for TAR and response phase")
        print("\n")
        print("Available commands are ")
        func_names = [ f[5:] for f in dir(sys.modules[__name__])
                       if f.startswith('espi_') ]
        for f in func_names:
           doc = eval('espi_%s.__doc__'%f)
           if doc:
              print('%-25s : %s' % (f, eval('espi_%s.__doc__' % f)))
        print("\n")
        print("- To generate a sequence of espi packets that satisfy the condition")
        print("  for advance trigger mode 1 example 'capture_espi_trig1.py' using")
        print("  Promira espi analyzer, use the following command:")
        print("  espi_host.py IP 0 set_config_10h get_status_pc_free perif_downstream_wr32")
        print("\n")
        print("- To generate a sequence of espi packets that satisfy the condition")
        print("  for advance trigger mode 2 example 'capture_espi_trig2.py' using")
        print("  Promira espi analyzer, use the following command:")
        print("  espi_host.py IP 0 set_config_10h get_status_np_free perif_downstream_rd64")
        print("\n")
        print("- To generate a sequence of espi packets that satisfy the condition")
        print("  for advance trigger eerror code mode example 'capture_espi_trig_err.py' using")
        print("  Promira espi analyzer, use the following command:")
        print("  espi_host.py IP 0 set_config_10h get_status perif_downstream_rd32")
        sys.exit()

    ip       = sys.argv[1]
    sim_mode = int(sys.argv[2])
    cmds     = sys.argv[3:]

    # Open the device
    simulator = NxpEspiSimulator(ip)
    simulator.espi_config_mode(sim_mode)

    if '--reset' in cmds:
        simulator.espi_inband_reset()
        cmds = [c for c in cmds if c != '--reset']

    commands = []
    for token in cmds:
        if hasattr(sys.modules[__name__], 'espi_' + token):
            commands.append([token])
        elif commands:
            commands[-1].append(token)

    for parts in commands:
        cmd, args = parts[0], parts[1:]
        try:
            print('simulating %s %s' % (cmd, ' '.join(args)))
            func = getattr(sys.modules[__name__], 'espi_' + cmd)
            func(simulator, *args)
        except Exception as e:
            print('unknown command or failed: %s (%s)' % (cmd, e))

    # Close the device
    simulator.close()
