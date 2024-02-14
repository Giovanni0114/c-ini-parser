#pragma once

#include <assert.h>
#include <ulfius.h>

#include "callbacks.c"

#define PORT 8888

struct _u_instance* _instance;

int start_api(void) {
    return ulfius_start_framework(_instance) == U_OK;
}

int stop_api(void) {
    return ulfius_stop_framework(_instance) == U_OK;
}

int restart_api(void) {
    // TODO: here I'm about to add function that will get an pointer to another function,
    // It will stop API, run function, and then start API.
    assert(0 && "Not implemented");
}

int add_endpoint(char *endpoint, Callback callback){
    return ulfius_add_endpoint_by_val(_instance, "GET", endpoint, NULL, 0, callback, NULL) == U_OK;
}

void cleanup(void) {
    // runned on atexit
    ulfius_stop_framework(_instance);
    ulfius_clean_instance(_instance);
    ulfius_global_close();
}

int init_api(void) {
    if (ulfius_global_init() != U_OK) {
        fprintf(stderr, "Error upon ulfius_global_init, abort\n");
        exit(EXIT_FAILURE);
    }

    if (ulfius_init_instance(_instance, PORT, NULL, NULL) != U_OK) {
        fprintf(stderr, "Error upon ulfius_init_instance, abort\n");
        exit(EXIT_FAILURE);
    }

    if (ulfius_set_default_endpoint(_instance, default_callback, NULL) != U_OK) {
        fprintf(stderr, "Setting of default callback failed, abort\n");
        exit(EXIT_FAILURE);
    }

    atexit(cleanup);

    return start_api();
}
