#pragma once

#include "../kernel/syscalls.h"
#include "assertions.h"
// #include "../dynamic_libs/os_functions.h"
#include <coreinit/memorymap.h>
#include <coreinit/cache.h>
#include <coreinit/debug.h>
#include "tcp_gecko.h"
#include "../utils/logger.h"

// TODO Variable size, not hard-coded
unsigned char* kernelCopyBufferOld[DATA_BUFFER_SIZE];

void kernelCopyData(unsigned char* destinationBuffer, unsigned char* sourceBuffer, unsigned int length)
{
	if (length > DATA_BUFFER_SIZE)
	{
		OSFatal("Kernel copy buffer size exceeded");
	}

	memcpy(kernelCopyBufferOld, sourceBuffer, length);
	SC0x25_KernelCopyData(
		OSEffectiveToPhysical((u32)destinationBuffer),
		(unsigned int)&kernelCopyBufferOld,
		length);
	DCFlushRange(destinationBuffer, (u32)length);
}

unsigned char* kernelCopyBuffer[sizeof(int)];

void kernelCopyInt(unsigned char* destinationBuffer, unsigned char* sourceBuffer, unsigned int length)
{
	memcpy(kernelCopyBuffer, sourceBuffer, length);
	SC0x25_KernelCopyData(
		OSEffectiveToPhysical((u32)destinationBuffer),
		(unsigned int)&kernelCopyBuffer, length);
	DCFlushRange(destinationBuffer, (u32)length);
}

void writeKernelMemory(const void* address, uint32_t value)
{
	((int*)kernelCopyBuffer)[0] = value;
	kernelCopyInt((unsigned char*)address, (unsigned char*)kernelCopyBuffer, sizeof(int));
}

int readKernelMemory(const void* address)
{
	// For addresses in that range use Chadderz' function to avoid crashing
	if (address > (const void*)0xF0000000)
	{
		log_print("Using Chadderz' kern_read()...\n");
		return kern_read(address);
	}

	log_print("Using dimok's kernelCopy()...\n");
	unsigned char* readBuffer[sizeof(int)];
	kernelCopyInt((unsigned char*)readBuffer, (unsigned char*)address, sizeof(int));

	return ((int*)readBuffer)[0];
}

#define KERNEL_COPY_SOURCE_ADDRESS 0x10100000

int kernelCopyService(int argc, const char** argv)
{
	while (true)
	{
		// Read the destination address from the source address
		int destinationAddress = *(int*)KERNEL_COPY_SOURCE_ADDRESS;

		// Avoid crashing
		if (OSIsAddressValid(destinationAddress))
		{
			// Perform memory copy
			unsigned char* valueBuffer = (unsigned char*)(KERNEL_COPY_SOURCE_ADDRESS + 4);
			kernelCopyInt((unsigned char*)destinationAddress, valueBuffer, 4);

			// "Consume" address and value for synchronization with the code handler for instance
			*(int*)KERNEL_COPY_SOURCE_ADDRESS = 0;
			*(((int*)KERNEL_COPY_SOURCE_ADDRESS) + 1) = 0;
		}
	}

	// This is here to avoid the warning
	return 0;
}

void startKernelCopyService()
{
	unsigned int stack = (unsigned int)memalign(0x40, 0x100);

	if (stack != 0)
	{
		stack += 0x100;
		// OSThread* thread = memalign(0x40, 0x1000);
		OSThread* thread = (OSThread*)memalign(0x40, sizeof(OSThread));
		ASSERT_ALLOCATED(thread, "Kernel copy thread");

		int status = OSCreateThread(thread, kernelCopyService, 1, NULL,
			(void*)(stack + sizeof(stack)), sizeof(stack), 31,
			OS_THREAD_ATTRIB_AFFINITY_CPU1 | OS_THREAD_ATTRIB_DETACHED);
		ASSERT_INTEGER(status, 1, "Creating kernel copy thread");
		// OSSetThreadName(thread, "Kernel Copier");
		OSResumeThread(thread);
	}
}

#define MINIMUM_KERNEL_COMPARE_LENGTH 4
#define KERNEL_MEMORY_COMPARE_STEP_SIZE 1

int kernelMemoryCompare(const char* sourceBuffer,
	const char* destinationBuffer,
	int length)
{
	if (length < MINIMUM_KERNEL_COMPARE_LENGTH)
	{
		ASSERT_MINIMUM_HOLDS(length, MINIMUM_KERNEL_COMPARE_LENGTH, "length");
	}

	bool loopEntered = false;
	while (kern_read(sourceBuffer) == kern_read(destinationBuffer))
	{
		loopEntered = true;
		sourceBuffer = (char*)sourceBuffer + KERNEL_MEMORY_COMPARE_STEP_SIZE;
		destinationBuffer = (char*)destinationBuffer + KERNEL_MEMORY_COMPARE_STEP_SIZE;
		length -= KERNEL_MEMORY_COMPARE_STEP_SIZE;

		if (length <= MINIMUM_KERNEL_COMPARE_LENGTH - 1)
		{
			break;
		}
	}

	if (loopEntered)
	{
		sourceBuffer -= KERNEL_MEMORY_COMPARE_STEP_SIZE;
		destinationBuffer -= KERNEL_MEMORY_COMPARE_STEP_SIZE;
	}

	return kern_read(sourceBuffer) - kern_read(destinationBuffer);
}

void executeAssembly(unsigned char buffer[], unsigned int size)
{
	// Write the assembly to an executable code region
	int destinationAddress = 0x10000000 - size;
	kernelCopyData((unsigned char*)destinationAddress, buffer, size);

	// Execute the assembly from there
	void (*function)() = (void (*)()) destinationAddress;
	function();

	// Clear the memory contents again
	memset((void*)buffer, 0, size);
	kernelCopyData((unsigned char*)destinationAddress, buffer, size);
}
