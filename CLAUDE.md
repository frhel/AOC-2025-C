# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an Advent of Code 2025 solution repository written in C. Each day's solution is a separate C program that reads input from stdin and outputs the answers to parts 1 and 2.

## Key Architecture

- **Day structure**: Each day has a corresponding `src/day_N.c` file with three main functions:
  - `load_data()`: Reads and parses input from stdin
  - `solve_part1()`: Solves part 1
  - `solve_part2()`: Solves part 2
  - `main()`: Orchestrates the solution with timing checkpoints

- **Timing utility** (`src/timer.h` and `src/timer.c`): Provides microsecond-precision timing for benchmarking solution phases. Use `timer_start()` to begin, `timer_checkpoint(label)` between phases, and `timer_total()` for overall time.

- **Input structure**: Each day has:
  - `input/day_N/in.txt`: The actual puzzle input (fetched from AOC)
  - `input/day_N/ex.txt`: Example/test input for the day

## Common Development Commands

**Build all days:**
```bash
make
```

**Build a specific day:**
```bash
make build/day_N
```

**Run a day's solution with real input:**
```bash
./run.sh N
```

**Run a day's solution with example input:**
```bash
./run.sh N ex
```

**Run with make directly:**
```bash
make run DAY=N       # Real input
make test DAY=N      # Example input
```

**Create a new day (scaffolds template, creates input dirs, fetches puzzle input):**
```bash
./new.sh N
```
Requires `AOC_SESSION_KEY` in `.env` file to fetch input from adventofcode.com.

**Clean build artifacts:**
```bash
make clean
```

## Compilation

- Compiler: `gcc`
- Flags: `-Wall -Wextra -std=c99 -O2`
- Standard: C99

## Input Handling Notes

- Input is read from stdin using `fgets()`
- Trailing newlines are stripped with `strcspn(line, "\n")`
- Actual input (in.txt) has trailing whitespace stripped by `new.sh`
- Buffer size is 1024 bytes per line (adjust in code if needed for larger inputs)

## User Preferences

When working on solutions, provide explanations of what to do and how to do it rather than implementing directly. The user prefers guidance and problem-solving discussion over completed code.
