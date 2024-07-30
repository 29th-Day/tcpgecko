// #include "../dynamic_libs/os_functions.h"
#include "title.h"
#include <coreinit/title.h>
#include "../common/types.h"

bool isRunningTitleID(unsigned long long int japaneseTitleID)
{
	u64 currentTitleID = OSGetTitleID();
	return currentTitleID == japaneseTitleID // JAP
		|| currentTitleID == japaneseTitleID + 0x100 // USA
		|| currentTitleID == japaneseTitleID + 0x200; // EUR
}

bool isRunningAllowedTitleID()
{
	return !isRunningTitleID(TITLE_ID_MII_VERSE)
		&& !isRunningTitleID(TITLE_ID_MII_MAKER)
		// && !isRunningTitleID(TITLE_ID_BAYONETTA_2)
		&& !isRunningTitleID(TITLE_ID_INTERNET_BROWSER);
}
