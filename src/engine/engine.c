#include "engine.h"

void errwindow(CHR *s)
{
	printf("ERROR: %s\n", s);

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
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "HAZE Fatal Exception", s, NULL);

	exit(EXIT_FAILURE);
}

void togglefullscreen(struct hzwinprop window)
{
	if (!window.fullscreen) {
		SDL_SetWindowFullscreen(primarywin.window, primarywin.winflags | SDL_WINDOW_FULLSCREEN_DESKTOP);
	} else {
		SDL_SetWindowFullscreen(primarywin.window, primarywin.winflags);
	}
}