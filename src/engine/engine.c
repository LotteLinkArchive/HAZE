#include "engine.h"
#include <stdio.h>
#include <stdlib.h>

X0 errwindow(const CHR *s, ...)
{
	#ifndef HZ_MAX_ERROR_LENGTH
	#define HZ_MAX_ERROR_LENGTH 4096
	#endif

	CHR buffer[HZ_MAX_ERROR_LENGTH];
	va_list args;

	va_start(args, s);
	if (vsnprintf(buffer, HZ_MAX_ERROR_LENGTH, s, args) < 0)
		strcpy(buffer,
			"errwindow() was unable to format the fatal exception message while handling an exception.\0");
	fprintf(stderr, "FATAL ERROR: %s\n", buffer);

	if(SDL_WasInit(SDL_INIT_VIDEO)) {
		SDL_ShowCursor(SDL_TRUE);
		SDL_SetRelativeMouseMode(SDL_FALSE);
		if(primarywin.window) SDL_SetWindowGrab(primarywin.window, SDL_FALSE);
		#ifdef __APPLE__
		if(primarywin.window) SDL_SetWindowFullscreen(screen, 0);
		#endif

		if(primarywin.window) SDL_DestroyWindow(primarywin.window);
	}

	SDL_Quit();
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "HAZE Fatal Exception", buffer, NULL);
	va_end(args);

	exit(EXIT_FAILURE);
}

X0 togglefullscreen()
{
	if (!primarywin.fullscreen) {
		SDL_SetWindowFullscreen(primarywin.window, primarywin.winflags | SDL_WINDOW_FULLSCREEN_DESKTOP);
	} else {
		SDL_SetWindowFullscreen(primarywin.window, primarywin.winflags);
	}
}

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