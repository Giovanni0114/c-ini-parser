CFLAGS=-Wall -pedantic -x c
APIFLAGS=-lulfius -lorcania -lyder -ljansson

all: parser api cli

parser:
	clang $(CFLAGS) -o build/parser src/main.c 

api: 
	clang $(CFLAGS) -o build/c-ini-parser-api.o -c src/api.c

cli:
	clang $(CFLAGS) -o build/c-ini-parser-api.o -c src/cli.c

tests: api-test parser-test cli-test

parser-test:
	:

cli-test:
	:

api-test:
	clang $(CFLAGS) -o build/test_api src/api.c src/api/* -DAPI_TEST $(APIFLAGS)
