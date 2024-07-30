#include "threads.h"
// #include "linked_list.h"
// #include "../dynamic_libs/os_functions.h"
#include <coreinit/thread.h>
#include <coreinit/interrupts.h>
#include <coreinit/scheduler.h>
#include "../utils/logger.h"
#include "../common/types.h"

#include <vector>

std::vector<OSThread*> getAllThreads()
{
	/*
	struct node* threads = NULL;
	OSThread* currentThread = OSGetCurrentThread();
	log_printf("Thread address: %08x\n", currentThread->);
	int iterationThreadAddress = currentThreadAddress;
	int temporaryThreadAddress;

	// Follow "previous thread" pointers back to the beginning
	while ((temporaryThreadAddress = *(int*)(iterationThreadAddress + PREVIOUS_THREAD)) != 0)
	{
		log_printf("Temporary thread address going backwards: %08x\n", temporaryThreadAddress);
		iterationThreadAddress = temporaryThreadAddress;
	}

	// Now iterate over all threads
	while ((temporaryThreadAddress = *(int*)(iterationThreadAddress + NEXT_THREAD)) != 0)
	{
		// Grab the thread's address
		log_printf("Temporary thread address going forward: %08x\n", temporaryThreadAddress);
		threads = insert(threads, (void*)iterationThreadAddress);
		log_printf("Inserted: %08x\n", iterationThreadAddress);
		iterationThreadAddress = temporaryThreadAddress;
	}

	// The previous while would skip the last thread so add it as well
	threads = insert(threads, (void*)iterationThreadAddress);
	log_printf("Inserted: %08x\n", iterationThreadAddress);

	reverse(&threads);
	*/

	// struct node* threads = NULL;

	const s32 numThreads = OSCheckActiveThreads();
	if (numThreads <= 0)
	{
		// maybe print some stuff but SHOULD NEVER HAPPEN
		return std::vector<OSThread*>();
	}

	auto threads = std::vector<OSThread*>(numThreads);

	OSThread* thread = *((OSThread**)0x100567F8); // https://github.com/ItzSwirlz/ThredList/blob/c9a2b249c0205896259d524357a9408b4af6ec69/src/main.c#L37C39-L37C49

	if (thread == NULL)
	{
		// maybe print some stuff but SHOULD NEVER HAPPEN
		return threads;
	}

	OSThread* currentThread = OSGetCurrentThread();
	int state = OSDisableInterrupts();
	__OSLockScheduler(currentThread);

	for (int i = 0; i < numThreads && thread; i++)
	{
		threads.push_back(thread);
		thread = thread->activeLink.next;
	}

	__OSUnlockScheduler(currentThread);
	OSRestoreInterrupts(state);

	return threads;
}
