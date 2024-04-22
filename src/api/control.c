#include "control.h"
#include "err_messages.h"

struct _u_instance instance = {0};

int get_status(void) {
    return instance.status;
}

int did_error_occured(void) {
    return instance.status == U_STATUS_ERROR;
}

int init_api(void) {
    int res = ulfius_init_instance(&instance, 8080, NULL, NULL);
    if (res != U_OK) {
        fprintf(stderr, "Error upon ulfius_init_instance, abort with error code %d\n", res);
        exit(EXIT_FAILURE);
    }

    instance.check_utf8 = 0;

    res = ulfius_add_endpoint_by_val(&instance, "GET", "/hello", NULL, 0, &hello_world_callback, NULL);
    if (res != U_OK) {
        fprintf(stderr, "Setting of custom callback failed, abort with error code %d\n", res);
        exit(EXIT_FAILURE);
    }

    atexit(cleanup);

    return restart_api();
}

int restart_api(void) {
    int res = ulfius_start_framework(&instance);
    if (res > 0) {
        fprintf(stderr, "restart_api: %s\n", translate_error_code(res));
    }
    return res == U_OK;
}

int stop_api(void) {
    int res = ulfius_stop_framework(&instance);
    if (res > 0) {
        fprintf(stderr, "stop_api: %s\n", translate_error_code(res));
    }
    return res == U_OK;
}

int add_endpoint(char* endpoint, Callback callback) {
    if (instance.status == U_STATUS_RUNNING) {
        if (!stop_api()) fprintf(stderr, "add_endpoint(): unable to stop API");
    }

    int ret = ulfius_add_endpoint_by_val(&instance, "GET", endpoint, NULL, 0, callback, NULL);

    if (instance.status == U_STATUS_STOP) {
        if (!restart_api()) fprintf(stderr, "add_endpoint(): unable to restart API");
    }

    return ret;
}

void cleanup(void) {
    // runned on atexit
    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);
    ulfius_global_close();
}
