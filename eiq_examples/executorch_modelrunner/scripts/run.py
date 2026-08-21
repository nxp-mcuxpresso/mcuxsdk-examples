#!/usr/bin/env python3
# Interactive PC-side CLI for executorch_modelrunner.
#
# Talks to the board over a binary-transparent serial protocol defined in:
#   examples/eiq_examples/executorch_modelrunner/protocol.h
#   examples/eiq_examples/executorch_modelrunner/serial_protocol.cpp
# Frame: [0x55][0xAA][CMD][LEN:4B LE][PAYLOAD:LEN][CRC16:2B LE]
#
# The board speaks a binary request/response protocol (no streaming text): it
# answers each command with exactly one status frame. This CLI parses those
# frames and prints friendly, real-time feedback after every command.
#
# Requirements: pyserial (pip install pyserial). pytest for the codec unit tests.
#
# Usage (interactive):
#   python scripts/run.py --port /dev/ttyACM4 --baud 115200
#   (modelrunner|no model)> help
#   (modelrunner|no model)> model npu.pte
#   (modelrunner|model loaded)> input img1.bin
#   (modelrunner|input ready)> run
#   (modelrunner|model loaded)> quit
import argparse
import os
import struct
import sys
import time

# pyserial is only needed for the live serial I/O; import lazily so the pure
# codec functions (and the pytest suite) work without it installed.
try:
    import serial  # type: ignore
except ImportError:  # pragma: no cover - exercised only without pyserial
    serial = None


# ANSI color output: enabled only on interactive terminals, so pipes and
# redirected files stay clean. Windows 10+ consoles are switched into
# virtual-terminal mode to parse the escapes.
_USE_COLOR = sys.stdout.isatty()


def _enable_win_ansi() -> None:
    """Flip on ENABLE_VIRTUAL_TERMINAL_PROCESSING on Windows 10+ consoles."""
    if sys.platform != "win32":
        return
    try:
        import ctypes
        kernel32 = ctypes.windll.kernel32
        ENABLE_VT = 0x0004  # ENABLE_VIRTUAL_TERMINAL_PROCESSING
        for h in (-11, -12):  # STD_OUTPUT_HANDLE, STD_ERROR_HANDLE
            handle = kernel32.GetStdHandle(h)
            mode = ctypes.c_uint32()
            if kernel32.GetConsoleMode(handle, ctypes.byref(mode)):
                kernel32.SetConsoleMode(handle, mode.value | ENABLE_VT)
    except Exception:
        pass


if _USE_COLOR:
    _enable_win_ansi()

_RST = "\033[0m"


def _c(text, *codes: str) -> str:
    """Wrap text in ANSI codes; no-op when stdout is not a tty."""
    if not _USE_COLOR or not codes:
        return str(text)
    return "".join(codes) + str(text) + _RST


def ok(t):     return _c(t, "\033[32m", "\033[1m")   # green bold  -> success / top-1
def fail(t):   return _c(t, "\033[31m", "\033[1m")   # red bold    -> errors
def action(t): return _c(t, "\033[36m", "\033[1m")   # cyan bold   -> actions (>>), command names
def warn(t):   return _c(t, "\033[33m")              # yellow      -> warnings, board/serial tags
def dim(t):    return _c(t, "\033[90m")              # gray        -> hints, prompt decoration
def bold(t):   return _c(t, "\033[1m")               # bold        -> emphasis
def hl(t):     return _c(t, "\033[35m")              # magenta     -> dtype / values


def _strip_quotes(s: str) -> str:
    """Remove one pair of matching surrounding quotes from a pasted path.

    Users often paste paths as "C:\path\file.pte" or 'file.bin' (shell
    style); this shell does not parse quotes, so strip a matching pair
    explicitly. Also enables quoted paths containing spaces.
    """
    if len(s) >= 2 and s[0] == s[-1] and s[0] in ('"', "'"):
        return s[1:-1]
    return s


# --- Protocol constants (mirror protocol.h) ---------------------------------
SYNC0, SYNC1 = 0x55, 0xAA
HEADER_LEN = 2 + 1 + 4  # SYNC0 + SYNC1 + CMD + LEN(4)
CRC_LEN = 2

# PC -> board commands
CMD_LOAD_MODEL = 0x01
CMD_LOAD_INPUT = 0x02
CMD_RUN = 0x03
CMD_GET_INFO = 0x04

# board -> PC status
ST_OK = 0x81
ST_INFO = 0x82
ST_RESULT = 0x83
ST_ERROR = 0xC0

ERROR_CODES = {
    0x00: "Ok",
    0x01: "Crc",
    0x02: "Protocol",
    0x03: "ModelTooBig",
    0x04: "ModelLoad",
    0x05: "InputSize",
    0x06: "NpuInit",
    0x07: "Execute",
    0x08: "NoModel",
}

# ExecuTorch ScalarType enum (runtime/core/portable_type/scalar_type.h).
# The board writes static_cast<uint8_t>(scalar_type), so the dtype byte is the
# raw enum index below. Float == 6 (NOT 1). Char == 1 (int8, used by cifarnet).
DTYPE_NAMES = {
    0: "Byte",
    1: "Char",       # int8 -- cifarnet quantized activations
    2: "Short",
    3: "Int",
    4: "Long",
    5: "Half",
    6: "Float",
    7: "Double",
    8: "ComplexHalf",
    9: "ComplexFloat",
    10: "ComplexDouble",
    11: "Bool",
    12: "QInt8",
    13: "QUInt8",
    14: "QInt32",
    15: "BFloat16",
    16: "QUInt4x2",
    17: "QUInt2x4",
    18: "Bits1x8",
    19: "Bits2x4",
    20: "Bits4x2",
    21: "Bits8",
    22: "Bits16",
    23: "Float8_e5m2",
    24: "Float8_e4m3fn",
    25: "Float8_e5m2fnuz",
    26: "Float8_e4m3fnuz",
    27: "UInt16",
    28: "UInt32",
    29: "UInt64",
}

# struct format codes for the contiguous-element decode path. Values that have
# no fixed-size native element (or we don't decode in bulk) map to None.
_DTYPE_STRUCT = {
    0: ("B", 1),    # Byte  -> uint8
    1: ("b", 1),    # Char  -> int8
    2: ("h", 2),    # Short -> int16
    3: ("i", 4),    # Int   -> int32
    4: ("q", 8),    # Long  -> int64
    5: ("e", 2),    # Half  -> float16
    6: ("f", 4),    # Float -> float32
    7: ("d", 8),    # Double-> float64
    11: ("?", 1),   # Bool
    27: ("H", 2),   # UInt16
    28: ("I", 4),   # UInt32
    29: ("Q", 8),   # UInt64
}


class BoardError(Exception):
    """Raised when the board returns an ST_ERROR frame.

    Carries the numeric error code and the optional message bytes so the shell
    can print a human-readable cause instead of exiting.
    """
    def __init__(self, code: int, msg: str = ""):
        self.code = code
        self.msg = msg
        self.name = ERROR_CODES.get(code, f"0x{code:02x}")
        super().__init__(f"{self.name}: {msg}" if msg else self.name)


# --- Frame codec (pure, no serial) ------------------------------------------
def crc16_ccitt(data: bytes) -> int:
    """CRC-16/CCITT-FALSE: init=0xFFFF, poly=0x1021, MSB-first, no final xor.
    Check value for ASCII '123456789' is 0x29B1. Mirrors protocol.cpp exactly.
    """
    crc = 0xFFFF
    for b in data:
        crc ^= b << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = ((crc << 1) ^ 0x1021) & 0xFFFF
            else:
                crc = (crc << 1) & 0xFFFF
    return crc


def build_frame(cmd: int, payload: bytes) -> bytes:
    """Encode [SYNC0][SYNC1][CMD][LEN:u32 LE][PAYLOAD][CRC16 LE].
    CRC is computed over CMD+LEN+PAYLOAD (head[2:], skipping the SYNC bytes),
    matching send_frame in serial_protocol.cpp.
    """
    head = bytes([SYNC0, SYNC1, cmd]) + struct.pack("<I", len(payload))
    crc = crc16_ccitt(head[2:] + payload)
    return head + payload + struct.pack("<H", crc)


def parse_frame(buf: bytes):
    """Parse one frame already known to start at index 0 (not a stream hunt).
    Returns (cmd, payload). Raises ValueError on SYNC/CRC/length mismatch.
    """
    if len(buf) < HEADER_LEN + CRC_LEN:
        raise ValueError(f"frame too short: {len(buf)} bytes")
    if buf[0] != SYNC0 or buf[1] != SYNC1:
        raise ValueError(f"bad SYNC: {buf[0]:#04x} {buf[1]:#04x}")
    cmd = buf[2]
    (length,) = struct.unpack("<I", buf[3:7])
    if len(buf) < HEADER_LEN + length + CRC_LEN:
        raise ValueError(
            f"truncated payload: declared {length}, have {len(buf) - HEADER_LEN - CRC_LEN}"
        )
    payload = buf[HEADER_LEN:HEADER_LEN + length]
    (recv_crc,) = struct.unpack("<H", buf[HEADER_LEN + length:HEADER_LEN + length + 2])
    calc_crc = crc16_ccitt(buf[2:HEADER_LEN] + payload)  # CMD+LEN+PAYLOAD
    if recv_crc != calc_crc:
        raise ValueError(
            f"CRC mismatch: recv {recv_crc:#06x} calc {calc_crc:#06x}"
        )
    return cmd, payload


# --- INFO / RESULT payload parsers -----------------------------------------
def _dtype_name(d: int) -> str:
    return DTYPE_NAMES.get(d, f"Unknown({d})")


def _read_tensor_spec(buf: bytes, off: int, with_dim_order: bool):
    """Read one tensor spec entry. Returns (entry_dict, new_off).
    INFO entries carry dim_order; RESULT entries do not.
    Layout: dtype:u8  ndim:u8  sizes:ndim*i32LE  [dim_order:ndim*u8]  nbytes:u32LE
    """
    dtype = buf[off]
    off += 1
    ndim = buf[off]
    off += 1
    shape = list(struct.unpack_from(f"<{ndim}i", buf, off))
    off += 4 * ndim
    entry = {"dtype": dtype, "dtype_name": _dtype_name(dtype), "shape": shape}
    if with_dim_order:
        dim_order = list(buf[off:off + ndim])
        off += ndim
        entry["dim_order"] = dim_order
    (nbytes,) = struct.unpack_from("<I", buf, off)
    off += 4
    entry["nbytes"] = nbytes
    return entry, off


def parse_info(b: bytes) -> dict:
    """Parse an INFO payload."""
    off = 0
    n_inputs = b[off]
    off += 1
    inputs = []
    for _ in range(n_inputs):
        entry, off = _read_tensor_spec(b, off, with_dim_order=True)
        inputs.append(entry)
    result = {"n_inputs": n_inputs, "inputs": inputs, "outputs": []}
    if off >= len(b):
        return result  # truncated: only inputs were sent
    n_outputs = b[off]
    off += 1
    outputs = []
    for _ in range(n_outputs):
        if off >= len(b):
            break
        entry, off = _read_tensor_spec(b, off, with_dim_order=True)
        outputs.append(entry)
    result["n_outputs"] = n_outputs
    result["outputs"] = outputs
    return result


def parse_result(b: bytes) -> dict:
    """Parse a RESULT payload."""
    off = 0
    (timing,) = struct.unpack_from("<Q", b, off)
    off += 8
    n_outputs = b[off]
    off += 1
    outputs = []
    for _ in range(n_outputs):
        dtype = b[off]
        off += 1
        ndim = b[off]
        off += 1
        shape = list(struct.unpack_from(f"<{ndim}i", b, off))
        off += 4 * ndim
        (nbytes,) = struct.unpack_from("<I", b, off)
        off += 4
        data = bytes(b[off:off + nbytes])
        off += nbytes
        outputs.append({
            "dtype": dtype,
            "dtype_name": _dtype_name(dtype),
            "shape": shape,
            "nbytes": nbytes,
            "data": data,
        })
    return {"timing_us": timing, "n_outputs": n_outputs, "outputs": outputs}


def decode_tensor(entry: dict):
    """Interpret a RESULT output's raw data bytes as a flat Python list."""
    data = entry["data"]
    dtype = entry["dtype"]
    spec = _DTYPE_STRUCT.get(dtype)
    if spec is None or len(data) == 0:
        return list(data)
    fmt, size = spec
    n = len(data) // size
    return list(struct.unpack(f"<{n}{fmt}", data[:n * size]))


# --- Serial stream helpers --------------------------------------------------
def _read_exact(ser, n: int) -> bytes:
    """Read exactly n bytes from the serial port (block until timeout)."""
    out = bytearray()
    while len(out) < n:
        chunk = ser.read(n - len(out))
        if not chunk:
            raise TimeoutError(
                f"serial read timeout: wanted {n} bytes, got {len(out)}"
            )
        out += chunk
    return bytes(out)


def drain_startup_banner(ser, idle_ms: int = 200, deadline_s: float = 5.0) -> bytes:
    """Read and discard bytes until the line is quiet for `idle_ms`.
    The board prints a startup banner once at boot on the same LPUART the
    protocol uses; if not drained, that text would be mistaken for SYNC bytes.
    """
    old_timeout = ser.timeout
    ser.timeout = idle_ms / 1000.0
    drained = bytearray()
    start = time.monotonic()
    try:
        while time.monotonic() - start < deadline_s:
            chunk = ser.read(256)
            if not chunk:
                break  # line quiet for idle_ms -> banner done
            drained += chunk
    finally:
        ser.timeout = old_timeout
    return bytes(drained)


def read_response(ser):
    """Read one frame from the live serial stream: hunt for SYNC0/SYNC1
    (discarding non-sync noise like stray banner bytes), then read
    CMD+LEN+PAYLOAD+CRC, verify CRC. Returns (status, payload).

    Raises BoardError on ST_ERROR (0xC0). Raises TimeoutError if the board
    stays silent past the serial read timeout.
    """
    # 1) SYNC hunt.
    have_sync0 = False
    while True:
        b = _read_exact(ser, 1)[0]
        if not have_sync0:
            if b == SYNC0:
                have_sync0 = True
            continue
        if b == SYNC1:
            break
        have_sync0 = (b == SYNC0)

    # 2) CMD + LEN.
    status = _read_exact(ser, 1)[0]
    length = struct.unpack("<I", _read_exact(ser, 4))[0]
    payload = _read_exact(ser, length) if length else b""

    # 3) CRC over CMD+LEN+PAYLOAD.
    crc_recv = struct.unpack("<H", _read_exact(ser, 2))[0]
    crc_calc = crc16_ccitt(bytes([status]) + struct.pack("<I", length) + payload)
    if crc_recv != crc_calc:
        raise ValueError(
            f"response CRC mismatch: recv {crc_recv:#06x} calc {crc_calc:#06x}"
        )

    # 4) Error frames surface as BoardError so the REPL can print and continue.
    if status == ST_ERROR:
        err_code = payload[0] if payload else 0xFF
        msg = payload[1:].decode("ascii", errors="replace") if len(payload) > 1 else ""
        raise BoardError(err_code, msg)

    return status, payload


# --- Interactive shell ------------------------------------------------------
class ModelRunnerShell:
    """Interactive REPL driving the board command-by-command.

    Maintains a mirror of the board's state machine (no model / model loaded /
    input ready) so commands can be guarded client-side and the prompt can show
    the current phase. Board-side errors come back as BoardError and are caught
    per command, so the REPL keeps running.
    """

    def __init__(self, ser, port=None, baud=115200, topk=3):
        self.ser = ser
        self.port = port
        self.baud = baud
        self.topk = topk
        self.labels = None
        self.model_loaded = False
        self.input_ready = False
        self.model_info = None  # last parsed GET_INFO

    @property
    def state(self) -> str:
        if self.input_ready:
            return "input ready"
        if self.model_loaded:
            return "model loaded"
        return "no model"

    def prompt(self) -> str:
        s = self.state
        if s == "input ready":
            state_c = ok(s)       # green bold
        elif s == "model loaded":
            state_c = action(s)   # cyan bold
        else:
            state_c = warn(s)     # yellow
        return dim("(modelrunner|") + state_c + dim(")> ")

    # -- low-level send/recv -------------------------------------------------
    def _send(self, frame: bytes):
        # Clear any stale bytes (e.g. leftover banner) before sending so the
        # next read_response sees only this command's reply.
        self.ser.reset_input_buffer()
        self.ser.write(frame)
        self.ser.flush()

    # -- command handlers ----------------------------------------------------
    def do_model(self, path: str):
        if self.ser is None:
            print("  not connected; use 'reconnect <port>' first")
            return
        if not os.path.isfile(path):
            print(f"  file not found: {path}")
            return
        with open(path, "rb") as f:
            data = f.read()
        name = os.path.basename(path)
        print(f"  {action('>>')} uploading model {bold(name)} ({len(data)} bytes)")
        print(f"     {dim('(board receives at the baud rate; large models take a while)')}")
        try:
            self._send(build_frame(CMD_LOAD_MODEL, data))
            status, _ = read_response(self.ser)
        except BoardError as e:
            print(f"  {fail('[FAIL]')} model load: {e}")
            self.model_loaded = False
            self.input_ready = False
            self.model_info = None
            return
        except (TimeoutError, ValueError) as e:
            print(f"  {warn('[serial]')} {e}")
            return
        if status == ST_OK:
            self.model_loaded = True
            self.input_ready = False
            self.model_info = None
            print(f"  {ok('[OK]')} model upload success ({len(data)} bytes)")
            print()
            # Auto-query the spec so the user sees what was loaded.
            self.do_info()
        else:
            print(f"  {warn('[board]')} unexpected status 0x{status:02x}")

    def do_input(self, path: str):
        if self.ser is None:
            print("  not connected; use 'reconnect <port>' first")
            return
        if not self.model_loaded:
            print("  no model loaded; run 'model <file>' first")
            return
        if not os.path.isfile(path):
            print(f"  file not found: {path}")
            return
        with open(path, "rb") as f:
            data = f.read()
        # If we know the model's input size, warn before the round-trip.
        if self.model_info and self.model_info["inputs"]:
            exp = self.model_info["inputs"][0]["nbytes"]
            if len(data) != exp:
                print(f"  warning: input {len(data)}B != model expects {exp}B "
                      f"(dtype={self.model_info['inputs'][0]['dtype_name']}, "
                      f"shape={self.model_info['inputs'][0]['shape']})")
        name = os.path.basename(path)
        print(f"  {action('>>')} uploading input {bold(name)} ({len(data)} bytes)")
        try:
            self._send(build_frame(CMD_LOAD_INPUT, data))
            status, _ = read_response(self.ser)
        except BoardError as e:
            print(f"  {fail('[FAIL]')} input load: {e}")
            self.input_ready = False
            return
        except (TimeoutError, ValueError) as e:
            print(f"  {warn('[serial]')} {e}")
            return
        if status == ST_OK:
            self.input_ready = True
            print(f"  {ok('[OK]')} input upload success ({len(data)} bytes)")
        else:
            print(f"  {warn('[board]')} unexpected status 0x{status:02x}")

    def do_info(self):
        if self.ser is None:
            print("  not connected; use 'reconnect <port>' first")
            return
        try:
            self._send(build_frame(CMD_GET_INFO, b""))
            status, payload = read_response(self.ser)
        except BoardError as e:
            print(f"  [board] info FAILED: {e}")
            return
        except (TimeoutError, ValueError) as e:
            print(f"  [serial] {e}")
            return
        if status != ST_INFO:
            print(f"  [board] expected INFO, got 0x{status:02x}")
            return
        info = parse_info(payload)
        self.model_info = info
        self.model_loaded = True  # a successful GET_INFO implies a model is loaded
        print(f"  {bold('model spec:')}")
        print(f"    {dim('inputs :')} {info['n_inputs']}")
        for i, e in enumerate(info["inputs"]):
            print(f"      [{i}] dtype={hl(e['dtype_name'])} shape={e['shape']} "
                  f"dim_order={e.get('dim_order')} nbytes={e['nbytes']}")
        n_out = info.get("n_outputs", 0)
        print(f"    {dim('outputs:')} {n_out}")
        for i, e in enumerate(info.get("outputs", [])):
            print(f"      [{i}] dtype={hl(e['dtype_name'])} shape={e['shape']} "
                  f"dim_order={e.get('dim_order')} nbytes={e['nbytes']}")

    def do_run(self):
        if self.ser is None:
            print("  not connected; use 'reconnect <port>' first")
            return
        if not self.input_ready:
            print("  no input loaded; run 'input <file>' first")
            return
        print(f"  {action('>>')} running inference...")
        try:
            self._send(build_frame(CMD_RUN, b""))
            status, payload = read_response(self.ser)
        except BoardError as e:
            print(f"  {fail('[FAIL]')} run: {e}")
            return
        except (TimeoutError, ValueError) as e:
            print(f"  {warn('[serial]')} {e}")
            return
        if status != ST_RESULT:
            print(f"  {warn('[board]')} expected RESULT, got 0x{status:02x}")
            return
        res = parse_result(payload)
        self.input_ready = False  # input consumed; back to model loaded
        print(f"  {ok('[OK]')} inference time: {bold(str(res['timing_us']))} us")
        for i, o in enumerate(res["outputs"]):
            vals = decode_tensor(o)
            print()
            print(f"  {bold(f'output[{i}]')}: dtype={hl(o['dtype_name'])} shape={o['shape']}")
            print(f"    {dim('values:')} {vals[:16]}")
            if self.labels is not None and len(o["shape"]) == 2 and o["shape"][0] == 1:
                top = sorted(enumerate(vals), key=lambda x: -x[1])[:self.topk]
                for rank, (idx, score) in enumerate(top, 1):
                    lbl = self.labels[idx] if idx < len(self.labels) else f"#{idx}"
                    if rank == 1:
                        print(f"    {ok(f'top-{rank}')}: {bold(lbl)} (idx {idx}, score {score})")
                    else:
                        print(f"    {dim(f'top-{rank}')}: {lbl} (idx {idx}, score {score})")

    def do_labels(self, path: str):
        if not os.path.isfile(path):
            print(f"  file not found: {path}")
            return
        with open(path) as f:
            self.labels = f.read().split()
        print(f"  loaded {len(self.labels)} labels from {path}")

    def do_topk(self, arg: str):
        if not arg:
            print(f"  topk = {self.topk}")
            return
        try:
            self.topk = int(arg)
            print(f"  topk = {self.topk}")
        except ValueError:
            print(f"  invalid topk: {arg!r} (expected an integer)")

    def do_status(self):
        connected = self.ser is not None and self.ser.is_open
        print(f"  port    : {self.port or '(none)'}  ({'open' if connected else 'closed'})")
        print(f"  baud    : {self.baud}")
        print(f"  state   : {self.state}")
        print(f"  topk    : {self.topk}")
        print(f"  labels  : {len(self.labels) if self.labels else 'none'}")

    def do_reconnect(self, port_arg: str):
        if serial is None:
            print("  pyserial is required: pip install pyserial")
            return
        port = port_arg or self.port
        if not port:
            print("  no port; usage: reconnect <port>")
            return
        try:
            if self.ser is not None:
                self.ser.close()
            self.ser = serial.Serial(port, self.baud, timeout=30)
            banner = drain_startup_banner(self.ser)
            self.port = port
            # A reconnect means the board may have reset -> IDLE.
            self.model_loaded = False
            self.input_ready = False
            self.model_info = None
            print(f"  connected to {port} @ {self.baud}")
            if banner:
                print(f"  [board] banner: {banner!r}")
            print("  state reset (no model loaded)")
        except Exception as e:
            print(f"  reconnect failed: {e}")

    def do_help(self):
        print(f"{bold('Commands:')}")
        rows = [
            ("model <file>",        "Load a .pte model (LOAD_MODEL), then auto-show its spec"),
            ("input <file>",        "Load input .bin (LOAD_INPUT; needs a loaded model)"),
            ("info",                "Query model input/output spec (GET_INFO)"),
            ("run",                 "Run inference (RUN), print timing + outputs"),
            ("labels <file>",       "Load labels file (whitespace-separated) for top-k printout"),
            ("topk [<n>]",          "Show or set number of top-k labels to print (default 3)"),
            ("status",              "Show connection / state / settings"),
            ("reconnect [<port>]",  "Re-open the serial port (resets state to no-model)"),
            ("help, h",             "Show this help"),
            ("quit, exit, Ctrl-D",  "Exit"),
        ]
        for cmd, desc in rows:
            print(f"  {action(cmd):<24}{dim(desc)}")

    # -- dispatch / REPL -----------------------------------------------------
    def dispatch(self, cmd: str, arg: str):
        if cmd in ("quit", "exit", "q"):
            raise EOFError  # breaks the REPL loop cleanly
        elif cmd in ("help", "h", "?"):
            self.do_help()
        elif cmd in ("model", "upload_model", "m"):
            if not arg:
                print("  usage: model <file>")
            else:
                self.do_model(arg)
        elif cmd in ("input", "upload_input", "i"):
            if not arg:
                print("  usage: input <file>")
            else:
                self.do_input(arg)
        elif cmd in ("info", "get_info"):
            self.do_info()
        elif cmd in ("run", "infer", "r"):
            self.do_run()
        elif cmd in ("labels", "label"):
            if not arg:
                print("  usage: labels <file>")
            else:
                self.do_labels(arg)
        elif cmd in ("topk", "k"):
            self.do_topk(arg)
        elif cmd == "status":
            self.do_status()
        elif cmd in ("reconnect", "connect"):
            self.do_reconnect(arg)
        else:
            print(f"  unknown command: {cmd!r} (try 'help')")

    def run(self):
        self.do_help()
        print()
        while True:
            try:
                line = input(self.prompt())
            except (EOFError, KeyboardInterrupt):
                print()
                break
            line = line.strip()
            if not line:
                continue
            parts = line.split(None, 1)
            cmd = parts[0].lower()
            arg = _strip_quotes(parts[1].strip()) if len(parts) > 1 else ""
            try:
                self.dispatch(cmd, arg)
            except EOFError:  # 'quit'/'exit' -> leave the REPL cleanly
                break
            except Exception as e:  # defensive: never let one command kill the REPL
                print(f"  [error] {e}")
            print()  # blank line between command blocks for readability


# --- entry point ------------------------------------------------------------
def _resolve_positional(tokens):
    """Map the free-form positional args to (model, input) paths.

    Accepted forms (keywords are optional, order-insensitive):
      model.pte input.bin
      model model.pte input input.bin
      model model.pte
      input input.bin        (rejected later: input requires a model)
    Plain tokens that are not the keywords are taken positionally.
    """
    model = input = None
    tokens = [_strip_quotes(t) for t in tokens]
    i = 0
    while i < len(tokens):
        t = tokens[i]
        if t.lower() == "model" and i + 1 < len(tokens):
            model = tokens[i + 1]; i += 2
        elif t.lower() == "input" and i + 1 < len(tokens):
            input = tokens[i + 1]; i += 2
        elif model is None:
            model = t; i += 1
        elif input is None:
            input = t; i += 1
        else:
            raise SystemExit(f"error: unexpected extra argument: {t}")
    return model, input


def main(argv=None):
    if serial is None:
        sys.stderr.write("pyserial is required: pip install pyserial\n")
        return 1

    ap = argparse.ArgumentParser(
        description="CLI for executorch_modelrunner over the serial protocol. "
                    "With model/input path arguments it runs ONCE and exits "
                    "(script friendly: single command, stdout, exit code); "
                    "without them it drops into an interactive shell."
    )
    # Positional args -> one-shot (non-interactive) mode; omit them for
    # the interactive REPL. Both plain positional order and the explicit
    # 'model <file> input <file>' keyword style are accepted.
    ap.add_argument("paths", nargs="*",
                    help="model [.pte] and optional input [.bin]; each may be "
                         "prefixed with the keywords 'model' and 'input'")
    ap.add_argument("--port", help="serial port (e.g. /dev/ttyACM4). Required for one-shot mode.")
    ap.add_argument("--baud", type=int, default=115200, help="baud rate (default 115200)")
    ap.add_argument("--labels", help="labels file (whitespace-separated) for top-k printout")
    ap.add_argument("--topk", type=int, default=3, help="number of top-k labels to print (default 3)")
    args = ap.parse_args(argv)
    model_path, input_path = _resolve_positional(args.paths)

    if model_path and not args.port:
        sys.stderr.write("error: --port is required for one-shot mode\n")
        return 2

    ser = None
    if args.port:
        try:
            ser = serial.Serial(args.port, args.baud, timeout=30)
        except Exception as e:
            sys.stderr.write(f"failed to open {args.port}: {e}\n")
            return 1
        banner = drain_startup_banner(ser)
        print(f"{ok('Connected')} to {args.port} @ {args.baud}.")
        if banner:
            print(f"{warn('[board]')} banner: {banner!r}")
        print()

    shell = ModelRunnerShell(ser, port=args.port, baud=args.baud, topk=args.topk)
    if args.labels:
        shell.do_labels(_strip_quotes(args.labels))

    rc = 0
    try:
        if model_path:
            # One-shot mode: load model (+ optional input + run), then exit.
            # Script friendly: single command, captured stdout, exit code
            # reflects success (0) vs model/input failure (1).
            shell.do_model(model_path)
            if not shell.model_loaded:
                rc = 1
            elif input_path:
                shell.do_input(input_path)
                if shell.input_ready:
                    shell.do_run()
                else:
                    rc = 1
        else:
            # Interactive REPL.
            shell.run()
    finally:
        if ser is not None:
            ser.close()
    return rc


if __name__ == "__main__":
    sys.exit(main())
