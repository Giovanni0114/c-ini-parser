#pragma once
#include <ulfius.h>

typedef int (*Callback)(const struct _u_request *request, struct _u_response *response, void *user_data);

/*
 * SERVER BACKBONE
 *
 *
 */

int shutdown_callback(const struct _u_request *request, struct _u_response *response, void *user_data);
int hello_world_callback(const struct _u_request *request, struct _u_response *response, void *user_data); 
int default_callback(const struct _u_request *request, struct _u_response *response, void *user_data);
int callback_echo_json(const struct _u_request *request, struct _u_response *response, void *user_data);
