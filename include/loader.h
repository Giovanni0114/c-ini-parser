#include <stdbool.h>

#define API_LIB_NAME "libc-ini-api.so"

typedef void (*api_function_t)(void);

bool loadSymbolFromLib(const char *name, void *handle, api_function_t api_function);
bool isApiLibAvailable(void *handle);
void closeApiHandle(void *handle);
