#include "memory.h"

X0 *oomrealloc(X0 *ptr, SX size)
{
	ptr = realloc(ptr, size);
	if (!ptr) {
		errwindow("[Possibly] Out of memory! Failed trying to allocate %zu bytes.\n\n"
			"This could allude to several problems. For example, there could be a memory leak.\n"
			"Alternatively, your computer might have too much garbage open in the background.\n"
			"Your computer also could just not have enough RAM, but this is very unlikely.\n"
			"Try uninstalling extremely memory-hungry applications, such as Windows or Discord.\n\n"
			"All jokes aside, if you keep getting this error or you suspect there may be a memory leak...\n"
			"CONTACT THE GAME DEVELOPERS. This may be a severe issue with their game, or even HAZE.\n"
			"Remember, if you don't complain about OOM issues, developers will just make worse programs!\n"
			"Always whine about memory usage! (That one IS NOT a joke)", size);
		return NULL;
	}
	return ptr;
}
