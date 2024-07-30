#pragma once

// #define THREAD_SIZE 0x6A0
// #define PREVIOUS_THREAD 0x390
// #define NEXT_THREAD 0x38C

#include <vector>
#include <coreinit/thread.h>

/**
 * @brief Get a list of all active threads
 * @attention DO NOT FREE the OSThread pointers. They BELONG TO THE OS.
 * @return Vector of thread pointers
 */
std::vector<OSThread*> getAllThreads();
