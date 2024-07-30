#pragma once

#include "../common/kernel_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "syscalls.h"

    extern ReducedCosAppXmlInfo cosAppXmlInfoStruct;

    void SetupKernelCallback(void);

    void KernelSetDBATs(bat_table_t* table);
    void KernelRestoreDBATs(bat_table_t* table);

#ifdef __cplusplus
}
#endif
