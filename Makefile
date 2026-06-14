CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include -g
LDFLAGS = -lm

SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin
OBJ_DIR = obj

TARGETS = $(BIN_DIR)/genealogy1 $(BIN_DIR)/genealogy2

all: $(BIN_DIR) $(OBJ_DIR) $(TARGETS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

structure1: $(BIN_DIR)/genealogy1

structure2: $(BIN_DIR)/genealogy2

$(BIN_DIR)/genealogy1: $(OBJ_DIR)/structure1.o $(OBJ_DIR)/main1.o $(OBJ_DIR)/common.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/genealogy2: $(OBJ_DIR)/structure2.o $(OBJ_DIR)/main2.o $(OBJ_DIR)/common.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean structure1 structure2
