#!/bin/bash

# USAGE: ./run.sh <day> [ex|example]

DAY=$1
MODE=${2:-real}

# Define colors
BLUE='\033[0;34m'
GREEN='\033[0;32m'
RED='\033[1;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Validate day input
if [ -z "$DAY" ]; then
    echo -e "${RED}Error: Day number is required${NC}"
    echo "USAGE: ./run.sh <day> [ex|example]"
    exit 1
fi

# Normalize mode
if [ "$MODE" == "ex" ] || [ "$MODE" == "example" ]; then
    MODE="ex"
    INPUT_FILE="input/day_${DAY}/ex.txt"
else
    MODE="real"
    INPUT_FILE="input/day_${DAY}/in.txt"
fi

# Rebuild (make will only recompile if source changed)
echo -e "${YELLOW}Building day ${DAY}...${NC}"
make build/day_${DAY}
if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to compile day ${DAY}${NC}"
    exit 1
fi

# Check that input file exists
if [ ! -f "$INPUT_FILE" ]; then
    echo -e "${RED}Error: Input file not found: ${BLUE}$INPUT_FILE${NC}"
    exit 1
fi

# Run the program
echo -e "${GREEN}Running ${BLUE}Day ${DAY}${GREEN} with ${MODE} input...${NC}"
echo -e "${YELLOW}---${NC}"
./build/day_${DAY} < "$INPUT_FILE"
echo -e "${YELLOW}---${NC}"
