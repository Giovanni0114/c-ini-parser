#pragma once

#include <ulfius.h>

typedef int (*Callback)(const struct _u_request *request, struct _u_response *response, void *user_data);

int shutdown_callback(const struct _u_request *request, struct _u_response *response, void *user_data) {
    fprintf(stderr, "shutdown via API");
    exit(420);
}

int hello_world_callback(const struct _u_request *request, struct _u_response *response, void *user_data) {
    ulfius_set_string_body_response(response, 200, "Hello, world!");
    return U_CALLBACK_CONTINUE;
}

int default_callback(const struct _u_request *request, struct _u_response *response, void *user_data) {
    ulfius_set_string_body_response(response, 404, "Undefined");
    return U_CALLBACK_CONTINUE;
}

int callback_echo_json(const struct _u_request *request, struct _u_response *response, void *user_data) {
    json_t *json_request_body;
    json_error_t json_error;

    // Parse the JSON in the request body
    json_request_body = ulfius_get_json_body_request(request, &json_error);

    if (json_request_body) {
        // Set the response header to application/json
        u_map_put(response->map_header, "Content-Type", "application/json");

        // Use the parsed JSON directly as the response body
        ulfius_set_json_body_response(response, 200, json_request_body);

        // Free the JSON object
        json_decref(json_request_body);
    } else {
        // If error in JSON parsing, send an error message
        json_t *error_json = json_pack("{s:s}", "error", "Invalid JSON data");
        ulfius_set_json_body_response(response, 400, error_json);
        json_decref(error_json);
    }

    return U_CALLBACK_CONTINUE;
}

