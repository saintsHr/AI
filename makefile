SRC := $(shell find src -name "*.c")
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))

CC := gcc

DEPFLAGS := -MMD -MP
CFLAGS := -Iinclude -Wall -Wextra $(DEPFLAGS) -lm -O3
LDFLAGS := -lm -O3

TARGET := AI

.PHONY: all clean

all: bin/$(TARGET)

bin/$(TARGET): $(OBJ)
	mkdir -p bin
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(OBJ:.o=.d)

clean:
	rm -rf build bin
