#!/usr/bin/env bash
set -euo pipefail

HW3_DIR="$HOME/csce765-agentsec/hw3"
INBOX_DIR="$HW3_DIR/inbox"
PARSER="$HW3_DIR/legacy_log_parser"

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <file_path>" >&2
    exit 1
fi

if [[ ! -f "$1" ]]; then
    echo "Error: Input file does not exist." >&2
    exit 1
fi

REAL_INBOX=$(realpath "$INBOX_DIR")
REAL_FILE=$(realpath "$1")

case "$REAL_FILE" in
    "$REAL_INBOX"/*)
        ;;
    *)
        echo "Error: File must be inside the inbox directory." >&2
        exit 1
        ;;
esac

exec "$PARSER" "$REAL_FILE"
