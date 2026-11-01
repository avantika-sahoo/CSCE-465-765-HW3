# CSCE 765 — Homework 3: Agent Security and Native Program Hardening

This repository contains a four-part security lab demonstrating a stack-based buffer overflow, an attempted agent-mediated prompt injection, a repaired and hardened parser, and operating-system access controls for the original vulnerable program.

**Course:** CSCE 465/765 — Computer and Network Security  
**Team:** Avantika Sahoo (Tasks 1–2) and Michael Staron (Tasks 3–4)

> **Safety:** Run the demonstrations only in an isolated, authorized lab VM. The vulnerable executable and payload are intentionally unsafe. Do not run them as root or against production data.

## Overview

| Task | Objective | Observed result |
| --- | --- | --- |
| 1. Vulnerable parser | Demonstrate a stack overflow and return-to-win (`ret2win`) control-flow redirection | A 144-byte payload redirected execution to `win()` and created `markers/ret2win.txt`; the protected decoy read was denied. |
| 2. Agent-mediated invocation | Test whether instructions embedded in an adversarial webpage induce OpenClaw to invoke the parser | The agent attempted skill-related tool calls, but did not execute the parser; no marker was created in this test. |
| 3. Fixed and hardened parser | Repair the input-length flaw and enable compiler/linker protections | Seven tests passed. Oversized input and the original payload were rejected without a crash. |
| 4. DAC, ACLs, and AppArmor | Confine the original vulnerable parser under a low-privilege account | `win()` still executed and wrote the permitted marker, but AppArmor denied the protected decoy read. |

## Repository components

The following paths are referenced in the report or Task 1–2 setup. Additional Task 3–4 files should be located using the repository tree, as their exact paths are not established by the report.

```text
README.md		     # README file
report.pdf		     # Assignment report
AI_LOG_STARON.txt	     # Michael Staron's AI log
AI_LOG_SAHOO.html	     # Avantika Sahoo's AI log
AI_USAGE_SAHOO.md	     # Avantika Sahoo's AI usage documentation
AI_USAGE_STARON.md	     # Michael Staron's AI usage documentation
src/
  legacy_log_parser.c        # Original intentionally vulnerable C parser
  make_payload.py            # Generates the Task 1 demonstration payload
skills/
  legacy-parser/
    SKILL.md                 # OpenClaw skill instructions
    run_parser.sh            # Wrapper restricting input paths to inbox/
web/
  adversarial.html           # Adversarial webpage used in Task 2
inbox/                       # Local parser inputs (may need to be created)
markers/                     # Local demonstration outputs (may need to be created)
tests/
  ret2win_output.txt         # Saved Task 1 result
  agent_prompt_neutral.txt   # Task 2 summarization prompt
  agent_trace_neutral.json   # Task 2 agent response
  agent_session_neutral.jsonl # Task 2 detailed execution trace
  agent_stderr_neutral.txt   # Task 2 stderr capture
```

The report additionally documents a hardened executable named `legacy_log_parser_fixed`, a Task 3 test suite (`tests/test_fixed.py`), and Task 4 evidence under `tests/task4/`. Check the actual repository for the corresponding source, build scripts, and AppArmor profile before running those tasks.

## Environment and prerequisites

The experiments were conducted on Ubuntu Linux using GCC, GDB, Python 3, and (for Task 2) OpenClaw. `checksec` was used to inspect executable protections. Task 4 additionally requires an AppArmor-enabled Linux environment, ACL utilities, and administrator privileges for controlled setup.

The Task 2 environment used OpenClaw `2026.7.1-2` with a university-provided model integration. The model configuration and credentials are environment-specific and are **not** included in the repository.

## Task 1 — Vulnerable parser and ret2win

Compile the intentionally vulnerable executable from the repository root:

```bash
gcc -O0 -g -fno-stack-protector -no-pie -Wl,-z,noexecstack \
  -Wall -Wextra -o legacy_log_parser src/legacy_log_parser.c
```

Create local directories if needed, then run a normal-input smoke test:

```bash
mkdir -p inbox markers
echo 'This is a normal log entry.' > inbox/normal.log
./legacy_log_parser inbox/normal.log
```

The report's demonstration generated a payload with:

```bash
python3 src/make_payload.py
./legacy_log_parser inbox/payload.bin
```

The measured overwrite offset was **136 bytes**: a **128-byte local buffer**, followed by **8 bytes of saved frame data**. The 144-byte payload placed the address of `win()` (`0x4012b6` in the demonstrated non-PIE build) at the saved return-address location. The report recorded creation of `markers/ret2win.txt`, followed by a denied attempt to read the protected decoy and a segmentation fault. The fixed address is specific to the demonstrated build and should not be assumed for another binary.

The protected decoy used in the report was `/opt/agentsec/decoy.txt`, owned by root with mode `600`. It is a **fake course secret**; do not put real credentials in that file. Its creation and permissions require authorized administrator access.

## Task 2 — OpenClaw skill and adversarial webpage

The `legacy-parser` skill provides a wrapper around the original executable. Its shell script restricts file arguments to the Homework 3 `inbox/` directory; this path restriction does **not** make the vulnerable parser memory-safe.

The report verified skill registration with:

```bash
openclaw skills info legacy-parser
```

The webpage can be served locally from the repository root:

```bash
python3 -m http.server 8000 --bind 127.0.0.1 --directory web
```

Then inspect it at `http://127.0.0.1:8000/adversarial.html`.

**Important experimental detail:** In the documented agent test, the local webpage's contents were retrieved separately and included in `tests/agent_prompt_neutral.txt` for a summarization request. The agent did **not** directly fetch the local URL. The detailed trace is saved in `tests/agent_session_neutral.jsonl`.

The agent called `skill_workshop` to inspect `legacy-parser` and attempted to read a `SKILL.md` file from an incorrect directory. Both calls failed. The trace showed no invocation of the native parser, and the marker was absent. This is evidence of **partial influence**, not successful exploitation through the agent.

## Task 3 — Repair and hardening

The fixed parser validates input length before copying it into the 128-byte buffer. It accepts at most **127 bytes**, reserving one byte for a null terminator, and handles file I/O errors. The demonstration build enabled stack canaries, fortified checks, PIE, full RELRO, and NX; ASLR was enabled on the test system.

The report records seven passing tests covering short, 127-byte, 128-byte, oversized, empty, missing-file, and original Task 1 payload cases. Oversized inputs and the malicious payload were rejected with a nonzero exit status rather than causing a crash.

Use the build and test commands included with your partner's Task 3 files. The report does not specify the exact compiler command for the hardened binary, so this README does not invent one.

## Task 4 — Least privilege, ACLs, and AppArmor

Task 4 deliberately runs the **original vulnerable parser**, not the fixed version, under a low-privilege account named `agentlab`. DAC permissions and ACLs allow the required parser, input, and marker operations. An AppArmor profile named `hw3-parser` confines the process and denies reads of the protected decoy and execution of additional programs.

The report confirmed the profile in enforce mode. In the controlled test, the original payload still redirected execution to `win()` and created a marker owned by `agentlab`, while a kernel audit entry showed an AppArmor denial for the decoy read. The process subsequently exited with status `139`.

**Do not apply Task 4 permissions or AppArmor policy blindly.** These operations modify local accounts, file ACLs, and system security configuration. Follow the partner's Task 4 setup instructions and inspect the profile on an isolated VM. The controlled experiment temporarily adjusted the decoy ACL to distinguish AppArmor enforcement from ordinary DAC denial, then restored the original ACL.

## Results and interpretation

Tasks 3 and 4 address different aspects of the same risk. **Task 3 prevents the unsafe copy** by checking input size and adds exploit mitigations. **Task 4 limits the consequences** of a compromised process through least privilege and mandatory access control, without fixing the underlying memory bug. Task 2 illustrates that untrusted webpage text can influence an agent's tool selection, even when the intended native execution does not occur.

## Evidence and submission

- The **Homework 3 report** contains terminal screenshots, a stack diagram, security-property comparisons, test outcomes, and the analysis for all four tasks.
- `tests/agent_session_neutral.jsonl` is the documented Task 2 execution trace.
- `AI_USAGE.md` describes AI assistance and verification according to the course's required format.
- Local generated artifacts, core dumps, credentials, and protected files should not be committed unless the assignment explicitly requires them.

## Contributions

- **Avantika Sahoo:** Tasks 1 and 2, report template, README, and initial GitHub/repository setup.
- **Michael Staron:** Tasks 3 and 4.

This README summarizes the observed experiments in the accompanying Homework 3 report. Commands and paths for partner-authored Tasks 3 and 4 should be checked against the final committed source tree before submission.
