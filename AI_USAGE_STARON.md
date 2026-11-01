# AI Usage Statement — Michael Staron
  
**AI tool/model and date:** OpenAI ChatGPT (GPT-6), November 1, 2026 

## Purpose
I used ChatGPT as a reference and troubleshooting assistant while working on Homework 3, primarily for GitHub access, Task 4 AppArmor confinement, and Task 3 parser hardening. I asked for example code to compare with my team's work and for a checklist of evidence to capture for the report.

## AI Conversation Log Files
AI_LOG_STARON.txt

## What I Used
- **GitHub troubleshooting:** I asked how to authenticate to GitHub on Ubuntu using a fine-grained personal access token and how to troubleshoot repository cloning errors (`GraphQL: Could not resolve to a Repository` and HTTP 403) when accessing my partner's repository. ChatGPT suggested checking account permissions, using browser-based GitHub CLI authentication, and configuring Git credential handling.
- **Task 4 — AppArmor:** I supplied the assignment PDF and our repository ZIP and requested an `apparmor.profile`. ChatGPT proposed a profile restricting input-file reads, allowing marker-file writes, explicitly denying and auditing access to `/opt/agentsec/decoy.txt`, blocking shell and other program execution, and allowing required runtime libraries.
- **Task 3 — Fixed parser:** I asked ChatGPT for a sample `legacy_log_parser_fixed.c` to compare against our existing implementation. It proposed input-length checking before copying to a 128-byte buffer, error handling for file operations, and safe string termination.
- **Tasks 3 and 4 — Evidence planning:** I asked what to screenshot. ChatGPT suggested documenting the compilation, `checksec` comparison, input tests, marker behavior, low-privilege user, file permissions, AppArmor enforcement, confined execution, and denial logs.
- **Documentation:** I asked ChatGPT to draft this AI-use disclosure in the format required by the homework.

## What I Changed
I treated ChatGPT's code as proposed examples for comparison, not as independently verified implementations. The AppArmor file paths, library rules, and permissions must be checked against the actual Ubuntu VM and repository layout; the fixed parser must be compared against the team's source and the assignment requirements. Any changes actually adopted into the repository should be described here before submission.

## How I Tested It
I checked my GitHub authentication with `gh auth status`, which showed the expected account, `Mstar05`. An attempt to clone the partner's repository over HTTPS initially returned HTTP 403. This chat does not establish that subsequent authentication steps resolved that error.

I treated the code that ChatGPT wrote the same way I treat my own written code. After I implemented a few of the AI's ideas, I tested it the same way I tested my first version to verify it worked. I made sure everything compiled successfully and double checked the resutls of the program.

## One Error, Limitation, or Rejected Suggestion
The suggested AppArmor profile contains absolute paths and runtime-library assumptions that may not match the final VM installation. In addition, ChatGPT's sample fixed parser uses a 127-byte maximum to reserve a NUL terminator in a 128-byte buffer; that choice needs to be checked against the team's intended definition of the maximum allowable input. I therefore cannot assume either suggested file is correct without testing. ChatGPT also could not verify the final repository behavior solely from the conversation.

