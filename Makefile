CC := clang
CFLAGS := -Wall -Wextra -Iinclude -pedantic -fPIC
LDFLAGS := -L./build -luuid
APIFLAGS := -lulfius -lorcania -lyder -ljansson 
LDLIB := -lc-ini-api

BUILD_DIR := build

PARSER := $(BUILD_DIR)/c-ini-parser
PARSER_API_LIB := $(BUILD_DIR)/libc-ini-api.so

EXEC_FILES := $(wildcard src/*.c)
TYPES_FILES := $(wildcard src/types/*.c)
LIB_FILES := $(wildcard src/api/*.c)

EXEC_OBJ := $(EXEC_FILES:src/%.c=build/obj/%.o)
TYPES_OBJ := $(TYPES_FILES:src/types/%.c=build/types/%.o)
LIB_OBJ := $(LIB_FILES:src/api/%.c=build/api/%.o)

CFLAGS += -g
APIFLAGS += -g

all: $(BUILD_DIR) $(PARSER)
lib: $(BUILD_DIR) $(PARSER_API_LIB)

$(PARSER): $(EXEC_OBJ) $(TYPES_OBJ) $(PARSER_API_LIB)
	$(CC) -o $@ $(EXEC_OBJ) $(TYPES_OBJ) $(LDFLAGS) $(LDLIB)

$(PARSER_API_LIB): $(LIB_OBJ)
	$(CC) -shared -o $@ $(LIB_OBJ) $(APIFLAGS)

build/obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

build/types/%.o: src/types/%.c
	$(CC) $(CFLAGS) -c $< -o $@

build/api/%.o: src/api/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR): 
	mkdir -p $(BUILD_DIR) $(BUILD_DIR)/api $(BUILD_DIR)/obj $(BUILD_DIR)/types

install: lib
	cp $(PARSER_API_LIB) /usr/lib

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean all lib install
