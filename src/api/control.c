#include "control.h"

int is_error_occured(void) {
    return instance.status == U_STATUS_ERROR;
}

int start_api(void) {
    return ulfius_start_framework(&instance) == U_OK;
}

int stop_api(void) {
    return ulfius_stop_framework(&instance) == U_OK;
}

int restart_api(void) {
    // TODO: here I'm about to add function that will get an pointer to another function,
    // It will stop API, run function, and then start API.
    assert(0 && "Not implemented");
}

int add_endpoint(char* endpoint, Callback callback) {
    if (instance.status == U_STATUS_RUNNING) {
        if (!stop_api()) fprintf(stderr, "add_endpoint(): unable to stop API");
    }

    int ret = ulfius_add_endpoint_by_val(&instance, "GET", endpoint, NULL, 0, callback, NULL);

    if (instance.status == U_STATUS_STOP) {
        if (!start_api()) fprintf(stderr, "add_endpoint(): unable to start API");
    }

    return ret;
}

void cleanup(void) {
    // runned on atexit
    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);
    ulfius_global_close();
}

int init_api(void) {
    if (ulfius_global_init() != U_OK) {
        fprintf(stderr, "Error upon ulfius_global_init, abort\n");
        exit(EXIT_FAILURE);
    }

    if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
        fprintf(stderr, "Error upon ulfius_init_instance, abort\n");
        exit(EXIT_FAILURE);
    }

    if (ulfius_set_default_endpoint(&instance, default_callback, NULL) != U_OK) {
        fprintf(stderr, "Setting of default callback failed, abort\n");
        exit(EXIT_FAILURE);
    }

    atexit(cleanup);

    return start_api();
}
