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

	cleanup();
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

	primarywin.fullscreen = !primarywin.fullscreen;
}

X0 cleanup()
{
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

	if (0 != gameconf) toml_free(gameconf);
}
