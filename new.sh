#!/bin/bash

# USAGE: ./new.sh <day>

DAY=$1

# Define some SOFT colors
BLUE='\033[0;34m'
GREEN='\033[0;32m'
RED='\033[1;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Fail early
set -e

# Validate day input
if [ -z "$DAY" ]; then
    echo -e "${RED}Error: Day number is required${NC}"
    echo "USAGE: ./new.sh <day>"
    exit 1
fi

echo -e "${GREEN}Creating new day: ${RED}Day ${DAY}${NC}"

# Create C source file if it doesn't already exist
if [ ! -f src/day_${DAY}.c ]; then
    cp src/day_template.c src/day_${DAY}.c

    # Replace DAY_NUMBER placeholder
    sed -i "s/DAY_NUMBER/${DAY}/g" src/day_${DAY}.c

    echo -e "Created new file: ${BLUE}src/day_${DAY}.c${NC}"
fi

# Create input directory and files if they don't already exist
if [ ! -d input/day_${DAY} ]; then
    mkdir -p input/day_${DAY}
    touch input/day_${DAY}/in.txt
    touch input/day_${DAY}/ex.txt

    echo -e "Created new files: ${BLUE}input/day_${DAY}/in.txt${NC} and ${BLUE}input/day_${DAY}/ex.txt${NC}"
fi

# Grab the puzzle input from Advent of Code
if [ ! -s input/day_${DAY}/in.txt ]; then
    echo -e "${YELLOW}Fetching puzzle input for Day ${DAY}...${NC}"

    # Load session key from .env
    if [ -f .env ]; then
        source .env
    else
        echo -e "${RED}Error: .env file not found${NC}"
        exit 1
    fi

    if [ -z "$AOC_SESSION_KEY" ]; then
        echo -e "${RED}Error: AOC_SESSION_KEY not set in .env${NC}"
        exit 1
    fi

    # Fetch the input
    curl -s -b "session=${AOC_SESSION_KEY}" \
        "https://adventofcode.com/2025/day/${DAY}/input" \
        -o input/day_${DAY}/in.txt

    if [ -s input/day_${DAY}/in.txt ]; then
        # Strip all trailing whitespace and newlines from entire file
        sed -i -e ':a' -e '$!{N;ba' -e '}; s/[[:space:]]\+$//' input/day_${DAY}/in.txt
        # Ensure no final newline
        printf '%s' "$(cat input/day_${DAY}/in.txt)" > input/day_${DAY}/in.txt
        echo -e "${GREEN}Successfully fetched input for Day ${DAY}${NC}"
    else
        echo -e "${RED}Failed to fetch input for Day ${DAY}${NC}"
        exit 1
    fi
fi

echo -e "${GREEN}Done creating new day: ${RED}Day ${DAY}${NC}"
