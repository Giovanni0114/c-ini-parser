CC=clang
CFLAGS=-Wall -Wextra -Iinclude -pedantic -fPIC
LDFLAGS=-L./build
APIFLAGS=-lulfius -lorcania -lyder -ljansson
LDLIB=-lc-ini-api $(APIFLAGS)

BUILD_DIR=build

PARSER=$(BUILD_DIR)/c-ini-parser
PARSER_API_LIB=$(BUILD_DIR)/libc-ini-api.so

EXEC_FILES=src/main.c
#EXEC_FILES=$(wildcard src/*.c)
LIB_FILES=$(wildcard src/api/*.c)

EXEC_OBJ=$(EXEC_FILES:src/%.c=build/obj/%.o)
LIB_OBJ=$(LIB_FILES:src/api/%.c=build/api/%.o)

all: $(BUILD_DIR) $(PARSER)
lib: $(BUILD_DIR) $(PARSER_API_LIB)

$(PARSER): $(EXEC_OBJ) $(PARSER_API_LIB)
	$(CC) -o $@ $(EXEC_OBJ) $(LDFLAGS) $(LDLIB)

$(PARSER_API_LIB): $(LIB_OBJ)
	$(CC) -shared -o $@ $(LIB_OBJ)

build/obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

build/api/%.o: src/api/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR): 
	mkdir -p $(BUILD_DIR) $(BUILD_DIR)/api $(BUILD_DIR)/obj

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean all lib
