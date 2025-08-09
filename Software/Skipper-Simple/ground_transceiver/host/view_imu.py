#!/usr/bin/env python3
"""
Live / demo / playback viewer for skipper-simplified IMUFrame packets.

  ┌────────────────────────────────────────────────────────────┐
  │  LIVE (USB)          python view_imu.py /dev/tty.usbmodem  │
  │  DEMO (no hardware)  python view_imu.py --demo             │
  │  PLAYBACK            python view_imu.py --file dump.bin    │
  └────────────────────────────────────────────────────────────┘
"""
import sys, struct, serial, argparse, glob, time, random, io

BAUD   = 115200
FMT    = "<6f"                    # ax,ay,az,gx,gy,gz  (float32 little-endian)
FRAME  = struct.calcsize(FMT)     # 24 bytes

# ───── COBS helper ────────────────────────────────────────────────────────
def cobs_decode(buf: bytes) -> bytes:
    out, idx = bytearray(), 0
    while idx < len(buf):
        code = buf[idx]; idx += 1
        if code == 0: break
        end = idx + code - 1
        out += buf[idx:end]; idx = end
        if code != 0xFF and idx < len(buf): out.append(0)
    return bytes(out)

# ───── Generators for different modes ────────────────────────────────────
def serial_generator(port: str):
    ser = serial.Serial(port, BAUD, timeout=1)
    print(f"Connected to {port} @ {BAUD} baud")
    buf = bytearray()
    while True:
        b = ser.read(1)
        if not b: continue
        if b == b'\x00':
            payload = cobs_decode(buf); buf.clear()
            if len(payload) == FRAME: yield payload
        else:
            buf += b

def demo_generator(freq_hz=10):
    """Sine-wave AY, constant AZ=1 g, small gyro noise."""
    dt = 1.0 / freq_hz
    t = 0.0
    while True:
        ay = 0.5 * random.random() * 0.02 + 0.01 * (time.time() % 60)
        payload = struct.pack(FMT,
                              0.0, ay, 1.0,            # ax, ay, az
                              *(0.05*random.random() for _ in range(3)))  # gx gy gz
        yield payload
        time.sleep(dt)
        t += dt

def file_generator(path: str):
    with open(path, "rb") as f:
        buf = bytearray()
        while (b := f.read(1)):
            if b == b'\x00':
                payload = cobs_decode(buf); buf.clear()
                if len(payload) == FRAME: yield payload
            else:
                buf += b

# ───── CLI ────────────────────────────────────────────────────────────────
if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("port", nargs="?",
                    help="USB serial port (e.g. /dev/tty.usbmodem1101 or COM4)")
    ap.add_argument("--demo", action="store_true",
                    help="Run without hardware, generate synthetic IMU data")
    ap.add_argument("--file", metavar="BIN",
                    help="Replay previously-captured COBS binary file")
    args = ap.parse_args()

    # pick data source
    if args.demo:
        gen = demo_generator()

    elif args.file:
        gen = file_generator(args.file)

    else:                      # live mode
        port = args.port
        if not port:
            # auto-detect first USB serial device
            ports = glob.glob("/dev/tty.usb*") + glob.glob("/dev/ttyACM*") + glob.glob("COM*")
            if not ports:
                ap.error("No USB serial port found. Use --demo or --file.")
            port = ports[0]
        gen = serial_generator(port)

    # ── display loop ──────────────────────────────────────────────────────
    for payload in gen:
        ax, ay, az, gx, gy, gz = struct.unpack(FMT, payload)
        print(f"\rAX:{ax:+6.2f}  AY:{ay:+6.2f}  AZ:{az:+6.2f}  "
              f"GX:{gx:+6.2f}  GY:{gy:+6.2f}  GZ:{gz:+6.2f}", end="", flush=True)
 