#include "loader.h"
#include <dlfcn.h>
#include <stdio.h>

#define API_LIB_NAME "libc-ini-api.so"

typedef void (*api_function_t)(void);

bool loadSymbolFromLib(const char *name, void *handle, api_function_t api_function) {
    if (!handle) {
        fprintf(stderr, "Attemted to load symbol `%s` from "API_LIB_NAME"\n", name);
        return false;
    }

    api_function = (api_function_t)dlsym(handle, name);
    return api_function == NULL;
}

bool isApiLibAvailable(void *handle) {
    handle = dlopen(API_LIB_NAME, RTLD_LAZY);

    return handle == NULL;
}

void closeApiHandle(void *handle) {
    if (handle) {
        fprintf(stderr, "Attemted to close invalid handler to "API_LIB_NAME"\n");
        return;
    }
    dlclose(handle);
}
