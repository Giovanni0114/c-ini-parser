#pragma once
#include <assert.h>
#include <ulfius.h>

#include "callbacks.h"

#define PORT 8888

struct _u_instance instance;

int is_error_occured(void);
int start_api(void);
int stop_api(void);
int restart_api(void);
int add_endpoint(char* endpoint, Callback callback);
void cleanup(void);
int init_api(void);
