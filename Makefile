# Project details
PROJECT_NAME := stringify
CC := $(shell command -v clang || command -v gcc)
CFLAGS := -Wall -Wextra -Werror -Wconversion -Wunused-result
CPPFLAGS := -Isrc
LDFLAGS := -lc-tools_dynamic-string

# Dirs
BUILD_DIR := build
TEST_EXE_DIR := $(BUILD_DIR)/tests
SRC_DIR := src
TEST_SRC_DIR := tests
OBJ_DIR := $(BUILD_DIR)/obj

# Files
MAIN_SRC := $(SRC_DIR)/main.c
SRC := $(filter-out $(MAIN_SRC), $(wildcard $(SRC_DIR)/*.c))
TEST_SRC := $(wildcard $(TEST_SRC_DIR)/*.c)
DOC := README.md

# Targets
EXE := $(BUILD_DIR)/$(PROJECT_NAME)
TEST_EXE := $(TEST_SRC:$(TEST_SRC_DIR)/%.c=$(TEST_EXE_DIR)/%)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Commands
RUN_TEST := $(TEST_EXE:$(TEST_EXE_DIR)/%=./$(TEST_EXE_DIR)/%)

# Rules
.PHONY: test clean

test: $(TEST_EXE)
	$(RUN_TEST)

# $(TEST_EXE): CPPFLAGS := -Isrc -DNDEBUG -DTESTING
$(TEST_EXE): CPPFLAGS := -Isrc -DTESTING
$(TEST_EXE_DIR)/%: $(TEST_SRC_DIR)/%.c $(OBJ) | $(TEST_EXE_DIR)
	bear -- $(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

$(TEST_EXE_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) compile_commands.json
