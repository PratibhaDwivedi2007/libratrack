#!/usr/bin/env bash
# setup.sh — One-step build setup for LibraTrack
# Usage: ./setup.sh

set -e

BOLD="\033[1m"
GREEN="\033[1;32m"
RED="\033[1;31m"
YELLOW="\033[1;33m"
CYAN="\033[1;36m"
RESET="\033[0m"

echo -e "${CYAN}${BOLD}╔══════════════════════════════════════╗${RESET}"
echo -e "${CYAN}${BOLD}║       LibraTrack — Setup             ║${RESET}"
echo -e "${CYAN}${BOLD}╚══════════════════════════════════════╝${RESET}"
echo ""

# ── Dependency checks ─────────────────────────────────────────────────────────
fail=0
for tool in cmake c++ git; do
    if ! command -v "$tool" &>/dev/null; then
        echo -e "${RED}✗ Missing dependency: ${tool}${RESET}"
        fail=1
    else
        echo -e "${GREEN}✓ Found: ${tool} ($(command -v "$tool"))${RESET}"
    fi
done

if [[ $fail -eq 1 ]]; then
    echo ""
    echo -e "${RED}Please install the missing tools and re-run setup.sh${RESET}"
    exit 1
fi

echo ""

# ── CMake configure ───────────────────────────────────────────────────────────
echo -e "${BOLD}[1/2] Configuring build...${RESET}"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug 2>&1 | grep -v "^--" || true
echo ""

# ── Build ─────────────────────────────────────────────────────────────────────
echo -e "${BOLD}[2/2] Building...${RESET}"
cmake --build build --parallel

echo ""
echo -e "${GREEN}${BOLD}✓ Setup complete!${RESET}"
echo ""
echo -e "  Run the test for a specific issue:"
echo -e "    ${CYAN}./check.sh <issue-number>${RESET}"
echo -e "  Example:"
echo -e "    ${CYAN}./check.sh 1${RESET}   — tests your fix for Issue #01"
echo -e "    ${CYAN}./check.sh 42${RESET}  — tests your fix for Issue #42"
echo ""
