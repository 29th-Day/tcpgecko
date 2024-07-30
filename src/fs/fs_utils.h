#pragma once

#include <gctypes.h>
#include <coreinit/filesystem.h>

#ifdef __cplusplus
extern "C" {
#endif
    // #include "../common/fs_defs.h"

    int MountFS(FSClient* pClient, FSCmdBlock* pCmd, char** mount_path);
    int UmountFS(FSClient* pClient, FSCmdBlock* pCmd, const char* mountPath);

    int LoadFileToMem(const char* filepath, u8** inbuffer, u32* size);

    //! todo: C++ class
    int CreateSubfolder(const char* fullpath);
    int CheckFile(const char* filepath);

#ifdef __cplusplus
}
#endif
