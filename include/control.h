#include <ulfius.h>

#include "callbacks.h"

#define PORT 8888

extern struct _u_instance instance;

int get_status(void);
int did_error_occured(void);
int restart_api(void);
int stop_api(void);
int add_endpoint(char* endpoint, Callback callback);
void cleanup(void);
int init_api(void);
