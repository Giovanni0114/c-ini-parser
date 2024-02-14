#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ulfius.h>

#include "api/callbacks.c"
#include "api/control.c"

void register_ini_section_on_api(char *name, Callback callback, void *data) {
    if (!stop_api()) {
        fprintf(stderr, "register_section_on_api(): unable to stop API");
    }

    char endpoint[100];
    sprintf(endpoint, "/%s", name);


    if (!start_api()) {
        fprintf(stderr, "register_section_on_api(): unable to start API");
    }
}

#ifdef API_TEST
int main(void) {
    init_api();
    ulfius_add_endpoint_by_val(&instance, "GET", "/helloworld", NULL, 0, &hello_world_callback, NULL);
    register_ini_section_on_api("hw", &hello_world_callback, NULL);
    start_api();

    return 0;
}
#endif /* ifdef API-TEST */
