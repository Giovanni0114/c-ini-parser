#include "err_messages.h"

#define DESC_OK "No error"                                    //  0
#define DESC_ERROR "Error"                                    //  1
#define DESC_ERROR_MEMORY "Error in memory allocation"        //  2
#define DESC_ERROR_PARAMS "Error in input parameters"         //  3
#define DESC_ERROR_LIBMHD "Error in libmicrohttpd execution"  //  4
#define DESC_ERROR_LIBCURL "Error in libcurl execution"       //  5
#define DESC_ERROR_NOT_FOUND "Something was not found"        //  6

char *translate_error_code(int errno) {
    switch (errno) {
        case 0:
            return DESC_OK;
        case 1:
            return DESC_ERROR;
        case 2:
            return DESC_ERROR_MEMORY;
        case 3:
            return DESC_ERROR_PARAMS;
        case 4:
            return DESC_ERROR_LIBMHD;
        case 5:
            return DESC_ERROR_LIBCURL;
        case 6:
            return DESC_ERROR_NOT_FOUND;
    }

    return "UNKNOWN";
}
