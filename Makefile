BUILD_DIR := build
SRC_DIR   := src
TARGET    := wavs
CC        := gcc
CFLAGS    := -g -O0 -Wall -Wextra -Iinclude -Ithirdparty
LFLAGS    := -lm

SRCS := src/main.c src/samples.c src/wave.c
OBJS := $(SRCS:src/%.c=build/%.o)

WAV_TARGET := file.wav

.PHONY: all
all: $(TARGET)

.PHONY: wav
wav: $(WAV_TARGET)

$(WAV_TARGET): $(TARGET)
	./$(TARGET) $(WAV_TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o $@

build/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

.PHONY: etags
etags:
	etags $(SRCS) include/*

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(WAV_TARGET)
