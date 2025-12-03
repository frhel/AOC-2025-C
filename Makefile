CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Ofast -march=native -flto
SRCDIR = src
BUILDDIR = build
INPUTDIR = input

# Get all C source files and convert to executable names
SOURCES = $(wildcard $(SRCDIR)/day_*.c)
LIBRARY_SRCS = $(SRCDIR)/timer.c
EXECUTABLES = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%,$(SOURCES))

# Default target
all: $(BUILDDIR) $(EXECUTABLES)

# Create build directory
$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

# Build individual executables
$(BUILDDIR)/%: $(SRCDIR)/%.c $(LIBRARY_SRCS) | $(BUILDDIR)
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Run a specific day: make run DAY=1
run:
	@if [ -z "$(DAY)" ]; then \
		echo "Usage: make run DAY=<day_number>"; \
		exit 1; \
	fi
	@$(BUILDDIR)/day_$(DAY) < $(INPUTDIR)/day_$(DAY)/in.txt

# Run with example input: make test DAY=1
test:
	@if [ -z "$(DAY)" ]; then \
		echo "Usage: make test DAY=<day_number>"; \
		exit 1; \
	fi
	@$(BUILDDIR)/day_$(DAY) < $(INPUTDIR)/day_$(DAY)/ex.txt

# Clean build artifacts
clean:
	@rm -rf $(BUILDDIR)
	@echo "Cleaned build directory"

.PHONY: all run test clean
