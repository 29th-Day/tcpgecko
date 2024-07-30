#pragma once

#include <coreinit/debug.h>
#include <string.h>
#include <whb/log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_LOGGER        1
#define COMPUTER_IP_ADDRESS "192.168.2.103"

#ifdef DEBUG_LOGGER
#define log_init(x) 
#define log_deinit()
#define log_print(STR) WHBLogPrint(STR);
#define log_printf(FMT, ARGS...) WHBLogPrintf(FMT, ##ARGS)
#else
#define log_init(x)
#define log_deinit()
#define log_print(x)
#define log_printf(x, ...)
#endif

#ifdef __cplusplus
}
#endif
