#pragma once

#include "../common/types.h"
// #include "../dynamic_libs/os_functions.h"

// Convert IP to u32
#define iptu(a, b, c, d) (uint32_t)((a << 24) | (b << 16) | (c << 8) | d)

// Convert u32 to IP
#define utip(x) ((x >> 24) & 0xFF), ((x >> 16) & 0xFF), ((x >> 8) & 0xFF), (x & 0xFF)

#ifdef __cplusplus
extern "C" {
#endif

    // 10.0.0.69
    const u32 hostIpAddress = iptu(10, 0, 0, 69);

    int Menu_Main();

    extern bool isCodeHandlerInstalled;
    extern bool areSDCheatsEnabled;

#ifdef __cplusplus
}
#endif
