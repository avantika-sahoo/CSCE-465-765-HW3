#!/usr/bin/env python3

from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
BINARY = ROOT / "legacy_log_parser_fixed"
INPUTS = ROOT / "tests" / "task3" / "inputs"
INPUTS.mkdir(parents=True, exist_ok=True)

if not BINARY.is_file():
    sys.exit("Build legacy_log_parser_fixed before running these tests.")

original_input = ROOT / "inbox" / "payload.bin"

if not original_input.is_file():
    sys.exit(
        "Missing inbox/payload.bin: copy the existing Task 1 input here."
    )

# The actual original fixture should exceed the fixed parser's limit.
if original_input.stat().st_size <= 127:
    sys.exit("The supplied Task 1 fixture is unexpectedly small.")

cases = [
    ("short input", "short.log", b"Normal log entry\n", True),
    ("exactly 127 bytes", "maximum.log", b"A" * 127, True),
    ("128 bytes", "over_limit.log", b"A" * 128, False),
    ("empty input", "empty.log", b"", True),
    ("large input", "large.log", b"A" * 4096, False),
]

tests = []

for label, filename, data, expected_success in cases:
    path = INPUTS / filename
    path.write_bytes(data)
    tests.append((label, path, expected_success))

import tempfile

failures = 0

with tempfile.TemporaryDirectory(prefix="hw3-missing-") as temp:
    tests.append(("missing file", Path(temp) / "missing.log", False))
    tests.append(("original Task 1 input", original_input, False))

    for label, path, expected_success in tests:
        result = subprocess.run(
            [str(BINARY), str(path)],
            cwd=ROOT,
            capture_output=True,
            text=True,
            errors="replace",
        )

        # Require a normal failure exit, not a crash or signal.
        expected_code = 0 if expected_success else 1
        passed = result.returncode == expected_code

        if expected_success:
            passed = passed and "Log preview:" in result.stdout
        else:
            passed = passed and bool(result.stderr.strip())

        failures += not passed

        print(f"{'PASS' if passed else 'FAIL'}: {label}")
        print(f"  expected exit: {expected_code}")
        print(f"  actual exit:   {result.returncode}")

        if result.stdout.strip():
            print(f"  stdout: {result.stdout.strip()}")
        if result.stderr.strip():
            print(f"  stderr: {result.stderr.strip()}")

        print()

print(f"Results: {len(tests) - failures}/{len(tests)} passed")
sys.exit(1 if failures else 0)
