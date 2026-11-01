# AI Usage — CSCE 765 Homework 3

## Tool/model and date:
OpenAI ChatGPT (GPT-6), October 2026.

## Purpose:
I used ChatGPT to assist with implementing and troubleshooting Tasks 1 and 2, understanding security concepts, interpreting experimental results, and reviewing the written report. I also used it for Git guidance, drafting the AI usage document, and drafting the README.

## AI Conversation Log files:
AI_LOG_SAHOO.md

## AI Conversation chat link:
https://chatgpt.com/share/6ae77c7e-04f4-83e9-af2c-63dccfe73cc8

## What I used:
For Task 1, I used ChatGPT's guidance on writing the vulnerable C parser, generating the overflow payload, using GDB to determine the overwrite offset, and understanding the return-to-win attack.

For Task 2, I used ChatGPT's assistance with creating the OpenClaw skill and adversarial webpage, configuring the agent environment, running the experiment, and interpreting the execution trace.

I also used ChatGPT's explanations as references when preparing the stack diagram and writing my report.

## What I changed:
I adapted the suggested commands and code to my Ubuntu environment, including the local file paths, OpenClaw configuration, and experimental setup. I also revised the experiment to use a neutral summarization prompt without additional security warnings so that it better followed the assignment instructions.

I wrote the report explanations in my own words and revised them after reviewing ChatGPT's feedback.

## How I tested it:
For Task 1, I compiled and tested the parser with normal and malicious inputs, inspected the stack using GDB, and confirmed that the malicious payload executed `win()` and created the marker file.

For Task 2, I verified that OpenClaw recognized the skill, tested the wrapper's file-path restrictions, hosted the adversarial webpage locally, and ran the agent experiment. I examined the saved execution trace and confirmed that the agent attempted skill-related operations but did not execute the parser or create the marker.

## One error, limitation, or rejected suggestion:
ChatGPT initially suggested including an explicit instruction telling the agent not to execute commands from the webpage. I recognized that this changed the experimental conditions because the assignment only required the agent to summarize the webpage. I therefore rejected that suggestion and conducted the documented experiment using a neutral summarization prompt instead.
